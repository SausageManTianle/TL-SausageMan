//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include "..\..\Main\pch.h"
struct Matrix
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
};
class VectorBox
{
public:
	float x;
	float y;
	float y1;
};

class D2D坐标
{
public:
    float x;
    float y;

    D2D坐标(const glm::vec2& v) : x(v.x), y(v.y) {}
    operator glm::vec2() const { return glm::vec2(x, y); }
    D2D坐标() = default;
    D2D坐标(float x_, float y_) : x(x_), y(y_) {}

    D2D坐标 operator*(float scalar) const {
        return D2D坐标(x * scalar, y * scalar);
    }
    D2D坐标 operator/(float scalar) const {
        return D2D坐标(x / scalar, y / scalar);
    }

    D2D坐标& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    D2D坐标& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    D2D坐标 operator+(const D2D坐标& other) const {
        return D2D坐标(x + other.x, y + other.y);
    }
    D2D坐标 operator-(const D2D坐标& other) const {
        return D2D坐标(x - other.x, y - other.y);
    }

    D2D坐标& operator+=(const D2D坐标& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    D2D坐标& operator-=(const D2D坐标& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    D2D坐标 operator*(const D2D坐标& other) const {
        return D2D坐标(x * other.x, y * other.y);
    }
    D2D坐标 operator/(const D2D坐标& other) const {
        return D2D坐标(x / other.x, y / other.y);
    }

    D2D坐标 operator-() const {
        return D2D坐标(-x, -y);
    }
};

inline D2D坐标 operator*(float scalar, const D2D坐标& vec) {
    return vec * scalar;
}

class D3D坐标
{
public:
    float x;
    float y;
    float z;

    D3D坐标(const glm::vec3& v) : x(v.x), y(v.y), z(v.z) {}
    operator glm::vec3() const { return glm::vec3(x, y, z); }
    D3D坐标() = default;
    D3D坐标(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    D3D坐标 operator*(float scalar) const {
        return D3D坐标(x * scalar, y * scalar, z * scalar);
    }
    D3D坐标 operator/(float scalar) const {
        return D3D坐标(x / scalar, y / scalar, z / scalar);
    }

    D3D坐标& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    D3D坐标& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    D3D坐标 operator+(const D3D坐标& other) const {
        return D3D坐标(x + other.x, y + other.y, z + other.z);
    }
    D3D坐标 operator-(const D3D坐标& other) const {
        return D3D坐标(x - other.x, y - other.y, z - other.z);
    }

    D3D坐标& operator+=(const D3D坐标& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    D3D坐标& operator-=(const D3D坐标& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    D3D坐标 operator*(const D3D坐标& other) const {
        return D3D坐标(x * other.x, y * other.y, z * other.z);
    }
    D3D坐标 operator/(const D3D坐标& other) const {
        return D3D坐标(x / other.x, y / other.y, z / other.z);
    }

    D3D坐标 operator-() const {
        return D3D坐标(-x, -y, -z);
    }
};

inline D3D坐标 operator*(float scalar, const D3D坐标& vec) {
    return vec * scalar;
}

class D4D坐标
{
public:
    float x;
    float y;
    float h; 
    float w;

    D4D坐标(const glm::vec4& v) : x(v.x), y(v.y), h(v.z), w(v.w) {}
    operator glm::vec4() const { return glm::vec4(x, y, h, w); }
    D4D坐标() = default;
    D4D坐标(float x_, float y_, float h_, float w_) : x(x_), y(y_), h(h_), w(w_) {}

    D4D坐标 operator*(float scalar) const {
        return D4D坐标(x * scalar, y * scalar, h * scalar, w * scalar);
    }
    D4D坐标 operator/(float scalar) const {
        return D4D坐标(x / scalar, y / scalar, h / scalar, w / scalar);
    }

    D4D坐标& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        h *= scalar;
        w *= scalar;
        return *this;
    }
    D4D坐标& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        h /= scalar;
        w /= scalar;
        return *this;
    }

    D4D坐标 operator+(const D4D坐标& other) const {
        return D4D坐标(x + other.x, y + other.y, h + other.h, w + other.w);
    }
    D4D坐标 operator-(const D4D坐标& other) const {
        return D4D坐标(x - other.x, y - other.y, h - other.h, w - other.w);
    }

    D4D坐标& operator+=(const D4D坐标& other) {
        x += other.x;
        y += other.y;
        h += other.h;
        w += other.w;
        return *this;
    }
    D4D坐标& operator-=(const D4D坐标& other) {
        x -= other.x;
        y -= other.y;
        h -= other.h;
        w -= other.w;
        return *this;
    }

    D4D坐标 operator*(const D4D坐标& other) const {
        return D4D坐标(x * other.x, y * other.y, h * other.h, w * other.w);
    }
    D4D坐标 operator/(const D4D坐标& other) const {
        return D4D坐标(x / other.x, y / other.y, h / other.h, w / other.w);
    }

    // 6. 一元负号
    D4D坐标 operator-() const {
        return D4D坐标(-x, -y, -h, -w);
    }
};

inline D4D坐标 operator*(float scalar, const D4D坐标& vec) {
    return vec * scalar;
}
const ImU32 白色 = IM_COL32(255, 255, 255, 255);
const ImU32 黑色 = IM_COL32(0, 0, 0, 255);
const ImU32 天蓝 = IM_COL32(0, 191, 255, 255);
const ImU32 橙黄 = IM_COL32(255, 165, 0, 255);
const ImU32 藏青 = IM_COL32(0, 0, 139, 255);
const ImU32 墨绿 = IM_COL32(0, 100, 0, 255);
const ImU32 红褐 = IM_COL32(139, 35, 35, 255);
const ImU32 紫红 = IM_COL32(139, 0, 139, 255);
const ImU32 褐绿 = IM_COL32(150, 75, 0, 255);
const ImU32 蓝色 = IM_COL32(0, 0, 255, 255);
const ImU32 绿色 = IM_COL32(0, 255, 0, 255);
const ImU32 艳青 = IM_COL32(0, 255, 255, 255);
const ImU32 红色 = IM_COL32(255, 0, 0, 255);
const ImU32 品红 = IM_COL32(255, 0, 255, 255);
const ImU32 黄色 = IM_COL32(255, 255, 0, 255);
const ImU32 桃红 = IM_COL32(255, 192, 203, 255);
const ImU32 蓝灰 = IM_COL32(173, 216, 230, 255);
const ImU32 藏蓝 = IM_COL32(0, 0, 128, 255);
const ImU32 嫩绿 = IM_COL32(144, 238, 144, 255);
const ImU32 青绿 = IM_COL32(46, 139, 87, 255);
const ImU32 黄褐 = IM_COL32(184, 134, 11, 255);
const ImU32 粉红 = IM_COL32(255, 192, 203, 255);
const ImU32 嫩黄 = IM_COL32(240, 230, 140, 255);
const ImU32 芙红 = IM_COL32(255, 0, 144, 255);
const ImU32 紫色 = IM_COL32(128, 0, 128, 255);
const ImU32 灰绿 = IM_COL32(144, 238, 144, 255);
const ImU32 青蓝 = IM_COL32(0, 128, 128, 255);

Matrix 获取矩阵();
ULONG64 取数组入口();
ImU32 取队伍颜色(UINT32 编号);
std::string 取角色名称(uintptr_t 玩家地址);
std::string 取角色手持(uintptr_t 玩家地址);
std::string 取角色手持英文(uintptr_t 玩家地址);
std::string 取角色ID(uintptr_t 玩家地址);
std::string 取人机名称(uintptr_t AI地址);
std::string 取载具名称(uintptr_t 载具地址);
std::string 取物资中文名(uintptr_t 物资地址);
std::string 取载具中文名(const std::string& 英文名称);
int 取敌我距离(const D3D坐标& 本人坐标, const D3D坐标& 对象坐标);
int 取准星距离(int 准星X, int 准星Y, int 对象X, int 对象Y);
BOOL WorldToScreenBox(VectorBox& ScreenPos, D3D坐标 WorldPos, Matrix MatrixView, D2D坐标 GameScreen);
BOOL D3D转2D坐标(D3D坐标 WorldPos, D2D坐标& ScreenPos, Matrix MatrixView, D2D坐标 GameScreen);
BOOL D3D转方框坐标(VectorBox& ScreenPos, D3D坐标 WorldPos, Matrix MatrixView, D2D坐标 GameScreen, float 顶部微调, float 底部微调);
void 鼠标自瞄(D3D坐标 WorldPos, Matrix 矩阵, D2D坐标 GameScreen);
void 内存自瞄(D3D坐标 WorldPos, Matrix 矩阵, float 平滑);
D2D坐标 取瞄准位置(D3D坐标 WorldPos, Matrix 矩阵, D2D坐标 GameScreen);
void 绘制信息(D4D坐标 方框, UINT32 距离, UINT32 编号, const char* 玩家名称, const char* 玩家ID, bool 是否被瞄, uintptr_t 玩家地址);
void 绘制信息1(D4D坐标 方框, UINT32 距离, UINT32 编号, const char* 玩家名称, const char* 玩家ID, bool 是否被瞄, uintptr_t 玩家地址);
bool 判断是否倒地(uintptr_t 玩家地址);
bool 判断是否死亡(uintptr_t 玩家地址);
bool 判断是否存在(uintptr_t 玩家地址);
bool 判断是否踩球(uintptr_t 玩家地址);
bool 判断是否被瞄(D3D坐标 本人坐标, D3D坐标 敌人坐标, D2D坐标 敌人朝向);
D2D坐标 取玩家朝向(uintptr_t 玩家地址);
UINT32 取蹲起状态(uintptr_t 玩家地址);

std::string GBK转UTF8(const std::string& gbkStr);
std::string UTF8转GBK(const std::string& utf8_str);
std::string 取中间文本(const std::string& 提取的文本, const std::string& 左边文本, const std::string& 右边文本);
uintptr_t 文本转uintptr_t(const std::string& s);
bool 验证黑名单(const char* 玩家ID, const char* 黑名单文本);
bool 验证白名单(const char* 玩家ID, const char* 白名单文本);
D3D坐标 PredictEnemyNextPos(D3D坐标 当前敌人坐标, float 我与敌人距离);
auto calc_quat(const glm::vec2 _euler) -> std::pair<glm::quat, glm::quat>;
glm::vec2 calculate_angles(const D3D坐标& camPos, const D3D坐标& targetPos);



void 边框方框(float x, float y, float w, float h, ImU32 颜色);
void 填充方框(float X, float Y, float W, float H, ImU32 颜色, int 透明度);
void 透明矩形(float 左边, float 顶边, float 宽度, float 高度, ImU32 颜色, int 透明度, ImU32 边框色);
void 绘制描边文本(float x, float y, const char* 绘制的文本, ImU32 绘制的颜色, float 描边宽度, ImU32 描边颜色);
void 绘制文本(float x, float y, const char* 绘制的文本, ImU32 绘制的颜色);
ImU32 转换颜色带透明度(ImU32 color, int alpha);
void 绘制圆形(float x, float y, float 半径, int 段数, int 线宽, ImU32 颜色);
void 绘制直线(float x1, float y1, float x2, float y2, ImU32 颜色, float 线宽);
void 彩虹圈形(D2D坐标 中心, int 半径, int 线宽);
void 绘制3D方框(D3D坐标 对象坐标, Matrix 矩阵, float 朝向, ImU32 方框颜色, float 顶边偏移, float 底边偏移, int 方框线粗, D2D坐标 屏幕);
void DrawFPSCounter(bool 显示FPS面板);
void RenderSnow(D3D坐标 rolePos, const Matrix& matrix, float speedFactor = 1.2f, int particleCount = 2000, float topHeight = 100.0f, float bottomThreshold = -100.0f, float horizontalRadius = 150.0f);
void 显示人数(D2D坐标 屏幕中心, int 玩家数量, int AI数量, bool 开启显示);
void 绘制准星(float 中心X, float 中心Y, ImU32 颜色, float 圆环半径, float 扩散范围, float 线条粗细, float 十字线长度, float 中心圆点半径);
void DrawFootCircle3D(glm::vec3 footPos, float worldRadius, int lineWidth, Matrix viewMatrix, glm::vec2 screenSize);




std::vector<unsigned char> 读字节集(uintptr_t targetAddr, size_t readLen);
bool 写字节集(uintptr_t targetAddr, const std::vector<unsigned char>& byteVec);
uintptr_t 申请内存(uintptr_t addr, size_t size);
uintptr_t 特征码搜索(const char* 模块名, const char* 字节数组, int 偏移, bool 加入模块 = true);
void HOOK_Jmp(uintptr_t 写入地址, uintptr_t 跳转地址);
void 追踪初始化();
void 追踪(D3D坐标 WorldPo);
void 关闭追踪();
void 开关追踪(bool 条件);
void 子弹瞬击(const float 值);
void 关闭瞬击();
void 落地无僵();
void 关闭无僵();
void 射击间隔(float 间隔);
void 关闭间隔();
void 功能开关(bool& 功能, int 功能快捷键);
void 保存配置();
void 读取配置();

void InitAllOffsets();
void PrintAllOffsets();



namespace 数据 {
	inline  uintptr_t 矩阵地址 = 0x1CF6D28;
	inline  uintptr_t 矩阵偏移[] = { 0xB8,0xA0,0x20,0x10,0x100 };
	inline  uintptr_t 世界地址 = 0x86843A8;//0x86843A8 
	inline  uintptr_t 本人地址[] = { 0xB8,0x228,0x548,0x50,0x10,0x20 };
	inline  uintptr_t 血量 = 0x934;
	inline  uintptr_t 坐标X = 0x740;
	inline  uintptr_t 坐标Y = 0x744;
	inline  uintptr_t 坐标Z = 0x748;
	inline  uintptr_t 阵营 = 0x6F8;
	inline  uintptr_t 蹲起状态 = 0x180;
    inline  uintptr_t 玩家大小 = 0xAB4;
	inline  uintptr_t 是否踩球 = 0x9B8;
	inline  uintptr_t 倒地血量 = 0x92C;
    inline  uintptr_t 玩家平台 = 0x8A8;
	inline  uintptr_t 人物名称[] = { 0x6B8,0x14 };
    inline  uintptr_t 玩家ID[] = { 0xB60,0x14 };
	inline  uintptr_t 手持武器[] = { 0x9B0,0x70,0x14 };
	//以下需要使用公用偏移
	inline  uintptr_t 是否倒地 = 0x561;
	inline  uintptr_t 是否死亡 = 0x827;
	inline  uintptr_t 是否存在 = 0x828;


	inline  uintptr_t AI地址[] = { 0xB8,0x228,0x548,0x298,0x18,0x10,0x20 };
	inline  uintptr_t AI坐标X = 0x64;
	inline  uintptr_t AI坐标Y = 0x68;
	inline  uintptr_t AI坐标Z = 0x6C;
	inline  uintptr_t AI血量 = 0x58;
	inline  uintptr_t AI名字[] = { 0x128,0x14 };
	inline  uintptr_t AI大小 = 0x1CC;


	inline  uintptr_t 载具地址[] = { 0xB8,0x228,0x548,0xB8,0x10,0x20 };
	inline  uintptr_t 载具坐标X = 0x70;
	inline  uintptr_t 载具坐标Y = 0x74;
	inline  uintptr_t 载具坐标Z = 0x78;
	inline  uintptr_t 信息偏移 = 0xB8;
	inline  uintptr_t 载具血量 = 0xBC;
	inline  uintptr_t 最大血量 = 0xB8;
	inline  uintptr_t 载具油量 = 0xB0;
	inline  uintptr_t 最大油量 = 0xB4;
	inline  uintptr_t 载具名字[] = { 0xB8,0x88,0x14 };


	inline  uintptr_t 物资地址[] = { 0xB8,0x228,0x558,0x38,0x28,0x18,0x30};
	inline  uintptr_t 物资坐标X = 0x8C;
	inline  uintptr_t 物资坐标Y = 0x90;
	inline  uintptr_t 物资坐标Z = 0x94;
	inline  uintptr_t 物资数量 = 0x20;
	inline  uintptr_t 是否拾取 = 0xA4;
	inline  uintptr_t 物资英文名[] = { 0x18,0x14 };
	inline  uintptr_t 物资中文名[] = { 0x70,0x14 };

	inline  uintptr_t 公用偏移[] = { 0xA38,0x30,0x4A0 };
	inline  uintptr_t 公用偏移1= 0x690;
	inline  uintptr_t 子弹数量 = 0x6C;
	inline  uintptr_t 枪械自动 = 0x74;
	inline  uintptr_t 枪械射速 = 0x7C;
	inline  uintptr_t 子弹扩散 = 0xA0;
    inline  uintptr_t 枪械热量 = 0x188;
    inline  uintptr_t 枪械预热 = 0x180;
	inline  uintptr_t 朝向X = 0x904;

   
	inline  uintptr_t 主播无后 = NULL;
	inline  uintptr_t 超级无后 = NULL;
	inline  uintptr_t 载具锁油 = NULL;
	inline  uintptr_t 无视缺氧 = NULL;
	inline  uintptr_t 子弹加速 = NULL;
    inline  uintptr_t 无视火焰 = NULL;
    inline  uintptr_t 无视雪球 = NULL;
	inline  uintptr_t 落地无僵 = NULL;
	inline  uintptr_t 枪械间隔 = NULL;
}

class Start_Game {
public:
    inline static int32_t AllCar;
    inline static uintptr_t Uworld;
    inline static int32_t RoleAIManager;
    inline static int32_t BattleRoleLogic;
    inline static int32_t ItemManager;
    inline static int32_t ItemManager1;
    inline static int32_t GameData;
    inline static int32_t StartGame;
    inline static int32_t BattleWorld;

    static uintptr_t GetUworldOffset() {
        uintptr_t add = 特征码搜索("GameAssembly.dll", "48 8b 05 ?? ?? ?? ?? 83 b8 ?? ?? ?? ?? ?? 75 ?? 48 8b c8 e8 ?? ?? ?? ?? 48 8b 05 ?? ?? ?? ?? 48 8b 0d ?? ?? ?? ?? 48 8b 80 ?? ?? ?? ?? 83 b9 ?? ?? ?? ?? ?? 48 8b 98 ?? ?? ?? ?? 75 ?? e8 ?? ?? ?? ?? 33 d2 48 8b cb e8 ?? ?? ?? ?? 48 8b 0d ?? ?? ?? ?? 84 c0", 0, true);
        if (add) {
            uint32_t disp = *reinterpret_cast<uint32_t*>(add + 3);
            uintptr_t Address = add + 7 + disp;
            return Address;
        }
        return 0;
    }
    static int32_t GetGameDataOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("GameData");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("LocalRole");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetStartGameOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRole");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("mStartGame");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetBattleWorldOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRole");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("gameWorld");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetRoleAIManagerOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("StartGame");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<MyRoleAIMgr>k__BackingField");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetBattleRoleLogicOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("StartGame");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("RoleList");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetAllCarOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("StartGame");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("AllCar");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetItemManagerOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleWorld");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("ItemManager");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetItemManager1Offset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("$d");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("$b");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
};

class BattleRoleLogic {
public:
    inline static int32_t roleNetClient;//角色网络客户端
	inline static int32_t NickName;//角色名称
	inline static int32_t HP;//血量
	inline static int32_t WeakValue;//倒地血量
	inline static int32_t TeamNum;//阵营
	inline static int32_t Pos;//坐标
    inline static int32_t SpotPoint;//标点坐标
	inline static int32_t UserCircusBallNet;//是否踩球
	inline static int32_t roleLogicClient;//角色逻辑客户端
	inline static int32_t RoleBuffControl;//角色buff控制
	inline static int32_t Weapon;//手持武器
	inline static int32_t KillRoleNum;//击杀数
	inline static int32_t RoleID;//角色ID
    inline static int32_t PlayerPlatform;//平台
    inline static int32_t RoleSize;//角色大小
    inline static int32_t HeadEquipPart;//头部装备
	inline static int32_t BodyEquipPart;//身体装备
	inline static int32_t PackEquip;//背包装备
	inline static int32_t FunctionalGarmentEquipPart;//功能服装备
	inline static int32_t isOnline;//是否在线


    static int32_t GetroleNetClientOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("RoleNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("roleNetClient");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }

    static int32_t GetNickNameOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("NickName");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetHPOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("WeakValue");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset) + 0x8;
    }
    static int32_t GetWeakValueOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("WeakValue");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetTeamNumOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("TeamNum");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetPosOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("NowRotation");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset) + 16;
    }
    static int32_t GetSpotPointOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("SpotPoint");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetUserCircusBallNetOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("UserCircusBalllNet");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetroleLogicClientOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("roleLogicClient");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetRoleBuffControlOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("MyRoleBuffControl");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetWeaponOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("UserCircusBalllNet");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset) - 0x8;
    }
    static int32_t GetKillRoleNumOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("KillRoleNum");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetRoleIDOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("InSliderArea");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset) + 0x4;
    }
    static int32_t GetPlayerPlatformOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("PlayerPlatform");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetRoleSizeOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("RoleSize");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
	}
    static int32_t GetHeadEquipPartOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("HeadEquipPart");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetBodyEquipPartOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("BodyEquipPart");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetPackEquipOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("PackEquip");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetFunctionalGarmentEquipPartOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("FunctionalGarmentEquipPart");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetisOnlineOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRoleLogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("isOnline");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
private:
};

class BattleRole {
public:
    inline static int32_t RoleClient;//入口
    inline static int32_t MyRoleControl;//RoleControl入口
    inline static int32_t tempIsWeak;//是否倒地
	inline static int32_t hideNoNetRole;//是否死亡
    inline static int32_t IsEventRoleShow;//是否显示
    inline static int32_t lastRotaY;//朝向
	inline static int32_t UserWeapon;//手持武器
	inline static int32_t playerId;//玩家标识
    
    static int32_t GetRoleClientOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("RoleLogicClient");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("RoleClient");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetMyRoleControlOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRole");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("MyRoleControl");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetUserWeaponOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRole");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("UserWeapon");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetlastRotaYOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRole");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("lastRotaY");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GettempIsWeakOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRole");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("tempIsWeak");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GethideNoNetRoleOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRole");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("$m");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetIsEventRoleShowOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRole");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("IsEventRoleShow");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetplayerIdOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("BattleRole");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("autoRoleId");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
private:
};

class RoleBuffControl {
public:
    inline static int32_t JumpNum;//跳跃高度
	inline static int32_t WalkNum;//行走速度
	inline static int32_t CameraRatio;//镜头距离

    static int32_t GetJumpNumOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("RoleBuffControl");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("$d");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetWalkNumOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("RoleBuffControl");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("$B");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetCameraRatioOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("RoleBuffControl");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("CameraRatio");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
private:
};

class AllCar {
public:
	inline static int32_t HP;//血量
	inline static int32_t Name;//载具名称
	inline static int32_t Mirror;//镜像
	inline static int32_t Position;//坐标
	inline static int32_t Consumption;//油量
    inline static int32_t carNetClient;//载具网络客户端
	inline static int32_t _carId;//载具ID


    static int32_t GetHPOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("CarNetMirror");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("SyncHp");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetConsumptionOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("CarNetMirror");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("SyncOilConsumption");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetNameOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("CarNetMirror");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("SyncCarSign");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetPositionOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("CarNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("MovePoint");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetMirrorOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("CarNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("mirror");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetcarNetClientOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("CarNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("carNetClient");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t Get_carIdOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("CarNetClient");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("_carId");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }



};

class RoleAIManager {
public:
	inline static int32_t HP;//血量
	inline static int32_t Name;//名字
	inline static int32_t Position;//坐标
	inline static int32_t RoleSize;//角色大小

    static int32_t GetHPOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("RoleAILogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<Hp>k__BackingField");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetNameOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("RoleAILogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<Name>k__BackingField");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetPositionOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("RoleAILogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<Position>k__BackingField");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetAISizeOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("RoleAILogic");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("RoleSize");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
};

class AbsPickItemNet {
public:
	inline static int32_t AutoId;//物资唯一ID
    inline static int32_t ItemSign;//物资英文名
	inline static int32_t ItemId;//物资ID
	inline static int32_t ItemType;//物资类型
	inline static int32_t ItemName;//物资名称
	inline static int32_t BulletNum;//子弹数量
	inline static int32_t ItemLevel;//物资等级
	inline static int32_t SyncPoint;//同步点数
	inline static int32_t PickRoleId;//拾取角色ID
	inline static int32_t NowValue;//当前值
	inline static int32_t skinSign;//物品皮肤标识
	inline static int32_t ShootSign;//物品射击标识


    static int32_t GetAutoIdOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<AutoId>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetItemSignOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<ItemSign>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetItemIdOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<ItemId>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetItemTypeOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<ItemType>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetItemNameOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<ItemName>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetBulletNumOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<BulletNum>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetItemLevelOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<ItemLevel>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetSyncPointOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<SyncPoint>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetPickRoleIdOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<PickRoleId>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetNowValueOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<NowValue>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetskinSignOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("skinSign");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetShootSignOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("AbsPickItemNet");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("<ShootSign>k__BackingField");
        if (!Field) return -1;
        return static_cast<int32_t>(Field->offset);
    }
};

class Camera_Controller {
public:
	inline static int32_t MyCameraRotationX;//摄像机旋转X
	inline static int32_t MyCameraRotationY;//摄像机旋转Y
    inline static int32_t MyCameraTran;//摄像机旋转Y


	inline static uintptr_t g_pCamObj = 0;//摄像机地址
	inline static bool g_bInitFlag = false;//是否初始化
	inline static bool g_bBusyState = false;//是否忙碌状态

    static int32_t GetMyCameraRotationXOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("CameraController");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("MyCameraRotationX");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetMyCameraRotationYOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("CameraController");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("MyCameraRotationY");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }
    static int32_t GetMyCameraTranOffset() {
        UnityResolve::Assembly* assembly = UnityResolve::Get("Assembly-CSharp.dll");
        UnityResolve::Class* pClass = assembly->Get("CameraController");
        UnityResolve::Field* Field = pClass->Get<UnityResolve::Field>("MyCameraTran");
        if (!Field) {
            return -1;
        }
        return static_cast<int32_t>(Field->offset);
    }


	// 获取摄像机对象地址
    static bool InitCoreModule()
    {
        if (g_bInitFlag && g_pCamObj != 0)
        {
            bool bRet = (g_pCamObj != 0 && g_pCamObj > 0x1000 && g_pCamObj < 0x7FFFFFFF);
            if (bRet) return true;
            g_bInitFlag = false;
        }

        if (g_bBusyState)
            return g_pCamObj != 0;

        g_bBusyState = true;
        bool bResult = false;
        uintptr_t tmpPtr = 0;

        try
        {
            auto* mod = UnityResolve::Get("Assembly-CSharp.dll");
            auto* clsInst = mod->Get("CameraController");
            auto objList = clsInst->FindObjectsByType<void*>();

            if (!objList.empty())
            {
                tmpPtr = (uintptr_t)objList[0];
                auto offX = Camera_Controller::GetMyCameraRotationXOffset();
                auto offY = Camera_Controller::GetMyCameraRotationYOffset();

                if (offX != -1 && offY != -1)
                {
                    Camera_Controller::MyCameraRotationX = offX;
                    Camera_Controller::MyCameraRotationY = offY;
                    g_pCamObj = tmpPtr;
                    bResult = true;
                }
            }
        }
        catch (...)
        {
            bResult = false;
        }
        g_bInitFlag = bResult;
        g_bBusyState = false;
        return bResult;
    }
	// 写入摄像机旋转角度
    static void WriteCamRot(const std::pair<glm::quat, glm::quat>& qData)
    {
        if (!g_bInitFlag)
        {
			Camera_Controller::InitCoreModule();
            if (!g_bInitFlag) return;
        }

        if (!(g_pCamObj != 0 && g_pCamObj > 0x1000 && g_pCamObj < 0x7FFFFFFF))
            return;

        auto offX = Camera_Controller::MyCameraRotationX;
        auto offY = Camera_Controller::MyCameraRotationY;

        glm::quat* pQx = reinterpret_cast<glm::quat*>(g_pCamObj + offX);
        glm::quat* pQy = reinterpret_cast<glm::quat*>(g_pCamObj + offY);

        if (!pQx || !pQy)
            return;

        DWORD dwOldProt;
        if (VirtualProtect(pQx, sizeof(glm::quat), PAGE_READWRITE, &dwOldProt))
        {
            *pQx = qData.first;
            VirtualProtect(pQx, sizeof(glm::quat), dwOldProt, &dwOldProt);
        }
        if (VirtualProtect(pQy, sizeof(glm::quat), PAGE_READWRITE, &dwOldProt))
        {
            *pQy = qData.second;
            VirtualProtect(pQy, sizeof(glm::quat), dwOldProt, &dwOldProt);
        }
    }

};




/*
class AC_JumpState final {
public:
    enum Value {
        None = 0,
        StartJump = 1,
        JumpUp = 2,
        JumpDown = 3,
        JumpToGround = 4,
        Fall = 5,
        ParabolaMoveUp = 6,
        ElasticMoveUp = 7
    };

    inline static UnityResolve::Class* class_ = nullptr;

    static void Init() {
        class_ = UnityResolve::Get("Assembly-CSharp.dll")->Get("AC_JumpState");
    }
};
//落地无僵
class AC_JumpStateHook final {
public:
    inline static UnityResolve::MethodPointer<void, AC_JumpStateHook*, AC_JumpState::Value> AC_JumpState_init;
    inline static UnityResolve::Class* class_ = nullptr;

    static void AC_JumpState_hook(AC_JumpStateHook* _this, AC_JumpState::Value _a);
    static void Init();
};*/
// 魔法子弹
class 魔术_BulletControl final {
public:
    inline static UnityResolve::MethodPointer<void,
        魔术_BulletControl*,
        UnityResolve::UnityType::Vector3,
        UnityResolve::UnityType::Vector3> local_role_weapon_init;

    inline static UnityResolve::Class* class_ = nullptr;

    static void local_role_weapon_init_hook(
        魔术_BulletControl* _bc,
        UnityResolve::UnityType::Vector3 _a,
        UnityResolve::UnityType::Vector3 _b
    );

    static void Init();
};
// 子弹瞬击
class WeaponControlHook final {
public:
    inline static UnityResolve::MethodPointer<UnityResolve::UnityType::Int32,WeaponControlHook*,float,BOOL*> local_role_weapon_init;

    inline static UnityResolve::Class* class_ = nullptr;

    static UnityResolve::UnityType::Int32 local_role_weapon_init_hook(
        WeaponControlHook* _this,
        float holdOnPower,
        BOOL* isBulletCost
    );

    static void Init();
};
/*
class AC_JumpState final {
public:
    enum Value {
        None = 0,
        StartJump = 1,
        JumpUp = 2,
        JumpDown = 3,
        JumpToGround = 4,
        Fall = 5,
        ParabolaMoveUp = 6,
        ElasticMoveUp = 7
    };
    inline static UnityResolve::Class* class_;

    static void Init() {
        class_ = UnityResolve::Get("Assembly-CSharp.dll")->Get("AC_JumpState");
    }
};
//落地无僵
class AC_JumpStateHook final {
public:
    inline static UnityResolve::MethodPointer<void, AC_JumpStateHook*, AC_JumpState::Value> AC_JumpState_init;
    inline static UnityResolve::Class* class_;

    static auto AC_JumpState_hook(AC_JumpStateHook* _this, AC_JumpState::Value _a) -> void {
        if (内存::落地无僵) {
            if (_a != 5) {
                return HardBreakPoint::call_origin(AC_JumpState_hook, _this, _a);
            }
        }
        else
        {
            return HardBreakPoint::call_origin(AC_JumpState_hook, _this, _a);
        }

    }

    static void Init() {
        class_ = UnityResolve::Get("Assembly-CSharp.dll")->Get("BattleRoleLogic");
        class_->Get<UnityResolve::Method>("$Pc")->Cast(AC_JumpState_init);
        HardBreakPoint::set_break_point(AC_JumpState_init, AC_JumpState_hook);
    }
};
//魔法子弹
class 魔术_BulletControl final {
public:
    inline static UnityResolve::MethodPointer<void, 魔术_BulletControl*, UnityResolve::UnityType::Vector3, UnityResolve::UnityType::Vector3> local_role_weapon_init;
    inline static UnityResolve::Class* class_;
    static auto local_role_weapon_init_hook(魔术_BulletControl* _bc, UnityResolve::UnityType::Vector3 _a, UnityResolve::UnityType::Vector3 _b) -> void {
        //HardBreakPoint::call_origin(local_role_weapon_init_hook, _bc, _a, _b);
        if (内存::魔法子弹) {
            if (GetAsyncKeyState(自瞄::自瞄热键) != 0) {
                UnityResolve::UnityType::Vector3 final_pos = 本人数据::目标坐标;
                if (final_pos.x != 0) {
                    final_pos.y = final_pos.y + 0.71;
                    _a = final_pos;                 // 起点
                    _b = glm::vec3(0, -5, -0.71);   // 终点
                }
            }
        }
        return HardBreakPoint::call_origin(local_role_weapon_init_hook, _bc, _a, _b);
    }
    static void Init() {
        class_ = UnityResolve::Get("Assembly-CSharp.dll")->Get("$QA");
        auto method = class_->Get<UnityResolve::Method>("$oB", { "UnityEngine.Vector3", "UnityEngine.Vector3" });
        if (method) {
            method->Cast(local_role_weapon_init);

            HardBreakPoint::set_break_point(local_role_weapon_init, local_role_weapon_init_hook);
        }
    }

};
//子弹瞬击
class WeaponControlHook final {
public:
    inline static UnityResolve::MethodPointer<UnityResolve::UnityType::Int32, WeaponControlHook*, float, BOOL*> local_role_weapon_init;
    inline static UnityResolve::Class* class_;
    static auto local_role_weapon_init_hook(WeaponControlHook* _this, float holdOnPower, BOOL* isBulletCost) -> UnityResolve::UnityType::Int32 {
        if (内存::子弹瞬击) {
            holdOnPower = 内存::瞬击值;
        }
        return HardBreakPoint::call_origin(local_role_weapon_init_hook, _this, holdOnPower, isBulletCost);
    }

    static void Init() {
        class_ = UnityResolve::Get("Assembly-CSharp.dll")->Get("WeaponControl");
        class_->Get<UnityResolve::Method>("Fire")->Cast(local_role_weapon_init);
        HardBreakPoint::set_break_point(local_role_weapon_init, local_role_weapon_init_hook);
    }
};*/

