//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include "AI.h"
#include "..\..\GUI\GUI.h"
#include "..\Other\Other.h"

void DrawBone(uintptr_t animator, Matrix 矩阵) {
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
		if (allVisible) {
			ImU32 骨骼颜色 = 颜色::人机颜色;
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

void AI_POS(uintptr_t client, D3D坐标 pos,float 本人朝向) {
	UnityResolve::Assembly* pClass = UnityResolve::Get("Assembly-CSharp.dll"); if (!pClass) return;
	UnityResolve::Method* Method = pClass->Get("ClientRoleAILogic")->Get<UnityResolve::Method>("SetPosition", { "UnityEngine.Vector3" });
	if (Method != nullptr && client != 0) {
		float 弧度 = (本人朝向 + 15) * 3.141592653589f / 180.0f; 
		pos.x = pos.x + sin(弧度);
		pos.z = pos.z + cos(弧度);
		Method->Invoke<void>(client, pos);
	}
}


void AI_main(){
	char AI名称[64];
	float AI血量 = 0;
	float AI敌我 = 0;
	D3D坐标 AI坐标 = { 0,0,0 };
	Matrix 矩阵 = 获取矩阵();
	uintptr_t 临时 = 0;	
	UINT32 AI数量 = 0;
	ImVec2 中心 = ImGui::GetIO().DisplaySize;
	D2D坐标 屏幕中心 = { 0 , 0 };
	屏幕中心.x = 中心.x * 0.5f;
	屏幕中心.y = 中心.y * 0.5f;
	临时 = 取数组入口();if (!临时) return;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + Start_Game::StartGame); if (!临时) return;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + Start_Game::RoleAIManager); if (!临时) return;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + 0x18);
	AI数量 = *reinterpret_cast<UINT32*>(临时 + 0x18);
	临时 = *reinterpret_cast<uintptr_t*>(临时 + 0x10);
	uintptr_t AI地址 = 0;
	for (int i = 0; i < AI数量; i++) {
		AI地址 = *reinterpret_cast<uintptr_t*>(临时 + 0x20 + i * 0x8);
		AI坐标.x = *reinterpret_cast<float*>(AI地址 + RoleAIManager::Position);
		AI坐标.y = *reinterpret_cast<float*>(AI地址 + RoleAIManager::Position + 4);
		AI坐标.z = *reinterpret_cast<float*>(AI地址 + RoleAIManager::Position + 8);
		AI血量 = *reinterpret_cast<float*>(AI地址 + RoleAIManager::HP);
		uintptr_t client = *reinterpret_cast<uintptr_t*>(AI地址 + 0x40);
		AI敌我 = 取敌我距离(本人数据::本人坐标, AI坐标);
		strcpy_s(AI名称, sizeof(AI名称), 取人机名称(AI地址).c_str());
		D4D坐标 BoxRect;
		VectorBox BoxScreen = { 0,0,0 };
		if (AI血量 < 0 || AI坐标.x == 0) {
			continue;
		}
		if (WorldToScreenBox(BoxScreen, AI坐标, 矩阵, 屏幕中心)) {
			float BoxH = BoxScreen.y1 - BoxScreen.y;
			BoxRect.x = BoxScreen.x - BoxH / 4;
			BoxRect.y = BoxScreen.y;
			BoxRect.w = BoxH / 2.5;
			BoxRect.h = BoxH;
			if(显示::显示人机){
				绘制直线(屏幕中心.x, 0, BoxRect.x + BoxRect.w * 0.5f, BoxRect.y, 颜色::人机颜色, 1);
				if (显示::方框类型 == 0) 边框方框(BoxRect.x, BoxRect.y, BoxRect.w, BoxRect.h, 颜色::人机颜色);
				if (显示::方框类型 == 1) 绘制3D方框(AI坐标, 矩阵, 0, 颜色::人机颜色, 1.9, -0.05, 1, 屏幕中心);
				char 缓冲[256];
				std::snprintf(缓冲, sizeof(缓冲), "%s%s HP:%.0f", GBK转UTF8("[人机]").c_str(), AI名称, AI血量);
				绘制描边文本(BoxRect.x + BoxRect.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect.y - 15, 缓冲, 白色, 1, IM_COL32(0, 0, 0, 180));
				std::snprintf(缓冲, sizeof(缓冲), "%.0fm", AI敌我);
				绘制描边文本(BoxRect.x + BoxRect.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect.y + BoxRect.h, 缓冲, 白色, 1, IM_COL32(0, 0, 0, 180));

			}
			/*
			if (!client) continue;
			uintptr_t ClientRoleAILogic = *reinterpret_cast<uintptr_t*>(client + 0x30); if (ClientRoleAILogic == 0) continue;
			uintptr_t animatorPtr = *reinterpret_cast<uintptr_t*>(ClientRoleAILogic + 0x20); if (animatorPtr == 0) continue;
			DrawBone(animatorPtr, 矩阵);
			*/
		}//if(WorldToScreenBox结束
		if(内存::人机变大){
			float* 大小 = reinterpret_cast<float*>(AI地址 + RoleAIManager::RoleSize);
			if(大小)*大小 = 内存::人机大小;

		}
		if(内存::吸取人机){
			if(内存::热键吸取 && GetAsyncKeyState(内存::吸取人机热键)&0x8000){
				AI_POS(client, 本人数据::本人坐标, 取玩家朝向(本人数据::本人地址).y);

			}
			if(!内存::热键吸取)AI_POS(client, 本人数据::本人坐标, 取玩家朝向(本人数据::本人地址).y);
		}

	}//for循环结束
	本人数据::AI数量 = AI数量;
}//AI_main函数结束

