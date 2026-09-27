#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#pragma comment(lib, "Psapi.lib")
using namespace std;

// 根据进程名获取PID（宽字符版本）
DWORD GetProcessIdByName(const wchar_t* processName)
{
    DWORD pid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    PROCESSENTRY32W pe32 = { 0 };
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe32))
    {
        do
        {
            if (_wcsicmp(pe32.szExeFile, processName) == 0)
            {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return pid;
}

// 获取目标进程中指定DLL的模块句柄
HMODULE GetRemoteModuleHandle(DWORD pid, const wchar_t* dllName)
{
    HMODULE hModule = nullptr;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProcess)
        return nullptr;

    HMODULE hModules[1024] = { 0 };
    DWORD cbNeeded = 0;

    if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded))
    {
        for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            wchar_t szModuleName[MAX_PATH] = { 0 };
            if (GetModuleBaseNameW(hProcess, hModules[i], szModuleName, _countof(szModuleName)))
            {
                if (_wcsicmp(szModuleName, dllName) == 0)
                {
                    hModule = hModules[i];
                    break;
                }
            }
        }
    }

    CloseHandle(hProcess);
    return hModule;
}

// 注入DLL到目标进程
bool InjectDLL(DWORD pid, const char* dllPath)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
    {
        cout << "打开进程失败！错误码：" << GetLastError() << endl;
        return false;
    }

    // 分配内存存储DLL路径
    LPVOID pRemoteAddr = VirtualAllocEx(hProcess, nullptr, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pRemoteAddr)
    {
        cout << "分配内存失败！错误码：" << GetLastError() << endl;
        CloseHandle(hProcess);
        return false;
    }

    // 写入DLL路径到目标进程内存
    if (!WriteProcessMemory(hProcess, pRemoteAddr, dllPath, strlen(dllPath) + 1, nullptr))
    {
        cout << "写入内存失败！错误码：" << GetLastError() << endl;
        VirtualFreeEx(hProcess, pRemoteAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // 创建远程线程调用LoadLibraryA加载DLL
    HANDLE hRemoteThread = CreateRemoteThread(hProcess, nullptr, 0,
        (LPTHREAD_START_ROUTINE)LoadLibraryA, pRemoteAddr, 0, nullptr);
    if (!hRemoteThread)
    {
        cout << "创建远程线程失败！错误码：" << GetLastError() << endl;
        VirtualFreeEx(hProcess, pRemoteAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // 等待线程结束
    WaitForSingleObject(hRemoteThread, INFINITE);

    // 释放资源
    CloseHandle(hRemoteThread);
    VirtualFreeEx(hProcess, pRemoteAddr, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return true;
}

// 从目标进程卸载指定DLL
bool UninjectDLL(DWORD pid, const wchar_t* dllName)
{
    // 获取DLL在目标进程中的模块句柄
    HMODULE hRemoteDll = GetRemoteModuleHandle(pid, dllName);
    if (!hRemoteDll)
    {
        cout << "未找到目标进程中的TL.dll模块！" << endl;
        return false;
    }

    // 打开目标进程
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess)
    {
        cout << "打开进程失败！错误码：" << GetLastError() << endl;
        return false;
    }

    // 创建远程线程调用FreeLibrary卸载DLL
    HANDLE hRemoteThread = CreateRemoteThread(hProcess, nullptr, 0,
        (LPTHREAD_START_ROUTINE)FreeLibrary, hRemoteDll, 0, nullptr);
    if (!hRemoteThread)
    {
        cout << "创建卸载线程失败！错误码：" << GetLastError() << endl;
        CloseHandle(hProcess);
        return false;
    }

    // 等待卸载线程完成
    WaitForSingleObject(hRemoteThread, INFINITE);
    cout << "TL.dll已成功卸载！" << endl;

    // 释放资源
    CloseHandle(hRemoteThread);
    CloseHandle(hProcess);
    return true;
}

int main()
{
    const wchar_t* targetProcess = L"Sausage Man.exe";
    const char* dllNameA = "TL.dll";
    const wchar_t* dllNameW = L"TL.dll";

    cout << "=== Sausage Man 注入器（带END键卸载）===" << endl;
    wcout << L"目标进程：" << targetProcess << endl;
    cout << "注入DLL：" << dllNameA << endl;
    cout << "-------------------------" << endl;
    cout << "提示：注入成功后按 END 键卸载DLL，按 ESC 键退出注入器" << endl;
    cout << "-------------------------" << endl;

    // 获取DLL绝对路径
    char dllPath[MAX_PATH] = { 0 };
    if (!GetFullPathNameA(dllNameA, MAX_PATH, dllPath, nullptr))
    {
        cout << "获取DLL路径失败！错误码：" << GetLastError() << endl;
        system("pause");
        return 1;
    }
    cout << "DLL完整路径：" << dllPath << endl;

    // 查找目标进程PID
    DWORD pid = GetProcessIdByName(targetProcess);
    if (pid == 0)
    {
        cout << "未找到进程Sausage Man.exe，请先启动游戏！" << endl;
        system("pause");
        return 1;
    }
    cout << "找到进程，PID：" << pid << endl;

    // 执行注入
    if (InjectDLL(pid, dllPath))
    {
        cout << "DLL注入成功！" << endl;
        cout << "等待您按下 END 键卸载DLL（ESC键退出）..." << endl;

        // 循环监听按键
        while (true)
        {
            // 检测ESC键：退出注入器
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            {
                cout << "用户按下ESC键，退出注入器！" << endl;
                break;
            }

            // 检测END键：卸载DLL
            if (GetAsyncKeyState(VK_END) & 0x8000)
            {
                cout << "检测到END键，开始卸载DLL..." << endl;
                UninjectDLL(pid, dllNameW);
                break;
            }

            // 降低CPU占用
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
    else
    {
        cout << "DLL注入失败！" << endl;
    }

    system("pause");
    return 0;
}