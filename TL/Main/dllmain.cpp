// dllmain.cpp : 定义 DLL 应用程序的入口点。

//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include <Windows.h>
#include <thread>
#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <chrono>
#include <TlHelp32.h>
#include <winternl.h>
#include <fstream>
#include <sstream>
#include <mutex>
#include <atomic>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "pch.h" //包含ImGui和MinHook头文件
#include "Font.h"//包含字体加载相关代码
#include "..\Library\AntiDebug.h" // 调试器检测头文件
#include "..\GUI\GUI.h"
#include "..\Class\Other\Other.h"
#include "..\Class\Role\Role.h"
#include "..\Class\AI\AI.h"
#include "..\Class\Car\Car.h"
#include "..\Class\Memory\Memory.h"
#include "..\Class\Item\Item.h"
#include "..\Class\Beautify\Beautify.h"

// ====================== 全局变量区 ======================
// 绘制模式互斥开关：true=仅外置透明窗口外绘；false=仅游戏内Present Hook内绘，二者互斥不会双绘制
bool g_UseExternalOverlay = true;

// 游戏内 Present Hook 渲染全局
static ID3D11Device* g_pd3dDevice = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11DeviceContext* g_pd3dContext = nullptr;
static ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
static HWND g_hGameWnd = nullptr;
WNDPROC g_OriginalWndProc = nullptr;
void* g_OriginalPresent = nullptr;
bool g_GameImGuiInit = false;

// 全局控制
static bool g_ShowMenu = true;
static bool g_HomeKeyDown = false;
std::string g_CardKey;
std::atomic<bool> g_ThreadQuitFlag = false;
static bool g_PopupOpened = false;
bool g_DebugMode = false;

// 计时相关全局
std::mutex g_TimeMutex;
std::string g_RunTimeStr = "0天0时0分0秒";
int g_Day = 0, g_Hour = 0, g_Min = 0, g_Sec = 0;

//卡密验证类
std::string 公告;
ApiResult 云端版本;




// 外置透明Overlay窗口 独立命名空间隔离
namespace OverlayWin
{
    std::thread g_OverlayThread;

    void DrawOverlayUI()
    {
        if (g_ShowMenu) Draw_Menu();
        DrawFPSCounter(显示::FPS面板);
        MiniMenu(&显示::开启提示);
        Beautifly_main(); Memory_Main(); Item_Main(); Car_main(); AI_main(); Role_main();
    }

    void OverlayWindowWork()
    {
        myimgui::CreateWindow_Violet("UnityWndClass", "Sausage Man", DrawOverlayUI);
    }
}

// ====================== 工具函数 ======================
void 输出调试文本() {
    printf(" ______    __        \n");
    printf("/\\__  _\\  /\\ \\       \n");
    printf("\\/_/\\ \\/  \\ \\ \\      \n");
    printf("   \\ \\ \\   \\ \\ \\   \n");
    printf("    \\ \\ \\   \\ \\ \\____ \n");
    printf("     \\ \\_\\   \\ \\_____\\\n");
    printf("      \\/_/    \\/_____/  \n");
    printf("\n欢迎使用TL！\n");
    printf("程序运行时请不要点击该黑色窗口\n");
    printf("HOME显示隐藏，Alt+End退出辅助。\n");
    printf("出现bug及时联系作者！\n");
    printf("QQ交流群：1028543252！\n");
}

// 帧率限制
void LimitFps(float targetFps)
{
    static LARGE_INTEGER perfFreq = { 0 };
    static LARGE_INTEGER lastFrameTime = { 0 };
    if (perfFreq.QuadPart == 0)
    {
        QueryPerformanceFrequency(&perfFreq);
        QueryPerformanceCounter(&lastFrameTime);
    }
    if (targetFps <= 0.0f) targetFps = 60.0f;
    const double frameIntervalMs = 1000.0 / targetFps;
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    double elapsedMs = (currentTime.QuadPart - lastFrameTime.QuadPart) * 1000.0 / perfFreq.QuadPart;
    if (elapsedMs < frameIntervalMs)
    {
        DWORD sleepMs = (DWORD)(frameIntervalMs - elapsedMs - 0.2);
        if (sleepMs > 0)
        {
            MsgWaitForMultipleObjects(0, nullptr, FALSE, sleepMs, QS_ALLINPUT);
        }
        while (true)
        {
            QueryPerformanceCounter(&currentTime);
            elapsedMs = (currentTime.QuadPart - lastFrameTime.QuadPart) * 1000.0 / perfFreq.QuadPart;
            if (elapsedMs >= frameIntervalMs) break;
        }
    }
    QueryPerformanceCounter(&lastFrameTime);
}

std::mutex g_timeMutex;
std::string g_runtimeString = "0天0时0分0秒";
int 天 = 0, 时 = 0, 分 = 0, 秒 = 0;
std::string 取程序运行时间_文本() {
    std::ostringstream oss;
    oss << 天 << (const char*)u8"天" << 时 << (const char*)u8"时" << 分 << (const char*)u8"分" << 秒 << (const char*)u8"秒";
    return oss.str();
}

std::string 时间_计算相差时间(long long expireTimestampSec) {
    time_t now = time(nullptr);
    long long diff = expireTimestampSec - now;
    if (diff <= 0) return GBK转UTF8("已过期").c_str();
    long long days = diff / 86400;
    long long hours = (diff % 86400) / 3600;
    long long minutes = (diff % 3600) / 60;
    long long seconds = diff % 60;
    char buf[128];
    sprintf_s(buf, (const char*)u8"%lld天%lld时%lld分%lld秒", days, hours, minutes, seconds);
    return std::string(buf);
}

// 时间戳转换
std::string timestampToLocalTime(long long timestamp) {
    if (timestamp <= 0) return "无效时间";
    if (timestamp > 1000000000000LL) timestamp /= 1000;
    time_t t = static_cast<time_t>(timestamp);
    struct tm local_tm = {};
    if (localtime_s(&local_tm, &t) != 0)
    {
        if (gmtime_s(&local_tm, &t) != 0)
            return "时间转换失败";
    }
    char buf[100];
    if (strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local_tm) == 0)
        return "时间格式化失败";
    return std::string(buf);
}

// 心跳循环线程
void 时间_循环() {
    while (!g_ThreadQuitFlag) {
    
        /*
        if (IsCheatEngineDetected()) {
            // 处理检测到 Cheat Engine 的情况
			printf("检测到 Cheat Engine，正在退出程序...\n");
        }
        if (IsCommonDebuggerDetected()) {
            // 处理检测到常见调试器的情况
			printf("检测到调试器，正在退出程序...\n");
        }
        if (IsBeingDebugged()) {
            // 处理检测到被调试的情况
			printf("检测到被调试，正在退出程序...\n");
        }
        if (IsStrongODDriverDetected()) {
            // 处理检测到 StrongOD 驱动的情况
			printf("检测到 StrongOD 驱动，正在退出程序...\n");
        }
        if (IsVirtualMachineDetected()) {
            // 处理检测到虚拟机的情况
			printf("检测到虚拟机，正在退出程序...\n");
        }
        */
        Sleep(10000);
    }
}

// 运行计时线程
void 计时() {
    while (!g_ThreadQuitFlag) {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 每 1 秒执行一次
        秒++;
        if (秒 == 60) { 秒 = 0; 分++; }  // 60秒 = 1分
        if (分 == 60) { 分 = 0; 时++; }  // 60分 = 1时
        if (时 == 24) { 时 = 0; 天++; }  // 24时 = 1天
        // 拼接文本
        std::ostringstream oss;
        oss << 天 << "天" << 时 << "时" << 分 << "分" << 秒 << "秒";
        // 线程安全写入
        {
            std::lock_guard<std::mutex> lock(g_timeMutex);
            g_runtimeString = oss.str();
        }
    }

}


// 启动心跳+计时线程
void 启动计时() {
    std::thread t(时间_循环);
    t.detach();
    std::thread s(计时);
    s.detach();
}

// 特征码批量搜索赋值
void 特征码取地址() {
    uintptr_t 世界地址 = 0;
    uintptr_t 地址 = 特征码搜索("GameAssembly.dll", "48 8b 05 ?? ?? ?? ?? 83 b8 ?? ?? ?? ?? ?? 75 ?? 48 8b c8 e8 ?? ?? ?? ?? 48 8b 05 ?? ?? ?? ?? 48 8b 0d ?? ?? ?? ?? 48 8b 80 ?? ?? ?? ?? 83 b9 ?? ?? ?? ?? ?? 48 8b 98 ?? ?? ?? ?? 75 ?? e8 ?? ?? ?? ?? 33 d2 48 8b cb e8 ?? ?? ?? ?? 48 8b 0d ?? ?? ?? ?? 84 c0", 0, true);
    if (地址) {
        uint32_t disp = *reinterpret_cast<uint32_t*>(地址 + 3);
        世界地址 = 地址 + 7 + disp - (uintptr_t)GetModuleHandleA("GameAssembly.dll");
    }
    uintptr_t 主播无后 = 特征码搜索("GameAssembly.dll", "48 8B C4 F3 0F 11 58 20 F3 0F 11 48 10 55 53 48 8D 68 A9", 0, false);
    uintptr_t 超级无后 = 特征码搜索("GameAssembly.dll", "40 53 48 83 EC 60 80 3D ?? ?? ?? ?? ?? 48 8B D9 75 37 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 43 20", 0, false);
    uintptr_t 无视缺氧 = 特征码搜索("GameAssembly.dll", "40 53 48 83 EC 20 80 3D ?? ?? ?? ?? 00 48 8B D9 75 13 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? 01 33 D2 48 8B CB E8 ?? ?? ?? ?? 84 C0 0F 85 ?? ?? ?? ?? 48 8B 43 20 48 85 C0", 0, false);
    uintptr_t 载具锁油 = 特征码搜索("GameAssembly.dll", "48 83 EC 28 83 79 38 01 74 0D F3 0F 10 05 ?? ?? ?? ?? 48 83 C4 28 C3 48 8B 81 ?? ?? ?? ?? 48 85 C0 74 0D F3 0F 10 80 ?? ?? ?? ?? 48 83 C4 28 C3 E8 ?? ?? ?? ?? CC CC CC CC CC CC CC CC CC CC CC 48 83 EC 28 83 79 38 01 74 07", 8, false);
    uintptr_t 子弹瞬击 = 特征码搜索("GameAssembly.dll", "F3 0F 11 4C 24 10 55 56 41 56 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 80 3D ?? ?? ?? ?? ?? 49 8B F0", 0, false);
    uintptr_t 无视火焰 = 特征码搜索("GameAssembly.dll", "57 48 83 EC 30 80 3D ?? ?? ?? ?? ?? 41 8B F8 8B F2 48 8B D9 75 13 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 33 D2 48 8B CB E8 ?? ?? ?? ?? 89 7C 24 24", 0, false);
    uintptr_t 无视雪球 = 特征码搜索("GameAssembly.dll", "40 53 57 41 57 48 83 EC 40 80 3D ?? ?? ?? ?? ?? 49 8B D8 48 8B FA", 0, false);
    uintptr_t 落地无僵 = 特征码搜索("GameAssembly.dll", "89 91 ?? ?? ?? ?? 85 D2 41 0F 95 C0 45 33 C9 41 8D 51 21 E9 ?? ?? ?? ??", 0, false);
    uintptr_t 枪械间隔 = 特征码搜索("GameAssembly.dll", "F3 0F 10 B0 14 01 00 00 0F", 0, false);


    if (世界地址) 数据::世界地址 = 世界地址;
    if (主播无后) 数据::主播无后 = 主播无后;
    if (超级无后) 数据::超级无后 = 超级无后;
    if (无视缺氧) 数据::无视缺氧 = 无视缺氧;
    if (载具锁油) 数据::载具锁油 = 载具锁油;
    if (子弹瞬击) 数据::子弹加速 = 子弹瞬击;
    if (无视火焰) 数据::无视火焰 = 无视火焰;
    if (无视雪球) 数据::无视雪球 = 无视雪球;
    if (落地无僵) 数据::落地无僵 = 落地无僵;
    if (枪械间隔) 数据::枪械间隔 = 枪械间隔;

    if (g_DebugMode) {
        printf("世界地址: GameAssembly.dll+0x%p\n", (void*)世界地址);
        printf("主播无后: GameAssembly.dll+0x%p\n", (void*)主播无后);
        printf("超级无后: GameAssembly.dll+0x%p\n", (void*)超级无后);
        printf("无视缺氧: GameAssembly.dll+0x%p\n", (void*)无视缺氧);
        printf("载具锁油: GameAssembly.dll+0x%p\n", (void*)载具锁油);
        printf("子弹瞬击: GameAssembly.dll+0x%p\n", (void*)子弹瞬击);
        printf("无视火焰: GameAssembly.dll+0x%p\n", (void*)无视火焰);
        printf("无视雪球: GameAssembly.dll+0x%p\n", (void*)无视雪球);
        printf("落地无僵: GameAssembly.dll+0x%p\n", (void*)落地无僵);
        printf("枪械间隔: GameAssembly.dll+0x%p\n", (void*)枪械间隔);
    }
}




// ====================== Present Hook 相关 ======================
using Present_t = HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc_Hook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    // HOME 开关菜单
    if (uMsg == WM_KEYDOWN && wParam == VK_HOME && !g_HomeKeyDown)
    {
        g_ShowMenu = !g_ShowMenu;
        g_HomeKeyDown = true;
        return 0;
    }
    if (uMsg == WM_KEYUP && wParam == VK_HOME)
    {
        g_HomeKeyDown = false;
    }
    if (g_ShowMenu && ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return 1;
    return CallWindowProc(g_OriginalWndProc, hwnd, uMsg, wParam, lParam);
}

HRESULT __stdcall Present_Hook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    if (!g_GameImGuiInit)
    {
        if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice)))
            return ((Present_t)g_OriginalPresent)(pSwapChain, SyncInterval, Flags);
        g_pd3dDevice->GetImmediateContext(&g_pd3dContext);
        DXGI_SWAP_CHAIN_DESC sd = { 0 };
        pSwapChain->GetDesc(&sd);
        g_hGameWnd = sd.OutputWindow;

        ID3D11Texture2D* pBackBuffer = nullptr;
        if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer)))
        {
            g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
            pBackBuffer->Release();
        }
        // Hook窗口过程
        g_OriginalWndProc = (WNDPROC)SetWindowLongPtrW(g_hGameWnd, GWLP_WNDPROC, (LONG_PTR)WndProc_Hook);
        // ImGui初始化
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameRounding = 6.0f;
        style.WindowRounding = 8.0f;
        style.ChildRounding = 4.0f;
        style.PopupRounding = 5.0f;
        style.GrabRounding = 4.0f;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.ScrollbarPadding = 2.0f;
        style.ScrollbarRounding = 4.0f;
        style.ScrollbarSize = 10.0f;
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        ImFontConfig DrawFontSet;
        DrawFontSet.FontDataOwnedByAtlas = false;
        ImFont* Font = io.Fonts->AddFontFromMemoryCompressedTTF((void*)DrawFont, DrawFontSize, 16.0f, &DrawFontSet);
        if (!Font)
        {
            MessageBoxA(nullptr, "内置字体加载失败！加载微软雅黑字体", "错误", MB_ICONERROR);
            Font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
            if (!Font)
                MessageBoxA(nullptr, "微软雅黑字体加载失败", "错误", MB_ICONERROR);
        }
        ImGui_ImplWin32_Init(g_hGameWnd);
        ImGui::StyleColorsLight();
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dContext);
        printf("游戏内ImGui初始化成功\n");
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        g_GameImGuiInit = true;
    }

    if (本人数据::限制FPS) LimitFps(本人数据::FPS值);

    if (g_GameImGuiInit)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //CardKey_Menu(公告, 云端版本, "3.8");
		OverlayWin::DrawOverlayUI();// 绘制游戏内UI

        
        ImGui::Render();
        g_pd3dContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
    return ((Present_t)g_OriginalPresent)(pSwapChain, SyncInterval, Flags);
}

// 启动外置透明窗口线程
void StartOverlayWindowThread()
{
    std::thread(OverlayWin::OverlayWindowWork).detach();
}

// 主Hook工作线程
DWORD WINAPI Hook_Thread(LPVOID lpParam)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    SetConsoleTitle("欢迎使用TL！");


    int result = MessageBox(NULL, "请选择渲染模式 (是: 外部绘制, 否: 游戏内绘)", "渲染模式选择", MB_YESNO | MB_ICONQUESTION);
    if (result == IDYES) {
        g_UseExternalOverlay = true;
        本人数据::外绘 = true;
    }
    else {
        g_UseExternalOverlay = false;
        本人数据::外绘 = false;
    }


    system("cls");
    输出调试文本();
    特征码取地址();
    printf("\n初始化特征码完成...\n");
    读取配置();
    printf("读取保存的配置完成...\n");
    InitConfig();
    printf("配置美化完成...\n");
    // 等待GameAssembly加载
    HMODULE hGameAssembly = nullptr;
    for (int i = 0; i < 50; i++)
    {
        hGameAssembly = GetModuleHandleA("GameAssembly.dll");
        if (hGameAssembly) break;
        Sleep(20);
    }
    UnityResolve::Init(hGameAssembly, UnityResolve::Mode::Il2Cpp);
    printf("Unity解析初始化完成\n");
    InitAllOffsets();
    printf("偏移初始化完成\n");
    HardBreakPoint::initialize();
    printf("硬件断点初始化完成\n");
    魔术_BulletControl::Init();
    printf("子弹逻辑初始化完成\n");
    WeaponControlHook::Init();
    printf("瞬击初始化完成\n");
    Camera_Controller::InitCoreModule();
    printf("自瞄模块初始化完成\n");
    if (g_DebugMode) PrintAllOffsets();

    // 启动独立顶层透明Overlay窗口
    if (g_UseExternalOverlay) {
        StartOverlayWindowThread();
        printf("外部窗口初始化完成\n");
		ShowWindow(GetConsoleWindow(), SW_HIDE);//隐藏控制台窗口

    }
// 创建临时D3D设备查找Present（仅内绘模式开启Hook）
    if (!g_UseExternalOverlay)
    {
        const UINT uiFeatureLevelsCount = 2;
        D3D_FEATURE_LEVEL FeatureLevels[uiFeatureLevelsCount] = {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0
        };
        DXGI_SWAP_CHAIN_DESC sd = { 0 };
        sd.BufferCount = 1;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = GetForegroundWindow();
        sd.SampleDesc.Count = 1;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        ID3D11Device* pTempDevice = nullptr;
        IDXGISwapChain* pTempSwapChain = nullptr;
        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            FeatureLevels, uiFeatureLevelsCount, D3D11_SDK_VERSION,
            &sd, &pTempSwapChain, &pTempDevice, nullptr, nullptr
        );
        if (SUCCEEDED(hr) && pTempSwapChain)
        {
            void*** pVTable = (void***)pTempSwapChain;
            void* pPresentFunc = (*pVTable)[8];
            if (MH_Initialize() == MH_OK)
            {
                printf("MinHook初始化成功\n");
                if (MH_CreateHook(pPresentFunc, Present_Hook, &g_OriginalPresent) == MH_OK)
                {
                    printf("Present Hook创建完成\n");
                    MH_EnableHook(pPresentFunc);
                }
            }
            pTempDevice->Release();
            pTempSwapChain->Release();
        }
    }

    // 循环等待退出标记
    while (true)
    {
        if (本人数据::安全退出)
        {
            printf("退出辅助...\n");
            //关闭功能
            写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::主播无后, { 0x48,0x8B,0xC4 });
            写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::超级无后, { 0x40,0x53 });
            写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::无视缺氧, { 0x40,0x53 });
            写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::载具锁油, { 0x74,0x0D });
            写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::无视雪球, { 0x40,0x53 });
            写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::无视火焰, { 0x57 });
            关闭追踪();
            关闭瞬击();
            关闭无僵();
            关闭间隔();
            g_ThreadQuitFlag = true; // 通知心跳线程退出
            exit(0);
            return 0;
        }
        Sleep(100);
    }
}

// ====================== DLL入口 ======================
BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    SetConsoleTitle("欢迎使用TL!");

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, Hook_Thread, hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        g_ThreadQuitFlag = true;
        break;
    }
    return TRUE;
}