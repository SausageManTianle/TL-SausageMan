//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include "Memory.h"
#include "..\..\GUI\GUI.h"
#include "..\Other\Other.h"

UINT64* 取子弹数量() {
	uintptr_t 临时 = 0;
	if (!本人数据::本人地址 || 本人数据::本人地址 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::roleLogicClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::UserWeapon);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	return reinterpret_cast<UINT64*>(临时 + 数据::子弹数量);
}

UINT64* 取全枪自动() {
	uintptr_t 临时 = 本人数据::本人地址;
	if (!临时 || 临时 == (uintptr_t)-1) return nullptr;

	uintptr_t 读取结果 = 0;
	SIZE_T 读取字节数 = 0;

	// 安全读取 roleLogicClient
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)(临时 + BattleRoleLogic::roleLogicClient), &读取结果, sizeof(uintptr_t), &读取字节数) || 读取字节数 != sizeof(uintptr_t)) {
		return nullptr;
	}
	临时 = 读取结果;
	if (!临时 || 临时 == (uintptr_t)-1) return nullptr;

	// 安全读取 0x30
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)(临时 + BattleRole::RoleClient), &读取结果, sizeof(uintptr_t), &读取字节数) || 读取字节数 != sizeof(uintptr_t)) {
		return nullptr;
	}
	临时 = 读取结果;
	if (!临时 || 临时 == (uintptr_t)-1) return nullptr;

	// 安全读取 UserWeapon
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)(临时 + BattleRole::UserWeapon), &读取结果, sizeof(uintptr_t), &读取字节数) || 读取字节数 != sizeof(uintptr_t)) {
		return nullptr;
	}
	临时 = 读取结果;
	if (!临时 || 临时 == (uintptr_t)-1) return nullptr;

	return reinterpret_cast<UINT64*>(临时 + 数据::枪械自动);
}

UINT32* 取射速加倍() {
	uintptr_t 临时 = 0;
	if (!本人数据::本人地址 || 本人数据::本人地址 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::roleLogicClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::UserWeapon);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	return reinterpret_cast<UINT32*>(临时 + 数据::枪械射速);
}

float* 取全枪聚点() {
	uintptr_t 临时 = 0;
	if (!本人数据::本人地址 || 本人数据::本人地址 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::roleLogicClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::UserWeapon);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	return reinterpret_cast<float*>(临时 + 数据::子弹扩散);
}

UINT64* 取加特林热() {
	uintptr_t 临时 = 0;
	if (!本人数据::本人地址 || 本人数据::本人地址 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::roleLogicClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::UserWeapon);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	return reinterpret_cast<UINT64*>(临时 + 数据::枪械热量);
}

float* 取加特林预热() {
	uintptr_t 临时 = 0;
	if (!本人数据::本人地址 || 本人数据::本人地址 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::roleLogicClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::UserWeapon);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	return reinterpret_cast<float*>(临时 + 数据::枪械预热);
}

float* 取人物高跳() {
	/*
	uintptr_t 临时 = 0;
	if (!本人数据::本人地址 || 本人数据::本人地址 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::RoleBuffControl);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	return reinterpret_cast<float*>(临时 + RoleBuffControl::JumpNum);
	*/
	uintptr_t 临时 = 本人数据::本人地址;
	if (!临时 || 临时 == (uintptr_t)-1) return nullptr;

	uintptr_t 读取结果 = 0;
	SIZE_T 读取字节数 = 0;

	// 安全读取 RoleBuffControl
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)(本人数据::本人地址 + BattleRoleLogic::RoleBuffControl), &读取结果, sizeof(uintptr_t), &读取字节数) || 读取字节数 != sizeof(uintptr_t)) {
		return nullptr;
	}
	临时 = 读取结果;
	if (!临时 || 临时 == (uintptr_t)-1) return nullptr;

	return reinterpret_cast<float*>(临时 + RoleBuffControl::JumpNum);
}

float* 取人物加速() {
	uintptr_t 临时 = 0;
	if (!本人数据::本人地址 || 本人数据::本人地址 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::RoleBuffControl);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	return reinterpret_cast<float*>(临时 + RoleBuffControl::WalkNum);
}

float* 取视角Z() {
	uintptr_t 临时 = 本人数据::本人地址;
	if (!临时 || 临时 == (uintptr_t)-1) return nullptr;

	uintptr_t 读取结果 = 0;
	SIZE_T 读取字节数 = 0;

	// 安全读取 RoleBuffControl
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)(临时 + BattleRoleLogic::RoleBuffControl), &读取结果, sizeof(uintptr_t), &读取字节数) || 读取字节数 != sizeof(uintptr_t)) {
		return nullptr;
	}
	临时 = 读取结果;
	if (!临时 || 临时 == (uintptr_t)-1) return nullptr;

	return reinterpret_cast<float*>(临时 + RoleBuffControl::CameraRatio + 8);
}

float* 取人物旋转() {
	if (!本人数据::本人地址 || 本人数据::本人地址 == (uintptr_t)-1)return nullptr;
	uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::roleLogicClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);
	if (!临时 || 临时 == (uintptr_t)-1)return nullptr;
	return reinterpret_cast<float*>(临时 + BattleRole::lastRotaY);
}

static bool 本人地址有效() {
	if (!本人数据::本人地址 || 本人数据::本人地址 == (uintptr_t)-1) return false;

	uintptr_t 读取结果 = 0;
	SIZE_T 读取字节数 = 0;

	// 读取 roleLogicClient 指针并校验
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)(本人数据::本人地址 + BattleRoleLogic::roleLogicClient), &读取结果, sizeof(uintptr_t), &读取字节数) || 读取字节数 != sizeof(uintptr_t))
		return false;
	if (!读取结果 || 读取结果 == (uintptr_t)-1) return false;

	// 读取 RoleClient 指针并校验
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)(读取结果 + BattleRole::RoleClient), &读取结果, sizeof(uintptr_t), &读取字节数) || 读取字节数 != sizeof(uintptr_t))
		return false;
	if (!读取结果 || 读取结果 == (uintptr_t)-1) return false;

	return true;
}


void Memory_Main() {
	if (!本人地址有效()) return;

	if (内存::无限子弹) {
		UINT64* 子弹 = 取子弹数量();
		if (子弹 && (uintptr_t)子弹 != (uintptr_t)-1) *子弹 = 5201314;
	}
	if (内存::子弹爆射) {
		UINT32* 射速 = 取射速加倍();
		if (射速 && (uintptr_t)射速 != (uintptr_t)-1) *射速 = 0x3F800000;
	}
	if (内存::全枪自动) {
		UINT64* 自动 = 取全枪自动();
		if (自动 && (uintptr_t)自动 != (uintptr_t)-1) *自动 = 1;
	}
	if (内存::全枪聚点) {
		float* 聚点 = 取全枪聚点();
		if (聚点 && (uintptr_t)聚点 != (uintptr_t)-1) *聚点 = 0;
	}
	if (内存::加特不热) {
		UINT64* 热量 = 取加特林热();
		float* 预热 = 取加特林预热();
		if (热量 && (uintptr_t)热量 != (uintptr_t)-1) *热量 = 0;
		if (预热 && (uintptr_t)预热 != (uintptr_t)-1) *预热 = 1.0f;
	}

	float* 视角 = 取视角Z();
	if (视角 && (uintptr_t)视角 != (uintptr_t)-1) {
		if (内存::广角视野) {
			float 目标广角 = 内存::广角大小;
			if (fabsf(*视角 - 目标广角) > 0.001f) *视角 = 目标广角;
		}
	}
	float* 高跳 = 取人物高跳();
	if (高跳 && (uintptr_t)高跳 != (uintptr_t)-1) {
		float 当前高跳值 = *高跳;
		float 目标高跳值 = 内存::人物高跳 ? 内存::高跳大小 : 0.0f;
		if (fabsf(当前高跳值 - 目标高跳值) > 0.001f)*高跳 = 目标高跳值;
	}
	float* 加速 = 取人物加速();
	if (加速 && (uintptr_t)加速 != (uintptr_t)-1) {
		float 当前加速值 = *加速;
		float 目标加速值 = 内存::超级加速 ? 内存::超级加速值 : 0.0f;
		if (fabsf(当前加速值 - 目标加速值) > 0.001f)*加速 = 目标加速值;
	}

	static float 人物旋转 = 0;
	if (内存::人物旋转) {
		if (人物旋转 >= 360) 人物旋转 = 0;
		人物旋转 = 人物旋转 + 20 * 0.5f;
		float* 旋转指针 = 取人物旋转();
		if (旋转指针 != nullptr && (uintptr_t)旋转指针 != (uintptr_t)-1) *旋转指针 = 人物旋转;
	}


	static bool 上次勾选[10] = {};

	if (内存::主播无后 != 上次勾选[0]) {
		if (内存::主播无后)
		写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::主播无后, { 0xC3 }); 
		else 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::主播无后, { 0x48,0x8B,0xC4 });
		上次勾选[0] = 内存::主播无后;
	}
	if (内存::超级无后 != 上次勾选[1]) {
		if (内存::超级无后) 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::超级无后, { 0xC3 });
		else 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::超级无后, { 0x40,0x53 });
		上次勾选[1] = 内存::超级无后;
	}
	if (内存::无视缺氧 != 上次勾选[2]) {
		if (内存::无视缺氧) 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::无视缺氧, { 0xC3 });
		else 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::无视缺氧, { 0x40,0x53 });
		上次勾选[2] = 内存::无视缺氧;
	}
	if (内存::载具锁油 != 上次勾选[3]) {
		if (内存::载具锁油) 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::载具锁油, { 0x90,0x90 });
		else 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::载具锁油, { 0x74,0x0D });
		上次勾选[3] = 内存::载具锁油;
	}
	if (内存::无视雪球 != 上次勾选[4]) {
		if (内存::无视雪球) 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::无视雪球, { 0xC3 });
		else 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::无视雪球, { 0x40,0x53 });
		上次勾选[4] = 内存::无视雪球;
	}
	if (内存::无视火焰 != 上次勾选[5]) {
		if (内存::无视火焰) 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::无视火焰, { 0xC3 });
		else 写字节集(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 数据::无视火焰, { 0x57 });
		上次勾选[5] = 内存::无视火焰;
	}
	/*
	if (内存::子弹瞬击 != 上次勾选[6]) {
		if (内存::子弹瞬击) 子弹瞬击(内存::瞬击值);
		else 关闭瞬击();
		上次勾选[6] = 内存::子弹瞬击;
	}*/
	if (内存::落地无僵 != 上次勾选[7]) {
		if (内存::落地无僵) 落地无僵();
		else 关闭无僵();
		上次勾选[7] = 内存::落地无僵;
	}
	if (内存::射击间隔 != 上次勾选[8]) {
		if (内存::射击间隔) 射击间隔(内存::射击间隔值);
		else 关闭间隔();
		上次勾选[8] = 内存::射击间隔;
	}

}

