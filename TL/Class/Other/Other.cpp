//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include <Windows.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

#include "Other.h"
#include "..\..\GUI\GUI.h"

std::vector<unsigned char> 读字节集(uintptr_t targetAddr, size_t readLen)
{
	std::vector<unsigned char> result;
	// 非法参数直接返回空
	if (readLen == 0 || targetAddr == 0)
		return result;

	void* srcAddr = reinterpret_cast<void*>(targetAddr);
	result.resize(readLen); // 预分配存储字节的空间
	DWORD oldProtect = 0;

	// 修改内存权限，保证可读（和写入统一使用 PAGE_EXECUTE_READWRITE 兼容代码段/数据段）
	if (!VirtualProtect(srcAddr, readLen, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		result.clear();
		return result;
	}

	// 读取内存到vector缓冲区
	memcpy(result.data(), srcAddr, readLen);

	// 恢复原本内存保护属性
	VirtualProtect(srcAddr, readLen, oldProtect, &oldProtect);

	// 读取不需要刷新指令缓存（FlushInstructionCache 仅写入机器码时用）
	return result;
}

bool 写字节集(uintptr_t targetAddr, const std::vector<unsigned char>& byteVec) {
	if (byteVec.empty() || targetAddr == 0) return false;

	DWORD oldProtect = 0;
	// 转目标地址为void*（配合memcpy）
	void* dest = reinterpret_cast<void*>(targetAddr);
	// 源地址：字节集的起始地址
	const void* src = byteVec.data();
	// 要拷贝的字节数
	size_t copySize = byteVec.size();

	// 1. 修改内存权限（必须）
	if (!VirtualProtect(dest, copySize, PAGE_EXECUTE_READWRITE, &oldProtect)) {
		return false;
	}

	// 2. 核心：用memcpy写入字节集
	memcpy(dest, src, copySize);
	// 刷新缓存（确保CPU立即生效）
	FlushInstructionCache(GetCurrentProcess(), dest, copySize);

	// 3. 恢复权限
	VirtualProtect(dest, copySize, oldProtect, &oldProtect);
	return true;
}

uintptr_t 申请内存(uintptr_t addr, SIZE_T size)
{
	LPVOID pAllocatedMem = NULL;

	// 原逻辑：无指定地址 → 系统自动分配
	if (addr == 0)
	{
		pAllocatedMem = VirtualAlloc(
			NULL,
			size,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_EXECUTE_READWRITE
		);
		return (uintptr_t)pAllocatedMem;
	}

	// 原逻辑：±1GB 扫描范围（一丝不动）
	uintptr_t scanStart = addr - 0x40000000;
	uintptr_t scanEnd = addr + 0x40000000;

	// 原逻辑：4KB 步长 + 同进程 VirtualAlloc（关键修复：换回 VirtualAlloc）
	for (uintptr_t tryAddr = scanStart; tryAddr <= scanEnd; tryAddr += 0x1000)
	{
		pAllocatedMem = VirtualAlloc(
			(LPVOID)tryAddr,       // 强制指定目标地址
			size,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_EXECUTE_READWRITE  // 原权限不变
		);

		if (pAllocatedMem != NULL) {
			break;
		}
	}

	return (uintptr_t)pAllocatedMem;
}

uintptr_t 特征码搜索(const char* 模块名, const char* 字节数组, int 偏移,bool 加入模块) {
	auto pattern_to_byte = [](const char* pattern) -> std::vector<int> {
		std::vector<int> bytes;
		const char* current = pattern;
		size_t len = strlen(pattern);

		while (current < pattern + len) {
			if (*current == ' ') {
				current++;
				continue;
			}
			if (*current == '?') {
				bytes.push_back(-1);
				if (*(current + 1) == '?')
					current++;
				current++;
			}
			else {
				char* end = nullptr;
				unsigned long val = strtoul(current, &end, 16);
				bytes.push_back((int)val);
				current = end;
			}
		}
		return bytes;
		};

	uintptr_t moduleBase = (uintptr_t)GetModuleHandleA(模块名);
	if (!moduleBase)
		return 0;

	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)moduleBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((uint8_t*)moduleBase + pDos->e_lfanew);
	uintptr_t moduleSize = pNt->OptionalHeader.SizeOfImage;

	std::vector<int> patternBytes = pattern_to_byte(字节数组);
	uint8_t* pScan = (uint8_t*)moduleBase;
	size_t patternLen = patternBytes.size();
	const int* pPattern = patternBytes.data();

	for (size_t i = 0; i < moduleSize - patternLen; i++) {
		bool found = true;
		for (size_t j = 0; j < patternLen; j++) {
			if (pPattern[j] == -1)
				continue;
			if (pScan[i + j] != pPattern[j]) {
				found = false;
				break;
			}
		}

		if (found) {
			uintptr_t addr = (uintptr_t)&pScan[i] + 偏移;
			if (加入模块 == false) {
				addr -= moduleBase;
			}

			return addr;
		}
	}

	return 0;
}

Matrix 获取矩阵(){
	uintptr_t 临时 = 0;
	__try {
		临时 = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(GetModuleHandle("UnityPlayer.dll")) + 数据::矩阵地址);
		if (!临时) { return Matrix{}; }
		临时 = *reinterpret_cast<uintptr_t*>(临时 + 数据::矩阵偏移[0]);
		if (!临时) { return Matrix{}; }
		临时 = *reinterpret_cast<uintptr_t*>(临时 + 数据::矩阵偏移[1]);
		if (!临时) { return Matrix{}; }
		临时 = *reinterpret_cast<uintptr_t*>(临时 + 数据::矩阵偏移[2]);
		if (!临时) { return Matrix{}; }
		临时 = *reinterpret_cast<uintptr_t*>(临时 + 数据::矩阵偏移[3]);
		if (!临时) { return Matrix{}; }

	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return Matrix{};
	}

	Matrix 返回;
	float* 遍历 = nullptr;
	__try {
		遍历 = reinterpret_cast<float*>(临时 + 数据::矩阵偏移[4]);
		if (!遍历) { return Matrix{}; }
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return Matrix{};
	}

	// Row 1
	返回._11 = 遍历[0];
	返回._12 = 遍历[1];
	返回._13 = 遍历[2];
	返回._14 = 遍历[3];

	// Row 2
	返回._21 = 遍历[4];
	返回._22 = 遍历[5];
	返回._23 = 遍历[6];
	返回._24 = 遍历[7];

	// Row 3
	返回._31 = 遍历[8];
	返回._32 = 遍历[9];
	返回._33 = 遍历[10];
	返回._34 = 遍历[11];

	返回._41 = 遍历[12];  // 本人坐标.x × -1
	返回._42 = 遍历[13];  // 本人坐标坐标.y × -1
	返回._43 = 遍历[14];
	返回._44 = 遍历[15];
	return 返回;
}

uintptr_t 取数组入口() {
	uintptr_t 临时 = 0;
	临时 = *reinterpret_cast<uintptr_t*>(Start_Game::Uworld);
	if (!临时) return 0;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + 0xB8);
	if (!临时) return 0;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + Start_Game::GameData);
	if (!临时) return 0;
	return 临时;
}

int 取敌我距离(const D3D坐标& 本人坐标, const D3D坐标& 对象坐标) {
	double a = 本人坐标.x - 对象坐标.x;
	double b = 本人坐标.y - 对象坐标.y;
	double h = 本人坐标.z - 对象坐标.z;
	double c = std::sqrt(a * a + b * b);
	double c2 = std::sqrt(c * c + h * h);
	return static_cast<int>(c2);
}

int 取准星距离(int 准星X,int 准星Y,int 对象X,int 对象Y) {
	double a = 准星X - 对象X;
	double b = 准星Y - 对象Y;
	double c = std::sqrt(a * a + b * b);
	return static_cast<int>(c);
}

BOOL WorldToScreenBox(VectorBox& ScreenPos, D3D坐标 WorldPos, Matrix MatrixView, D2D坐标 GameScreen)
{
	FLOAT CameraZ = MatrixView._14 * WorldPos.x + MatrixView._24 * WorldPos.y + MatrixView._34 * WorldPos.z + MatrixView._44;
	if (CameraZ <= 0.01)
		return FALSE;
	CameraZ = 1 / CameraZ;
	ScreenPos.x = GameScreen.x + (MatrixView._11 * WorldPos.x + MatrixView._21 * (WorldPos.y + 0.24) + MatrixView._31 * WorldPos.z + MatrixView._41) * CameraZ * GameScreen.x;
	ScreenPos.y = GameScreen.y - (MatrixView._12 * WorldPos.x + MatrixView._22 * (WorldPos.y + 2) + MatrixView._32 * WorldPos.z + MatrixView._42) * CameraZ * GameScreen.y;
	ScreenPos.y1 = GameScreen.y - (MatrixView._12 * WorldPos.x + MatrixView._22 * (WorldPos.y - 0.5) + MatrixView._32 * WorldPos.z + MatrixView._42) * CameraZ * GameScreen.y;

	return TRUE;
}

BOOL D3D转2D坐标(D3D坐标 WorldPos, D2D坐标& ScreenPos, Matrix MatrixView, D2D坐标 GameScreen)
{
	FLOAT CameraZ = MatrixView._14 * WorldPos.x + MatrixView._24 * WorldPos.y + MatrixView._34 * WorldPos.z + MatrixView._44;
	if (CameraZ <= 0.01)
		return FALSE;
	CameraZ = 1 / CameraZ;
	ScreenPos.x = GameScreen.x + (MatrixView._11 * WorldPos.x + MatrixView._21 * WorldPos.y + MatrixView._31 * WorldPos.z + MatrixView._41) * CameraZ * GameScreen.x;
	ScreenPos.y = GameScreen.y - (MatrixView._12 * WorldPos.x + MatrixView._22 * WorldPos.y + MatrixView._32 * WorldPos.z + MatrixView._42) * CameraZ * GameScreen.y;
	return TRUE;
}

BOOL D3D转方框坐标(VectorBox& ScreenPos, D3D坐标 WorldPos, Matrix MatrixView, D2D坐标 GameScreen, float 顶部微调, float 底部微调)
{
	FLOAT CameraZ = MatrixView._14 * WorldPos.x + MatrixView._24 * WorldPos.y + MatrixView._34 * WorldPos.z + MatrixView._44;
	if (CameraZ <= 0.01)
		return FALSE;
	CameraZ = 1 / CameraZ;
	ScreenPos.x = GameScreen.x + (MatrixView._11 * WorldPos.x + MatrixView._21 * WorldPos.y + MatrixView._31 * WorldPos.z + MatrixView._41) * CameraZ * GameScreen.x;
	ScreenPos.y = GameScreen.y - (MatrixView._12 * WorldPos.x + MatrixView._22 * (WorldPos.y + 底部微调) + MatrixView._32 * WorldPos.z + MatrixView._42) * CameraZ * GameScreen.y;
	ScreenPos.y1 = GameScreen.y - (MatrixView._12 * WorldPos.x + MatrixView._22 * (WorldPos.y + 顶部微调) + MatrixView._32 * WorldPos.z + MatrixView._42) * CameraZ * GameScreen.y;

	return TRUE;
}

void 鼠标自瞄(D3D坐标 WorldPos, Matrix 矩阵, D2D坐标 GameScreen){
	if (WorldPos.x == 0 || WorldPos.y == 0 || WorldPos.z == 0) return;
	D2D坐标 ScreenPos = { 0,0 };
	FLOAT CameraZ = 矩阵._14 * WorldPos.x + 矩阵._24 * WorldPos.y + 矩阵._34 * WorldPos.z + 矩阵._44;
	CameraZ = 1 / CameraZ;
	ScreenPos.x = GameScreen.x + (矩阵._11 * WorldPos.x + 矩阵._21 * WorldPos.y  + 矩阵._31 * WorldPos.z + 矩阵._41) * CameraZ * GameScreen.x;
	ScreenPos.y = GameScreen.y - (矩阵._12 * WorldPos.x + 矩阵._22 * (WorldPos.y+自瞄::位置)  + 矩阵._32 * WorldPos.z + 矩阵._42) * CameraZ * GameScreen.y;
	绘制直线(GameScreen.x, GameScreen.y, ScreenPos.x, ScreenPos.y, IM_COL32(255, 255, 255, 255), 1);
	
	if (ScreenPos.x< ScreenPos.x + 自瞄::自瞄范围 && ScreenPos.x> ScreenPos.x - 自瞄::自瞄范围) {
		if (ScreenPos.y< GameScreen.y + 自瞄::自瞄范围 && ScreenPos.y> GameScreen.y - 自瞄::自瞄范围){
			mouse_event(MOUSEEVENTF_MOVE, (ScreenPos.x - GameScreen.x)*自瞄::鼠标自瞄::自瞄速度  / 30,(ScreenPos.y - GameScreen.y)*自瞄::鼠标自瞄::自瞄速度 / 30,0, 0);

		}
	}
}

void 内存自瞄(D3D坐标 WorldPos, Matrix 矩阵, float 平滑) {
	float 临时坐标缓存[4] = { 0.0f };
	D3D坐标 相机坐标 = {};
	static uint64_t 上次_本人地址 = 0;
	if (本人数据::本人地址 != 上次_本人地址){
		上次_本人地址 = 本人数据::本人地址;
		Camera_Controller::g_pCamObj = NULL;
	}
	if (!Camera_Controller::g_pCamObj) Camera_Controller::InitCoreModule();

	相机坐标 = (*reinterpret_cast<UnityResolve::UnityType::Transform**>((uintptr_t)Camera_Controller::g_pCamObj + Camera_Controller::MyCameraTran))->GetPosition();

	if (fabs(WorldPos.x) < 0.001f && fabs(WorldPos.y) < 0.001f && fabs(WorldPos.z) < 0.001f)return;
	if (WorldPos.x == 0 && WorldPos.y == 0 && WorldPos.z == 0) return;

	try
	{
		// 计算相机指向敌人的欧拉角
		glm::vec2 瞄准欧拉角 = calculate_angles(相机坐标, WorldPos);

		// 转换为游戏专用的四元数
		auto 目标四元数 = calc_quat({ 瞄准欧拉角.x, 瞄准欧拉角.y });

		// 获取相机旋转的内存地址
		glm::quat* X轴旋转地址 = reinterpret_cast<glm::quat*>(Camera_Controller::g_pCamObj + Camera_Controller::MyCameraRotationX);
		glm::quat* Y轴旋转地址 = reinterpret_cast<glm::quat*>(Camera_Controller::g_pCamObj + Camera_Controller::MyCameraRotationY);

		// 读取相机当前旋转状态
		glm::quat 当前X旋转 = *X轴旋转地址;
		glm::quat 当前Y旋转 = *Y轴旋转地址;

		// 计算平滑插值速度
		float 插值步长 = glm::clamp(平滑 / 10.0f, 0.05f, 1.0f);

		// 平滑旋转计算
		glm::quat 最终X旋转 = glm::slerp(当前X旋转, 目标四元数.first, 插值步长);
		glm::quat 最终Y旋转 = glm::slerp(当前Y旋转, 目标四元数.second, 插值步长);


		if (glm::dot(最终X旋转, 目标四元数.first) > 0.99999f) 最终X旋转 = 目标四元数.first;
		if (glm::dot(最终Y旋转, 目标四元数.second) > 0.99999f) 最终Y旋转 = 目标四元数.second;


		DWORD 旧内存权限;
		// 写入X轴旋转
		if (VirtualProtect(X轴旋转地址, sizeof(glm::quat), PAGE_READWRITE, &旧内存权限))
		{
			*X轴旋转地址 = 最终X旋转;
			VirtualProtect(X轴旋转地址, sizeof(glm::quat), 旧内存权限, &旧内存权限);
		}
		// 写入Y轴旋转
		if (VirtualProtect(Y轴旋转地址, sizeof(glm::quat), PAGE_READWRITE, &旧内存权限))
		{
			*Y轴旋转地址 = 最终Y旋转;
			VirtualProtect(Y轴旋转地址, sizeof(glm::quat), 旧内存权限, &旧内存权限);
		}
	}
	catch (...)
	{

	}
}


D2D坐标 取瞄准位置(D3D坐标 WorldPos, Matrix 矩阵, D2D坐标 GameScreen) {
	if (WorldPos.x == 0 || WorldPos.y == 0 || WorldPos.z == 0) return {};
	D2D坐标 ScreenPos = { 0,0 };
	FLOAT CameraZ = 矩阵._14 * WorldPos.x + 矩阵._24 * WorldPos.y + 矩阵._34 * WorldPos.z + 矩阵._44;
	CameraZ = 1 / CameraZ;
	ScreenPos.x = GameScreen.x + (矩阵._11 * WorldPos.x + 矩阵._21 * WorldPos.y + 矩阵._31 * WorldPos.z + 矩阵._41) * CameraZ * GameScreen.x;
	ScreenPos.y = GameScreen.y - (矩阵._12 * WorldPos.x + 矩阵._22 * (WorldPos.y + 自瞄::位置) + 矩阵._32 * WorldPos.z + 矩阵._42) * CameraZ * GameScreen.y;
	return ScreenPos;
}


std::string 取角色名称(uintptr_t 玩家地址){
	if (!玩家地址)return "";
	char playerName[64] = "";
	uintptr_t namePtr1 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::NickName);
	if (namePtr1) {

		const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(namePtr1 + 0x14);
		if (nameWide) {
			char nameBuffer[64];
			int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
			if (length > 0) {
					strncpy_s(playerName, nameBuffer, sizeof(playerName) - 1);
			}
		}
	}
	char 真人名称[64];
	strncpy_s(真人名称, playerName, sizeof(真人名称) - 1);
	真人名称[sizeof(真人名称) - 1] = '\0';
	return 真人名称;
}

std::string 取角色ID(uintptr_t 玩家地址) {
	if (!玩家地址)return "";
	char playerName[64] = "";
	uintptr_t namePtr1 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::RoleID);
	if (namePtr1) {

		const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(namePtr1 + 0x14);
		if (nameWide) {
			char nameBuffer[64];
			int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
			if (length > 0) {
				strncpy_s(playerName, nameBuffer, sizeof(playerName) - 1);
			}
		}
	}
	char 文本[64];
	strncpy_s(文本, playerName, sizeof(文本) - 1);
	文本[sizeof(文本) - 1] = '\0';
	return 文本;
}

std::string 取角色手持(uintptr_t 玩家地址) {
	if (!玩家地址)return "";
	char 手持武器 [64] = "";
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::Weapon);
	if (临时) {
		临时 = *reinterpret_cast<uintptr_t*>(临时 + AbsPickItemNet::ItemName);
		if (临时){
			const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(临时 + 0x14);
			if (nameWide) {
				char nameBuffer[64];
				int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
				if (length > 0) {
					strncpy_s(手持武器, nameBuffer, sizeof(手持武器) - 1);
				}
			}
		}
	}
	char 武器名字[64];
	strncpy_s(武器名字, 手持武器, sizeof(武器名字) - 1);
	武器名字[sizeof(武器名字) - 1] = '\0';
	return 武器名字;
}

std::string 取角色手持英文(uintptr_t 玩家地址) {
	if (!玩家地址)return "";
	char 手持武器[64] = "";
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::Weapon);
	if (临时) {
		临时 = *reinterpret_cast<uintptr_t*>(临时 + AbsPickItemNet::ItemSign);
		if (临时) {
			const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(临时 + 0x14);
			if (nameWide) {
				char nameBuffer[64];
				int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
				if (length > 0) {
					strncpy_s(手持武器, nameBuffer, sizeof(手持武器) - 1);
				}
			}
		}
	}
	char 武器名字[64];
	strncpy_s(武器名字, 手持武器, sizeof(武器名字) - 1);
	武器名字[sizeof(武器名字) - 1] = '\0';
	return 武器名字;
}

std::string 取人机名称(uintptr_t AI地址) {
	if (!AI地址)return "";
	char playerName[64] = "";
	uintptr_t namePtr1 = *reinterpret_cast<uintptr_t*>(AI地址 + RoleAIManager::Name);
	if (namePtr1) {

		const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(namePtr1 + 0x14);
		if (nameWide) {
			char nameBuffer[64];
			int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
			if (length > 0) {
				strncpy_s(playerName, nameBuffer, sizeof(playerName) - 1);
			}
		}
	}
	char 人机名称[64];
	strncpy_s(人机名称, playerName, sizeof(人机名称) - 1);
	人机名称[sizeof(人机名称) - 1] = '\0';
	return 人机名称;
}

std::string 取载具名称(uintptr_t 载具地址) {
	if (!载具地址)return "";
	char Name[64] = "";
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(载具地址 + AllCar::Mirror);
	if (临时) {
		临时 = *reinterpret_cast<uintptr_t*>(临时 + AllCar::Name);
		if (临时) {
			const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(临时 + 0x14);
			if (nameWide) {
				char nameBuffer[64];
				int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
				if (length > 0) {
					strncpy_s(Name, nameBuffer, sizeof(Name) - 1);
				}
			}
		}
	}
	char 载具名称[64];
	strncpy_s(载具名称, Name, sizeof(载具名称) - 1);
	载具名称[sizeof(载具名称) - 1] = '\0';
	return 载具名称;
}

std::string 取物资中文名(uintptr_t 物资地址) {
	if (!物资地址)return "";
	char playerName[64] = "";
	uintptr_t namePtr1 = *reinterpret_cast<uintptr_t*>(物资地址 + AbsPickItemNet::ItemName);
	if (namePtr1) {
		const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(namePtr1 + 0x14);
		if (nameWide) {
			char nameBuffer[64];
			int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
			if (length > 0) {
				strncpy_s(playerName, nameBuffer, sizeof(playerName) - 1);
			}
		}
	}
	char 物资名称[64];
	strncpy_s(物资名称, playerName, sizeof(物资名称) - 1);
	物资名称[sizeof(物资名称) - 1] = '\0';
	return 物资名称;
}

std::string 取载具中文名(const std::string& 英文名称) {
	if (英文名称 == "SwordTiger") {
		return GBK转UTF8("小脑斧");
	}
	if (英文名称 == "FlyingBroom") {
		return GBK转UTF8("哈利波特扫帚");
	}
	else if (英文名称 == "Dragon") {
		return GBK转UTF8("咆哮恶龙");
	}
	else if (英文名称 == "Triceratops") {
		return GBK转UTF8("奶龙");
	}
	else if (英文名称 == "TRexKing") {
		return GBK转UTF8("烧油龙王");
	}
	else if (英文名称 == "Kayak") {
		return GBK转UTF8("泰坦尼克号");
	}
	else if (英文名称 == "Raptors") {
		return GBK转UTF8("迅猛龙");
	}
	else if (英文名称 == "Peterosaur") {
		return GBK转UTF8("鸟");
	}
	else if (英文名称 == "UFO") {
		return GBK转UTF8("逆碟");
	}
	else if (英文名称 == "ArmoredBus") {
		return GBK转UTF8("宝宝巴士");
	}
	else if (英文名称 == "Machine_Carrier") {
		return GBK转UTF8("坤甲");
	}
	else if (英文名称 == "JetCar") {
		return GBK转UTF8("极品飞车");
	}
	else if (英文名称 == "Jeep") {
		return GBK转UTF8("吉普");
	}
	else if (英文名称 == "Buggy") {
		return GBK转UTF8("三蹦子");
	}
	else if (英文名称 == "ShenLong") {
		return GBK转UTF8("顶流龙帝");
	}
	else if (英文名称 == "PonyVehicle") {
		return GBK转UTF8("泥马");
	}
	else if (英文名称 == "HoverBoard") {
		return GBK转UTF8("滑板");
	}
	else if (英文名称 == "NeptuneShark") {
		return GBK转UTF8("鲨鱼");
	}
	else if (英文名称 == "CyberTitan") {
		return GBK转UTF8("大刀坤甲");
	}
	else if (英文名称 == "SheepVehicle") {
		return GBK转UTF8("白鸟竞速版");
	}
	else {
		return 英文名称; // 无匹配则返回原英文
	}
}

std::string 取头盔名字(uintptr_t 玩家地址) {
	if (!玩家地址)return "";
	char Name[64] = "";
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::HeadEquipPart);
	if (临时) {
		临时 = *reinterpret_cast<uintptr_t*>(临时 + AbsPickItemNet::ItemName);
		if (临时) {
			const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(临时 + 0x14);
			if (nameWide) {
				char nameBuffer[64];
				int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
				if (length > 0) {
					strncpy_s(Name, nameBuffer, sizeof(Name) - 1);
				}
			}
		}
	}
	char 头盔名字[64];
	strncpy_s(头盔名字, Name, sizeof(头盔名字) - 1);
	头盔名字[sizeof(头盔名字) - 1] = '\0';
	return 头盔名字;
}

std::string 取护甲名字(uintptr_t 玩家地址) {
	if (!玩家地址)return "";
	char Name[64] = "";
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::BodyEquipPart);
	if (临时) {
		临时 = *reinterpret_cast<uintptr_t*>(临时 + AbsPickItemNet::ItemName);
		if (临时) {
			const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(临时 + 0x14);
			if (nameWide) {
				char nameBuffer[64];
				int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
				if (length > 0) {
					strncpy_s(Name, nameBuffer, sizeof(Name) - 1);
				}
			}
		}
	}
	char 护甲名字[64];
	strncpy_s(护甲名字, Name, sizeof(护甲名字) - 1);
	护甲名字[sizeof(护甲名字) - 1] = '\0';
	return 护甲名字;
}

std::string 取背包名字(uintptr_t 玩家地址) {
	if (!玩家地址)return "";
	char Name[64] = "";
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::PackEquip);
	if (临时) {
		临时 = *reinterpret_cast<uintptr_t*>(临时 + AbsPickItemNet::ItemName);
		if (临时) {
			const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(临时 + 0x14);
			if (nameWide) {
				char nameBuffer[64];
				int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
				if (length > 0) {
					strncpy_s(Name, nameBuffer, sizeof(Name) - 1);
				}
			}
		}
	}
	char 背包名字[64];
	strncpy_s(背包名字, Name, sizeof(背包名字) - 1);
	背包名字[sizeof(背包名字) - 1] = '\0';
	return 背包名字;
}

std::string 取身份卡名字(uintptr_t 玩家地址) {
	if (!玩家地址)return "";
	char Name[64] = "";
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::FunctionalGarmentEquipPart);
	if (临时) {
		临时 = *reinterpret_cast<uintptr_t*>(临时 + AbsPickItemNet::ItemName);
		if (临时) {
			const wchar_t* nameWide = reinterpret_cast<const wchar_t*>(临时 + 0x14);
			if (nameWide) {
				char nameBuffer[64];
				int length = WideCharToMultiByte(CP_UTF8, 0, nameWide, -1, nameBuffer, sizeof(nameBuffer), NULL, NULL);
				if (length > 0) {
					strncpy_s(Name, nameBuffer, sizeof(Name) - 1);
				}
			}
		}
	}
	char 身份卡名字[64];
	strncpy_s(身份卡名字, Name, sizeof(身份卡名字) - 1);
	身份卡名字[sizeof(身份卡名字) - 1] = '\0';
	return 身份卡名字;
}

int 获取头盔等级(const char* name){
	if (strcmp(name, "顶级头盔") == 0) return 5;
	if (strcmp(name, "4级头盔") == 0) return 4;
	if (strcmp(name, "3级头盔") == 0) return 3;
	if (strcmp(name, "3级头盔-侦察大师") == 0) return 3;
	if (strcmp(name, "3级头盔-霰弹勇士") == 0) return 3;
	if (strcmp(name, "2级头盔") == 0) return 2;
	if (strcmp(name, "1级头盔") == 0) return 1;
	if (strcmp(name, "") == 0) return 0;
	return -1;
}

int 获取护甲等级(const char* name) {
	if (strcmp(name, "顶级防弹衣") == 0) return 5;
	if (strcmp(name, "4级防弹衣") == 0) return 4;
	if (strcmp(name, "3级防弹衣") == 0) return 3;
	if (strcmp(name, "2级防弹衣") == 0) return 2;
	if (strcmp(name, "1级防弹衣") == 0) return 1;
	if (strcmp(name, "") == 0) return 0;
	return -1;
}
int 获取背包等级(const char* name){
	if (strcmp(name, "4级背包") == 0) return 4;
	if (strcmp(name, "3级背包") == 0) return 3;
	if (strcmp(name, "2级背包") == 0) return 2;
	if (strcmp(name, "1级背包") == 0) return 1;
	if (strcmp(name, "") == 0) return 0;
	return -1;
}

ImU32 取队伍颜色(UINT32 编号) {
	switch (编号) {
	case 1:  return 橙黄;
	case 2:  return 藏青;
	case 3:  return 墨绿;
	case 4:  return 红褐;
	case 5:  return 紫红;
	case 6:  return 褐绿;
	case 7:  return 蓝色;
	case 8:  return 绿色;
	case 9:  return 艳青;
	case 10: return 红色;
	case 11: return 品红;
	case 12: return 黄色;
	case 13: return 桃红;
	case 14: return 蓝灰;
	case 15: return 藏蓝;
	case 16: return 嫩绿;
	case 17: return 青绿;
	case 18: return 黄褐;
	case 19: return 粉红;
	case 20: return 嫩黄;
	case 21: return 芙红;
	case 22: return 紫色;
	case 23: return 天蓝;
	case 24: return 灰绿;
	case 25: return 青蓝;
	case 26: return 橙黄;
	case 27: return 藏青;
	case 28: return 墨绿;
	case 29: return 红褐;
	case 30: return 紫红;
	case 31: return 褐绿;
	case 32: return 蓝色;
	case 33: return 绿色;
	case 34: return 艳青;
	case 35: return 红色;
	case 36: return 品红;
	case 37: return 黄色;
	case 38: return 桃红;
	case 39: return 蓝灰;
	case 40: return 藏蓝;
	case 41: return 嫩绿;
	case 42: return 青绿;
	case 43: return 黄褐;
	case 44: return 粉红;
	case 45: return 嫩黄;
	case 46: return 芙红;
	case 47: return 紫色;
	case 48: return 天蓝;
	case 49: return 灰绿;
	case 50: return 青蓝;
	case 51: return 橙黄;
	case 52: return 藏青;
	case 53: return 墨绿;
	case 54: return 红褐;
	case 55: return 紫红;
	case 56: return 褐绿;
	case 57: return 蓝色;
	case 58: return 绿色;
	case 59: return 艳青;
	case 60: return 红色;
	case 61: return 品红;
	case 62: return 黄色;
	case 63: return 桃红;
	case 64: return 蓝灰;
	case 65: return 藏蓝;
	case 66: return 嫩绿;
	case 67: return 青绿;
	case 68: return 黄褐;
	case 69: return 粉红;
	case 70: return 嫩黄;
	case 71: return 芙红;
	case 72: return 紫色;
	case 73: return 天蓝;
	case 74: return 灰绿;
	case 75: return 青蓝;
	case 76: return 橙黄;
	case 77: return 藏青;
	case 78: return 墨绿;
	case 79: return 红褐;
	case 80: return 紫红;
	case 81: return 褐绿;
	case 82: return 蓝色;
	case 83: return 绿色;
	case 84: return 艳青;
	case 85: return 红色;
	case 86: return 品红;
	case 87: return 黄色;
	case 88: return 桃红;
	case 89: return 蓝灰;
	case 90: return 藏蓝;
	case 91: return 嫩绿;
	case 92: return 青绿;
	case 93: return 黄褐;
	case 94: return 粉红;
	case 95: return 嫩黄;
	case 96: return 芙红;
	case 97: return 紫色;
	case 98: return 天蓝;
	case 99: return 灰绿;
	default: return 天蓝;
	}
}

bool 判断是否倒地(uintptr_t 玩家地址) {
	if (!玩家地址)return false;
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::roleLogicClient);
	if (临时)临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);
	if (临时)临时 = *reinterpret_cast<ULONG32*>(临时 + BattleRole::tempIsWeak);
	if (临时==1) return true;
	else return false;
}

bool 判断是否死亡(uintptr_t 玩家地址) {
	if (!玩家地址)return true;
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::roleLogicClient);
	if(临时)临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);
	if (临时)临时 = *reinterpret_cast<ULONG32*>(临时 + 数据::是否死亡);
	if (临时 == 0) return true;
	else return false;
}

bool 判断是否存在(uintptr_t 玩家地址) {
	if (!玩家地址)return false;
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::roleLogicClient);
	if (临时)临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);
	if (临时)临时 = *reinterpret_cast<ULONG32*>(临时 + BattleRole::IsEventRoleShow);
	if (临时 == 0) return true;
	else return false;
}

bool 判断是否踩球(uintptr_t 玩家地址) {
	if (!玩家地址)return false;
	UINT64 临时 = *reinterpret_cast<UINT64*>(玩家地址 + BattleRoleLogic::UserCircusBallNet);
	if (临时 != 0) return true;
	else return false;
}

float 计算角度差(float a, float b)
{
	float diff = fabs(a - b);
	if (diff > 180.0f) diff = 360.0f - diff;
	return diff;
}
bool 判断是否被瞄(D3D坐标 本人坐标, D3D坐标 敌人坐标, D2D坐标 敌人朝向) {
	D3D坐标 d = {};
	本人坐标.y = 本人坐标.y + 1.5f; // 适配头部位置
	敌人坐标.y = 敌人坐标.y + 1.5f; // 适配头部位置
	d.x = 本人坐标.x - 敌人坐标.x;
	d.y = 本人坐标.y - 敌人坐标.y;
	d.z = 本人坐标.z - 敌人坐标.z;
	float desiredYaw = atan2(d.x, d.z) * 180.0f / 3.141592653589f;  // 水平角度
	if (desiredYaw < 0) desiredYaw += 360.0f;
	float horizontalDist = sqrt(d.x * d.x + d.z * d.z);
	float standardPitch = atan2(d.y, horizontalDist) * 180.0f / 3.14159265f;
	float desiredPitch;
	if (standardPitch > 0) desiredPitch = 360.0f - standardPitch;  // 向上
	else desiredPitch = -standardPitch;          // 向下

	float yawDiff = 计算角度差(敌人朝向.y, desiredYaw);
	float pitchDiff = 计算角度差(敌人朝向.x, desiredPitch);

	const float THRESHOLD = 6.0f;// 阈值，单位：度

	return (yawDiff < THRESHOLD) && (pitchDiff < THRESHOLD);
}

D2D坐标 取玩家朝向(uintptr_t 玩家地址) {
	if (!玩家地址)return {};
	D2D坐标 朝向 = {};
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::roleLogicClient);
	if (临时)临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);
	if (临时)朝向.y = *reinterpret_cast<float*>(临时 + BattleRole::lastRotaY);
	朝向.x = *reinterpret_cast<float*>(临时 + BattleRole::lastRotaY + 4);
	return 朝向;
}
//重点更新对象！！↓
UINT32 取蹲起状态(uintptr_t 玩家地址) {
	if (!玩家地址)return 0;
	return *reinterpret_cast<UINT32*>(玩家地址 + 0x218);
}

void HOOK_Jmp(uintptr_t 写入地址, uintptr_t 跳转地址){
	INT32 jmp_offset = (INT32)(跳转地址 - 写入地址 - 5);

	std::vector<unsigned char> jmpCode = {
		0xE9,
		(BYTE)(jmp_offset & 0xFF),
		(BYTE)((jmp_offset >> 8) & 0xFF),
		(BYTE)((jmp_offset >> 16) & 0xFF),
		(BYTE)((jmp_offset >> 24) & 0xFF)
	};

	写字节集(写入地址, jmpCode);
}

void 追踪初始化() {
	uintptr_t 追踪地址 = reinterpret_cast<uintptr_t>(GetModuleHandle("UnityPlayer.dll")) + 0x112E40;
	uintptr_t 申请地址 = 申请内存(追踪地址, 2048);
	INT32 偏移 = (INT32)((申请地址 + 0x50) - (申请地址 + 0x7));
	std::vector<unsigned char> 写入字节 = {
		0x48,
		0x8D,
		0x15,
		(BYTE)(偏移 & 0xFF),
		(BYTE)((偏移 >> 8) & 0xFF),
		(BYTE)((偏移 >> 16) & 0xFF),
		(BYTE)((偏移 >> 24) & 0xFF)
	};

	写字节集(申请地址, 写入字节);
	HOOK_Jmp((申请地址 + 0x7), 追踪地址 + 5);
	HOOK_Jmp(追踪地址, 申请地址);
	本人数据::追踪地址 = 申请地址;
}

void 追踪(D3D坐标 WorldPo) {
	float* x = reinterpret_cast<float*>(本人数据::追踪地址 + 0x50);
	float* y = reinterpret_cast<float*>(本人数据::追踪地址 + 0x54);
	float* z = reinterpret_cast<float*>(本人数据::追踪地址 + 0x58);
	* x = WorldPo.x;
	* y = WorldPo.y;
	* z = WorldPo.z;
}

void 关闭追踪() {
	if (本人数据::追踪地址 == 0) return;
	写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("UnityPlayer.dll")) + 0x112E40, { 0x48,0x89,0x5C,0x24,0x10 });
	VirtualFreeEx(GetCurrentProcess(), (LPVOID)本人数据::追踪地址, 0, MEM_RELEASE);
	本人数据::追踪地址 = NULL;
}

void 开关追踪(bool 条件) {
	static bool 上次条件 = false;

	if (条件 == true && 上次条件 == false) {
		追踪初始化();
		上次条件 = true;
	}
	else if (条件 == false && 上次条件 == true) {
		关闭追踪();
		上次条件 = false;
	}
}

void 子弹瞬击(const float 值) {
	uintptr_t 目标地址 = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll")) + 数据::子弹加速;
	uintptr_t 申请地址 = 申请内存(目标地址, 2048);
	if (!申请地址) return;
	unsigned char float_bytes[4];
	memcpy(float_bytes, &值, 4);
	std::vector<unsigned char> 注入代码 = {
		// --- mov eax, +值 ---
		0xB8, 
		float_bytes[0],// 第1个字节（小端序低位）
		float_bytes[1],// 第2个字节
		float_bytes[2],// 第3个字节
		float_bytes[3],// 第4个字节（小端序高位）
		// --- movd xmm1, eax ---
		0x66, 0x0F, 0x6E, 0xC8,
		// --- movss [rsp+10], xmm1 ---
		0xF3, 0x0F, 0x11, 0x4C, 0x24, 0x10
	};

	写字节集(申请地址, 注入代码);
	HOOK_Jmp((申请地址 + 15), 目标地址 + 6);
	HOOK_Jmp(目标地址, 申请地址);
	写字节集(目标地址 + 5, { 0x90 });
	本人数据::瞬击地址 = 申请地址;
}

void 关闭瞬击() {
	if (本人数据::瞬击地址 == 0) return;
	uintptr_t 目标地址 = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll")) + 数据::子弹加速;
	写字节集(目标地址, { 0xF3, 0x0F, 0x11, 0x4C, 0x24, 0x10 }); // 恢复原指令
	VirtualFreeEx(GetCurrentProcess(), (LPVOID)本人数据::瞬击地址, 0, MEM_RELEASE);
	本人数据::瞬击地址 = NULL;
}

std::vector<unsigned char> 无僵原偏移 = {};
void 落地无僵() {
	uintptr_t 目标地址 = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll")) + 数据::落地无僵;
	uintptr_t 申请地址 = 申请内存(目标地址, 2048);
	if (!申请地址) return;
	std::vector<unsigned char> 原偏移 = 读字节集(目标地址 + 2, 3);
	无僵原偏移 = 原偏移;
	std::vector<unsigned char> 注入代码 = {
		0x83, 0xFA, 0x05, // cmp edx, 5
		0x0F, 0x84, 0x06, 0x00, 0x00, 0x00, // je + 6
		0x89, 0x91
	};
	注入代码.insert(注入代码.end(), 原偏移.begin(), 原偏移.end()); // 添加原偏移的字节
	写字节集(申请地址, 注入代码);
	HOOK_Jmp((申请地址 + 15), 目标地址 + 6);
	HOOK_Jmp(目标地址, 申请地址);
	本人数据::无僵地址 = 申请地址;
}

void 关闭无僵() {
	if (本人数据::无僵地址 == 0) return;
	uintptr_t 目标地址 = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll")) + 数据::落地无僵;
	std::vector<unsigned char> 恢复代码 = { 0x89, 0x91 };// mov [rcx+ 读取原来的偏移], edx
	恢复代码.insert(恢复代码.end(), 无僵原偏移.begin(), 无僵原偏移.end()); // 添加原偏移的字节
	写字节集(目标地址, 恢复代码); // 恢复原指令
	VirtualFreeEx(GetCurrentProcess(), (LPVOID)本人数据::无僵地址, 0, MEM_RELEASE);
	本人数据::无僵地址 = NULL;
}

std::vector<unsigned char> 间隔原偏移 = {};
void 射击间隔(float 间隔) {
	uintptr_t 目标地址 = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll")) + 数据::枪械间隔;
	uintptr_t 申请地址 = 申请内存(目标地址, 2048);
	if (!申请地址) return;
	unsigned char float_bytes[4];
	memcpy(float_bytes, &间隔, 4);
	std::vector<unsigned char> 原偏移 = 读字节集(目标地址 + 4, 4);
	间隔原偏移 = 原偏移;
	std::vector<unsigned char> 注入代码 = { 0xC7, 0x80 };   // mov [rcx+偏移量]
	注入代码.insert(注入代码.end(), 原偏移.begin(), 原偏移.end()); // 插入动态偏移
	注入代码.insert(注入代码.end(), { float_bytes[0],float_bytes[1],float_bytes[2],float_bytes[3] });// 间隔的字节类型
	注入代码.insert(注入代码.end(), { 0xF3,0x0F,0x10,0xB0 });
	注入代码.insert(注入代码.end(), 原偏移.begin(), 原偏移.end()); // 插入相同动态偏移
	写字节集(申请地址, 注入代码);
	HOOK_Jmp((申请地址 + 18), 目标地址 + 8);
	HOOK_Jmp(目标地址, 申请地址);
	本人数据::间隔地址 = 申请地址;
}

void 关闭间隔() {
	if (本人数据::间隔地址 == 0) return;
	uintptr_t 目标地址 = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll")) + 数据::枪械间隔;
	std::vector<unsigned char> 恢复代码 = { 0xF3,0x0F,0x10,0xB0 };// movss xmm6,[rax+偏移量]
	恢复代码.insert(恢复代码.end(), 间隔原偏移.begin(), 间隔原偏移.end()); // 添加原偏移的字节
	写字节集(目标地址, 恢复代码); // 恢复原指令
	VirtualFreeEx(GetCurrentProcess(), (LPVOID)本人数据::间隔地址, 0, MEM_RELEASE);
	本人数据::间隔地址 = NULL;
}



//给功能加上快捷键函数，并且有按键锁，防止长按触发多次
void 功能开关(bool& 功能, int 功能快捷键) {
	static bool 按键锁[0xFF] = { false };
	if (!按键锁[功能快捷键] && GetAsyncKeyState(功能快捷键) & 0x8000) {
		功能 = !功能;       // 切换 true/false
		按键锁[功能快捷键] = true;   // 上锁，防止长按触发多次
	}
	if (!(GetAsyncKeyState(功能快捷键) & 0x8000)) {
		按键锁[功能快捷键] = false;  // 松开按键，解锁
	}
}

/*
//落地无僵
void AC_JumpStateHook::AC_JumpState_hook(AC_JumpStateHook* _this, AC_JumpState::Value _a) {
	if (内存::落地无僵) {
		if (_a != 5) {
			return HardBreakPoint::call_origin(AC_JumpState_hook, _this, _a);
		}
	}
	else {
		return HardBreakPoint::call_origin(AC_JumpState_hook, _this, _a);
	}
}
void AC_JumpStateHook::Init() {
	class_ = UnityResolve::Get("Assembly-CSharp.dll")->Get("BattleRoleLogic");
	class_->Get<UnityResolve::Method>("$Pc")->Cast(AC_JumpStateHook::AC_JumpState_init);
	HardBreakPoint::set_break_point(AC_JumpStateHook::AC_JumpState_init, AC_JumpState_hook);
}*/
//魔法子弹
void 魔术_BulletControl::local_role_weapon_init_hook(魔术_BulletControl* _bc,UnityResolve::UnityType::Vector3 _a,UnityResolve::UnityType::Vector3 _b) {
	if (内存::魔法子弹) {
		if (GetAsyncKeyState(自瞄::自瞄热键) != 0) {
			UnityResolve::UnityType::Vector3 final_pos = 本人数据::目标坐标;
			if (final_pos.x != 0) {
				final_pos.y += 0.71f;
				_a = final_pos;					// 起点
				_b = glm::vec3(0, -5, -0.71f);	// 终点
			}
		}
	}
	HardBreakPoint::call_origin(local_role_weapon_init_hook, _bc, _a, _b);
}
void 魔术_BulletControl::Init() {
	class_ = UnityResolve::Get("Assembly-CSharp.dll")->Get("$Sb");
	auto method = class_->Get<UnityResolve::Method>("$oB", { "UnityEngine.Vector3", "UnityEngine.Vector3" });
	if (method) {
		method->Cast(魔术_BulletControl::local_role_weapon_init);
		HardBreakPoint::set_break_point(魔术_BulletControl::local_role_weapon_init, local_role_weapon_init_hook);
	}
}
//子弹瞬击
UnityResolve::UnityType::Int32 WeaponControlHook::local_role_weapon_init_hook(WeaponControlHook* _this,float holdOnPower,BOOL* isBulletCost) {
	if (内存::子弹瞬击) {
		if(内存::魔法子弹 && 自瞄::功能范围 != 自瞄::自瞄范围)holdOnPower = -7;
		else holdOnPower = 内存::瞬击值;
	}
	else if (内存::魔法子弹 && 自瞄::功能范围 != 自瞄::自瞄范围) holdOnPower = -7;
	return HardBreakPoint::call_origin(local_role_weapon_init_hook, _this, holdOnPower, isBulletCost);
}
void WeaponControlHook::Init() {
	class_ = UnityResolve::Get("Assembly-CSharp.dll")->Get("WeaponControl");
	class_->Get<UnityResolve::Method>("Fire")->Cast(WeaponControlHook::local_role_weapon_init);
	HardBreakPoint::set_break_point(WeaponControlHook::local_role_weapon_init, local_role_weapon_init_hook);
}


//获取所有偏移地址
void InitAllOffsets()
{
	std::cout << std::hex << std::uppercase;

	// StartGame
	Start_Game::Uworld = Start_Game::GetUworldOffset();
	Start_Game::GameData = Start_Game::GetGameDataOffset();
	Start_Game::StartGame = Start_Game::GetStartGameOffset();
	Start_Game::BattleWorld = Start_Game::GetBattleWorldOffset();
	Start_Game::RoleAIManager = Start_Game::GetRoleAIManagerOffset();
	Start_Game::BattleRoleLogic = Start_Game::GetBattleRoleLogicOffset();
	Start_Game::ItemManager = Start_Game::GetItemManagerOffset();
	Start_Game::ItemManager1 = Start_Game::GetItemManager1Offset();
	Start_Game::AllCar = Start_Game::GetAllCarOffset();

	// BattleRoleLogic
	BattleRoleLogic::roleNetClient = BattleRoleLogic::GetroleNetClientOffset();
	BattleRoleLogic::NickName = BattleRoleLogic::GetNickNameOffset();
	BattleRoleLogic::HP = BattleRoleLogic::GetHPOffset();
	BattleRoleLogic::WeakValue = BattleRoleLogic::GetWeakValueOffset();
	BattleRoleLogic::TeamNum = BattleRoleLogic::GetTeamNumOffset();
	BattleRoleLogic::Pos = BattleRoleLogic::GetPosOffset();
	BattleRoleLogic::SpotPoint = BattleRoleLogic::GetSpotPointOffset();
	BattleRoleLogic::UserCircusBallNet = BattleRoleLogic::GetUserCircusBallNetOffset();
	BattleRoleLogic::roleLogicClient = BattleRoleLogic::GetroleLogicClientOffset();
	BattleRoleLogic::RoleBuffControl = BattleRoleLogic::GetRoleBuffControlOffset();
	BattleRoleLogic::Weapon = BattleRoleLogic::GetWeaponOffset();
	BattleRoleLogic::KillRoleNum = BattleRoleLogic::GetKillRoleNumOffset();
	BattleRoleLogic::RoleID = BattleRoleLogic::GetRoleIDOffset();
	BattleRoleLogic::RoleSize = BattleRoleLogic::GetRoleSizeOffset();
	BattleRoleLogic::PlayerPlatform = BattleRoleLogic::GetPlayerPlatformOffset();
	BattleRoleLogic::HeadEquipPart = BattleRoleLogic::GetHeadEquipPartOffset();
	BattleRoleLogic::BodyEquipPart = BattleRoleLogic::GetBodyEquipPartOffset();
	BattleRoleLogic::PackEquip = BattleRoleLogic::GetPackEquipOffset();
	BattleRoleLogic::PackEquip = BattleRoleLogic::GetPackEquipOffset();
	BattleRoleLogic::FunctionalGarmentEquipPart = BattleRoleLogic::GetFunctionalGarmentEquipPartOffset();
	BattleRoleLogic::isOnline = BattleRoleLogic::GetisOnlineOffset();


	//BattleRole
	BattleRole::RoleClient = BattleRole::GetRoleClientOffset();
	BattleRole::MyRoleControl = BattleRole::GetMyRoleControlOffset();
	BattleRole::UserWeapon = BattleRole::GetUserWeaponOffset();
	BattleRole::lastRotaY = BattleRole::GetlastRotaYOffset();
	BattleRole::IsEventRoleShow = BattleRole::GetIsEventRoleShowOffset();
	BattleRole::tempIsWeak = BattleRole::GettempIsWeakOffset();
	BattleRole::hideNoNetRole = BattleRole::GethideNoNetRoleOffset();
	BattleRole::playerId = BattleRole::GetplayerIdOffset();

	//RoleBuffControl
	RoleBuffControl::JumpNum = RoleBuffControl::GetJumpNumOffset();
	RoleBuffControl::WalkNum = RoleBuffControl::GetWalkNumOffset();
	RoleBuffControl::CameraRatio = RoleBuffControl::GetCameraRatioOffset();



	// AllCar
	AllCar::HP = AllCar::GetHPOffset();
	AllCar::Name = AllCar::GetNameOffset();
	AllCar::Mirror = AllCar::GetMirrorOffset();
	AllCar::Position = AllCar::GetPositionOffset();
	AllCar::Consumption = AllCar::GetConsumptionOffset();
	AllCar::carNetClient = AllCar::GetcarNetClientOffset();
	AllCar::_carId = AllCar::Get_carIdOffset();

	// AbsPickItemNet
	AbsPickItemNet::AutoId = AbsPickItemNet::GetAutoIdOffset();
	AbsPickItemNet::ItemSign = AbsPickItemNet::GetItemSignOffset();
	AbsPickItemNet::ItemId = AbsPickItemNet::GetItemIdOffset();
	AbsPickItemNet::ItemType = AbsPickItemNet::GetItemTypeOffset();
	AbsPickItemNet::ItemName = AbsPickItemNet::GetItemNameOffset();
	AbsPickItemNet::BulletNum = AbsPickItemNet::GetBulletNumOffset();
	AbsPickItemNet::ItemLevel = AbsPickItemNet::GetItemLevelOffset();
	AbsPickItemNet::SyncPoint = AbsPickItemNet::GetSyncPointOffset();
	AbsPickItemNet::PickRoleId = AbsPickItemNet::GetPickRoleIdOffset();
	AbsPickItemNet::NowValue = AbsPickItemNet::GetNowValueOffset();
	AbsPickItemNet::skinSign = AbsPickItemNet::GetskinSignOffset();
	AbsPickItemNet::ShootSign = AbsPickItemNet::GetShootSignOffset();


	// RoleAIManager
	RoleAIManager::HP = RoleAIManager::GetHPOffset();
	RoleAIManager::Name = RoleAIManager::GetNameOffset();
	RoleAIManager::Position = RoleAIManager::GetPositionOffset();
	RoleAIManager::RoleSize = RoleAIManager::GetAISizeOffset();



	// Camera
	Camera_Controller::MyCameraRotationX = Camera_Controller::GetMyCameraRotationXOffset();
	Camera_Controller::MyCameraRotationY = Camera_Controller::GetMyCameraRotationYOffset();
	Camera_Controller::MyCameraTran = Camera_Controller::GetMyCameraTranOffset();
}

// 输出所有偏移到控制台
void PrintAllOffsets()
{
	// 控制台输出标题
	std::cout << "           地址输出" << std::endl;
	std::cout << "" << std::endl;

	// 开局模块
	std::cout << "[开局模块] 世界地址：0x" << std::uppercase << std::hex << Start_Game::Uworld << std::dec << std::nouppercase << std::endl;
	std::cout << "[开局模块] 世界偏移：0x" << std::uppercase << std::hex << Start_Game::GameData << std::dec << std::nouppercase << std::endl;
	std::cout << "[开局模块] 人物偏移：0x" << std::uppercase << std::hex << Start_Game::StartGame << std::dec << std::nouppercase << std::endl;
	std::cout << "[战斗世界] 物品偏移：0x" << std::uppercase << std::hex << Start_Game::BattleWorld << std::dec << std::nouppercase << std::endl;
	std::cout << "[开局模块] 角色AI管理器：0x" << std::uppercase << std::hex << Start_Game::RoleAIManager << std::dec << std::nouppercase << std::endl;
	std::cout << "[开局模块] 战斗角色逻辑：0x" << std::uppercase << std::hex << Start_Game::BattleRoleLogic << std::dec << std::nouppercase << std::endl;
	std::cout << "[开局模块] 全部载具：0x" << std::uppercase << std::hex << Start_Game::AllCar << std::dec << std::nouppercase << std::endl;
	std::cout << "[战斗世界] 物品管理器：0x" << std::uppercase << std::hex << Start_Game::ItemManager << std::dec << std::nouppercase << std::endl;
	std::cout << "[战斗世界] 物品偏移：0x" << std::uppercase << std::hex << Start_Game::ItemManager1 << std::dec << std::nouppercase << std::endl;
	std::cout << "" << std::endl;

	// 角色逻辑
	std::cout << "[角色] 昵称：0x" << std::uppercase << std::hex << BattleRoleLogic::NickName << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 血量：0x" << std::uppercase << std::hex << BattleRoleLogic::HP << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 倒地血量：0x" << std::uppercase << std::hex << BattleRoleLogic::WeakValue << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 编号：0x" << std::uppercase << std::hex << BattleRoleLogic::TeamNum << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 坐标：0x" << std::uppercase << std::hex << BattleRoleLogic::Pos << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 标点坐标：0x" << std::uppercase << std::hex << BattleRoleLogic::SpotPoint << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 手持：0x" << std::uppercase << std::hex << BattleRoleLogic::Weapon << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 杀敌：0x" << std::uppercase << std::hex << BattleRoleLogic::KillRoleNum << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] ID：0x" << std::uppercase << std::hex << BattleRoleLogic::RoleID << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 公共偏移：0x" << std::uppercase << std::hex << BattleRoleLogic::roleLogicClient << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] Buff控制：0x" << std::uppercase << std::hex << BattleRoleLogic::RoleBuffControl << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 角色大小：0x" << std::uppercase << std::hex << BattleRoleLogic::RoleSize << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 是否踩球：0x" << std::uppercase << std::hex << BattleRoleLogic::UserCircusBallNet << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 玩家平台：0x" << std::uppercase << std::hex << BattleRoleLogic::PlayerPlatform << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 头部装备：0x" << std::uppercase << std::hex << BattleRoleLogic::HeadEquipPart << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 身体装备：0x" << std::uppercase << std::hex << BattleRoleLogic::BodyEquipPart << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 背包装备：0x" << std::uppercase << std::hex << BattleRoleLogic::PackEquip << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 功能服装备：0x" << std::uppercase << std::hex << BattleRoleLogic::FunctionalGarmentEquipPart << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 是否在线：0x" << std::uppercase << std::hex << BattleRoleLogic::isOnline << std::dec << std::nouppercase << std::endl;
	std::cout << "" << std::endl;
	std::cout << "[角色] RoleClient：0x" << std::uppercase << std::hex << BattleRoleLogic::roleNetClient << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] RoleClient：0x" << std::uppercase << std::hex << BattleRole::RoleClient << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] MyRoleControl：0x" << std::uppercase << std::hex << BattleRole::MyRoleControl << std::dec << std::nouppercase << std::endl;
	std::cout << "" << std::endl;
	std::cout << "[角色] 手持武器：0x" << std::uppercase << std::hex << BattleRole::UserWeapon << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 玩家朝向：0x" << std::uppercase << std::hex << BattleRole::lastRotaY << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 是否显示：0x" << std::uppercase << std::hex << BattleRole::IsEventRoleShow << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 是否倒地：0x" << std::uppercase << std::hex << BattleRole::tempIsWeak << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 是否死亡：0x" << std::uppercase << std::hex << BattleRole::hideNoNetRole << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色] 玩家标识：0x" << std::uppercase << std::hex << BattleRole::playerId << std::dec << std::nouppercase << std::endl;
	std::cout << "" << std::endl;
	std::cout << "[角色Buff] 跳跃高度：0x" << std::uppercase << std::hex << RoleBuffControl::JumpNum << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色Buff] 行走速度：0x" << std::uppercase << std::hex << RoleBuffControl::WalkNum << std::dec << std::nouppercase << std::endl;
	std::cout << "[角色Buff] 镜头距离：0x" << std::uppercase << std::hex << RoleBuffControl::CameraRatio << std::dec << std::nouppercase << std::endl;

	std::cout << "" << std::endl;

	// 载具
	std::cout << "[载具] 血量：0x" << std::uppercase << std::hex << AllCar::HP << std::dec << std::nouppercase << std::endl;
	std::cout << "[载具] 名称：0x" << std::uppercase << std::hex << AllCar::Name << std::dec << std::nouppercase << std::endl;
	std::cout << "[载具] 镜像：0x" << std::uppercase << std::hex << AllCar::Mirror << std::dec << std::nouppercase << std::endl;
	std::cout << "[载具] 坐标：0x" << std::uppercase << std::hex << AllCar::Position << std::dec << std::nouppercase << std::endl;
	std::cout << "[载具] 油量：0x" << std::uppercase << std::hex << AllCar::Consumption << std::dec << std::nouppercase << std::endl;
	std::cout << "[载具] 载具网络客户端：0x" << std::uppercase << std::hex << AllCar::carNetClient << std::dec << std::nouppercase << std::endl;
	std::cout << "[载具] ID：0x" << std::uppercase << std::hex << AllCar::_carId << std::dec << std::nouppercase << std::endl;
	std::cout << "" << std::endl;

	// 人机
	std::cout << "[人机] 血量：0x" << std::uppercase << std::hex << RoleAIManager::HP << std::dec << std::nouppercase << std::endl;
	std::cout << "[人机] 名称：0x" << std::uppercase << std::hex << RoleAIManager::Name << std::dec << std::nouppercase << std::endl;
	std::cout << "[人机] 坐标：0x" << std::uppercase << std::hex << RoleAIManager::Position << std::dec << std::nouppercase << std::endl;
	std::cout << "[人机] 大小：0x" << std::uppercase << std::hex << RoleAIManager::RoleSize << std::dec << std::nouppercase << std::endl;
	std::cout << "" << std::endl;

	// 物品
	std::cout << "[物品] 编号：0x" << std::uppercase << std::hex << AbsPickItemNet::AutoId << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 物品英文名：0x" << std::uppercase << std::hex << AbsPickItemNet::ItemSign << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 物品ID：0x" << std::uppercase << std::hex << AbsPickItemNet::ItemId << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 物品类型：0x" << std::uppercase << std::hex << AbsPickItemNet::ItemType << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 物品名称：0x" << std::uppercase << std::hex << AbsPickItemNet::ItemName << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 子弹数量：0x" << std::uppercase << std::hex << AbsPickItemNet::BulletNum << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 物品等级：0x" << std::uppercase << std::hex << AbsPickItemNet::ItemLevel << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 坐标：0x" << std::uppercase << std::hex << AbsPickItemNet::SyncPoint << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 拾取角色ID：0x" << std::uppercase << std::hex << AbsPickItemNet::PickRoleId << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 当前数值：0x" << std::uppercase << std::hex << AbsPickItemNet::NowValue << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 射击标识：0x" << std::uppercase << std::hex << AbsPickItemNet::ShootSign << std::dec << std::nouppercase << std::endl;
	std::cout << "[物品] 皮肤标识：0x" << std::uppercase << std::hex << AbsPickItemNet::skinSign << std::dec << std::nouppercase << std::endl;
	std::cout << "" << std::endl;

	// 相机
	std::cout << "[相机] 旋转X：0x" << std::uppercase << std::hex << Camera_Controller::MyCameraRotationX << std::dec << std::nouppercase << std::endl;
	std::cout << "[相机] 旋转Y：0x" << std::uppercase << std::hex << Camera_Controller::MyCameraRotationY << std::dec << std::nouppercase << std::endl;
	std::cout << "[相机] 相机坐标：0x" << std::uppercase << std::hex << Camera_Controller::MyCameraTran << std::dec << std::nouppercase << std::endl;
	std::cout << "[相机] 地址：0x" << std::uppercase << std::hex << Camera_Controller::g_pCamObj << std::dec << std::nouppercase << std::endl;
}

std::string GBK转UTF8(const std::string& gbkStr)
{
	int len = MultiByteToWideChar(936, 0, gbkStr.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	MultiByteToWideChar(936, 0, gbkStr.c_str(), -1, wstr, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* utf8Str = new char[len + 1];
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8Str, len, NULL, NULL);

	std::string res = utf8Str;
	delete[] wstr;
	delete[] utf8Str;
	return res;
}

std::string UTF8转GBK(const std::string& utf8_str)
{
	// 1. 先把 UTF-8 转为 宽字符(WCHAR/UTF-16)
	// 计算需要的宽字符缓冲区长度
	int wide_len = MultiByteToWideChar(
		CP_UTF8,        // 源编码：UTF-8
		0,              // 无特殊标志
		utf8_str.c_str(),// 源字符串
		-1,             // 自动计算字符串长度
		NULL,           // 输出缓冲区为NULL，仅计算长度
		0               // 缓冲区大小
	);
	if (wide_len <= 0) return "";

	// 分配宽字符缓冲区
	wchar_t* wide_buf = new wchar_t[wide_len];
	MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, wide_buf, wide_len);

	// 2. 再把 宽字符 转为 GBK
	// 计算GBK缓冲区长度
	int gbk_len = WideCharToMultiByte(
		CP_ACP,         // 目标编码：系统本地编码(中文Windows=GBK)
		0,
		wide_buf,       // 宽字符字符串
		-1,
		NULL,
		0,
		NULL, NULL
	);
	if (gbk_len <= 0) {
		delete[] wide_buf;
		return "";
	}

	// 分配GBK缓冲区
	char* gbk_buf = new char[gbk_len];
	WideCharToMultiByte(CP_ACP, 0, wide_buf, -1, gbk_buf, gbk_len, NULL, NULL);

	// 构造结果字符串并释放内存
	std::string gbk_str(gbk_buf);
	delete[] wide_buf;
	delete[] gbk_buf;

	return gbk_str;
}

std::string 取中间文本(const std::string& 提取的文本, const std::string& 左边文本, const std::string& 右边文本) {
	// 查找左标记位置
	size_t left_pos = 提取的文本.find(左边文本);
	if (left_pos == std::string::npos) {
		return ""; // 没找到左标记
	}

	// 计算起始位置（左标记结束处）
	size_t start = left_pos + 左边文本.length();

	// 从起始位置查找右标记
	size_t right_pos = 提取的文本.find(右边文本, start);
	if (right_pos == std::string::npos) {
		return ""; // 没找到右标记
	}

	// 截取中间内容
	return 提取的文本.substr(start, right_pos - start);
}

uintptr_t 文本转uintptr_t(const std::string& s)
{
	if (s.empty()) return 0;

	// 检查是否是 0x / 0X 开头（强制十六进制）
	bool is_hex = false;
	size_t start = 0;

	if (s.length() >= 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
	{
		is_hex = true;
		start = 2;
	}
	// 否则检查是否包含 A-F 字符 → 判定为十六进制
	else
	{
		for (char c : s)
		{
			if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
			{
				is_hex = true;
				break;
			}
		}
	}

	uintptr_t val = 0;

	if (is_hex)
	{
		// 十六进制解析
		for (size_t i = start; i < s.size(); ++i)
		{
			char c = s[i];
			val *= 16;
			if (c >= '0' && c <= '9') val += c - '0';
			else if (c >= 'A' && c <= 'F') val += 10 + (c - 'A');
			else if (c >= 'a' && c <= 'f') val += 10 + (c - 'a');
		}
	}
	else
	{
		// 十进制解析
		for (char c : s)
		{
			if (c >= '0' && c <= '9')
			{
				val *= 10;
				val += c - '0';
			}
		}
	}

	return val;
}

bool 验证黑名单(const char* 玩家ID, const char* 黑名单文本)
{
	if (!玩家ID || !黑名单文本) return false;

	std::string id = 玩家ID;
	const char* p = 黑名单文本;
	std::string current;

	while (*p)
	{
		if (*p == GBK转UTF8("，")[0])  // 中文逗号分隔
		{
			// 删首尾空
			size_t start = 0, end = current.size();
			while (start < end && current[start] <= ' ') start++;
			while (end > start && current[end - 1] <= ' ') end--;
			std::string trim_id = current.substr(start, end - start);

			if (trim_id == id) return true;

			current.clear();
		}
		else
		{
			current += *p;
		}
		p++;
	}

	// 最后一段
	if (!current.empty())
	{
		size_t start = 0, end = current.size();
		while (start < end && current[start] <= ' ') start++;
		while (end > start && current[end - 1] <= ' ') end--;
		std::string trim_id = current.substr(start, end - start);
		if (trim_id == id) return true;
	}

	return false;
}

bool 验证白名单(const char* 玩家ID, const char* 白名单文本)
{
	if (!玩家ID || !白名单文本) return false;

	std::string id = 玩家ID;
	const char* p = 白名单文本;
	std::string current;

	while (*p)
	{
		if (*p == GBK转UTF8("，")[0])  // 中文逗号分隔
		{
			// 删首尾空
			size_t start = 0, end = current.size();
			while (start < end && current[start] <= ' ') start++;
			while (end > start && current[end - 1] <= ' ') end--;
			std::string trim_id = current.substr(start, end - start);

			if (trim_id == id) return true;

			current.clear();
		}
		else
		{
			current += *p;
		}
		p++;
	}

	// 最后一段
	if (!current.empty())
	{
		size_t start = 0, end = current.size();
		while (start < end && current[start] <= ' ') start++;
		while (end > start && current[end - 1] <= ' ') end--;
		std::string trim_id = current.substr(start, end - start);
		if (trim_id == id) return true;
	}

	return false;
}

//欧拉角转四元数
auto calc_quat(const glm::vec2 _euler) -> std::pair<glm::quat, glm::quat> {
	auto quaternion_x = glm::angleAxis(glm::radians(_euler.x), glm::vec3(0, 1, 0));
	auto quaternion_y = glm::angleAxis(glm::radians(_euler.y), glm::vec3(1, 0, 0));
	return { quaternion_x,quaternion_y };
}
//计算瞄准欧拉角
glm::vec2 calculate_angles(const D3D坐标& camPos, const D3D坐标& targetPos)
{
	const glm::vec3 direction = targetPos - camPos;

	// 计算水平偏角 Yaw
	float angle_yaw = glm::degrees(atan2f(direction.x, direction.z));
	if (angle_yaw < 0) angle_yaw += 360.0f;

	// 计算俯仰角 Pitch
	float horizontal_distance = sqrtf(direction.x * direction.x + direction.z * direction.z);
	float angle_pitch = -glm::degrees(static_cast<float>(std::atan2(direction.y, horizontal_distance)));
	angle_pitch = glm::clamp(angle_pitch, -65.0f, 75.0f);

	return glm::vec2(angle_yaw, angle_pitch);
}

D3D坐标 PredictEnemyNextPos(D3D坐标 当前敌人坐标, float 我与敌人距离)
{
	if (当前敌人坐标.x == 0 && 当前敌人坐标.y == 0 && 当前敌人坐标.z == 0) return {};

	// 单线程专用静态缓存，高频调用绝对稳定
	static D3D坐标 上次坐标 = 当前敌人坐标;
	static bool 首次调用 = true;
	static float 累计移动距离 = 0.0f;

	// 首次调用初始化
	if (首次调用)
	{
		上次坐标 = 当前敌人坐标;
		首次调用 = false;
		return 当前敌人坐标;
	}

	// 计算敌人移动方向
	float dx = 当前敌人坐标.x - 上次坐标.x;
	float dy = 当前敌人坐标.y - 上次坐标.y;
	float dz = 当前敌人坐标.z - 上次坐标.z;


	const float 实际移动距离 = sqrtf(dx * dx + dy * dy + dz * dz);
	累计移动距离 += 实际移动距离;
	// 重置上限，防止数值溢出
	if (累计移动距离 > 1.0f) 累计移动距离 = 1.0f;

	const float 抖动过滤阈值 = 0.04f;
	if (实际移动距离 < 抖动过滤阈值){
		dx = dy = dz = 0.0f;
	}

	// 计算移动距离，判断静止
	float 帧移动距离 = sqrtf(dx * dx + dy * dy + dz * dz);
	if (帧移动距离 < 0.0001f){
		上次坐标 = 当前敌人坐标;
		return 当前敌人坐标;
	}

	// 单位方向向量
	float 归一化X = dx / 帧移动距离;
	float 归一化Y = dy / 帧移动距离;
	float 归一化Z = dz / 帧移动距离;

	// 预测下一个位置（匀速直线运动，高频精准）
	float 预判幅度 = 我与敌人距离 / 100.0f;
	D3D坐标 预测坐标;
	预测坐标.x = 当前敌人坐标.x + 归一化X * 预判幅度;
	预测坐标.y = 当前敌人坐标.y + 归一化Y * 预判幅度;
	预测坐标.z = 当前敌人坐标.z + 归一化Z * 预判幅度;

	// 更新缓存，为下一次高频调用做准备
	上次坐标 = 当前敌人坐标;

	return 预测坐标;
}














void 绘制信息(D4D坐标 方框, UINT32 距离, UINT32 编号, const char* 玩家名称, const char* 玩家ID, bool 是否被瞄, uintptr_t 玩家地址) {
	float 玩家血量 = 0;
	float 倒地血量 = 0;
	UINT64 玩家平台 = 0;
	char 玩家手持[64];

	玩家血量 = *reinterpret_cast<float*>(玩家地址 + BattleRoleLogic::HP);
	倒地血量 = *reinterpret_cast<float*>(玩家地址 + BattleRoleLogic::WeakValue);
	玩家平台 = *reinterpret_cast<UINT64*>(玩家地址 + BattleRoleLogic::PlayerPlatform);
	strcpy_s(玩家手持, sizeof(玩家手持), 取角色手持(玩家地址).c_str());


	const float 宽度 = 130.0f;
	const float 高度 = 26.0f;

	float 左边 = 方框.x + 方框.w * 0.5f - 宽度 * 0.5f;
	float 顶边 = 方框.y - 5 - 高度;

	ImGuiIO& io = ImGui::GetIO();
	float 屏幕宽 = io.DisplaySize.x;
	float 屏幕高 = io.DisplaySize.y;
	const float 边距 = 15.0f;
	// 1. 限制左边：不能小于 "边距"
	if (左边 < 边距) {
		左边 = 边距;
	}
	// 2. 限制右边：右边不能大于 "屏幕宽 - 宽度 - 边距"
	if (左边 + 宽度 > 屏幕宽 - 边距) {
		左边 = 屏幕宽 - 宽度 - 边距;
	}
	// 3. 限制上边：不能小于 "边距"
	if (顶边 < 边距) {
		顶边 = 边距;
	}
	// 4. 限制下边：不能大于 "屏幕高 - 高度 - 边距"
	if (顶边 + 高度 > 屏幕高 - 边距) {
		顶边 = 屏幕高 - 高度 - 边距;
	}


	ImU32 字体色 = 白色;
	ImU32 背景色 = 取队伍颜色(编号);
	ImU32 紫罗兰色 = IM_COL32(238, 130, 238, 255);

	ImU32 血条颜色;
	if (玩家血量 >= 75)      血条颜色 = IM_COL32(255, 255, 255, 255);   // 白
	else if (玩家血量 > 30)  血条颜色 = IM_COL32(255, 165, 0, 255); // 橙黄
	else                 血条颜色 = IM_COL32(255, 0, 0, 255);   // 红
	填充方框(左边 - 1, 顶边 - 1, 宽度 + 2, 高度 + 2, 背景色, 100);
	填充方框(左边, 顶边, 宽度, 高度, IM_COL32(0, 0, 0, 50), 50);
	填充方框(左边 + 3, 顶边 + 3, 20, 20, 背景色, 255);

	char 缓冲[256];
	std::snprintf(缓冲, sizeof(缓冲), "%d", 编号);
	绘制描边文本(左边 + 12 - static_cast<float>(std::strlen(缓冲)) * 3.2f, 顶边 + 5, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));//编号

	float 血量计算 = 0.0f;
	if (玩家血量 != 0) {
		血量计算 = static_cast<float>(玩家血量) * 0.9f;
	}
	else {
		血量计算 = static_cast<float>(倒地血量) * 0.9f;
	}

	float 血条宽 = (90 < 血量计算) ? 90 : 血量计算;
	填充方框(左边 + 30, 顶边 + 19, 91, 5, IM_COL32(0, 0, 0, 80), 80);
	if (玩家血量 != 0) {
		填充方框(左边 + 30, 顶边 + 20, 血条宽, 2.5, 血条颜色, 255);
	}
	else {
		填充方框(左边 + 30, 顶边 + 20, 血条宽, 2.5, 紫色, 255);
	}
	ImU32 文字颜色 = 白色;
	if (玩家平台 != 5) 文字颜色 = 艳青;
	if (玩家ID == "" || 编号 == 0) 文字颜色 = 绿色;
	if (验证黑名单(玩家ID, 本人数据::黑名单.c_str())) 文字颜色 = 红色;

	std::snprintf(缓冲, sizeof(缓冲), "%s", 玩家名称);
	绘制描边文本(左边 + 28, 顶边 + 4, 缓冲, 文字颜色, 1, IM_COL32(0, 0, 0, 180));//名字

	if(是否被瞄 && 显示::被瞄提醒)绘制描边文本(方框.x + 方框.w * 0.5f - ImGui::CalcTextSize(GBK转UTF8("<正在瞄准你>").c_str()).x * 0.5f, 顶边 - 30, GBK转UTF8("<正在瞄准你>").c_str(), 红色, 1, IM_COL32(255, 255, 255, 180));

	std::snprintf(缓冲, sizeof(缓冲), "%s", 玩家手持);
	绘制描边文本(方框.x + 方框.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, 顶边 - 15, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));//手持

	std::snprintf(缓冲, sizeof(缓冲), "%dm", 距离);
	绘制描边文本(方框.x + 方框.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, 方框.y + 方框.h, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));//距离
}

void 绘制信息1(D4D坐标 方框, UINT32 距离, UINT32 编号, const char* 玩家名称, const char* 玩家ID, bool 是否被瞄, uintptr_t 玩家地址) {
	float 玩家血量 = 0;
	float 倒地血量 = 0;
	UINT64 玩家平台 = 0;
	UINT64 玩家杀敌 = 0;
	UINT32 是否在线 = 0;
	char 玩家手持[64];
	char 玩家头盔[64];
	char 玩家护甲[64];
	char 玩家背包[64];
	char 玩家身份卡[64];
	玩家血量 = *reinterpret_cast<float*>(玩家地址 + BattleRoleLogic::HP);
	倒地血量 = *reinterpret_cast<float*>(玩家地址 + BattleRoleLogic::WeakValue);
	玩家平台 = *reinterpret_cast<UINT64*>(玩家地址 + BattleRoleLogic::PlayerPlatform);
	玩家杀敌 = *reinterpret_cast<UINT64*>(玩家地址 + BattleRoleLogic::KillRoleNum);
	是否在线 = *reinterpret_cast<UINT32*>(玩家地址 + BattleRoleLogic::isOnline);
	strcpy_s(玩家手持, sizeof(玩家手持), 取角色手持(玩家地址).c_str());
	strcpy_s(玩家头盔, sizeof(玩家头盔), 取头盔名字(玩家地址).c_str());
	strcpy_s(玩家护甲, sizeof(玩家护甲), 取护甲名字(玩家地址).c_str());
	strcpy_s(玩家背包, sizeof(玩家背包), 取背包名字(玩家地址).c_str());
	strcpy_s(玩家身份卡, sizeof(玩家身份卡), 取身份卡名字(玩家地址).c_str());

	const float 宽度 = 150.0f;
	const float 高度 = 20.0f;

	float 左边 = 方框.x + 方框.w * 0.5f - 宽度 * 0.5f;
	float 顶边 = 方框.y - 5 - 高度;

	ImU32 字体色 = 白色;
	ImU32 背景色 = 取队伍颜色(编号);
	ImU32 紫罗兰色 = IM_COL32(238, 130, 238, 255);

	ImU32 血条颜色;
	if (玩家血量 >= 75)      血条颜色 = 白色;
	else if (玩家血量 > 30)  血条颜色 = 橙黄;
	else                 血条颜色 = 红色;
	填充方框(左边 - 1, 顶边 - 1, 宽度 + 2, 高度 + 2, 背景色, 100);//背景
	填充方框(左边, 顶边, 宽度, 高度, IM_COL32(0, 0, 0, 50), 50);//背景
	填充方框(左边, 顶边, 20, 20, 背景色, 150);//编号背景
	填充方框(左边 + 115, 顶边, 35, 20, 背景色, 100);//距离背景

	ImU32 文字颜色 = 白色;
	if (验证黑名单(玩家ID, 本人数据::黑名单.c_str())) 绘制描边文本(左边, 顶边 - 16, GBK转UTF8("悬赏").c_str(), 红色, 1, IM_COL32(255, 255, 255, 180));
	else {
		if (是否在线 == 0 || 编号 == 0) 绘制描边文本(左边, 顶边 - 16, GBK转UTF8("掉线").c_str(), 绿色, 1, IM_COL32(0, 0, 0, 180));
		else {
			if (玩家平台 != 5) 绘制描边文本(左边, 顶边 - 16, GBK转UTF8("手机").c_str(), 艳青, 1, IM_COL32(0, 0, 0, 180));
			else 绘制描边文本(左边, 顶边 - 16, GBK转UTF8("玩家").c_str(), 红色, 1, IM_COL32(0, 0, 0, 180));
		}
	}
	if (是否被瞄 && 显示::被瞄提醒)绘制描边文本(方框.x + 方框.w * 0.5f - ImGui::CalcTextSize(GBK转UTF8("<正在瞄准你>").c_str()).x * 0.5f, 顶边 - 64, GBK转UTF8("<正在瞄准你>").c_str(), 红色, 1, IM_COL32(255, 255, 255, 180));

	char 缓冲[256];
	std::snprintf(缓冲, sizeof(缓冲), "%d", 编号);
	绘制描边文本(左边 + 8.5 - static_cast<float>(std::strlen(缓冲)) * 3.2f, 顶边 + 2.5, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));//编号

	std::snprintf(缓冲, sizeof(缓冲), "%dm", 距离);
	绘制描边文本(左边 + 117, 顶边 + 2.5, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));//距离

	float 血量计算 = 0.0f;
	if (玩家血量 != 0) 血量计算 = 玩家血量;
	else 血量计算 = 倒地血量;
	float 血条总宽度 = 148.0f;
	float 血条宽 = (血量计算 / 100.0f) * 血条总宽度;
	if (血条宽 > 血条总宽度) 血条宽 = 血条总宽度;
	if (血条宽 < 0) 血条宽 = 0;
	填充方框(左边, 顶边 + 19, 宽度 - 1, 5, IM_COL32(0, 0, 0, 80), 80);
	if (玩家血量 != 0) 填充方框(左边 + 1, 顶边 + 20, 血条宽, 2.5, 血条颜色, 255);
	else 填充方框(左边 + 1, 顶边 + 20, 血条宽, 2.5, 紫色, 255);

	std::snprintf(缓冲, sizeof(缓冲), "%s", 玩家名称);
	绘制描边文本(左边 + 25, 顶边 + 1.5, 缓冲, 白色, 1, IM_COL32(0, 0, 0, 180));//名字

	std::snprintf(缓冲, sizeof(缓冲), "%s", 玩家手持);
	绘制描边文本(左边 + 宽度 * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, 顶边 - 15, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));//手持

	std::snprintf(缓冲, sizeof(缓冲), "%s%d", GBK转UTF8("头:").c_str(), 获取头盔等级(UTF8转GBK(玩家头盔).c_str()));
	绘制描边文本(左边 + 25, 顶边 - 30, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));//头盔

	std::snprintf(缓冲, sizeof(缓冲), "%s%d", GBK转UTF8("甲:").c_str(), 获取护甲等级(UTF8转GBK(玩家护甲).c_str()));
	绘制描边文本(左边 + 55, 顶边 - 30, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));//护甲

	std::snprintf(缓冲, sizeof(缓冲), "%s%d", GBK转UTF8("包:").c_str(), 获取背包等级(UTF8转GBK(玩家背包).c_str()));
	绘制描边文本(左边 + 85, 顶边 - 30, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));//背包

	std::snprintf(缓冲, sizeof(缓冲), "%s", 玩家身份卡);
	绘制描边文本(左边 + 宽度 * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, 顶边 - 48, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));//身份卡

	std::snprintf(缓冲, sizeof(缓冲), "K:%d", 玩家杀敌);
	绘制描边文本(左边 + 120, 顶边 - 30, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));

	if (玩家血量 != 0)std::snprintf(缓冲, sizeof(缓冲), "HP:%.0f", 玩家血量);
	else std::snprintf(缓冲, sizeof(缓冲), "HP:%.0f", 倒地血量);
	绘制描边文本(左边 + 120, 顶边 - 16, 缓冲, 字体色, 1, IM_COL32(0, 0, 0, 180));

}

void 边框方框(float x, float y, float w, float h, ImU32 颜色) {
	ImGui::GetForegroundDrawList()->AddRect(
		ImVec2(x, y),
		ImVec2(x + w, y + h),
		颜色,
		3.0f,           // 圆角
		0,              // 角落标志
		1.0f
	);
}

void 填充方框(float X, float Y, float W, float H, ImU32 颜色, int 透明度)
{
	int 临时透明度 = 透明度;
	if (透明度 == 0) { 临时透明度 = 255; }
	透明矩形(X, Y, W, H, 颜色, 临时透明度, IM_COL32(0, 0, 0, 0));
}

void 透明矩形(float 左边, float 顶边, float 宽度, float 高度, ImU32 颜色, int 透明度, ImU32 边框色)
{
	if (透明度 < 0) 透明度 = 0;
	if (透明度 > 255) 透明度 = 255;
	ImU32 填充颜色 = 转换颜色带透明度(颜色, 透明度);
	ImU32 边框颜色 = 转换颜色带透明度(边框色, 透明度);
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(左边, 顶边), ImVec2(左边 + 宽度, 顶边 + 高度), 边框颜色, 5.0f, 0, 2.0f);
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(左边, 顶边), ImVec2(左边 + 宽度, 顶边 + 高度), 填充颜色, 5.0f);
}

void 绘制描边文本(float x, float y, const char* 绘制的文本, ImU32 绘制的颜色,float 描边宽度,ImU32 描边颜色) {
	ImDrawList* 屏幕 = ImGui::GetForegroundDrawList();
	屏幕->AddText(ImVec2(x- 描边宽度, y), 描边颜色, 绘制的文本);
	屏幕->AddText(ImVec2(x+ 描边宽度, y), 描边颜色, 绘制的文本);
	屏幕->AddText(ImVec2(x, y- 描边宽度), 描边颜色, 绘制的文本);
	屏幕->AddText(ImVec2(x, y+ 描边宽度), 描边颜色, 绘制的文本);

	屏幕->AddText(ImVec2(x, y), 绘制的颜色, 绘制的文本);
}

void 绘制文本(float x, float y, const char* 绘制的文本, ImU32 绘制的颜色) {
	ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y),
		绘制的颜色,
		绘制的文本);
}

ImU32 转换颜色带透明度(ImU32 color, int alpha){
	int r = (color >> IM_COL32_R_SHIFT) & 0xFF;
	int g = (color >> IM_COL32_G_SHIFT) & 0xFF;
	int b = (color >> IM_COL32_B_SHIFT) & 0xFF;
	return IM_COL32(r, g, b, alpha);
}

void 绘制圆形(float x, float y, float 半径, int 段数, int 线宽, ImU32 颜色){
	ImGui::GetForegroundDrawList()->AddCircle(ImVec2(x, y), 半径, 颜色, 段数, 线宽);
}

void 绘制直线(float x1, float y1, float x2, float y2, ImU32 颜色, float 线宽){
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), 颜色, 线宽);
}

double 角度转弧度(double 角度){
	const double PI = 3.14159265358979323846;
	return 角度 * PI / 180.0;
}

void 彩虹圈形(D2D坐标 中心, int 半径, int 线宽)
{
	static DWORD 上次时间 = 0;
	static float 旋转角度 = 0;
	std::vector<ImU32> 颜色数组 = { 红色, 黄色, 绿色, 蓝色, 紫色, 艳青, 橙黄 };
	DWORD 当前时间 = GetTickCount();
	if (上次时间 == 0) {
		上次时间 = 当前时间;
	}
	if (当前时间 - 上次时间 >= 10) {
		旋转角度 += 0.5f;
		上次时间 = 当前时间;
		if (旋转角度 >= 360) {
			旋转角度 = 0;
		}
	}
	const float PI = 3.14159265358979323846f;
	for (int i = 0; i < 360; i++) {
		int 颜色索引 = (i / 14) % 7;
		ImU32 当前颜色 = 颜色数组[颜色索引];
		float 角度1 = (i * 5 * PI / 180.0f) + (旋转角度 * PI / 180.0f);
		float 角度2 = ((i * 5 + 2.5f) * PI / 180.0f) + (旋转角度 * PI / 180.0f);
		float x1 = 中心.x + cos(角度1) * 半径;
		float y1 = 中心.y + sin(角度1) * 半径;
		float x2 = 中心.x + cos(角度2) * 半径;
		float y2 = 中心.y + sin(角度2) * 半径;
		绘制直线(x1, y1, x2, y2, 当前颜色, 线宽);
	}
}

void 绘制3D方框(D3D坐标 对象坐标, Matrix 矩阵, float 朝向, ImU32 方框颜色, float 顶边偏移, float 底边偏移, int 方框线粗, D2D坐标 屏幕){

	if (方框线粗 == 0) { 方框线粗 = 1; }

	D2D坐标 方框数组[8] = {};
	D3D坐标 朝向坐标[8] = {};

	// 设置方框比例
	float 方框比例 = 0.7f;

	// 左上角
	朝向坐标[0].x = 对象坐标.x + sin(角度转弧度(朝向 - 45)) * 方框比例;
	朝向坐标[0].z = 对象坐标.z + cos(角度转弧度(朝向 - 45)) * 方框比例;
	朝向坐标[0].y = 对象坐标.y + 顶边偏移;

	if (D3D转2D坐标(朝向坐标[0], 方框数组[0], 矩阵, 屏幕)) {
		// 右上角
		朝向坐标[1].x = 对象坐标.x + sin(角度转弧度(朝向 + 45)) * 方框比例;
		朝向坐标[1].z = 对象坐标.z + cos(角度转弧度(朝向 + 45)) * 方框比例;
		朝向坐标[1].y = 对象坐标.y + 顶边偏移;

		if (D3D转2D坐标(朝向坐标[1], 方框数组[1], 矩阵, 屏幕)) {
			// 右下角
			朝向坐标[2].x = 对象坐标.x + sin(角度转弧度(朝向 + 135)) * 方框比例;
			朝向坐标[2].z = 对象坐标.z + cos(角度转弧度(朝向 + 135)) * 方框比例;
			朝向坐标[2].y = 对象坐标.y + 顶边偏移;

			if (D3D转2D坐标(朝向坐标[2], 方框数组[2], 矩阵, 屏幕)) {
				// 左下角
				朝向坐标[3].x = 对象坐标.x + sin(角度转弧度(朝向 + 225)) * 方框比例;
				朝向坐标[3].z = 对象坐标.z + cos(角度转弧度(朝向 + 225)) * 方框比例;
				朝向坐标[3].y = 对象坐标.y + 顶边偏移;

				if (D3D转2D坐标(朝向坐标[3], 方框数组[3], 矩阵, 屏幕)) {
					// 左上角
					朝向坐标[4].x = 对象坐标.x + sin(角度转弧度(朝向 - 45)) * 方框比例;
					朝向坐标[4].z = 对象坐标.z + cos(角度转弧度(朝向 - 45)) * 方框比例;
					朝向坐标[4].y = 对象坐标.y + 底边偏移;

					if (D3D转2D坐标(朝向坐标[4], 方框数组[4], 矩阵, 屏幕)) {
						// 右上角
						朝向坐标[5].x = 对象坐标.x + sin(角度转弧度(朝向 + 45)) * 方框比例;
						朝向坐标[5].z = 对象坐标.z + cos(角度转弧度(朝向 + 45)) * 方框比例;
						朝向坐标[5].y = 对象坐标.y + 底边偏移;

						if (D3D转2D坐标(朝向坐标[5], 方框数组[5], 矩阵, 屏幕)) {
							// 右下角
							朝向坐标[6].x = 对象坐标.x + sin(角度转弧度(朝向 + 135)) * 方框比例;
							朝向坐标[6].z = 对象坐标.z + cos(角度转弧度(朝向 + 135)) * 方框比例;
							朝向坐标[6].y = 对象坐标.y + 底边偏移;

							if (D3D转2D坐标(朝向坐标[6], 方框数组[6], 矩阵, 屏幕)) {
								// 左下角
								朝向坐标[7].x = 对象坐标.x + sin(角度转弧度(朝向 + 225)) * 方框比例;
								朝向坐标[7].z = 对象坐标.z + cos(角度转弧度(朝向 + 225)) * 方框比例;
								朝向坐标[7].y = 对象坐标.y + 底边偏移;

								if (D3D转2D坐标(朝向坐标[7], 方框数组[7], 矩阵, 屏幕)) {
									绘制直线(方框数组[0].x, 方框数组[0].y, 方框数组[1].x, 方框数组[1].y, 方框颜色, 方框线粗);
									绘制直线(方框数组[1].x, 方框数组[1].y, 方框数组[2].x, 方框数组[2].y, 方框颜色, 方框线粗);
									绘制直线(方框数组[2].x, 方框数组[2].y, 方框数组[3].x, 方框数组[3].y, 方框颜色, 方框线粗);
									绘制直线(方框数组[3].x, 方框数组[3].y, 方框数组[0].x, 方框数组[0].y, 方框颜色, 方框线粗);

									绘制直线(方框数组[4].x, 方框数组[4].y, 方框数组[5].x, 方框数组[5].y, 方框颜色, 方框线粗);
									绘制直线(方框数组[5].x, 方框数组[5].y, 方框数组[6].x, 方框数组[6].y, 方框颜色, 方框线粗);
									绘制直线(方框数组[6].x, 方框数组[6].y, 方框数组[7].x, 方框数组[7].y, 方框颜色, 方框线粗);
									绘制直线(方框数组[7].x, 方框数组[7].y, 方框数组[4].x, 方框数组[4].y, 方框颜色, 方框线粗);

									绘制直线(方框数组[0].x, 方框数组[0].y, 方框数组[4].x, 方框数组[4].y, 方框颜色, 方框线粗);
									绘制直线(方框数组[1].x, 方框数组[1].y, 方框数组[5].x, 方框数组[5].y, 方框颜色, 方框线粗);
									绘制直线(方框数组[2].x, 方框数组[2].y, 方框数组[6].x, 方框数组[6].y, 方框颜色, 方框线粗);
									绘制直线(方框数组[3].x, 方框数组[3].y, 方框数组[7].x, 方框数组[7].y, 方框颜色, 方框线粗);
								}
							}
						}
					}
				}
			}
		}
	}
}

void DrawFPSCounter(bool 显示FPS面板)
{
	static float fps_history[120] = { 0 };
	static int   fps_index = 0;
	static float min_fps = 0;
	static float max_fps = 0;

	// 弹出动画变量
	static float animY = 0.0f;
	// 使用“每秒响应”而非帧级步进（指数平滑，帧率无关）
	const float 动画响应 = 8.0f; // 响应速度（越大越快），单位：每秒

	ImGuiIO& io = ImGui::GetIO();
	float delta = io.DeltaTime;
	if (delta <= 0.0f) delta = 1.0f / 60.0f; // 容错

	// 计算帧无关的插值因子
	float 动画因子 = 1.0f - expf(-动画响应 * delta);

	// 动画逻辑：打开 ↓ 弹下来，关闭 ↑ 收回去
	if (显示FPS面板)
		animY = ImLerp(animY, 1.0f, 动画因子);
	else
		animY = ImLerp(animY, 0.0f, 动画因子);

	// 动画完全关闭就不画
	if (animY < 0.01f)
		return;

	float fps = (delta > 0.0f) ? (1.0f / delta) : 0.0f;

	// 记录数据
	fps_history[fps_index] = fps;
	fps_index = (fps_index + 1) % 120;

	// 平均（同样使用帧无关的指数平滑）
	static float 平滑平均值 = 0;
	const float 平滑响应 = 2.0f; // 每秒响应
	float 平滑因子 = 1.0f - expf(-平滑响应 * delta);

	float 真实平均 = 0;
	int cnt = 0;
	for (int i = 0; i < 120; i++) {
		if (fps_history[i] > 0) { 真实平均 += fps_history[i]; cnt++; }
	}
	if (cnt > 0) 真实平均 /= cnt;

	平滑平均值 = ImLerp(平滑平均值, 真实平均, 平滑因子);
	float avg = 平滑平均值;

	// 最小最大
	if (min_fps == 0 || fps < min_fps) min_fps = fps;
	if (fps > max_fps) max_fps = fps;

	// 屏幕正中间上方
	float w = io.DisplaySize.x;
	float cx = w * 0.5f;

	// 动画 Y 坐标（弹出/收回）
	float 基准Y = 20.0f;
	float 当前Y = -50.0f + (基准Y + 50.0f) * animY; // 从屏幕外弹下来

	ImDrawList* dl = ImGui::GetForegroundDrawList();

	// 背景固定大小（你原来的）
	dl->AddRectFilled(
		{ cx - 210, 当前Y - 10 },
		{ cx + 210, 当前Y + 25 },
		IM_COL32(255, 255, 255, 200), 8.0f
	);

	char buf[128];

	// FPS
	snprintf(buf, sizeof(buf), "FPS: %.1f", fps);
	绘制描边文本(cx - 150 - ImGui::CalcTextSize(buf).x * 0.5f, 当前Y, buf,
		IM_COL32(0, 255, 255, 255), 1.0f, IM_COL32(0, 0, 0, 100));

	// Avg
	snprintf(buf, sizeof(buf), "Avg: %.1f", avg);
	绘制描边文本(cx - 50 - ImGui::CalcTextSize(buf).x * 0.5f, 当前Y, buf,
		IM_COL32(0, 255, 0, 255), 1.0f, IM_COL32(0, 0, 0, 100));

	// Min
	snprintf(buf, sizeof(buf), "Min: %.1f", min_fps);
	绘制描边文本(cx + 50 - ImGui::CalcTextSize(buf).x * 0.5f, 当前Y, buf,
		IM_COL32(255, 0, 0, 255), 1.0f, IM_COL32(0, 0, 0, 100));

	// Max
	snprintf(buf, sizeof(buf), "Max: %.1f", max_fps);
	绘制描边文本(cx + 150 - ImGui::CalcTextSize(buf).x * 0.5f, 当前Y, buf,
		IM_COL32(200, 100, 255, 255), 1.0f, IM_COL32(0, 0, 0, 100));
}

void 显示人数(D2D坐标 屏幕中心, int 玩家数量, int AI数量, bool 功能开关)
{
	// 动画静态变量
	static float s_animPlayer = 0.0f;
	static float s_animAi = 0.0f;
	static float s_curLeft = 0.0f;
	static float s_curRight = 0.0f;

	// 原先为帧级速度，改为每秒速度 + 帧无关平滑
	const float s_animSpeedPerSec = 3.6f;   // 相当于原来 0.06 在 60FPS 下的每秒速度
	const float s_lerp响应 = 12.0f;        // 位置平滑响应（每秒）
	const float s_threshold = 0.01f;

	ImGuiIO& io = ImGui::GetIO();
	float delta = io.DeltaTime;
	if (delta <= 0.0f) delta = 1.0f / 60.0f; // 容错

	// 1. 判断当前有没有内容
	bool hasPlayer = (玩家数量 > 0);
	bool hasAi = (AI数量 > 0);

	// 2. 控制淡入/淡出（基于 delta 实现帧无关速度）
	if (功能开关)
	{
		// 开启：按每秒速度增减
		if (hasPlayer) s_animPlayer = ImMin(s_animPlayer + s_animSpeedPerSec * delta, 1.0f);
		else           s_animPlayer = ImMax(s_animPlayer - s_animSpeedPerSec * delta, 0.0f);

		if (hasAi)     s_animAi = ImMin(s_animAi + s_animSpeedPerSec * delta, 1.0f);
		else           s_animAi = ImMax(s_animAi - s_animSpeedPerSec * delta, 0.0f);
	}
	else
	{
		// 关闭：全部按每秒速度减到0（退场动画）
		s_animPlayer = ImMax(s_animPlayer - s_animSpeedPerSec * delta, 0.0f);
		s_animAi = ImMax(s_animAi - s_animSpeedPerSec * delta, 0.0f);
	}

	// 整体动画透明度
	float animAlpha = ImMax(s_animPlayer, s_animAi);

	// 完全看不见才重置位置，避免下次开启突兀
	if (animAlpha <= s_threshold)
	{
		s_curLeft = 0.0f;
		s_curRight = 0.0f;
		return;
	}

	// 拼接文字
	std::string strPlayer, strAi;
	if (hasPlayer) strPlayer = GBK转UTF8("玩家: ") + std::to_string(玩家数量);
	if (hasAi)     strAi = GBK转UTF8("人机: ") + std::to_string(AI数量);

	ImVec2 pad = { 16, 10 };
	ImVec2 szPlayer = ImGui::CalcTextSize(strPlayer.c_str());
	ImVec2 szAi = ImGui::CalcTextSize(strAi.c_str());
	float 最大文字高度 = ImMax(szPlayer.y, szAi.y);

	// 计算目标左右边界
	float leftTarget = 0.0f, rightTarget = 0.0f;
	if (hasPlayer && !hasAi)
	{
		leftTarget = -szPlayer.x * 0.5f - pad.x;
		rightTarget = szPlayer.x * 0.5f + pad.x;
	}
	else if (!hasPlayer && hasAi)
	{
		leftTarget = -szAi.x * 0.5f - pad.x;
		rightTarget = szAi.x * 0.5f + pad.x;
	}
	else
	{
		leftTarget = -szPlayer.x - pad.x - 10.0f;
		rightTarget = szAi.x + pad.x + 10.0f;
	}

	// 平滑插值位置（帧无关：使用指数响应转换为当前帧因子）
	float lerpFactor = 1.0f - expf(-s_lerp响应 * delta);
	s_curLeft = ImLerp(s_curLeft, leftTarget, lerpFactor);
	s_curRight = ImLerp(s_curRight, rightTarget, lerpFactor);

	// 绘制
	float boxY = 130.0f;
	float 文字Y坐标 = boxY + pad.y;
	ImDrawList* draw = ImGui::GetForegroundDrawList();

	int alpha = (int)(200 * animAlpha);
	ImU32 背景色 = IM_COL32(255, 255, 255, alpha);
	ImU32 边框色 = IM_COL32(180, 180, 180, alpha);
	ImU32 文字阴影 = IM_COL32(0, 0, 0, 100);

	ImVec2 方框左上 = { 屏幕中心.x + s_curLeft, boxY };
	ImVec2 方框右下 = { 屏幕中心.x + s_curRight, boxY + 最大文字高度 + pad.y * 2.0f };

	draw->AddRectFilled(方框左上, 方框右下, 背景色, 8.0f);
	draw->AddRect(方框左上, 方框右下, 边框色, 8.0f, 0, 1.2f);

	// 玩家文字
	if (s_animPlayer > s_threshold)
	{
		int tAlpha = (int)(255 * s_animPlayer);
		float x = 屏幕中心.x + s_curLeft + pad.x;
		绘制描边文本(x, 文字Y坐标, strPlayer.c_str(), IM_COL32(255, 0, 0, tAlpha), 1.0f, 文字阴影);
	}

	// 人机文字
	if (s_animAi > s_threshold)
	{
		int tAlpha = (int)(255 * s_animAi);
		float x = 屏幕中心.x + s_curRight - szAi.x - pad.x;
		绘制描边文本(x, 文字Y坐标, strAi.c_str(), IM_COL32(0, 255, 255, tAlpha), 1.0f, 文字阴影);
	}
}

void 绘制准星(float 中心X,float 中心Y,ImU32 颜色,float 圆环半径,float 扩散范围,float 线条粗细,float 十字线长度,float 中心圆点半径 ){
	ImDrawList* 绘制列表 = ImGui::GetForegroundDrawList();

	绘制列表->AddCircle(ImVec2(中心X, 中心Y), 圆环半径, 颜色, 12, 线条粗细);

	// 上方向十字线（从圆环顶端向上延伸）
	绘制列表->AddLine(
		ImVec2(中心X, 中心Y - 扩散范围),
		ImVec2(中心X, 中心Y - 扩散范围 - 十字线长度),
		颜色,
		线条粗细
	);
	// 下方向十字线（从圆环底端向下延伸）
	绘制列表->AddLine(
		ImVec2(中心X, 中心Y + 扩散范围),
		ImVec2(中心X, 中心Y + 扩散范围 + 十字线长度),
		颜色,
		线条粗细
	);
	// 左方向十字线（从圆环左端向左延伸）
	绘制列表->AddLine(
		ImVec2(中心X - 扩散范围, 中心Y),
		ImVec2(中心X - 扩散范围 - 十字线长度, 中心Y),
		颜色,
		线条粗细
	);
	// 右方向十字线（从圆环右端向右延伸）
	绘制列表->AddLine(
		ImVec2(中心X + 扩散范围, 中心Y),
		ImVec2(中心X + 扩散范围 + 十字线长度, 中心Y),
		颜色,
		线条粗细
	);

	// 中心瞄准点
	绘制列表->AddCircleFilled(ImVec2(中心X, 中心Y), 中心圆点半径, 红色);
}
static ImU32 LerpColorU32(ImU32 a, ImU32 b, float t) {
	ImColor ca(a), cb(b);
	ImVec4 A = ca.Value, B = cb.Value;
	ImVec4 R;
	R.x = A.x * (1.0f - t) + B.x * t;
	R.y = A.y * (1.0f - t) + B.y * t;
	R.z = A.z * (1.0f - t) + B.z * t;
	R.w = A.w * (1.0f - t) + B.w * t;
	return ImGui::ColorConvertFloat4ToU32(R);
}

struct _SnowParticle {
	D3D坐标 pos;
	D3D坐标 vel;
	float size;
	float life;
	float seed;
	float delay;
	bool active;
};

void RenderSnow(D3D坐标 rolePos, const Matrix& matrix,
	float speedFactor,// 雪花速度因子
	int particleCount,// 粒子数量
	float topHeight, // 雪花生成的顶部高度
	float bottomThreshold, // 雪花的底部阈值
	float horizontalRadius //粒子水平范围
) {
	if (particleCount <= 0) return;
	if (topHeight <= 0.1f) topHeight = 12.0f;
	if (bottomThreshold <= 0.1f) bottomThreshold = 30.0f;
	if (horizontalRadius <= 0.1f) horizontalRadius = 10.0f;
	if (speedFactor <= 0.0f) speedFactor = 1.0f;

	static std::vector<_SnowParticle> s_particles;
	static std::mt19937_64 s_rng((unsigned)std::chrono::steady_clock::now().time_since_epoch().count());
	static std::uniform_real_distribution<float> s_u01(0.0f, 1.0f);

	auto randf = [&](float a, float b) { return a + (b - a) * s_u01(s_rng); };

	if (rolePos.x == 0 && rolePos.y == 0 && rolePos.z == 0) return;

	if ( (int)s_particles.size() != particleCount) {
		s_particles.clear();
		s_particles.reserve(particleCount);

		float totalSpawnTime = randf(3.0f, 5.0f);
		for (int i = 0; i < particleCount; ++i) {
			_SnowParticle p;
			p.pos.x = rolePos.x + randf(-horizontalRadius, horizontalRadius);
			p.pos.y = rolePos.y + randf(0.8f * topHeight, topHeight);
			p.pos.z = rolePos.z + randf(-horizontalRadius, horizontalRadius);
			p.vel.x = randf(-0.3f, 0.3f);
			p.vel.y = randf(-0.5f, -1.5f);
			p.vel.z = randf(-0.3f, 0.3f);
			p.size = randf(2.0f, 6.0f);
			p.life = randf(6.0f, 25.0f);
			p.seed = randf(0.0f, 1000.0f);
			p.delay = randf(0.0f, totalSpawnTime);
			p.active = false;
			s_particles.push_back(p);
		}
		if (particleCount > 0) {
			s_particles[0].delay = 0.0f;
			s_particles[0].active = true;
		}
	}

	ImGuiIO& io = ImGui::GetIO();
	float dt = io.DeltaTime;
	if (dt <= 0.0f) dt = 1.0f / 60.0f;
	float step = dt * speedFactor;

	float gravity = 9.0f * 0.22f * speedFactor;
	float windBase = 0.35f * speedFactor;
	float t = ImGui::GetTime();

	ImDrawList* dl = ImGui::GetForegroundDrawList();
	ImVec2 disp = io.DisplaySize;
	D2D坐标 screenCenter = { disp.x * 0.5f, disp.y * 0.5f };

	const float kSnowMaxSize = 24.0f; // 若项目中已有 Display::snowMaxSize 可替换

	for (auto& p : s_particles) {
		if (!p.active) {
			p.delay -= step;
			if (p.delay <= 0.0f) {
				p.active = true;
				p.pos.x = rolePos.x + randf(-horizontalRadius, horizontalRadius);
				p.pos.y = rolePos.y + randf(0.2f * topHeight, topHeight);
				p.pos.z = rolePos.z + randf(-horizontalRadius, horizontalRadius);
				p.vel.x = randf(-0.35f, 0.35f);
				p.vel.y = randf(-0.6f, -1.8f);
				p.vel.z = randf(-0.35f, 0.35f);
				p.size = randf(2.0f, 6.0f);
				p.life = randf(6.0f, 25.0f);
				p.seed = randf(0.0f, 1000.0f);
			}
			continue;
		}

		float windX = sinf(t * 0.18f + p.seed * 0.01f) * windBase;
		float windZ = cosf(t * 0.13f + p.seed * 0.017f) * (windBase * 0.6f);

		p.vel.x += windX * step * 0.6f;
		p.vel.z += windZ * step * 0.6f;
		p.vel.y -= gravity * step;

		p.pos.x += p.vel.x * step;
		p.pos.y += p.vel.y * step;
		p.pos.z += p.vel.z * step;

		p.life -= step;

		if (p.life <= 0.0f || p.pos.y < (rolePos.y - bottomThreshold)) {
			p.pos.x = rolePos.x + randf(-horizontalRadius, horizontalRadius);
			p.pos.y = rolePos.y + randf(0.2f * topHeight, topHeight);
			p.pos.z = rolePos.z + randf(-horizontalRadius, horizontalRadius);
			p.vel.x = randf(-0.35f, 0.35f);
			p.vel.y = randf(-0.6f, -1.8f);
			p.vel.z = randf(-0.35f, 0.35f);
			p.size = randf(2.0f, 6.0f);
			p.life = randf(6.0f, 25.0f);
			p.seed = randf(0.0f, 1000.0f);
		}

		D2D坐标 sp{};
		// 使用工程内投影函数，screenCenter 作为 GameScreen
		if (D3D转2D坐标(p.pos, sp, matrix, screenCenter)) {
			float depth = fabsf(p.pos.z - rolePos.z) + fabsf(p.pos.x - rolePos.x) + 1.0f;
			float scale = 1.0f / (0.02f * depth + 0.12f);
			float drawSize = p.size * scale;
			if (drawSize < 0.6f) drawSize = 0.6f;
			if (drawSize > kSnowMaxSize) drawSize = kSnowMaxSize;
			float alpha = ImSaturate(0.95f * (p.life / 12.0f));
			if (alpha < 0.06f) alpha = 0.06f;
			int ia = static_cast<int>(ImClamp(255.0f * alpha, 0.0f, 255.0f));
			ImU32 col = IM_COL32(255, 255, 255, ia);
			dl->AddCircleFilled(ImVec2(sp.x, sp.y), drawSize, col, 10);
			int haloA = static_cast<int>(ImClamp(120.0f * alpha, 0.0f, 255.0f));
			ImU32 halo = IM_COL32(240, 250, 255, haloA);
			dl->AddCircle(ImVec2(sp.x, sp.y), drawSize * 1.6f, halo, 8, 1.0f);
		}
	}
}

// 全局随机数引擎
static std::default_random_engine rng(std::chrono::steady_clock::now().time_since_epoch().count());
static std::uniform_real_distribution<float> noiseDist(-1.0f, 1.0f);
// 绘制脚底伪3D圆（彩虹旋转 + 轻微心电图）
void DrawFootCircle3D(glm::vec3 footPos, float worldRadius, int lineWidth, Matrix viewMatrix, glm::vec2 screenSize) {
	// 每帧更新一次（防止同一帧多次调用导致重复累加）
	static int lastFrame = -1;
	int frame = ImGui::GetFrameCount();
	// 动态状态
	static float rotationAngle = 0.0f;    // deg
	static float phase = 0.0f;            // 心电相位
	static float ecgOffset = 0.0f;

	// 时间步（以秒计），使用 ImGui 的 DeltaTime（回退到 1ms）
	ImGuiIO& io = ImGui::GetIO();
	float dt = ImMax(1.0f / 1000.0f, io.DeltaTime);

	// 只在新帧更新状态
	if (frame != lastFrame) {
		lastFrame = frame;

		const float rotationSpeedDegPerSec = 45.0f;    // 每秒旋转角度（可调）
		const float phaseSpeedPerSec = 4.5f;           // 相位速度（rad/s），控制心跳频率

		rotationAngle += rotationSpeedDegPerSec * dt;
		if (rotationAngle >= 360.0f) rotationAngle = fmodf(rotationAngle, 360.0f);

		phase += phaseSpeedPerSec * dt;
		// 心电振幅 + 小幅噪声（noiseDist/rng 在文件顶部已定义）
		float beat = sinf(phase) * 0.01f;            // ±1%
		float noise = noiseDist(rng) * 0.003f;      // ±0.3%
		ecgOffset = beat + noise;
	}

	// 颜色表
	std::vector<ImU32> colors = {
		IM_COL32(255, 0, 0, 255),
		IM_COL32(255, 255, 0, 255),
		IM_COL32(0, 255, 0, 255),
		IM_COL32(0, 0, 255, 255),
		IM_COL32(255, 0, 255, 255),
		IM_COL32(0, 255, 255, 255),
		IM_COL32(255, 165, 0, 255)
	};

	const float PI = 3.14159265358979323846f;
	const int segments = 360;
	std::vector<glm::vec2> screenPts(segments);
	std::vector<char> valid(segments, 0);

	float currentRadius = worldRadius * (1.0f + ecgOffset);
	if (currentRadius <= 0.0f) currentRadius = worldRadius;

	for (int i = 0; i < segments; ++i) {
		float angleDeg = static_cast<float>(i) + rotationAngle;
		float rad = angleDeg * PI / 180.0f;

		glm::vec3 worldPt = footPos + glm::vec3(
			currentRadius * cosf(rad),
			0.0f,
			currentRadius * sinf(rad)
		);

		D2D坐标 screenPt;
		if (D3D转2D坐标(worldPt, screenPt, viewMatrix, screenSize)) {
			screenPts[i] = screenPt;
			valid[i] = 1;
		}
		else {
			valid[i] = 0;
		}
	}

	// 绘制
	ImDrawList* drawList = ImGui::GetForegroundDrawList();
	for (int i = 0; i < segments; ++i) {
		int next = (i + 1) % segments;
		if (valid[i] && valid[next]) {
			int colorIdx = (i / 14) % (int)colors.size();
			drawList->AddLine(
				ImVec2(screenPts[i].x, screenPts[i].y),
				ImVec2(screenPts[next].x, screenPts[next].y),
				colors[colorIdx],
				(float)lineWidth
			);
		}
	}
}











// 写入配置
bool WriteConfig(LPCSTR key, LPCSTR value) {
	return WritePrivateProfileStringA("保存配置", key, value, ".\\TL-C++.ini");
}
// 读取字符串
std::string ReadConfig(LPCSTR key, LPCSTR defaultVal = "0") {
	char buf[256] = { 0 };
	GetPrivateProfileStringA("保存配置", key, defaultVal, buf, sizeof(buf), ".\\TL-C++.ini");
	return std::string(buf);
}
// 读取bool（开关）
bool ReadConfigBool(LPCSTR key, bool defaultVal = false) {
	std::string defaultStr = defaultVal ? "1" : "0";
	char buf[256] = { 0 };
	GetPrivateProfileStringA("保存配置", key, defaultStr.c_str(), buf, sizeof(buf), ".\\TL-C++.ini");
	return atoi(buf) != 0;
}
// 读取int（整数）
int ReadConfigInt(LPCSTR key, int defaultVal = 0) {
	std::string defaultStr = std::to_string(defaultVal);
	char buf[256] = { 0 };
	GetPrivateProfileStringA("保存配置", key, defaultStr.c_str(), buf, sizeof(buf), ".\\TL-C++.ini");
	return atoi(buf);
}
// 读取float（浮点数）
float ReadConfigFloat(LPCSTR key, float defaultVal = 0.0f) {
	std::string defaultStr = std::to_string(defaultVal);
	char buf[256] = { 0 };
	GetPrivateProfileStringA("保存配置", key, defaultStr.c_str(), buf, sizeof(buf), ".\\TL-C++.ini");
	return (float)atof(buf);
}

// 从 ImU32 解出分量
static void ImU32_To_RGBA(ImU32 c, unsigned int& r, unsigned int& g, unsigned int& b, unsigned int& a) {
	r = (c) & 0xFF;
	g = (c >> 8) & 0xFF;
	b = (c >> 16) & 0xFF;
	a = (c >> 24) & 0xFF;
}

// 按可读顺序保存为 0xRRGGBBAA（或分别保存分量）
bool WriteConfigColorHexHuman(LPCSTR key, ImU32 color) {
	unsigned int r, g, b, a;
	ImU32_To_RGBA(color, r, g, b, a);
	char buf[16];
	sprintf_s(buf, "0x%02X%02X%02X%02X", r, g, b, a); // 人类可读顺序
	return WriteConfig(key, buf);
}

ImU32 ReadConfigColorHexHuman(LPCSTR key, ImU32 defaultColor) {
	std::string s = ReadConfig(key);
	if (s.empty()) return defaultColor;
	// 去掉可能的 "0x"
	if (s.rfind("0x", 0) == 0 || s.rfind("0X", 0) == 0) s = s.substr(2);
	if (s.length() != 8) return defaultColor;
	unsigned int r = (unsigned int)strtoul(s.substr(0, 2).c_str(), nullptr, 16);
	unsigned int g = (unsigned int)strtoul(s.substr(2, 2).c_str(), nullptr, 16);
	unsigned int b = (unsigned int)strtoul(s.substr(4, 2).c_str(), nullptr, 16);
	unsigned int a = (unsigned int)strtoul(s.substr(6, 2).c_str(), nullptr, 16);
	return IM_COL32(r, g, b, a); // 正确重建 ImU32
}


void 保存配置(){
	// 保存 显示 模块
	WriteConfig("绘制方框", 显示::绘制方框 ? "1" : "0");
	WriteConfig("绘制信息", 显示::绘制信息 ? "1" : "0");
	WriteConfig("绘制队友", 显示::绘制队友 ? "1" : "0");
	WriteConfig("显示人数", 显示::显示人数 ? "1" : "0");
	WriteConfig("绘制射线", 显示::绘制射线 ? "1" : "0");
	WriteConfig("显示头部", 显示::显示头部 ? "1" : "0");
	WriteConfig("显示骨骼", 显示::显示骨骼 ? "1" : "0");
	WriteConfig("显示人机", 显示::显示人机 ? "1" : "0");
	WriteConfig("显示载具", 显示::显示载具 ? "1" : "0");
	WriteConfig("被瞄提醒", 显示::被瞄提醒 ? "1" : "0");
	WriteConfig("开启提示", 显示::开启提示 ? "1" : "0");
	WriteConfig("绘制准星", 显示::绘制准星 ? "1" : "0");
	WriteConfig("主播模式", 显示::主播模式 ? "1" : "0");
	WriteConfig("FPS面板", 显示::FPS面板 ? "1" : "0");
	WriteConfig("信息类型", std::to_string(显示::信息类型).c_str());
	WriteConfig("方框类型", std::to_string(显示::方框类型).c_str());
	WriteConfig("射线位置", std::to_string(显示::射线位置).c_str());
	WriteConfig("载具类型", std::to_string(显示::载具类型).c_str());

	// 保存 物资 模块
	WriteConfig("显示物资", 物资::显示物资 ? "1" : "0");
	WriteConfig("物资堆叠", 物资::物资堆叠 ? "1" : "0");
	WriteConfig("步枪", 物资::步枪 ? "1" : "0");
	WriteConfig("冲锋枪", 物资::冲锋枪 ? "1" : "0");
	WriteConfig("射手步枪", 物资::射手步枪 ? "1" : "0");
	WriteConfig("狙击枪", 物资::狙击枪 ? "1" : "0");
	WriteConfig("机枪", 物资::机枪 ? "1" : "0");
	WriteConfig("霰弹枪", 物资::霰弹枪 ? "1" : "0");
	WriteConfig("手枪", 物资::手枪 ? "1" : "0");
	WriteConfig("特殊武器", 物资::特殊武器 ? "1" : "0");
	WriteConfig("暗器", 物资::暗器 ? "1" : "0");
	WriteConfig("常用装备", 物资::常用装备 ? "1" : "0");
	WriteConfig("常用身份", 物资::常用身份 ? "1" : "0");
	WriteConfig("常用药品", 物资::常用药品 ? "1" : "0");
	WriteConfig("常用倍镜", 物资::常用倍镜 ? "1" : "0");
	WriteConfig("常用配件", 物资::常用配件 ? "1" : "0");
	WriteConfig("常用芯片", 物资::常用芯片 ? "1" : "0");
	WriteConfig("常用投掷", 物资::常用投掷 ? "1" : "0");
	WriteConfig("常用近战", 物资::常用近战 ? "1" : "0");
	WriteConfig("常用子弹", 物资::常用子弹 ? "1" : "0");

	// 保存 自瞄 模块
	WriteConfig("开启自瞄", 自瞄::开启自瞄 ? "1" : "0");
	WriteConfig("显示范围", 自瞄::显示范围 ? "1" : "0");
	WriteConfig("空手不瞄", 自瞄::空手不瞄 ? "1" : "0");
	WriteConfig("瞄准位置", 自瞄::瞄准位置 ? "1" : "0");
	WriteConfig("开启预判", 自瞄::开启预判 ? "1" : "0");
	WriteConfig("鼠标自瞄位置", std::to_string(自瞄::鼠标自瞄::自瞄位置).c_str());
	WriteConfig("自瞄热键", std::to_string(自瞄::自瞄热键).c_str());
	WriteConfig("鼠标自瞄速度", std::to_string(自瞄::鼠标自瞄::自瞄速度).c_str());
	WriteConfig("自瞄范围", std::to_string(自瞄::自瞄范围).c_str());
	WriteConfig("自瞄算法", std::to_string(自瞄::自瞄算法).c_str());
	WriteConfig("内存自瞄速度", std::to_string(自瞄::内存自瞄::自瞄速度).c_str());
	WriteConfig("内存自瞄位置", std::to_string(自瞄::内存自瞄::自瞄位置).c_str());

	//保存 颜色 模块
	WriteConfigColorHexHuman("方框颜色", 颜色::方框颜色);
	WriteConfigColorHexHuman("射线颜色", 颜色::射线颜色);
	WriteConfigColorHexHuman("自瞄范围颜色", 颜色::自瞄范围);
	WriteConfigColorHexHuman("骨骼颜色", 颜色::骨骼颜色);
	WriteConfigColorHexHuman("人机颜色", 颜色::人机颜色);
	WriteConfigColorHexHuman("载具背景", 颜色::载具背景);

	//保存 菜单 模块
	WriteConfig("开启提示位置x", std::to_string(本人数据::开启提示位置x).c_str());
	WriteConfig("开启提示位置y", std::to_string(本人数据::开启提示位置y).c_str());
	WriteConfig("开启提示透明度", std::to_string(本人数据::开启提示透明度).c_str());

}

void 读取配置(){
	// 加载 显示 模块
	显示::绘制方框 = ReadConfigBool("绘制方框", true);
	显示::绘制信息 = ReadConfigBool("绘制信息", true);
	显示::绘制队友 = ReadConfigBool("绘制队友", false);
	显示::显示人数 = ReadConfigBool("显示人数", true);
	显示::绘制射线 = ReadConfigBool("绘制射线", true);
	显示::显示头部 = ReadConfigBool("显示头部", true);
	显示::显示骨骼 = ReadConfigBool("显示骨骼", true);
	显示::显示人机 = ReadConfigBool("显示人机", true);
	显示::显示载具 = ReadConfigBool("显示载具", true);
	显示::被瞄提醒 = ReadConfigBool("被瞄提醒", true);
	显示::开启提示 = ReadConfigBool("开启提示", true);
	显示::绘制准星 = ReadConfigBool("绘制准星", true);
	显示::主播模式 = ReadConfigBool("主播模式", false);
	显示::FPS面板 = ReadConfigBool("FPS面板", true);
	显示::信息类型 = ReadConfigInt("信息类型", 0);
	显示::方框类型 = ReadConfigInt("方框类型", 0);
	显示::射线位置 = ReadConfigInt("射线位置", 0);
	显示::载具类型 = ReadConfigInt("载具类型", 0);

	// 加载 物资 模块
	物资::显示物资 = ReadConfigBool("显示物资", false);
	物资::物资堆叠 = ReadConfigBool("物资堆叠", true);
	物资::步枪 = ReadConfigBool("步枪", false);
	物资::冲锋枪 = ReadConfigBool("冲锋枪", false);
	物资::射手步枪 = ReadConfigBool("射手步枪", false);
	物资::狙击枪 = ReadConfigBool("狙击枪", false);
	物资::机枪 = ReadConfigBool("机枪", false);
	物资::霰弹枪 = ReadConfigBool("霰弹枪", false);
	物资::手枪 = ReadConfigBool("手枪", false);
	物资::特殊武器 = ReadConfigBool("特殊武器", false);
	物资::暗器 = ReadConfigBool("暗器", false);
	物资::常用装备 = ReadConfigBool("常用装备", false);
	物资::常用身份 = ReadConfigBool("常用身份", false);
	物资::常用药品 = ReadConfigBool("常用药品", false);
	物资::常用倍镜 = ReadConfigBool("常用倍镜", false);
	物资::常用配件 = ReadConfigBool("常用配件", false);
	物资::常用芯片 = ReadConfigBool("常用芯片", false);
	物资::常用投掷 = ReadConfigBool("常用投掷", false);
	物资::常用近战 = ReadConfigBool("常用近战", false);
	物资::常用子弹 = ReadConfigBool("常用子弹", false);

	// 加载 自瞄 模块
	自瞄::开启自瞄 = ReadConfigBool("开启自瞄", false);
	自瞄::显示范围 = ReadConfigBool("显示范围", true);
	自瞄::空手不瞄 = ReadConfigBool("空手不瞄", true);
	自瞄::瞄准位置 = ReadConfigBool("瞄准位置", true);
	自瞄::开启预判 = ReadConfigBool("开启预判", true);
	自瞄::鼠标自瞄::自瞄位置 = ReadConfigInt("鼠标自瞄位置", 1);
	自瞄::自瞄热键 = ReadConfigInt("自瞄热键", 2);
	自瞄::鼠标自瞄::自瞄速度 = ReadConfigFloat("鼠标自瞄速度", 7.0f);
	自瞄::自瞄范围 = ReadConfigInt("自瞄范围", 300);
	自瞄::自瞄算法 = ReadConfigInt("自瞄算法", 0);
	自瞄::内存自瞄::自瞄位置 = ReadConfigInt("内存自瞄位置", 0);
	自瞄::内存自瞄::自瞄速度 = ReadConfigFloat("内存自瞄速度", 10.0f);

	// 加载 颜色 模块
	颜色::方框颜色 = ReadConfigColorHexHuman("方框颜色", IM_COL32(0, 255, 255, 255));
	颜色::射线颜色 = ReadConfigColorHexHuman("射线颜色", IM_COL32(255, 255, 255, 255));
	颜色::自瞄范围 = ReadConfigColorHexHuman("自瞄范围颜色", IM_COL32(255, 255, 255, 255));
	颜色::骨骼颜色 = ReadConfigColorHexHuman("骨骼颜色", IM_COL32(0, 255, 255, 255));
	颜色::人机颜色 = ReadConfigColorHexHuman("人机颜色", IM_COL32(255, 0, 255, 255));
	颜色::载具背景 = ReadConfigColorHexHuman("载具背景", IM_COL32(255, 0, 255, 255));

	//加载 菜单 模块
	本人数据::开启提示位置x = ReadConfigInt("开启提示位置x", 12);
	本人数据::开启提示位置y = ReadConfigInt("开启提示位置y", 320);
	本人数据::开启提示透明度 = ReadConfigFloat("开启提示透明度", 0.8f);

}