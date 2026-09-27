#include <Windows.h>
#include <ShellAPI.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <filesystem>
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
using namespace std;

//检查文件是否存在
bool isFileExists(const std::string& filePath) {
    // GetFileAttributesA 用于获取文件属性，返回INVALID_FILE_ATTRIBUTES表示文件不存在
    DWORD fileAttr = GetFileAttributesA(filePath.c_str());
    // 检查：1. 属性获取成功 2. 不是目录（确保是文件）
    return (fileAttr != INVALID_FILE_ATTRIBUTES) && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY);
}

// 根据进程名获取PID
DWORD GetProcessIdByName(const char* processName) {
    DWORD pid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            // 宽字符转多字节
            char exeName[MAX_PATH];
            WideCharToMultiByte(CP_ACP, 0, pe32.szExeFile, -1, exeName, MAX_PATH, nullptr, nullptr);
            // 再用 _stricmp 比较
            if (_stricmp(exeName, processName) == 0) {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return pid;
}

// 注入DLL到目标进程
bool InjectDLL(DWORD pid, const char* dllPath) {
    // 打开目标进程（获取足够权限）
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        cout << "打开进程失败！错误码：" << GetLastError() << endl;
        MessageBoxA(NULL, "打开进程失败！请使用管理员身份运行此程序", "错误", MB_ICONERROR);
        return false;
    }

    // 分配内存存储DLL路径
    LPVOID pRemoteAddr = VirtualAllocEx(hProcess, nullptr, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pRemoteAddr) {
        cout << "分配内存失败！错误码：" << GetLastError() << endl;
        MessageBoxA(NULL, "分配内存失败！", "错误", MB_ICONERROR);
        CloseHandle(hProcess);
        return false;
    }

    // 写入DLL路径到目标进程内存
    if (!WriteProcessMemory(hProcess, pRemoteAddr, dllPath, strlen(dllPath) + 1, nullptr)) {
        cout << "写入内存失败！错误码：" << GetLastError() << endl;
        MessageBoxA(NULL, "写入内存失败！", "错误", MB_ICONERROR);
        VirtualFreeEx(hProcess, pRemoteAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // 创建远程线程调用LoadLibraryA加载DLL
    HANDLE hRemoteThread = CreateRemoteThread(hProcess, nullptr, 0,
        (LPTHREAD_START_ROUTINE)LoadLibraryA, pRemoteAddr, 0, nullptr);
    if (!hRemoteThread) {
        cout << "创建远程线程失败！错误码：" << GetLastError() << endl;
        MessageBoxA(NULL, "创建远程线程失败！", "错误", MB_ICONERROR);
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

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);// 隐藏控制台窗口

    const char* targetProcess = "Sausage Man.exe"; // 修正拼写
    const char* dllName = "TL.dll";

    // 获取DLL绝对路径（确保注入器和TL.dll同目录）
    char dllPath[MAX_PATH] = { 0 };
    GetFullPathNameA(dllName, MAX_PATH, dllPath, nullptr);

    if (!isFileExists("TL.dll")) {
        //cout << "未找到文件：TL.dll！" << endl;
        MessageBoxA(NULL, "未找到文件：TL.dll！", "错误", MB_ICONERROR);
        return 2;
    }

    // 查找目标进程PID
    DWORD pid = GetProcessIdByName(targetProcess);
    if (pid == 0) {
        //cout << "未找到进程 " << targetProcess << "，请先启动游戏！" << endl;
        MessageBoxA(NULL, "请进入到游戏大厅！", "错误", MB_ICONERROR);
		//system("pause");// 暂停以便查看消息
        return 1;
    }
    cout << "找到进程，PID：" << pid << endl;
    // 执行注入
    if (InjectDLL(pid, dllPath)) {
        //cout << "DLL注入成功！" << endl;
        //MessageBoxA(NULL, "DLL注入成功！", "成功", MB_ICONINFORMATION);
        return 0;
    }
    else {
       // cout << "DLL注入失败！" << endl;
        //MessageBoxA(NULL, "DLL注入失败！", "错误", MB_ICONERROR);
        return 3;
    }
    //system("pause");// 暂停以便查看消息
}