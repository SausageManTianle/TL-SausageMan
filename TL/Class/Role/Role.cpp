//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include <map>
#include "..\Other\Other.h"
#include "..\..\GUI\GUI.h"
#include "Role.h"
void Role_POS(uintptr_t client, D3D坐标 pos) {
	uintptr_t roleLogicClient = *reinterpret_cast<uintptr_t*>(client + BattleRoleLogic::RoleBuffControl - 0x8);//RoleNet $m
	if (roleLogicClient) {
		uintptr_t BattleRole = *reinterpret_cast<uintptr_t*>(roleLogicClient + BattleRoleLogic::roleNetClient);//RoleNetClient
		if (BattleRole) {
			UnityResolve::Assembly* pClass = UnityResolve::Get("Assembly-CSharp.dll"); if (!pClass) return;
			UnityResolve::Method* Method = pClass->Get("RoleNetClient")->Get<UnityResolve::Method>("TargetRoleMoveToPoint", { "UnityEngine.Vector3" });
			if (Method != nullptr) {
				Method->Invoke<void>(BattleRole, pos);
			}
		}
	}
}


static auto SendHitPartDownHp(uintptr_t 玩家地址, UnityResolve::UnityType::Vector3 Aimpos) -> void {
	static UnityResolve::Method* method;
	if (!method)
		method = UnityResolve::Get("Assembly-CSharp.dll")->Get("BattleRole")->Get<UnityResolve::Method>("$fG");
	if (method) {
		uintptr_t roleLogicClient = *reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::roleLogicClient);
		if (!roleLogicClient) return;
		uintptr_t RoleNetClient = *reinterpret_cast<uintptr_t*>(roleLogicClient + BattleRole::RoleClient);
		if (!玩家地址 || !本人数据::本人地址) return;
		int playerId = *reinterpret_cast<int*>(RoleNetClient + BattleRole::playerId);
		int AimID = *reinterpret_cast<int*>(*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::roleLogicClient) + BattleRole::RoleClient) + BattleRole::playerId);
		method->Invoke<void>(RoleNetClient, (int)AimID, (int)playerId, (int)6, (int64_t)-1, 20.0f, Aimpos, (int64_t)0);
	}
}





void Role_main() {
	uintptr_t 玩家地址 = 0;
	uintptr_t 本人地址 = 0;
	UINT32 玩家数量 = 0;
	int 数量 = 0;
	UINT32 本人阵营 = 0;
	D3D坐标 本人坐标 = {};
	UINT32 玩家阵营 = 0;
	D3D坐标 玩家坐标 = {};
	char 玩家名称[64];
	char 玩家ID[64];
	uintptr_t 临时 = 0;
	Matrix 矩阵 = 获取矩阵();
	ImVec2 中心 = ImGui::GetIO().DisplaySize;
	D2D坐标 屏幕中心 = {};
	屏幕中心.x = 中心.x * 0.5f;
	屏幕中心.y = 中心.y * 0.5f;
	D4D坐标 BoxRect = {};
	D2D坐标 XY = {};
	VectorBox BoxScreen = {};
	float 准星距离 = 0;
	uintptr_t 自瞄目标 = 0;
	自瞄::功能范围 = 自瞄::自瞄范围;
	临时 = 取数组入口(); if (!临时) return;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + Start_Game::StartGame); if (!临时) return;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + Start_Game::BattleRoleLogic); if (!临时) return;
	玩家数量 = *reinterpret_cast<UINT32*>(临时 + 0x18);
	数量 = 玩家数量;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + 0x10);
	本人地址 = *reinterpret_cast<uintptr_t*>(临时 + 0x20);
	本人数据::本人地址 = 本人地址;
	for (int i = 0; i < 玩家数量; i++) {
		玩家地址 = *reinterpret_cast<uintptr_t*>(临时 + i * 0x8 + 0x20);
		if (!玩家地址)continue;
		玩家坐标.x = *reinterpret_cast<float*>(玩家地址 + BattleRoleLogic::Pos);
		玩家坐标.y = *reinterpret_cast<float*>(玩家地址 + BattleRoleLogic::Pos + 4);
		玩家坐标.z = *reinterpret_cast<float*>(玩家地址 + BattleRoleLogic::Pos + 8);
		玩家阵营 = *reinterpret_cast<UINT32*>(玩家地址 + BattleRoleLogic::TeamNum);
		strcpy_s(玩家名称, sizeof(玩家名称), 取角色名称(玩家地址).c_str());
		strcpy_s(玩家ID, sizeof(玩家ID), 取角色ID(玩家地址).c_str());

		if (i == 0) {
			本人阵营 = 玩家阵营;
			本人坐标 = 玩家坐标;
			本人数据::本人坐标 = 本人坐标;
		}

		if (玩家地址 == 本人地址 && !显示::显示自己) {
			数量 = 数量 - 1;
			continue;
		}

		if (!显示::绘制队友) {
			if (玩家阵营 == 本人阵营) {
				数量 = 数量 - 1;
				continue;
			}
		}
		//if (判断是否死亡(玩家地址)) {
			//数量 = 数量 - 1;
			//continue;
		//}//过时的判断+找不到判断死亡的地址
		if (判断是否存在(玩家地址)) {
			数量 = 数量 - 1;
			continue;
		}
		if (验证白名单(玩家ID, 本人数据::白名单.c_str())) {
			数量 = 数量 - 1;
			continue;
		}

		if (判断是否被瞄(本人坐标, 玩家坐标, 取玩家朝向(玩家地址)) && 显示::被瞄提醒) {
			绘制描边文本(屏幕中心.x - ImGui::CalcTextSize(GBK转UTF8("警告，你正在被瞄准！").c_str()).x * 0.5f, 250, GBK转UTF8("警告，你正在被瞄准！").c_str(), IM_COL32(255, 0, 0, 255), 1.0f, IM_COL32(255, 255, 255, 150));
		}


		if (WorldToScreenBox(BoxScreen, 玩家坐标, 矩阵, 屏幕中心)) {
			float BoxH = BoxScreen.y1 - BoxScreen.y;
			BoxRect.x = BoxScreen.x - BoxH / 4;
			BoxRect.y = BoxScreen.y;
			BoxRect.w = BoxH / 2.5;
			BoxRect.h = BoxH;
			准星距离 = 取准星距离(屏幕中心.x, 屏幕中心.y, BoxScreen.x, BoxScreen.y);//计算敌人到准星的距离
			if (自瞄::功能范围 >= 准星距离) {
				自瞄::功能范围 = 准星距离;
				自瞄目标 = 玩家地址;
			}
			if (显示::绘制射线) {
				if (显示::射线位置 == 0) 绘制直线(屏幕中心.x, 0, BoxRect.x + BoxRect.w * 0.5f, BoxRect.y, 颜色::射线颜色, 1);
				if (显示::射线位置 == 1) 绘制直线(屏幕中心.x, 屏幕中心.y, BoxRect.x + BoxRect.w * 0.5f, BoxRect.y, 颜色::射线颜色, 1);
				if (显示::射线位置 == 2) 绘制直线(屏幕中心.x, 屏幕中心.y * 2, BoxRect.x + BoxRect.w * 0.5f, BoxRect.y, 颜色::射线颜色, 1);
			}
			if (显示::绘制方框) {
				if (显示::方框类型 == 0) 边框方框(BoxRect.x, BoxRect.y, BoxRect.w, BoxRect.h, 颜色::方框颜色);
				if (显示::方框类型 == 1) 绘制3D方框(玩家坐标, 矩阵, 取玩家朝向(玩家地址).y, 取队伍颜色(玩家阵营), 1.9, -0.05, 1, 屏幕中心);
			}
			if (显示::绘制信息) {
				if (显示::信息类型 == 0)绘制信息(BoxRect, 取敌我距离(本人坐标, 玩家坐标), 玩家阵营, 玩家名称, 玩家ID, 判断是否被瞄(本人坐标, 玩家坐标, 取玩家朝向(玩家地址)), 玩家地址);
				if (显示::信息类型 == 1)绘制信息1(BoxRect, 取敌我距离(本人坐标, 玩家坐标), 玩家阵营, 玩家名称, 玩家ID, 判断是否被瞄(本人坐标, 玩家坐标, 取玩家朝向(玩家地址)), 玩家地址);
			}

			uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址+ BattleRoleLogic::roleLogicClient);//roleLogicClient
			if (临时) 临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::RoleClient);//BattleRole
			if (临时) 临时 = *reinterpret_cast<uintptr_t*>(临时 + BattleRole::MyRoleControl);//RoleControl
			if (临时) 临时 = *reinterpret_cast<uintptr_t*>(临时 + 0x48);//AnimatorControl
			if (临时) 临时 = *reinterpret_cast<uintptr_t*>(临时 + 0x1B0 - 0x8); //-0x4  animatorPtr
			D3D坐标 人物头部 = ReadRoleBone(玩家地址, 0x540);
			D2D坐标 p1 = {};
			VectorBox p = {};
			D4D坐标 p2 = {};
			if (显示::显示头部) {
				if (人物头部.x != 0 && 人物头部.z != 0 && 人物头部.y != 0) {
					if (D3D转2D坐标(人物头部, p1, 矩阵, 屏幕中心)) {
						if (D3D转方框坐标(p, 人物头部, 矩阵, 屏幕中心, 1.9, 0)) {
							p2.h = p.y - p.y1;           //高度
							p2.y = p2.h / 1.5;           //宽度
							绘制圆形(p1.x, p1.y, p2.y / 5, 100, 2, 颜色::骨骼颜色);
						}
					}
				}
			}
			if (显示::显示骨骼) {
				Draw_Bone(临时, 矩阵);
			}


		}//if转屏幕结束

		if (内存::改碰撞体 == true) {
			float* 大小 = reinterpret_cast<float*>(玩家地址 + BattleRoleLogic::RoleSize);
			if (大小)*大小 = 2.5f;

		}
		if (内存::意念拳人) {
			if (取敌我距离(本人坐标, 玩家坐标) <= 5) {
				玩家坐标.y = 玩家坐标.y + 1;
				if (玩家地址)SendHitPartDownHp(玩家地址, ReadRoleBone(玩家地址, 0x540));
			}
		}

	}//for循环结束
	if (内存::意念拳人) DrawFootCircle3D(本人坐标, 6.0f, 2, 矩阵, 屏幕中心);//绘制范围圈

	if (显示::绘制准星) 绘制准星(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f, 白色, 10, 6, 1, 8, 1);
	
	/*
	if (显示::显示人数) {
		char 缓存[256];
		std::snprintf(缓存, sizeof(缓存), "%s%d", GBK转UTF8("玩家:").c_str(), 数量);
		绘制描边文本(屏幕中心.x- 75 - ImGui::CalcTextSize(缓存).x * 0.5f, 130, 缓存, IM_COL32(255, 0, 0, 255), 1.0f, IM_COL32(0, 0, 0, 255));

		std::snprintf(缓存, sizeof(缓存), "%s%d", GBK转UTF8("人机:").c_str(), 本人数据::AI数量);
		绘制描边文本(屏幕中心.x + 75 - ImGui::CalcTextSize(缓存).x * 0.5f, 130, 缓存, IM_COL32(0, 255, 255, 255), 1.0f, IM_COL32(0, 0, 0, 255));

	}*/


	ImGuiRainbowTextBottomLeft("TLnb6666",0.6f, 10, ImGui::GetIO().DisplaySize.y - 200);
	显示人数(屏幕中心, 数量, 本人数据::AI数量, 显示::显示人数);

	if (本人数据::落雪特效) RenderSnow(本人坐标, 矩阵);


	
	if (自瞄::显示范围 && (数量 != 0 || 本人数据::AI数量 != 0)) 绘制圆形(屏幕中心.x, 屏幕中心.y, 自瞄::自瞄范围, 9999, 1, 颜色::自瞄范围);


	if (自瞄::鼠标自瞄::自瞄位置 == 0) 自瞄::位置 = 2;
	if (自瞄::内存自瞄::自瞄位置 == 0) 自瞄::位置 = 1;//如果头骨消失了，就切换到身体自瞄
	if (自瞄::鼠标自瞄::自瞄位置 == 1 || 自瞄::内存自瞄::自瞄位置 == 1) 自瞄::位置 = 1.5;
	if (自瞄::鼠标自瞄::自瞄位置 == 2 || 自瞄::内存自瞄::自瞄位置 == 2) 自瞄::位置 = 1.2;
	if (自瞄::鼠标自瞄::自瞄位置 == 3 || 自瞄::内存自瞄::自瞄位置 == 3) 自瞄::位置 = 1;

	D3D坐标 自瞄目标坐标 = {};
	D3D坐标 自瞄目标头部 = {};
	if (自瞄目标) {
		自瞄目标头部 = ReadRoleBone(自瞄目标, 0x540);
		自瞄目标坐标.x = *reinterpret_cast<float*>(自瞄目标 + BattleRoleLogic::Pos);
		自瞄目标坐标.y = *reinterpret_cast<float*>(自瞄目标 + BattleRoleLogic::Pos + 4);
		自瞄目标坐标.z = *reinterpret_cast<float*>(自瞄目标 + BattleRoleLogic::Pos + 8);
		if (取蹲起状态(自瞄目标) == 2) 自瞄::位置 = 1;
		if (取蹲起状态(自瞄目标) == 3) 自瞄::位置 = 0.5;
		//if (判断是否倒地(自瞄目标)) 自瞄::位置 = 1;
		if (*reinterpret_cast<float*>(自瞄目标 + BattleRoleLogic::HP) <= 0) 自瞄::位置 = 1;
		if (判断是否踩球(自瞄目标)) 自瞄::位置 = 2.5;
	}

	if (自瞄::开启预判)自瞄目标头部 = PredictEnemyNextPos(自瞄目标头部, 取敌我距离(本人坐标, 自瞄目标坐标));

	if (自瞄::开启自瞄) {
		if (自瞄目标) {
			if (自瞄::瞄准位置) { 
				if (自瞄::自瞄算法 == 1 && 自瞄::内存自瞄::自瞄位置 == 0 && 自瞄目标头部.x != 0 && 自瞄目标头部.y != 0 && 自瞄目标头部.z != 0) {//内存自瞄
					D2D坐标 p1 = {};
					if (!自瞄::开启预判) {
						D3D转2D坐标(自瞄目标头部, p1, 矩阵, 屏幕中心);
						绘制准星(p1.x, p1.y, 艳青, 10, 6, 1, 8, 1);
						绘制直线(屏幕中心.x, 屏幕中心.y, p1.x, p1.y, IM_COL32(255, 255, 255, 255), 1);
					}
					else {
						D3D转2D坐标(自瞄目标头部, p1, 矩阵, 屏幕中心);
						绘制直线(屏幕中心.x, 屏幕中心.y, p1.x, p1.y, IM_COL32(255, 255, 255, 255), 1);
						if (自瞄目标头部.x != 0) 绘制准星(p1.x, p1.y, 艳青, 10, 6, 1, 8, 1);
					}
				}
				else {//鼠标自瞄
					绘制准星(取瞄准位置(自瞄目标坐标, 矩阵, 屏幕中心).x, 取瞄准位置(自瞄目标坐标, 矩阵, 屏幕中心).y, 艳青, 10, 6, 1, 8, 1);
					绘制直线(屏幕中心.x, 屏幕中心.y, 取瞄准位置(自瞄目标坐标, 矩阵, 屏幕中心).x, 取瞄准位置(自瞄目标坐标, 矩阵, 屏幕中心).y, IM_COL32(255, 255, 255, 255), 1);
				}
			}
			if (自瞄::空手不瞄) {
				if (取角色手持(本人地址) != "") {
					if (GetAsyncKeyState(自瞄::自瞄热键) && 自瞄::功能范围 != 自瞄::自瞄范围) {//自瞄开启
						if(自瞄::自瞄算法 == 0)鼠标自瞄(自瞄目标坐标, 矩阵, 屏幕中心);
						else {
							自瞄目标坐标.y = 自瞄目标坐标.y + 自瞄::位置;
							if (自瞄::内存自瞄::自瞄位置 == 0 && 自瞄目标头部.x != 0 && 自瞄目标头部.y != 0 && 自瞄目标头部.z != 0) { 
								内存自瞄(自瞄目标头部, 矩阵, 自瞄::内存自瞄::自瞄速度);
							}
							else 内存自瞄(自瞄目标坐标, 矩阵, 自瞄::内存自瞄::自瞄速度);
						}
					}
				}
			}
			else {
				if (GetAsyncKeyState(自瞄::自瞄热键) && 自瞄::功能范围 != 自瞄::自瞄范围) {//自瞄开启
					if (自瞄::自瞄算法 == 0)鼠标自瞄(自瞄目标坐标, 矩阵, 屏幕中心);
					else {
						自瞄目标坐标.y = 自瞄目标坐标.y + 自瞄::位置;
						if (自瞄::内存自瞄::自瞄位置 == 0 && 自瞄目标头部.x != 0 && 自瞄目标头部.y != 0 && 自瞄目标头部.z != 0) { 
							内存自瞄(自瞄目标头部, 矩阵, 自瞄::内存自瞄::自瞄速度); 
						}
						else 内存自瞄(自瞄目标坐标, 矩阵, 自瞄::内存自瞄::自瞄速度);
					}
				}
			}
		}
	}
	开关追踪(自瞄::子弹追踪 && 自瞄::功能范围 != 自瞄::自瞄范围);
	if (自瞄目标) {
		if (自瞄::子弹追踪 && 自瞄::功能范围 != 自瞄::自瞄范围) {
			自瞄目标坐标.y = 自瞄目标坐标.y + 自瞄::位置;
			if (内存::魔法子弹)追踪(自瞄目标坐标); 
			else { 
				if(自瞄目标头部.x != 0 && 自瞄目标头部.y != 0 && 自瞄目标头部.z != 0)追踪(自瞄目标头部);
				else 追踪(自瞄目标坐标);
			};
		}
	}
	本人数据::目标坐标 = 自瞄目标坐标;



	if (内存::标点传送) {
		bool 是否传送 = false;
		if (!内存::渐进模式)	功能开关(是否传送, 内存::标点传送热键);
		else 是否传送 = GetAsyncKeyState(内存::标点传送热键) != 0;
		D3D坐标 标点坐标 = {};
		标点坐标.x = *reinterpret_cast<float*>(本人地址 + BattleRoleLogic::SpotPoint);
		标点坐标.y = *reinterpret_cast<float*>(本人地址 + BattleRoleLogic::SpotPoint + 4);
		标点坐标.z = *reinterpret_cast<float*>(本人地址 + BattleRoleLogic::SpotPoint + 8);
		if (标点坐标.x != 0 && 标点坐标.y != 0 && 标点坐标.z != 0) {
			D2D坐标 标点屏幕{};
			char 缓存[256];
			std::snprintf(缓存, sizeof(缓存), "%s[%dM]", GBK转UTF8("标点").c_str(), 取敌我距离(本人坐标, 标点坐标));
			if (D3D转2D坐标(标点坐标, 标点屏幕, 矩阵, 屏幕中心)) {
				绘制描边文本(标点屏幕.x - ImGui::CalcTextSize(缓存).x * 0.5f, 标点屏幕.y, 缓存, 绿色, 1, IM_COL32(0, 0, 0, 255));
			}
			if (是否传送) {
				if (内存::渐进模式) {
					static DWORD 上次瞬移时间 = 0;
					DWORD 当前时间 = GetTickCount();
					if (当前时间 - 上次瞬移时间 >= (DWORD)内存::传送频率) {
						上次瞬移时间 = 当前时间;
						D3D坐标 当前位置;
						当前位置.x = *reinterpret_cast<float*>(本人地址 + BattleRoleLogic::Pos);
						当前位置.y = *reinterpret_cast<float*>(本人地址 + BattleRoleLogic::Pos + 4);
						当前位置.z = *reinterpret_cast<float*>(本人地址 + BattleRoleLogic::Pos + 8);
						D3D坐标 方向 = 标点坐标 - 当前位置;
						float 距离 = sqrtf(方向.x * 方向.x + 方向.y * 方向.y + 方向.z * 方向.z);
						if (距离 > 0.1f) {
							float 步长 = (距离 < 内存::单次距离) ? 距离 : 内存::单次距离;
							D3D坐标 目标点 = 当前位置 + 方向 * (步长 / 距离);
							Role_POS(本人地址, 目标点);
						}
					}
				}
				else Role_POS(本人数据::本人地址, 标点坐标);
			}
		}
	}

	static D3D坐标 原始坐标;   // 保存：按下热键前自己的位置
	static bool 返回坐标 = false; // 标记：是否已经保存过原始位置
	if (内存::传送敌人) {
		bool 热键按住 = (GetAsyncKeyState(内存::传送敌人热键) & 0x8000) != 0;
		D3D坐标 目标坐标 = 自瞄目标坐标;
		if (内存::回弹模式 && !热键按住 && 返回坐标){
			Role_POS(本人数据::本人地址, 原始坐标);
			返回坐标 = false;
			原始坐标 = {}; // 清空缓存
		}
		if (内存::回弹模式 && 热键按住 && 目标坐标.x != 0)
		{
			// 计算敌人背后坐标
			float 朝向弧度 = 取玩家朝向(自瞄目标).y * 3.14159265f / 180.0f;
			D3D坐标 背后坐标 = 目标坐标;
			背后坐标.x -= sinf(朝向弧度) * 2;
			背后坐标.z -= cosf(朝向弧度) * 2;
			// 仅第一次按住时保存自己坐标
			if (!返回坐标){
				原始坐标 = 本人坐标;
				返回坐标 = true;
			}
			// 传送至敌人背后
			Role_POS(本人数据::本人地址, 背后坐标);
		}
		if (!内存::回弹模式) {
			bool 是否传送 = false;
			功能开关(是否传送, 内存::传送敌人热键);
			float 朝向弧度 = 取玩家朝向(自瞄目标).y * 3.141592653589f / 180.0f;
			D3D坐标 背后坐标 = 自瞄目标坐标;
			if (背后坐标.x != 0) {
				背后坐标.x -= sinf(朝向弧度) * 2;
				背后坐标.z -= cosf(朝向弧度) * 2;
				if (是否传送) {
					Role_POS(本人数据::本人地址, 背后坐标);
				}
			}
		}
	}
	static bool 移动已初始化 = false;
	if (内存::自由飞天 && 本人地址 != 0) {
		static glm::vec3 锁定坐标 = { 0, 0, 0 };
		static DWORD 上次移动时间 = 0;
		// 首次开启：锁定当前位置
		if (!移动已初始化) {
			锁定坐标 = 本人坐标;
			上次移动时间 = GetTickCount();
			移动已初始化 = true;
		}
		DWORD 当前时间 = GetTickCount();
		float dt = (当前时间 - 上次移动时间) / 1000.0f;
		上次移动时间 = 当前时间;
		if (dt > 0.1f) dt = 0.016f;

		// 读取朝向
		float 朝向弧度 = 取玩家朝向(本人地址).y * 3.141592653589f / 180.0f;

		float spd = 内存::飞天速度 * dt;
		// WASD 空格C 更新锁定坐标
		if (GetAsyncKeyState('W') & 0x8000) { 锁定坐标.x += sinf(朝向弧度) * spd; 锁定坐标.z += cosf(朝向弧度) * spd; }
		if (GetAsyncKeyState('S') & 0x8000) { 锁定坐标.x -= sinf(朝向弧度) * spd; 锁定坐标.z -= cosf(朝向弧度) * spd; }
		if (GetAsyncKeyState('A') & 0x8000) { 锁定坐标.x -= cosf(朝向弧度) * spd; 锁定坐标.z += sinf(朝向弧度) * spd; }
		if (GetAsyncKeyState('D') & 0x8000) { 锁定坐标.x += cosf(朝向弧度) * spd; 锁定坐标.z -= sinf(朝向弧度) * spd; }

		if (GetAsyncKeyState('C') & 0x8000) 锁定坐标.y -= spd;
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) 锁定坐标.y += spd;

		// 每帧强制拉回锁定坐标
		Role_POS(本人地址, 锁定坐标);
	}
	else {
		移动已初始化 = false;
	}





}//Role_main结束





D3D坐标 ReadRoleBone(ULONG64 add, ULONG64 Index){
	ULONG64 Bone_temp = 0;
	ULONG64 bone_array;
	D3D坐标 BonePos = { 0,0,0 };

	Bone_temp = *reinterpret_cast<UINT64*>(add + BattleRoleLogic::roleLogicClient);//roleLogicClient
	if (!Bone_temp)return BonePos;
	Bone_temp = *reinterpret_cast<UINT64*>(Bone_temp + BattleRole::RoleClient);//BattleRole
	if (!Bone_temp)return BonePos;
	Bone_temp = *reinterpret_cast<UINT64*>(Bone_temp + BattleRole::MyRoleControl);//RoleControl
	if (!Bone_temp)return BonePos;
	Bone_temp = *reinterpret_cast<UINT64*>(Bone_temp + 0x48);//AnimatorControl
	if (!Bone_temp)return BonePos;
	Bone_temp = *reinterpret_cast<UINT64*>(Bone_temp + 0x1B0);
	if (!Bone_temp)return BonePos;
	Bone_temp = *reinterpret_cast<UINT64*>(Bone_temp + 0xE0);
	if (!Bone_temp)return BonePos;
	Bone_temp = *reinterpret_cast<UINT64*>(Bone_temp + 0x188);
	if (!Bone_temp)return BonePos;
	Bone_temp = *reinterpret_cast<UINT64*>(Bone_temp + 0x18);

	bone_array = *reinterpret_cast<UINT64*>(Bone_temp + Index);
	if (!bone_array)return BonePos;
	bone_array = *reinterpret_cast<UINT64*>(bone_array + 0x10);
	if (!bone_array)return BonePos;
	bone_array = *reinterpret_cast<UINT64*>(bone_array + 0x30);
	if (!bone_array)return BonePos;
	bone_array = *reinterpret_cast<UINT64*>(bone_array + 0x30);
	if (!bone_array)return BonePos;
	bone_array = *reinterpret_cast<UINT64*>(bone_array + 0x28);
	if (!bone_array)return BonePos;
	bone_array = *reinterpret_cast<UINT64*>(bone_array + 0x48);
	if (!bone_array)return BonePos;
	bone_array = *reinterpret_cast<UINT64*>(bone_array + 0x28);

	// 最后读取坐标结构体
	if (!bone_array)return BonePos;
	BonePos = *reinterpret_cast<D3D坐标*>(bone_array + 0xA0);

	return BonePos;
}

void Draw_Bone(uintptr_t animator, Matrix 矩阵) {
	if (animator) {
		UnityResolve::UnityType::Transform* transform;
		UnityResolve::Assembly* pClass = UnityResolve::Get("UnityEngine.AnimationModule.dll");
		UnityResolve::Method* Method = pClass->Get("Animator")->Get<UnityResolve::Method>("GetBoneTransform", { "UnityEngine.HumanBodyBones" });
		ImVec2 中心 = ImGui::GetIO().DisplaySize;
		D2D坐标 屏幕中心 = { 0 , 0 };
		屏幕中心.x = 中心.x * 0.5f;
		屏幕中心.y = 中心.y * 0.5f;

		const struct {
			UnityResolve::UnityType::Animator::HumanBodyBones bone;
		} bones[] = {
			{ UnityResolve::UnityType::Animator::HumanBodyBones::Head },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::Neck },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::Hips },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::LeftUpperArm },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::RightUpperArm },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::LeftLowerArm },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::RightLowerArm },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::LeftUpperLeg },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::RightUpperLeg },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::LeftLowerLeg },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::RightLowerLeg },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::RightHand },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::LeftHand },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::RightFoot },
			{ UnityResolve::UnityType::Animator::HumanBodyBones::LeftFoot },
		};

		const int boneCount = sizeof(bones) / sizeof(bones[0]);
		UnityResolve::UnityType::Vector3 boneWorldPositions[boneCount];
		bool allBonesValid = true;

		for (int i = 0; i < boneCount; i++) {

			auto boneTransform = Method->Invoke<UnityResolve::UnityType::Transform*>(animator, bones[i].bone);
			if (boneTransform) {
				boneWorldPositions[i] = boneTransform->GetTransform()->GetPosition();
				if (boneWorldPositions[i].x == 0 && boneWorldPositions[i].y == 0 && boneWorldPositions[i].z == 0) {
					allBonesValid = false;
				}
			}
			else {
				allBonesValid = false;
			}
		}


		if (!allBonesValid)
			return;


		D3D坐标 customWorldPositions[boneCount];
		float headHeightIncrease = 0.2f;
		for (int i = 0; i < boneCount; i++) {

			customWorldPositions[i].x = boneWorldPositions[i].x;
			customWorldPositions[i].y = boneWorldPositions[i].y;
			customWorldPositions[i].z = boneWorldPositions[i].z;


		}

		D2D坐标 screenPositions[boneCount];
		bool isVisible[boneCount] = { false };
		bool allVisible = true;
		for (int i = 0; i < boneCount; i++) {
			if (!D3D转2D坐标(customWorldPositions[i], screenPositions[i], 矩阵, 屏幕中心)) {
				allVisible = false;
			}

		}
		if (allVisible){
			ImU32 骨骼颜色 = 颜色::骨骼颜色;
			const auto bg = ImGui::GetForegroundDrawList();
			bg->AddLine(ImVec2(screenPositions[0].x, screenPositions[0].y), ImVec2(screenPositions[1].x, screenPositions[1].y), 骨骼颜色, 1.5f);//头部-颈部
			bg->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[4].x, screenPositions[4].y), 骨骼颜色, 1.5f);//颈部-右肩
			bg->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[3].x, screenPositions[3].y), 骨骼颜色, 1.5f);//颈部-左肩
			bg->AddLine(ImVec2(screenPositions[4].x, screenPositions[4].y), ImVec2(screenPositions[6].x, screenPositions[6].y), 骨骼颜色, 1.5f);//右肩-右肘
			bg->AddLine(ImVec2(screenPositions[6].x, screenPositions[6].y), ImVec2(screenPositions[11].x, screenPositions[11].y), 骨骼颜色, 1.5f);//右肘-右手
			bg->AddLine(ImVec2(screenPositions[3].x, screenPositions[3].y), ImVec2(screenPositions[5].x, screenPositions[5].y), 骨骼颜色, 1.5f);//左肩-左肘
			bg->AddLine(ImVec2(screenPositions[5].x, screenPositions[5].y), ImVec2(screenPositions[12].x, screenPositions[12].y), 骨骼颜色, 1.5f);//左肘-左手
			bg->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[2].x, screenPositions[2].y), 骨骼颜色, 1.5f);//颈部-骨盆
			bg->AddLine(ImVec2(screenPositions[2].x, screenPositions[2].y), ImVec2(screenPositions[8].x, screenPositions[8].y), 骨骼颜色, 1.5f);//骨盆-右膝
			bg->AddLine(ImVec2(screenPositions[2].x, screenPositions[2].y), ImVec2(screenPositions[7].x, screenPositions[7].y), 骨骼颜色, 1.5f);//骨盆-左膝
			bg->AddLine(ImVec2(screenPositions[8].x, screenPositions[8].y), ImVec2(screenPositions[10].x, screenPositions[10].y), 骨骼颜色, 1.5f);//右膝-右脚
			bg->AddLine(ImVec2(screenPositions[10].x, screenPositions[10].y), ImVec2(screenPositions[13].x, screenPositions[13].y), 骨骼颜色, 1.5f);//右脚-右脚尖
			bg->AddLine(ImVec2(screenPositions[7].x, screenPositions[7].y), ImVec2(screenPositions[9].x, screenPositions[9].y), 骨骼颜色, 1.5f);//左膝-左脚
			bg->AddLine(ImVec2(screenPositions[9].x, screenPositions[9].y), ImVec2(screenPositions[14].x, screenPositions[14].y), 骨骼颜色, 1.5f);//左脚-左脚尖


		}


	}
}