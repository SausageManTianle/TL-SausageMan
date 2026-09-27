//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include "Car.h"
#include "..\..\GUI\GUI.h"
#include "..\Other\Other.h"

void Car_CmdSetUserRole(uintptr_t 目标地址,int 载具座位 = 0) {
	static bool 初始化 = false;
	uintptr_t client = *reinterpret_cast<uintptr_t*>(目标地址 + AllCar::carNetClient);
	int carId = *reinterpret_cast<int*>(client + AllCar::_carId);
	static UnityResolve::Method* Method;
	if (!初始化) {
		UnityResolve::Assembly* pClass = UnityResolve::Get("Assembly-CSharp.dll"); if (!pClass) return;
		Method = pClass->Get("RoleNetClient")->Get<UnityResolve::Method>("CmdSetUserRole", { "UnityEngine.Int32", "UnityEngine.Int32" });
		if (Method != nullptr) {
			初始化 = true;
		}
	}
	if (初始化 && client != 0) {
		Method->Invoke<void>(client, carId, 载具座位);
	}
}

void CallCmdDownCarHp(uintptr_t carPtr, glm::vec3 hitPoint) {
	static bool 初始化 = false;
	static UnityResolve::Method* Method;
	if (!初始化) {
		Method = UnityResolve::Get("Assembly-CSharp.dll")->Get("RoleNetClient")->Get<UnityResolve::Method>("CmdDownCarHp");
		if (Method != nullptr) {
			初始化 = true;
		}
	}
	uintptr_t RoleNet = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::RoleBuffControl - 0x8);//本人地址 + RoleNet
	if(!RoleNet) return;
	uintptr_t RoleNetClient = *reinterpret_cast<uintptr_t*>(RoleNet + BattleRoleLogic::roleNetClient);
	if(!本人数据::本人地址) return;
	uintptr_t roleLogicClient = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::roleLogicClient);
	uintptr_t RoleClient = *reinterpret_cast<uintptr_t*>(roleLogicClient + BattleRole::RoleClient);
	int playerId = *reinterpret_cast<int*>(RoleClient + BattleRole::playerId);
	if (初始化 || RoleNetClient == 0 || carPtr == 0) {
		Method->Invoke<void>(
			RoleNetClient,                 // Invoke 所需实例对象
			playerId,                  // 参数1: autoId         → 攻击者玩家 ID
			6,                         // 参数2: attackType     → 拳头攻击枚举
			(DWORD64)carPtr,           // 参数3: hitCar         → 目标车辆指针
			(long long)-1,             // 参数4: attackDownHpId → 时间戳验证-1
			hitPoint,                  // 参数5: hitPoint       → 命中点坐标
			3,                         // 参数6: hitType        → DownHpType.HitPart
			0                          // 参数7: bulletHurt     → 无Buff
		);
	}
}

void Car_main() {
	uintptr_t 载具地址 = 0;
	UINT32 载具数量 = 0;
	D3D坐标 载具坐标 = { 0,0,0 };
	uintptr_t 信息偏移 = 0;
	float 载具血量 = 0;
	float 最大血量 = 0;
	float 载具油量 = 0;
	float 最大油量 = 0;
	char 载具名称[64];
	float 载具敌我 = 0;
	float 准星距离 = 0;
	uintptr_t 目标地址 = 0;
	float 最小距离 = 1000000;
	Matrix 矩阵 = 获取矩阵();
	uintptr_t 临时 = 0;
	临时 = 取数组入口(); if (!临时) return;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + Start_Game::StartGame); if (!临时) return ;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + Start_Game::AllCar);if (!临时) return;
	载具数量 = *reinterpret_cast<UINT32*>(临时 + 0x18); if (!载具数量) return;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + 0x10);
	for (int i = 0; i < 载具数量; i++) {
		载具地址 = *reinterpret_cast<uintptr_t*>(临时 + 0x20 + i * 0x8);
		if (!载具地址) continue;
		载具坐标.x = *reinterpret_cast<float*>(载具地址 + AllCar::Position);
		载具坐标.y = *reinterpret_cast<float*>(载具地址 + AllCar::Position + 4);
		载具坐标.z = *reinterpret_cast<float*>(载具地址 + AllCar::Position + 8);
		信息偏移 = *reinterpret_cast<uintptr_t*>(载具地址 + AllCar::Mirror);
		if (!信息偏移) continue;
		载具血量 = *reinterpret_cast<float*>(信息偏移 + AllCar::HP);
		最大血量 = *reinterpret_cast<float*>(信息偏移 + AllCar::HP - 4);
		载具油量 = *reinterpret_cast<float*>(信息偏移 + AllCar::Consumption);
		最大油量 = *reinterpret_cast<float*>(信息偏移 + AllCar::Consumption + 4);
		载具敌我 = 取敌我距离(本人数据::本人坐标, 载具坐标);
		strcpy_s(载具名称, sizeof(载具名称), 取载具中文名(取载具名称(载具地址)).c_str());
		if(载具血量 < 1) continue;
		ImVec2 中心 = ImGui::GetIO().DisplaySize;
		D2D坐标 屏幕中心 = { 0 , 0 };
		屏幕中心.x = 中心.x * 0.5f;
		屏幕中心.y = 中心.y * 0.5f;
		VectorBox BoxScreen = { 0,0,0 };
		D4D坐标 BoxRect;
		if (WorldToScreenBox(BoxScreen,载具坐标,矩阵,屏幕中心)) {
			float BoxH = BoxScreen.y1 - BoxScreen.y;
			BoxRect.x = BoxScreen.x - BoxH / 4;
			BoxRect.y = BoxScreen.y;
			BoxRect.w = BoxH / 2.5;
			BoxRect.h = BoxH;
			准星距离 = (float)取准星距离(屏幕中心.x, 屏幕中心.y, BoxScreen.x, BoxScreen.y);//计算载具到准星的2D距离
			if (最小距离 >= 准星距离) {
				最小距离 = 准星距离;
				目标地址 = 载具地址;
			}
			//边框方框(BoxRect.x, BoxRect.y, BoxRect.w, BoxRect.h, 红色);
			if (显示::显示载具) {
				if (显示::载具类型 == 2) {
					const float 宽度 = 50.0f;
					const float 高度 = 30.0f;
					float 左边 = BoxRect.x + BoxRect.w * 0.5f - 宽度 * 0.5f;
					float 顶边 = BoxRect.y + 高度;

					char 缓存[256];
					std::snprintf(缓存, sizeof(缓存), "%s  %.0fm", 载具名称, 载具敌我);
					透明矩形(左边, 顶边, 宽度+static_cast<float>(std::strlen(缓存))*3, 高度, 颜色::载具背景, 100, IM_COL32(0, 0, 0, 255));
					绘制描边文本(左边+7, 顶边+3, 缓存, 白色, 1.0f, 黑色);

					ImU32 油量颜色;
					if (载具油量 >= 1000) 油量颜色 = 艳青;
					else if (载具油量 <= 1000) 油量颜色 = 橙黄;
					else if (载具油量 == 0) 油量颜色 = 红色;

					ImU32 血条颜色;
					if (载具血量 >= 500) 血条颜色 = 白色;
					else 血条颜色 = 红色;

					float 油百分比 = (载具油量 / 最大油量) * 100.0f;
					float 油量计算 = 油百分比 / 100.0f;
					float 油量总宽度 = 40.0f + static_cast<float>(std::strlen(缓存)) * 3.0f;
					float 油量宽 = (油百分比 / 100.0f) * 油量总宽度;
					油量宽 = (油量宽 > 油量总宽度) ? 油量总宽度 : (油量宽 < 0) ? 0 : 油量宽;
					填充方框(左边 + 5, 顶边 + 18, 油量总宽度+2, 4, IM_COL32(0, 0, 0, 80), 80);
					填充方框(左边 + 6, 顶边 + 19, 油量宽, 2, 油量颜色, 255);

					float 血百分比 = (载具血量 / 最大血量) * 100.0f;
					float 血量计算 = 血百分比 / 100.0f;
					float 血条总宽度 = 40.0f + static_cast<float>(std::strlen(缓存)) * 3.0f;
					float 血条宽 = (血百分比 / 100.0f) * 血条总宽度;
					血条宽 = (血条宽 > 血条总宽度) ? 血条总宽度 : (血条宽 < 0) ? 0 : 血条宽;
					填充方框(左边 + 5, 顶边 + 22, 血条总宽度+2, 4, IM_COL32(0, 0, 0, 80), 80);
					填充方框(左边 + 6, 顶边 + 23, 血条宽, 2, 血条颜色, 255);
				}
				if (显示::载具类型 == 0) {
					char 缓存[256];
					std::snprintf(缓存, sizeof(缓存), "%s  %.0fm", 载具名称, 载具敌我);
					绘制描边文本(BoxRect.x, BoxRect.y, 缓存, 绿色, 1.0f, 黑色);
					std::snprintf(缓存, sizeof(缓存), "%s %.0f/%.0f", GBK转UTF8("血").c_str(), 载具血量, 最大血量);
					if (载具血量 > 500)绘制描边文本(BoxRect.x, BoxRect.y + 12, 缓存, 黄色, 1.0f, 黑色);
					if (载具血量 <= 500)绘制描边文本(BoxRect.x, BoxRect.y + 12, 缓存, 红色, 1.0f, 黑色);
					std::snprintf(缓存, sizeof(缓存), "%s %.0f/%.0f", GBK转UTF8("油").c_str(), 载具油量, 最大油量);
					if (载具油量 >= 1000)绘制描边文本(BoxRect.x, BoxRect.y + 24, 缓存, 艳青, 1.0f, 黑色);
					if (载具油量 < 1000)绘制描边文本(BoxRect.x, BoxRect.y + 24, 缓存, 橙黄, 1.0f, 黑色);
					if (载具油量 == 0)绘制描边文本(BoxRect.x, BoxRect.y + 24, 缓存, 红色, 1.0f, 黑色);
				}
				if (显示::载具类型 == 1) {
					char 缓存[256];
					std::snprintf(缓存, sizeof(缓存), "%s  %.0fm", 载具名称, 载具敌我);
					绘制描边文本(BoxRect.x, BoxRect.y, 缓存, 绿色, 1.0f, 黑色);
				}


			}//if显示载具结束
		}//if转屏幕结束
	}//for循环结束
	if ((内存::意念上车|| 内存::意念炸车) && 目标地址) {
		D4D坐标 BoxRect = { 0,0,0,0 };
		VectorBox BoxScreen = { 0,0,0 };
		D3D坐标 目标坐标 = { 0,0,0 };
		char 目标名称[64];
		目标坐标.x = *reinterpret_cast<float*>(目标地址 + AllCar::Position);
		目标坐标.y = *reinterpret_cast<float*>(目标地址 + AllCar::Position + 4);
		目标坐标.z = *reinterpret_cast<float*>(目标地址 + AllCar::Position + 8);
		strcpy_s(目标名称, sizeof(目标名称), 取载具中文名(取载具名称(目标地址)).c_str());
		if (WorldToScreenBox(BoxScreen, 目标坐标, 矩阵, { ImGui::GetIO().DisplaySize.x*0.5f, ImGui::GetIO().DisplaySize.y*0.5f })) {
			float BoxH = BoxScreen.y1 - BoxScreen.y;
			BoxRect.x = BoxScreen.x - BoxH / 4;
			BoxRect.y = BoxScreen.y;
			BoxRect.w = BoxH / 2.5;
			BoxRect.h = BoxH;
			if (显示::显示载具) {
				if (显示::载具类型 == 2) {
					const float 宽度 = 50.0f;
					const float 高度 = 30.0f;
					float 左边 = BoxRect.x + BoxRect.w * 0.5f - 宽度 * 0.5f;
					float 顶边 = BoxRect.y + 高度;

					char 缓存[256];
					std::snprintf(缓存, sizeof(缓存), "%s", 目标名称);
					绘制描边文本(左边 + 7, 顶边 + 3, 缓存, 红色, 1.0f, 黑色);

				}
				if (显示::载具类型 == 0) {
					char 缓存[256];
					std::snprintf(缓存, sizeof(缓存), "%s", 目标名称);
					绘制描边文本(BoxRect.x, BoxRect.y, 缓存, 红色, 1.0f, 黑色);
				}
				if (显示::载具类型 == 1) {
					char 缓存[256];
					std::snprintf(缓存, sizeof(缓存), "%s", 目标名称);
					绘制描边文本(BoxRect.x, BoxRect.y, 缓存, 红色, 1.0f, 黑色);
				}
			}
		}
		if (内存::意念上车) {
			if (目标地址) {
				bool 是否上车 = false;
				功能开关(是否上车, 内存::意念上车热键);
				if(是否上车)Car_CmdSetUserRole(目标地址, 内存::意念上车位置);
			}
		}
		if (内存::意念炸车) {
			if (目标地址) {
				uintptr_t Car = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(目标地址 + AllCar::carNetClient) + 0x70);
				if (GetAsyncKeyState(内存::意念炸车热键))CallCmdDownCarHp(Car, 载具坐标);
			}
		}

	}//if意念上车&&意念炸车结束
}//Car_main结束
