//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include <vector>
#include <map>
#include <cstring>
#include <unordered_map>
#include "..\Other\Other.h"
#include "..\..\GUI\GUI.h"
#include "Beautify.h"
// 编码_Ansi到Unicode
std::vector<unsigned char> AnsiToUnicodeBytes(const std::string& ansiStr)
{
    std::vector<unsigned char> byteData;
    if (ansiStr.empty()) return byteData;

    // Ansi转宽字符WCHAR
    int wcharLen = MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), -1, nullptr, 0);
    if (wcharLen <= 0) return byteData;

    std::wstring wideStr(wcharLen, 0);
    MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), -1, &wideStr[0], wcharLen);

    // WCHAR直接转字节
    size_t byteCount = wideStr.size() * sizeof(WCHAR);
    byteData.resize(byteCount);
    memcpy(byteData.data(), wideStr.data(), byteCount);

    return byteData;
}
// 到字节集
std::vector<unsigned char> IntToBytes(int num)
{
    std::vector<unsigned char> res(4);
    // 小端存储
    res[0] = (num >> 0) & 0xFF;
    res[1] = (num >> 8) & 0xFF;
    res[2] = (num >> 16) & 0xFF;
    res[3] = (num >> 24) & 0xFF;
    return res;
}
//uintptr_t到字节集
std::vector<unsigned char> UintPtrToBytes(uintptr_t val)
{
    std::vector<unsigned char> bytes(sizeof(uintptr_t), 0);
    unsigned char* p = reinterpret_cast<unsigned char*>(&val);
    memcpy(bytes.data(), p, sizeof(uintptr_t));
    return bytes;
}

std::string 取特效名称(const std::string& Name)
{
    // 武器名 -> 特效名 映射表
    static const std::unordered_map<std::string, std::string> effectMap = {
        {"UMP9_15_A", "HTRshoot_1"},
        {"UMP9_15_B", "HTRshoot_1"},
        {"UMP9_15_C", "HTRshoot_1"},

        {"RailGun_7_A", "HTRshoot_4"},
        {"RailGun_7_B", "HTRshoot_4"},
        {"RailGun_7_C", "HTRshoot_4"},

        {"M416_18_A", "HTRshoot_7"},
        {"M416_18_B", "HTRshoot_7"},
        {"M416_18_C", "HTRshoot_7"},

        {"FocusGun_15_A", "HTRshoot_8"},
        {"FocusGun_15_B", "HTRshoot_8"},
        {"FocusGun_15_C", "HTRshoot_8"},

        {"EnergySmg_19_A", "HTRshoot_9"},
        {"EnergySmg_19_B", "HTRshoot_9"},
        {"EnergySmg_19_C", "HTRshoot_9"},

        {"Galil_3_A", "HTRshoot_10"},
        {"Galil_3_B", "HTRshoot_10"},
        {"Galil_3_C", "HTRshoot_10"},

        {"PP19_2_A", "HTRshoot_11"},
        {"PP19_2_B", "HTRshoot_11"},
        {"PP19_2_C", "HTRshoot_11"},

        {"MP5_7_A", "HTRshoot_14"},
        {"MP5_7_B", "HTRshoot_14"},
        {"MP5_7_C", "HTRshoot_14"},

        {"QBZ192_6_A", "HTRshoot_15"},
        {"QBZ192_6_B", "HTRshoot_15"},
        {"QBZ192_6_C", "HTRshoot_15"},

        {"QBZ192_10", "HTRshoot_16"},
        {"QBZ192_35", "HTRshoot_17"},
        {"QBZ192_36", "HTRshoot_18"},
        {"QBZ192_37", "HTRshoot_19"},
        {"QBZ192_38", "HTRshoot_20"},
        {"QBZ192_39", "HTRshoot_21"},

        {"SCARL_20", "HTRshoot_22"},
        {"SCARL_45", "HTRshoot_23"},
        {"SCARL_46", "HTRshoot_24"},
        {"SCARL_47", "HTRshoot_25"},
        {"SCARL_48", "HTRshoot_26"},
        {"SCARL_49", "HTRshoot_27"},

        {"AUG_21_A", "HTRshoot_28"},
        {"AUG_21_B", "HTRshoot_28"},
        {"AUG_21_C", "HTRshoot_28"},
        
        {"UMP9_27", "HTRshoot_29"},
        {"UMP9_31", "HTRshoot_30"},
        {"UMP9_32", "HTRshoot_31"},
        {"UMP9_33", "HTRshoot_32"},
        {"UMP9_34", "HTRshoot_33"},
        {"UMP9_35", "HTRshoot_34"},


    };

    // 查找key，找不到返回空字符串
    auto iter = effectMap.find(Name);
    if (iter != effectMap.end())
    {
        return iter->second;
    }
    return "";
}

// 常量定义
constexpr const char* INI_PATH = ".\\TL-C++_MH.ini";
// 扩展分类：暗器、魔法武器、近战、背包都有独立ID
enum SkinType { GUN = 1, THROW = 2, MAGIC = 3, MELEE = 4, BACKPACK = 5 };
const char* Sec[] = { "","枪械皮肤配置","暗器皮肤配置","魔法武器皮肤配置","近战皮肤配置","背包皮肤配置"};

// 分类存储
std::unordered_map<std::string, std::string> g_Map[6];

int GetSecIndex(int skinType)
{
    switch (skinType)
    {
    case GUN: return 1;
    case THROW: return 2;
    case MAGIC: return 3;
    case MELEE: return 4;
    case BACKPACK: return 5;
    default: return 0;
    }
}

// 武器键表：{分类ID,武器名}
const std::pair<int, const char*> WeaponList[] = {
    // 枪械 GUN=1
    {1,"QBZ03"},{1,"QBZ192"},{1,"M416"},{1,"M16A4"},{1,"SCARL"},{1,"QBZ"},{1,"AUG"},{1,"EnergyRifle"},
    {1,"AKM"},{1,"AK12"},{1,"Groza"},{1,"Tavor"},{1,"Galil"},{1,"MP5"},{1,"PP19"},{1,"Vector"},
    {1,"TommyGun"},{1,"MicroUZI"},{1,"UMP9"},{1,"P90"},{1,"EnergySmg"},{1,"VSS"},{1,"Mini14"},{1,"MK14"},
    {1,"SLR"},{1,"SKS"},{1,"QBU191"},{1,"RailGun"},{1,"Kar98"},{1,"AWM"},{1,"M24"},{1,"Barrett"},
    {1,"M249"},{1,"HK13"},{1,"Minigun"},{1,"DP28"},{1,"FocusGun"},{1,"FunnyGunBadStick"},{1,"RPG"},{1,"Bow"},
    {1,"ParticleCannon"},{1,"HandCannon"},{1,"ZiZiBeng"},{1,"FireBallLauncher"},{1,"KSG"},{1,"S12K"},{1,"S686"},{1,"S1897"},{1,"EnergySG"},
    // 暗器 THROW=2
    {2,"FlyKnife"},{2,"FlyProbe"},
    // 魔法武器 MAGIC=3
    {3,"Thundetalisama"},{3,"Gourd"},{3,"Crystalball"},{3,"Harrywand"},
    // 近战 MELEE=4
    {4,"Bat"},{4,"Pan"},{4,"Plunger"},{4,"Pole"},{4,"Fork"},{4,"DemonMachete"},{4,"HolySword"},
    // 背包 BACKPACK=5
    {5,"BackpackLevel1"},{5,"BackpackLevel2"},{5,"BackpackLevel3"},{5,"BackpackLevel4"},
};
std::string GetWeaponSkin(const std::string& weaponName, int skinType)
{
    if (skinType < 1 || skinType > 5) return "";
    auto it = g_Map[skinType].find(weaponName);
    return it == g_Map[skinType].end() ? "" : it->second;
}

// 新增：按武器名查分类（用于把暗器/魔法武器也包含到手持武器处理）
int GetWeaponCategoryByName(const std::string& name)
{
    for (const auto& item : WeaponList) {
        if (name == item.second) return item.first;
    }
    return GUN; // 默认视为枪械
}

bool isFileExists(const std::string& filePath) {
    // GetFileAttributesA 用于获取文件属性，返回INVALID_FILE_ATTRIBUTES表示文件不存在
    DWORD fileAttr = GetFileAttributesA(filePath.c_str());
    // 检查：1. 属性获取成功 2. 不是目录（确保是文件）
    return (fileAttr != INVALID_FILE_ATTRIBUTES) && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY);
}
// 生成空白配置
void CreateEmptySkinConfig()
{
    for (auto& item : WeaponList)
    {
        int type = item.first;
        int secIdx = GetSecIndex(type); // 关键：和读取统一映射
        const char* key = item.second;
        WritePrivateProfileStringA(Sec[secIdx], key, "", INI_PATH);
    }
}

// 读取全部配置
void LoadSkinConfig()
{
    char buf[1024]{};
    for (auto& map : g_Map) map.clear();
    for (auto& item : WeaponList)
    {
        int t = item.first;
        int secIdx = GetSecIndex(t); // 转换正确分区下标
        const char* key = item.second;
        GetPrivateProfileStringA(Sec[secIdx], key, "", buf, 1024, INI_PATH);
        g_Map[t][key] = buf;
        ZeroMemory(buf, 1024);
    }
}
void InitConfig()
{
    if (!isFileExists(INI_PATH))CreateEmptySkinConfig();
    LoadSkinConfig();
}




std::string 取背包英文名(uintptr_t 玩家地址) {
    if (!玩家地址)return "";
    char Name[64] = "";
    uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::PackEquip);
    if (临时) {
        临时 = *reinterpret_cast<uintptr_t*>(临时 + AbsPickItemNet::ItemSign);
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

std::string 取近战英文名(uintptr_t 玩家地址) {
    if (!玩家地址)return "";
    char Name[64] = "";
    uintptr_t 临时 = *reinterpret_cast<uintptr_t*>(玩家地址 + BattleRoleLogic::Weapon - 0x8);
    if (临时) {
        临时 = *reinterpret_cast<uintptr_t*>(临时 + AbsPickItemNet::ItemSign);
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
    char 近战名字[64];
    strncpy_s(近战名字, Name, sizeof(近战名字) - 1);
    近战名字[sizeof(近战名字) - 1] = '\0';
    return 近战名字;
}

void 枪械美化(std::string 皮肤名字, std::string 特效名字) {
    if (皮肤名字.empty()) return;
    uintptr_t 申请地址 = 0;
    uintptr_t 原本地址 = 0;
    std::vector<unsigned char> 改写字节 = {};
    uintptr_t 特效申请地址 = 0;
    uintptr_t 特效原本地址 = 0;
    std::vector<unsigned char> 特效改写字节 = {};
    申请地址 = 申请内存(0, 128);
    特效申请地址 = 申请内存(0, 128);
    if (!本人数据::本人地址)return;
    if (!*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::Weapon))return;
    原本地址 = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::Weapon) + AbsPickItemNet::skinSign);
    if (!原本地址)return;
    特效原本地址 = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::Weapon) + AbsPickItemNet::ShootSign);
    if (!特效原本地址 && 特效名字.empty()) {
        // 如果没有有效特效地址且没有传入特效名称，允许继续只改皮肤
        特效原本地址 = 0;
    }
    改写字节 = 读字节集(原本地址, 16);
    auto 皮肤字节段1 = IntToBytes(static_cast<int>(皮肤名字.size()));
    改写字节.insert(改写字节.end(), 皮肤字节段1.begin(), 皮肤字节段1.end());
    auto 皮肤字节段2 = AnsiToUnicodeBytes(皮肤名字);
    改写字节.insert(改写字节.end(), 皮肤字节段2.begin(), 皮肤字节段2.end());
    if (特效原本地址) {
        特效改写字节 = 读字节集(特效原本地址, 16);
        auto 特效字节段1 = IntToBytes(static_cast<int>(特效名字.size()));
        特效改写字节.insert(特效改写字节.end(), 特效字节段1.begin(), 特效字节段1.end());
        auto 特效字节段2 = AnsiToUnicodeBytes(特效名字);
        特效改写字节.insert(特效改写字节.end(), 特效字节段2.begin(), 特效字节段2.end());
        写字节集(特效申请地址, 特效改写字节);
        std::vector<unsigned char> 特效申请地址字节 = UintPtrToBytes(特效申请地址);
        写字节集(*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::Weapon) + AbsPickItemNet::ShootSign, 特效申请地址字节);
    }
    写字节集(申请地址, 改写字节);
    写字节集(*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::Weapon) + AbsPickItemNet::skinSign + 0x8, { 0x01 });
    std::vector<unsigned char> 申请地址字节 = UintPtrToBytes(申请地址);
    写字节集(*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::Weapon) + AbsPickItemNet::skinSign, 申请地址字节);
}
void 背包美化(std::string 皮肤名字) {
    if (皮肤名字.empty()) return;
    uintptr_t 申请地址 = 0;
    uintptr_t 原本地址 = 0;
    std::vector<unsigned char> 改写字节 = {};
    申请地址 = 申请内存(0, 128);
    if (!本人数据::本人地址)return;
    if (!*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::PackEquip))return;
    原本地址 = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::PackEquip) + AbsPickItemNet::skinSign);
    if (!原本地址)return;
    改写字节 = 读字节集(原本地址, 16);
    auto 皮肤字节段1 = IntToBytes(static_cast<int>(皮肤名字.size()));
    改写字节.insert(改写字节.end(), 皮肤字节段1.begin(), 皮肤字节段1.end());
    auto 皮肤字节段2 = AnsiToUnicodeBytes(皮肤名字);
    改写字节.insert(改写字节.end(), 皮肤字节段2.begin(), 皮肤字节段2.end());
    写字节集(申请地址, 改写字节);
    std::vector<unsigned char> 申请地址字节 = UintPtrToBytes(申请地址);
    写字节集(*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::PackEquip) + AbsPickItemNet::skinSign, 申请地址字节);
}
void 近战美化(std::string 皮肤名字) {
    if (皮肤名字.empty()) return;
    uintptr_t 申请地址 = 0;
    uintptr_t 原本地址 = 0;
    std::vector<unsigned char> 改写字节 = {};
    申请地址 = 申请内存(0, 128);
    if (!本人数据::本人地址)return;
    if (!*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::Weapon - 0x8))return;
    原本地址 = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::Weapon - 0x8) + AbsPickItemNet::skinSign);
    if (!原本地址)return;
    改写字节 = 读字节集(原本地址, 16);
    auto 皮肤字节段1 = IntToBytes(static_cast<int>(皮肤名字.size()));
    改写字节.insert(改写字节.end(), 皮肤字节段1.begin(), 皮肤字节段1.end());
    auto 皮肤字节段2 = AnsiToUnicodeBytes(皮肤名字);
    改写字节.insert(改写字节.end(), 皮肤字节段2.begin(), 皮肤字节段2.end());
    写字节集(申请地址, 改写字节);
    std::vector<unsigned char> 申请地址字节 = UintPtrToBytes(申请地址);
    写字节集(*reinterpret_cast<uintptr_t*>(本人数据::本人地址 + BattleRoleLogic::Weapon - 0x8) + AbsPickItemNet::skinSign, 申请地址字节);
}

void Beautifly_main() {
    if (!美化::开启) return;
    static std::string 缓存枪械名字 = "";
    std::string 枪械名字 = 取角色手持英文(本人数据::本人地址);
    if (枪械名字 != 缓存枪械名字) {
        int cat = GetWeaponCategoryByName(枪械名字);
        std::string 皮肤 = GetWeaponSkin(枪械名字, cat);
        std::string 特效 = (cat == GUN) ? 取特效名称(皮肤) : "";
        if (!皮肤.empty()) {
            枪械美化(皮肤, 特效);
        }
        缓存枪械名字 = 枪械名字;
    }
    static std::string 缓存背包名字 = "";
    std::string 背包名字 = 取背包英文名(本人数据::本人地址);
    if (背包名字 != 缓存背包名字) {
        背包美化(GetWeaponSkin(背包名字, BACKPACK));
        缓存背包名字 = 背包名字;
    }
    static std::string 缓存近战名字 = "";
    std::string 近战名字 = 取近战英文名(本人数据::本人地址);
    if (近战名字 != 缓存近战名字) {
        近战美化(GetWeaponSkin(近战名字, MELEE));
        printf("近战皮肤: %s\n", GetWeaponSkin(近战名字, MELEE).c_str());
        缓存近战名字 = 近战名字;
    }
}