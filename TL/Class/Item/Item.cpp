//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include <vector>
#include <map>
#include <unordered_map>
#include "..\Other\Other.h"
#include "..\..\GUI\GUI.h"
#include "Item.h"

bool 范围(int 对比值, int 最小值, int 最大值) {
	return 对比值 >= 最小值 && 对比值 <= 最大值;
}

void 叠加(glm::vec4& 叠加坐标, std::vector<glm::vec4>& 全_物品叠加) {

	for (int i = 0; i < 全_物品叠加.size(); i++) {
		int minX = abs(全_物品叠加[i].x - 80);
		int maxX = abs(全_物品叠加[i].x + 80);
		int minY = abs(全_物品叠加[i].y - 16);
		int maxY = abs(全_物品叠加[i].y + 16);
		if (范围((int)叠加坐标.x, minX, maxX) && 范围((int)叠加坐标.y, minY, maxY)) {
			叠加坐标 = 全_物品叠加[i];
			叠加坐标.y = 叠加坐标.y - 16 * 0.8f;
		}
	}
}

//1步枪 2冲锋枪 3射手步枪 4狙击枪 5机枪 6霰弹枪 7手枪 8特殊武器 9暗器 10常用装备 11常用身份 12常用药品 13常用倍镜 14常用配件 15常用芯片 16常用投掷 17常用近战 18常用子弹 19信号枪

//预定义物资类型映射表（静态哈希表，仅初始化一次）
static std::unordered_map<std::string, int> 物资类型映射表;
//哈希表初始化函数（仅执行一次）
static void 初始化物资类型映射表() {
	if (!物资类型映射表.empty()) return; // 避免重复初始化

	// 批量插入映射关系：{UTF8物资名, 类型值}
	// 1-步枪
	物资类型映射表.emplace(GBK转UTF8("QBZ-03"), 1);
	物资类型映射表.emplace(GBK转UTF8("QBZ-192"), 1);
	物资类型映射表.emplace(GBK转UTF8("M416"), 1);
	物资类型映射表.emplace(GBK转UTF8("M16A4"), 1);
	物资类型映射表.emplace(GBK转UTF8("SCAR-L"), 1);
	物资类型映射表.emplace(GBK转UTF8("QBZ-95"), 1);
	物资类型映射表.emplace(GBK转UTF8("AUG"), 1);
	物资类型映射表.emplace(GBK转UTF8("量子利刃"), 1);
	物资类型映射表.emplace(GBK转UTF8("AKM"), 1);
	物资类型映射表.emplace(GBK转UTF8("AK-12"), 1);
	物资类型映射表.emplace(GBK转UTF8("Groza"), 1);
	物资类型映射表.emplace(GBK转UTF8("Tavor"), 1);
	物资类型映射表.emplace(GBK转UTF8("Galil"), 1);

	// 2-冲锋枪
	物资类型映射表.emplace(GBK转UTF8("MP5"), 2);
	物资类型映射表.emplace(GBK转UTF8("PP-19"), 2);
	物资类型映射表.emplace(GBK转UTF8("Kriss Vector"), 2);
	物资类型映射表.emplace(GBK转UTF8("TommyGun"), 2);
	物资类型映射表.emplace(GBK转UTF8("UZI"), 2);
	物资类型映射表.emplace(GBK转UTF8("UMP9"), 2);
	物资类型映射表.emplace(GBK转UTF8("P90"), 2);
	物资类型映射表.emplace(GBK转UTF8("能量双枪"), 2);

	// 3-射手步枪
	物资类型映射表.emplace(GBK转UTF8("VSS"), 3);
	物资类型映射表.emplace(GBK转UTF8("Mini14"), 3);
	物资类型映射表.emplace(GBK转UTF8("MK14"), 3);
	物资类型映射表.emplace(GBK转UTF8("SLR"), 3);
	物资类型映射表.emplace(GBK转UTF8("SKS"), 3);
	物资类型映射表.emplace(GBK转UTF8("QBU-191"), 3);
	物资类型映射表.emplace(GBK转UTF8("蓄能炮"), 3);

	// 4-狙击枪
	物资类型映射表.emplace(GBK转UTF8("Kar98"), 4);
	物资类型映射表.emplace(GBK转UTF8("M24"), 4);
	物资类型映射表.emplace(GBK转UTF8("AWM"), 4);
	物资类型映射表.emplace(GBK转UTF8("巴雷特"), 4);

	// 5-机枪
	物资类型映射表.emplace(GBK转UTF8("M249"), 5);
	物资类型映射表.emplace(GBK转UTF8("HK13"), 5);
	物资类型映射表.emplace(GBK转UTF8("加特林"), 5);
	物资类型映射表.emplace(GBK转UTF8("DP28"), 5);
	物资类型映射表.emplace(GBK转UTF8("能量加速机枪"), 5);
	物资类型映射表.emplace(GBK转UTF8("DP-28"), 5);

	// 6-霰弹枪
	物资类型映射表.emplace(GBK转UTF8("KSG"), 6);
	物资类型映射表.emplace(GBK转UTF8("S12K"), 6);
	物资类型映射表.emplace(GBK转UTF8("S686"), 6);
	物资类型映射表.emplace(GBK转UTF8("S1897"), 6);
	物资类型映射表.emplace(GBK转UTF8("能量弩"), 6);

	// 7-手枪
	物资类型映射表.emplace(GBK转UTF8("P1911"), 7);
	物资类型映射表.emplace(GBK转UTF8("P18C"), 7);
	物资类型映射表.emplace(GBK转UTF8("P92"), 7);
	物资类型映射表.emplace(GBK转UTF8("R1895"), 7);

	// 8-特殊武器
	物资类型映射表.emplace(GBK转UTF8("魔武-恶棍"), 8);
	物资类型映射表.emplace(GBK转UTF8("RPG"), 8);
	物资类型映射表.emplace(GBK转UTF8("爆炸弓"), 8);
	物资类型映射表.emplace(GBK转UTF8("能量粒子炮"), 8);
	物资类型映射表.emplace(GBK转UTF8("超级脉冲手炮"), 8);
	物资类型映射表.emplace(GBK转UTF8("能量灸灸炮"), 8);
	物资类型映射表.emplace(GBK转UTF8("爆燃榴弹炮"), 8);

	// 9-暗器
	物资类型映射表.emplace(GBK转UTF8("筷子"), 9);
	物资类型映射表.emplace(GBK转UTF8("牙签"), 9);
	物资类型映射表.emplace(GBK转UTF8("雷符"), 9);
	物资类型映射表.emplace(GBK转UTF8("水晶球"), 9);
	物资类型映射表.emplace(GBK转UTF8("法杖"), 9);

	// 10-常用装备
	物资类型映射表.emplace(GBK转UTF8("顶级头盔"), 10);
	物资类型映射表.emplace(GBK转UTF8("顶级防弹衣"), 10);
	物资类型映射表.emplace(GBK转UTF8("4级头盔"), 10);
	物资类型映射表.emplace(GBK转UTF8("3级头盔"), 10);
	物资类型映射表.emplace(GBK转UTF8("3级头盔-霰弹勇士"), 10);
	物资类型映射表.emplace(GBK转UTF8("3级头盔-侦察大师"), 10);
	物资类型映射表.emplace(GBK转UTF8("2级头盔"), 10);
	物资类型映射表.emplace(GBK转UTF8("1级头盔"), 10);
	物资类型映射表.emplace(GBK转UTF8("4级防弹衣"), 10);
	物资类型映射表.emplace(GBK转UTF8("3级防弹衣"), 10);
	物资类型映射表.emplace(GBK转UTF8("2级防弹衣"), 10);
	物资类型映射表.emplace(GBK转UTF8("1级防弹衣"), 10);
	物资类型映射表.emplace(GBK转UTF8("4级背包"), 10);
	物资类型映射表.emplace(GBK转UTF8("3级背包"), 10);
	物资类型映射表.emplace(GBK转UTF8("2级背包"), 10);
	物资类型映射表.emplace(GBK转UTF8("1级背包"), 10);
	物资类型映射表.emplace(GBK转UTF8("吉利服"), 10);
	物资类型映射表.emplace(GBK转UTF8("游泳圈"), 10);
	物资类型映射表.emplace(GBK转UTF8("全息装置"), 10);

	// 11-常用身份
	物资类型映射表.emplace(GBK转UTF8("神王身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("冥王身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("海王身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("军师身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("武圣身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("枭雄身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("影武者身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("甜心身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("球球身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("飞翼身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("狼王身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("雪女身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("彩虹王子身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("咸鱼身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("太阳神身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("猫猫身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("魔法师身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("幽灵船长身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("爆炸小丑身份卡"), 11);
	物资类型映射表.emplace(GBK转UTF8("唐老板身份卡"), 11);

	// 12-常用药品
	物资类型映射表.emplace(GBK转UTF8("彩虹能源"), 12);
	物资类型映射表.emplace(GBK转UTF8("变大大"), 12);
	物资类型映射表.emplace(GBK转UTF8("奶瓶"), 12);
	物资类型映射表.emplace(GBK转UTF8("传送胶囊"), 12);
	物资类型映射表.emplace(GBK转UTF8("能量饮料"), 12);
	物资类型映射表.emplace(GBK转UTF8("止痛药"), 12);
	物资类型映射表.emplace(GBK转UTF8("绷带"), 12);
	物资类型映射表.emplace(GBK转UTF8("急救包"), 12);
	物资类型映射表.emplace(GBK转UTF8("医疗箱"), 12);
	物资类型映射表.emplace(GBK转UTF8("紫金红葫芦"), 12);

	// 13-常用倍镜
	物资类型映射表.emplace(GBK转UTF8("红点瞄准镜"), 13);
	物资类型映射表.emplace(GBK转UTF8("全息瞄准镜"), 13);
	物资类型映射表.emplace(GBK转UTF8("2倍瞄准镜"), 13);
	物资类型映射表.emplace(GBK转UTF8("3倍瞄准镜"), 13);
	物资类型映射表.emplace(GBK转UTF8("4倍瞄准镜"), 13);
	物资类型映射表.emplace(GBK转UTF8("6倍瞄准镜"), 13);
	物资类型映射表.emplace(GBK转UTF8("8倍瞄准镜"), 13);
	物资类型映射表.emplace(GBK转UTF8("15倍瞄准镜"), 13);

	// 14-常用配件
	物资类型映射表.emplace(GBK转UTF8("霰弹枪子弹袋"), 14);
	物资类型映射表.emplace(GBK转UTF8("霰弹枪收束器"), 14);
	物资类型映射表.emplace(GBK转UTF8("Kar98子弹袋"), 14);
	物资类型映射表.emplace(GBK转UTF8("战术枪托"), 14);
	物资类型映射表.emplace(GBK转UTF8("直角握把"), 14);
	物资类型映射表.emplace(GBK转UTF8("垂直握把"), 14);
	物资类型映射表.emplace(GBK转UTF8("高级垂直握把"), 14);
	物资类型映射表.emplace(GBK转UTF8("狙击枪快速弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("狙击枪扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("狙击枪快速扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("高级狙击枪快速扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("冲锋枪快速弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("冲锋枪扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("冲锋枪快速扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("高级冲锋枪快速扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("步枪快速弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("步枪扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("步枪快速扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("高级步枪快速扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("步枪枪口弹道辅助器"), 14);
	物资类型映射表.emplace(GBK转UTF8("冲锋枪枪口子弹增速器"), 14);
	物资类型映射表.emplace(GBK转UTF8("冲锋枪枪口弹道辅助器"), 14);
	物资类型映射表.emplace(GBK转UTF8("步枪枪口子弹增速器"), 14);
	物资类型映射表.emplace(GBK转UTF8("阻手器"), 14);
	物资类型映射表.emplace(GBK转UTF8("激光瞄准器"), 14);
	物资类型映射表.emplace(GBK转UTF8("高级狙击枪托腮板"), 14);
	物资类型映射表.emplace(GBK转UTF8("狙击枪托腮板"), 14);
	物资类型映射表.emplace(GBK转UTF8("手枪快速弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("手枪快速扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("手枪扩容弹匣"), 14);
	物资类型映射表.emplace(GBK转UTF8("手枪消音器"), 14);
	物资类型映射表.emplace(GBK转UTF8("狙击枪补偿器"), 14);
	物资类型映射表.emplace(GBK转UTF8("狙击枪消焰器"), 14);
	物资类型映射表.emplace(GBK转UTF8("狙击枪消音器"), 14);
	物资类型映射表.emplace(GBK转UTF8("高级狙击枪消音器"), 14);
	物资类型映射表.emplace(GBK转UTF8("UZI枪托"), 14);
	物资类型映射表.emplace(GBK转UTF8("冲锋枪补偿器"), 14);
	物资类型映射表.emplace(GBK转UTF8("冲锋枪消焰器"), 14);
	物资类型映射表.emplace(GBK转UTF8("冲锋枪消音器"), 14);
	物资类型映射表.emplace(GBK转UTF8("高级冲锋枪消音器"), 14);
	物资类型映射表.emplace(GBK转UTF8("步枪补偿器"), 14);
	物资类型映射表.emplace(GBK转UTF8("步枪消焰器"), 14);
	物资类型映射表.emplace(GBK转UTF8("步枪消音器"), 14);
	物资类型映射表.emplace(GBK转UTF8("高级步枪消音器"), 14);

	// 15-常用芯片
	物资类型映射表.emplace(GBK转UTF8("滋滋芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("蓄能芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("放大芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("弹匣扩容芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("高速预热芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("快速装填芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("腰射精准度特化芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("垂直后坐力特化芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("水平后坐力特化芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("棍花芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("1级能量八翼芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("2级能量八翼芯片"), 15);
	物资类型映射表.emplace(GBK转UTF8("3级能量八翼芯片"), 15);


	// 16-常用投掷
	物资类型映射表.emplace(GBK转UTF8("积木"), 16);
	物资类型映射表.emplace(GBK转UTF8("拉钩钩"), 16);
	物资类型映射表.emplace(GBK转UTF8("火球"), 16);
	物资类型映射表.emplace(GBK转UTF8("传送大炮"), 16);
	物资类型映射表.emplace(GBK转UTF8("啵啵"), 16);
	物资类型映射表.emplace(GBK转UTF8("虫洞手雷"), 16);
	物资类型映射表.emplace(GBK转UTF8("治疗弹"), 16);
	物资类型映射表.emplace(GBK转UTF8("爱心云雾弹"), 16);
	物资类型映射表.emplace(GBK转UTF8("战术掩体"), 16);
	物资类型映射表.emplace(GBK转UTF8("云雾弹"), 16);
	物资类型映射表.emplace(GBK转UTF8("雪球"), 16);
	物资类型映射表.emplace(GBK转UTF8("手雷"), 16);
	物资类型映射表.emplace(GBK转UTF8("手雷Max"), 16);
	物资类型映射表.emplace(GBK转UTF8("土炮炮"), 16);

	// 17-常用近战
	物资类型映射表.emplace(GBK转UTF8("棒球棍"), 17);
	物资类型映射表.emplace(GBK转UTF8("武功秘籍"), 17);
	物资类型映射表.emplace(GBK转UTF8("平底锅"), 17);
	物资类型映射表.emplace(GBK转UTF8("马桶搋"), 17);
	物资类型映射表.emplace(GBK转UTF8("撑衣杆"), 17);
	物资类型映射表.emplace(GBK转UTF8("小叉子"), 17);
	物资类型映射表.emplace(GBK转UTF8("魔法棒"), 17);


	// 18-常用子弹
	物资类型映射表.emplace(GBK转UTF8("9mm"), 18);
	物资类型映射表.emplace(GBK转UTF8("5.56mm"), 18);
	物资类型映射表.emplace(GBK转UTF8("7.62mm"), 18);
	物资类型映射表.emplace(GBK转UTF8("能量子弹"), 18);
	物资类型映射表.emplace(GBK转UTF8("5.8mm"), 18);
	物资类型映射表.emplace(GBK转UTF8(".45ACP"), 18);
	物资类型映射表.emplace(GBK转UTF8("300马格南"), 18);
	物资类型映射表.emplace(GBK转UTF8(".50BMG"), 18);
	物资类型映射表.emplace(GBK转UTF8("火箭弹"), 18);
	物资类型映射表.emplace(GBK转UTF8("爆燃弹"), 18);
	物资类型映射表.emplace(GBK转UTF8("霰弹"), 18);


	// 19-信号枪
	物资类型映射表.emplace(GBK转UTF8("信号枪"), 19);
	物资类型映射表.emplace(GBK转UTF8("信号弹"), 19);
}
// 优化后的取物资类型函数（O(1)查找）
int 取物资类型(const char* 物资名字) {
	// 首次调用时初始化哈希表（仅执行一次）
	static bool 已初始化 = false;
	if (!已初始化) {
		初始化物资类型映射表();
		已初始化 = true;
	}

	// 空指针保护
	if (物资名字 == nullptr) return 0;

	// 字符串内容查找（修复原指针比较的错误）
	std::string 物资名_UTF8(物资名字);
	auto 迭代器 = 物资类型映射表.find(物资名_UTF8);
	return (迭代器 != 物资类型映射表.end()) ? 迭代器->second : 0;
}

void Item_Main() {
	uintptr_t 物资地址 = 0;
	UINT32 物资数量 = 0;
	D3D坐标 物资坐标 = { 0,0,0 };
	char 物资中文名[64];
	UINT32 是否拾取 = 0;
	int 物资类型 = 0;
	ImVec2 中心 = ImGui::GetIO().DisplaySize;
	D2D坐标 屏幕中心 = { 0 , 0 };
	屏幕中心.x = 中心.x * 0.5f;
	屏幕中心.y = 中心.y * 0.5f;
	Matrix 矩阵 = 获取矩阵();
	uintptr_t 临时 = 0; 
	临时 = 取数组入口(); if (!临时) return; 
	临时 = *reinterpret_cast<uintptr_t*>(临时 + Start_Game::BattleWorld); if (!临时) return; 
	临时 = *reinterpret_cast<uintptr_t*>(临时 + Start_Game::ItemManager); if (!临时) return; 
	临时 = *reinterpret_cast<uintptr_t*>(临时 + 数据::物资地址[4]); if (!临时) return; 
	物资数量 = *reinterpret_cast<UINT32*>(临时 + 数据::物资数量); 
	if (物资数量 == 0) return;
	if (物资数量 > 1024) 物资数量 = 1024;
	临时 = *reinterpret_cast<uintptr_t*>(临时 + 数据::物资地址[5]);if (!临时) return; 
	std::vector<glm::vec4> 全_物品叠加;
	for (int i = 0; i < 物资数量; i++) {
		物资地址 = *reinterpret_cast<uintptr_t*>(临时 + 数据::物资地址[6] + i * 0x18);
		if (!物资地址) continue;
		物资坐标.x = *reinterpret_cast<float*>(物资地址 + 数据::物资坐标X);
		物资坐标.y = *reinterpret_cast<float*>(物资地址 + 数据::物资坐标Y);
		物资坐标.z = *reinterpret_cast<float*>(物资地址 + 数据::物资坐标Z);
		是否拾取 = *reinterpret_cast<UINT32*>(物资地址 + 数据::是否拾取);
		if (是否拾取 != 0) continue;
		strcpy_s(物资中文名, sizeof(物资中文名), 取物资中文名(物资地址).c_str());
		if (物资::显示物资) 物资类型 = 取物资类型(物资中文名);
		glm::vec4 BoxRect;
		VectorBox BoxScreen = { 0,0,0 };
		if (WorldToScreenBox(BoxScreen, 物资坐标, 矩阵, 屏幕中心)) {
			float BoxH = BoxScreen.y1 - BoxScreen.y;
			BoxRect.x = BoxScreen.x - BoxH / 4;
			BoxRect.y = BoxScreen.y;
			BoxRect.w = BoxH / 2.5;
			BoxRect.z = BoxH;
			char 缓冲[256];
			std::snprintf(缓冲, sizeof(缓冲), "%s", 物资中文名);
			glm::vec4 BoxRect_c = BoxRect;
			if (物资::显示物资) {
				//绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 白色, 1, IM_COL32(0, 0, 0, 180));
				if (物资::步枪 && 物资类型 == 1) {
					if (物资::物资堆叠) {叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c);}
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 天蓝, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::冲锋枪 && 物资类型 == 2) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 橙黄, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::射手步枪 && 物资类型 == 3) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 品红, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::狙击枪 && 物资类型 == 4) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 灰绿, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::机枪 && 物资类型 == 5) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, IM_COL32(255, 255, 255, 255), 1, 紫红);
				}
				if (物资::霰弹枪 && 物资类型 == 6) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, IM_COL32(255, 255, 255, 255), 1, 蓝灰);
				}
				if (物资::手枪 && 物资类型 == 7) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, IM_COL32(255, 255, 255, 255), 1, 藏蓝);
				}
				if (物资::特殊武器 && 物资类型 == 8) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 蓝灰, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::暗器 && 物资类型 == 9) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 白色, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::常用装备 && 物资类型 == 10) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 嫩黄, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::常用身份 && 物资类型 == 11) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 艳青, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::常用药品 && 物资类型 == 12) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 绿色, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::常用倍镜 && 物资类型 == 13) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 桃红, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::常用配件 && 物资类型 == 14) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 嫩绿, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::常用芯片 && 物资类型 == 15) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, IM_COL32(255, 255, 255, 255), 1, 藏青);
				}
				if (物资::常用投掷 && 物资类型 == 16) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 青绿, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::常用近战 && 物资类型 == 17) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 白色, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资::常用子弹 && 物资类型 == 18) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 黄色, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资类型 == 0) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 白色, 1, IM_COL32(0, 0, 0, 180));
				}
				if (物资类型 == 19) {
					if (物资::物资堆叠) { 叠加(BoxRect_c, 全_物品叠加);全_物品叠加.push_back(BoxRect_c); }
					绘制直线(屏幕中心.x, 0, BoxRect_c.x + BoxRect_c.w * 0.5f, BoxRect_c.y + BoxH * 0.7f + ImGui::CalcTextSize(缓冲).y, 红色, 1);
					绘制描边文本(BoxRect_c.x + BoxRect_c.w * 0.5f - ImGui::CalcTextSize(缓冲).x * 0.5f, BoxRect_c.y + BoxH * 0.7f, 缓冲, 红色, 1, IM_COL32(0, 0, 0, 180));
				}











			}



		}
	}
}