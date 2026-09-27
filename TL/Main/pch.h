#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define GLM_ENABLE_EXPERIMENTAL
#define USE_GLM

#include <Windows.h>
#include <iostream>
#include <cstring>
#include <cstdint>
#include <vector>
#include <string>
#include <tchar.h>
#include <dwmapi.h>
//library
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <UnityResolve.hpp>
#include <HardBreakPoint.h>

#include "..\Library\imgui\imconfig.h"
#include "..\Library\imgui\imgui.h"
#include "..\Library\imgui\imgui_impl_dx11.h"
#include "..\Library\imgui\imgui_impl_win32.h"
#include "..\Library\imgui\imgui_internal.h"
#include "..\Library\imgui\imstb_rectpack.h"
#include "..\Library\imgui\imstb_textedit.h"
#include "..\Library\imgui\imstb_truetype.h"

#include "..\minhook\include\MinHook.h"
#include "myimgui.h"          // 透明外置窗口头文件
#include "..\Library\WeiYan\WeiYanApi.h" // 卡密验证接口头文件
#include "..\Library\WeiYan\json.hpp"    // JSON解析头文件
#include <d3d11.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"minhook.x64d.lib" )

std::string 取程序运行时间_文本();
std::string 时间_计算相差时间(long long expireTimestampSec);
std::string timestampToLocalTime(long long timestamp);
void 启动计时();
