//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include <atomic>
#include "..\Main\pch.h"


namespace 显示 {
	inline bool 绘制方框 = true;
	inline bool 绘制信息 = true;
	inline bool 绘制队友 = false;
	inline bool 显示人数 = true;
	inline bool 绘制射线 = true;
	inline bool 显示头部 = true;
	inline bool 显示骨骼 = true;
	inline bool 显示人机 = true;
	inline bool 显示载具 = true;
	inline bool 被瞄提醒 = true;
	inline bool 开启提示 = true;
	inline bool 绘制准星 = true;
	inline bool FPS面板 = true;
	inline bool 主播模式 = false;
	inline bool 显示自己 = false;
	inline int 信息类型 = 0;
	inline int 方框类型 = 0;
	inline int 射线位置 = 0;
	inline int 载具类型 = 0;


}
namespace 物资 {
	//1步枪 2冲锋枪 3射手步枪 4狙击枪 5机枪 6霰弹枪 7手枪 8特殊武器 9暗器 10常用装备 11常用身份 12常用药品 13常用倍镜 14常用配件 15常用芯片 16常用投掷 17常用近战 18常用子弹 19信号枪
	inline bool 显示物资 = false;
	inline bool 物资堆叠 = true;
	inline bool 步枪 = false;
	inline bool 冲锋枪 = false;
	inline bool 射手步枪 = false;
	inline bool 狙击枪 = false;
	inline bool 机枪 = false;
	inline bool 霰弹枪 = false;
	inline bool 手枪 = false;
	inline bool 特殊武器 = false;
	inline bool 暗器 = false;
	inline bool 常用装备 = false;
	inline bool 常用身份 = false;
	inline bool 常用药品 = false;
	inline bool 常用倍镜 = false;
	inline bool 常用配件 = false;
	inline bool 常用芯片 = false;
	inline bool 常用投掷 = false;
	inline bool 常用近战 = false;
	inline bool 常用子弹 = false;
}
namespace 自瞄 {
	inline bool 开启自瞄 = false;
	inline bool 显示范围 = true;
	inline bool 空手不瞄 = true;
	inline bool 瞄准位置 = true;
	inline bool 子弹追踪 = false;
	inline bool 开启预判 = true;

	namespace 鼠标自瞄 {
		inline int 自瞄位置 = 1;
		inline float 自瞄速度 = 7;
	}
	namespace 内存自瞄 {
		inline int 自瞄位置 = 1;
		inline float 自瞄速度 = 7;
	}
	inline int 自瞄算法 = 0;
	inline float 位置 = 1.6;
	inline int 自瞄热键 = 2;
	inline int 自瞄范围 = 300;
	inline int 功能范围 = 0;



	
}
namespace 内存 {
	inline bool 无限子弹 = false;
	inline bool 子弹爆射 = false;
	inline bool 全枪自动 = false;
	inline bool 全枪聚点 = false;
	inline bool 加特不热 = false;
	inline bool 广角视野 = false;
	inline bool 人物高跳 = false;
	inline bool 主播无后 = false;
	inline bool 超级无后 = false;
	inline bool 载具锁油 = false;
	inline bool 无视缺氧 = false;
	inline bool 无视雪球 = false;
	inline bool 无视火焰 = false;
	inline bool 人机变大 = false;
	inline bool 人物旋转 = false;
	inline bool 子弹瞬击 = false;
	inline bool 落地无僵 = false;

	inline bool 射击间隔 = false;
	inline float 射击间隔值 = 0.1f;

	inline bool 吸取人机 = false;
	inline bool 热键吸取 = false;
	inline int 吸取人机热键 = 67;

	inline bool 意念上车 = false;
	inline int 意念上车热键 = 84;
	inline int 意念上车位置 = 0;

	inline bool 意念炸车 = false;
	inline int 意念炸车热键 = 88;

	inline int 人机大小 = 1;
	inline float 广角大小 = 1;
	inline float 高跳大小 = 0;
	inline float 瞬击值 = 7;

	inline bool 改碰撞体 = false;
	inline bool 超级加速 = false;
	inline bool 魔法子弹 = false;
	inline bool 自由飞天 = false;
	inline bool 意念拳人 = false;
	inline float 飞天速度 = 50;
	inline float 超级加速值 = 0;

	inline bool 标点传送 = false;
	inline int 标点传送热键 = 71;
	inline bool 渐进模式 = false;
	inline float 传送频率 = 10;
	inline float 单次距离 = 5;
	inline bool 传送敌人 = false;
	inline int 传送敌人热键 = 86;
	inline bool 回弹模式 = false;



}
namespace 颜色 {
	inline ImU32 方框颜色 = IM_COL32(0, 255, 255, 255);
	inline ImU32 射线颜色 = IM_COL32(255, 255, 255, 255);
	inline ImU32 骨骼颜色 = IM_COL32(0, 255, 255, 255);
	inline ImU32 自瞄范围 = IM_COL32(255, 255, 255, 255);
	inline ImU32 人机颜色 = IM_COL32(255, 0, 255, 255);
	inline ImU32 载具背景 = IM_COL32(255, 0, 255, 255);


}
namespace 美化 {
	inline bool 开启 = false;
	inline std::string 枪械美化 = "";





}
namespace 本人数据 {
	inline uintptr_t 本人地址 = 0;
	inline glm::vec3 本人坐标 = {};
	inline glm::vec3 目标坐标 = {};
	inline char 本人名字[64];
	inline int AI数量 = 0;
	inline std::atomic<bool> 安全退出 = false;
	inline bool 追踪初始化 = false;
	inline bool Debug = false;
	inline uintptr_t 追踪地址 = NULL;
	inline uintptr_t 瞬击地址 = NULL;
	inline uintptr_t 无僵地址 = NULL;
	inline uintptr_t 间隔地址 = NULL;
	inline std::string 黑名单 = "";
	inline std::string 白名单 = "";
	inline long long 到期时间 = 0;
	inline float 开启提示位置x = 12;
	inline float 开启提示位置y = 320;
	inline float 开启提示透明度 = 0.8f;
	inline bool 限制FPS = false;
	inline float FPS值 = 60;
	inline bool 落雪特效 = false;
	inline bool 外绘 = false;
}
bool CardKey_Menu(std::string 公告, ApiResult 云端版本, std::string 当前版本);
void ImGuiRainbowTextBottomLeft(const char* text, float speed, float marginX, float marginY);
void Draw_Menu();
void MiniMenu(bool* p_open);
void Debug_Menu();