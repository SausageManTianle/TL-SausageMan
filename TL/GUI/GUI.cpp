//TLnb666 天乐开源，盗版二改死全家 QQ 2738114690
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "GUI.h"
#include "..\Class\Other\Other.h"
#include "..\Class\Role\Role.h"
#define U8(x) ((const char*)u8##x)




// ImGui窗口绘制函数

//选择滑动块系统
static bool ImGuiSlidingCapsuleSelector(const char* id, const std::vector<const char*>& labels, int& selected, ImVec2 itemSize = ImVec2(-FLT_MIN, 50.0f))
{
    ImGui::PushID(id);
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 cur = ImGui::GetCursorScreenPos();

    float W = (itemSize.x < 0.0f) ? ImGui::GetContentRegionAvail().x : itemSize.x;
    float H = (itemSize.y <= 0.0f) ? ImGui::GetFrameHeight() : itemSize.y;
    float gap = ImGui::GetStyle().ItemSpacing.y;
    int n = (int)labels.size();
    if (n <= 0) { ImGui::PopID(); return false; }

    ImVec2 total = ImVec2(W, n * H + (n - 1) * gap);
    ImGui::InvisibleButton(id, total);
    bool clicked = false;
    bool itemHovered = ImGui::IsItemHovered();
    ImVec2 mouse = io.MousePos;

    ImGuiID uid = ImGui::GetID(id);
    ImGuiStorage* st = ImGui::GetStateStorage();

    float pos = st->GetFloat(uid, selected * (H + gap));
    float vel = st->GetFloat(uid ^ 0xA1A1, 0.0f);

    st->SetInt(uid ^ 0x1337, selected);
    float target = selected * (H + gap);
    float dt = ImMax(1.0f / 1000.0f, io.DeltaTime);

    // 弹簧动画
    const float K = 380.0f;
    const float D = 28.0f;
    vel += ((target - pos) * K - D * vel) * dt;
    pos += vel * dt;
    st->SetFloat(uid, pos);
    st->SetFloat(uid ^ 0xA1A1, vel);

    // 使用当前主题颜色
    ImVec4 themeColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImU32 sliderCol = ImGui::GetColorU32(themeColor);
    ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);
    ImU32 textColorDim = ImGui::GetColorU32(ImGuiCol_TextDisabled);

    // 滑块 - 稍大一点
    float padX = 2.0f;
    float radius = 8.0f;

    float sliderH = H * 1.08f;
    float sliderY = pos - (sliderH - H) * 0.5f;
    ImVec2 sMin(cur.x + padX, cur.y + sliderY);
    ImVec2 sMax(cur.x + W - padX, cur.y + sliderY + sliderH);

    // 滑块阴影（柔和）
    draw->AddRectFilled(
        ImVec2(sMin.x + 2, sMin.y + 2),
        ImVec2(sMax.x + 2, sMax.y + 2),
        IM_COL32(0, 0, 0, 30), radius
    );

    // 滑块主体
    draw->AddRectFilled(sMin, sMax, sliderCol, radius);

    // 极淡描边
    draw->AddRect(sMin, sMax, IM_COL32(255, 255, 255, 15), radius, 0, 0.8f);

    // 每个选项 - 只画文字
    for (int i = 0; i < n; ++i) {
        float y = cur.y + i * (H + gap);
        ImVec2 bMin(cur.x, y);
        ImVec2 bMax(cur.x + W, y + H);

        // 用 InvisibleButton 的 hover 状态，但检查鼠标是否在当前选项区域内
        bool hovered = itemHovered && (mouse.x >= bMin.x && mouse.y >= bMin.y && mouse.x <= bMax.x && mouse.y <= bMax.y);

        // 点击动画
        ImGuiID clickId = uid ^ (0x1000 + i);
        float clickT = st->GetFloat(clickId, 0.0f);
        clickT = ImMax(0.0f, clickT - dt * 3.6f);
        st->SetFloat(clickId, clickT);

        // 悬停效果：极淡背景
        if (hovered && i != selected) {
            draw->AddRectFilled(bMin, bMax, IM_COL32(0, 0, 0, 10), radius);
        }

        // 文字颜色
        ImU32 col = (i == selected) ? IM_COL32(0, 0, 0, 255) : textColorDim;

        // 文字缩放动画
        float scale = 1.0f + 0.04f * clickT;
        ImVec2 ts = ImGui::CalcTextSize(labels[i]);
        ts.x *= scale;
        ts.y *= scale;
        ImVec2 tp(bMin.x + (W - ts.x) * 0.5f, bMin.y + (H - ts.y) * 0.5f);

        draw->AddText(tp, col, labels[i]);

        // 点击检测 - 直接用鼠标位置判断，覆盖整个选项区域
        if (ImGui::IsMouseClicked(0) && hovered) {
            if (selected != i) {
                selected = i;
                st->SetInt(uid ^ 0x1337, selected);
            }
            clicked = true;
            st->SetFloat(clickId, 1.0f);
        }
    }

    ImGui::PopID();
    return clicked;
}

// HSV 转 RGB
ImU32 HSVToRGB(float h, float s = 1.0f, float v = 1.0f) {
    float r, g, b;
    int i = (int)(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch (i % 6) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    default: r = v; g = p; b = q; break;
    }
    return IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 255);
}

// UTF-8 下一个字符指针（简单实现）
static const char* Utf8Next(const char* s) {
    if (!s || *s == '\0') return s;
    unsigned char c = (unsigned char)*s;
    if (c < 0x80) return s + 1;
    if ((c >> 5) == 0x6) return s + 2;   // 110x xxxx
    if ((c >> 4) == 0xE) return s + 3;   // 1110 xxxx
    if ((c >> 3) == 0x1E) return s + 4;  // 1111 0xxx
    return s + 1;
}

// 从左上角开始绘制流光文字（每字符单独绘制，参与 ImGui 布局）
void ImGuiRainbowTextBottomLeft(const char* text, float speed, float marginX, float marginY)
{
    if (!text) return;
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 display = io.DisplaySize;

    // pivot (0,0) 表示位置以左上为锚点
    ImGui::SetNextWindowPos(ImVec2(marginX, marginY), ImGuiCond_Always, ImVec2(0.0f, 0.0f));
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground
        | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings;
    if (ImGui::Begin("##TopLeftRainbow", nullptr, flags))
    {
        const char* p = text;
        bool first = true;
        int idx = 0;
        float baseTime = ImGui::GetTime() * speed;
        while (*p) {
            const char* q = Utf8Next(p);
            float hue = std::fmodf(baseTime + idx * 0.06f, 1.0f);
            ImU32 col = HSVToRGB(hue);
            std::string s(p, q); // 单个 UTF-8 字符子串
            if (!first) ImGui::SameLine(0.0f, 0.0f);
            ImGui::TextColored(ImColor(col), "%s", s.c_str());
            first = false;
            p = q;
            idx++;
        }
    }
    ImGui::End();
}

void ImGuiRainbowText(const char* text, float speed)
{
    if (text == nullptr) return;
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImFont* font = ImGui::GetFont();
    float fontSize = ImGui::GetFontSize();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 cur = pos;

    // 时间驱动色相偏移
    float t = std::fmodf(ImGui::GetTime() * speed, 1.0f);

    const char* p = text;
    int idx = 0;
    while (*p)
    {
        const char* q = Utf8Next(p);
        float hue = std::fmodf(t + idx * 0.06f, 1.0f); // 每字符小偏移
        ImU32 col = HSVToRGB(hue);
        // 使用 AddText 的 (font, size, pos, col, text_begin, text_end) 重载
        draw->AddText(font, fontSize, cur, col, p, q);
        // 计算该子串宽度并推进位置
        ImVec2 sz = ImGui::CalcTextSize(p, q, false, FLT_MAX);
        cur.x += sz.x;
        p = q;
        idx++;
    }

    // 用 Dummy 推进 ImGui 布局（保持换行/布局一致）
    ImGui::Dummy(ImVec2(cur.x - pos.x, fontSize));
}

//颜色调整控件
bool ImGui_ColorEditImU32_Beauty(const char* label, ImU32* color, const ImVec2& preview_size)
{
    if (!color) return false;
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (!window) return false;

    ImGui::PushID(label);

    ImGui::TextUnformatted(label);
    ImGui::SameLine();

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = preview_size;
    ImGui::InvisibleButton("##color_preview_btn", size);
    //ImGui::SameLine();

    ImVec4 col4 = ImGui::ColorConvertU32ToFloat4(*color);
    float rgba[4] = { col4.x, col4.y, col4.z, col4.w };
    bool changed = false;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float rounding = size.y * 0.5f; // 胶囊
    ImU32 fill_col = *color;
    ImU32 border_col = IM_COL32(0, 0, 0, 120);

    // 使用 AddRectFilled / AddRect，并传入 rounding 与 flags
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), fill_col, rounding, ImDrawFlags_RoundCornersAll);
    draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), border_col, rounding, ImDrawFlags_RoundCornersAll, 1.0f);

    if (ImGui::IsItemHovered()) {
        float t = ImSaturate((sinf((float)ImGui::GetTime() * 6.0f) * 0.5f + 0.5f));
        ImU32 glow = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.06f + 0.12f * t));
        draw_list->AddRectFilled(ImVec2(pos.x - 2, pos.y - 2), ImVec2(pos.x + size.x + 2, pos.y + size.y + 2), glow, rounding + 2.0f, ImDrawFlags_RoundCornersAll);
        ImGui::SetTooltip("#%02X%02X%02X%02X", (int)(col4.x * 255), (int)(col4.y * 255), (int)(col4.z * 255), (int)(col4.w * 255));
    }

    if (ImGui::IsItemClicked()) ImGui::OpenPopup("##color_picker_popup");

    if (ImGui::BeginPopup("##color_picker_popup")) {
        if (ImGui::ColorPicker4("##picker", rgba, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview)) {
            ImU32 newu32 = ImGui::ColorConvertFloat4ToU32(ImVec4(rgba[0], rgba[1], rgba[2], rgba[3]));
            if (newu32 != *color) {
                *color = newu32;
                changed = true;
            }
        }
        ImGui::EndPopup();
    }

    ImGui::PopID();
    return changed;
}

// 流光条
void ImGuiColorBar(float speed) {
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float width = ImGui::GetContentRegionAvail().x;
    if (width <= 0.0f) return;

    const float height = 2.0f;                     // 降低高度
    static auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration<float>(now - start).count();
    float hue = std::fmod(elapsed * speed, 1.0f);

    const int segments = 256;                      // 增加分段数，消除马赛克
    for (int i = 0; i < segments; ++i) {
        float t = (float)i / segments;
        float segWidth = width / segments;
        float x0 = pos.x + i * segWidth;
        float x1 = x0 + segWidth;
        ImVec2 p0(x0, pos.y);
        ImVec2 p1(x1, pos.y + height);
        // 色相平滑过渡
        float segHue = hue + t * 0.8f;
        if (segHue > 1.0f) segHue -= 1.0f;
        draw->AddRectFilled(p0, p1, HSVToRGB(segHue));
    }

    // 占位
    ImGui::Dummy(ImVec2(width, height));
}

// 系统键码转显示名称
std::string GetSystemKeyName(UINT vk)
{
    if (vk == 0 || vk == -1 ) return U8("未设置");

    // 鼠标键
    if (vk == VK_LBUTTON)      return U8("鼠标左键");
    if (vk == VK_RBUTTON)      return U8("鼠标右键");
    if (vk == VK_MBUTTON)      return U8("鼠标中键");
    if (vk == VK_XBUTTON1)     return U8("后侧键");
    if (vk == VK_XBUTTON2)     return U8("前侧键");

    // 功能键
    switch (vk)
    {
    case VK_ESCAPE:      return U8("Esc");
    case VK_TAB:         return U8("Tab");
    case VK_MENU:        return U8("Alt");
    case VK_SHIFT:       return U8("Shift");
    case VK_CONTROL:     return U8("Ctrl");
    case VK_BACK:        return U8("Backspace");
    case VK_RETURN:      return U8("Enter");
    case VK_SPACE:       return U8("空格");
    case VK_UP:          return U8("Up");
    case VK_DOWN:        return U8("Down");
    case VK_LEFT:        return U8("Left");
    case VK_RIGHT:       return U8("Right");
    case VK_INSERT:      return U8("Insert");
    case VK_DELETE:      return U8("Delete");
    case VK_HOME:        return U8("Home");
    case VK_END:         return U8("End");
    case VK_PRIOR:       return U8("PageUp");
    case VK_NEXT:        return U8("PageDown");
    case VK_CAPITAL:     return U8("CapsLock");
    case VK_NUMLOCK:     return U8("NumLock");
    case VK_SCROLL:      return U8("ScrollLock");
    case VK_PAUSE:       return U8("Pause");
    case VK_SNAPSHOT:    return U8("PrintScreen");
    case VK_DIVIDE:      return U8("Numpad /");
    case VK_MULTIPLY:    return U8("Numpad *");
    case VK_SUBTRACT:    return U8("Numpad -");
    case VK_ADD:         return U8("Numpad +");
    case VK_DECIMAL:     return U8("Numpad .");
    case VK_CLEAR:       return U8("Clear");
    }

    // F1-F12
    if (vk >= VK_F1 && vk <= VK_F12)
    {
        char buf[16];
        std::snprintf(buf, sizeof(buf), U8("F%d"), vk - VK_F1 + 1);
        return std::string(buf);
    }

    // 字母 A-Z
    if (vk >= 'A' && vk <= 'Z')
    {
        return std::string(1, (char)vk);
    }

    // 数字 0-9
    if (vk >= '0' && vk <= '9')
    {
        return std::string(1, (char)vk);
    }

    // 小键盘数字
    if (vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9)
    {
        char buf[16];
        std::snprintf(buf, sizeof(buf), U8("Num %d"), vk - VK_NUMPAD0);
        return std::string(buf);
    }

    // 符号键
    switch (vk)
    {
    case VK_OEM_1:       return U8(";:");
    case VK_OEM_2:       return U8("/?");
    case VK_OEM_3:       return U8("`~");
    case VK_OEM_4:       return U8("[");
    case VK_OEM_5:       return U8("\\|");
    case VK_OEM_6:       return U8("]");
    case VK_OEM_7:       return U8("'\"");
    case VK_OEM_PLUS:    return U8("=+");
    case VK_OEM_COMMA:   return U8(",<");
    case VK_OEM_MINUS:   return U8("-");
    case VK_OEM_PERIOD:  return U8(".>");
    }

    return U8("未知按键");
}
// 热键输入框
bool ImGuiHotKey(const char* label, int& keyCode) {
    ImGui::PushID(label);
    bool changed = false;

    ImGuiID id = ImGui::GetID(label);
    ImGuiStorage* storage = ImGui::GetStateStorage();
    bool listening = storage->GetBool(id, false);

    // 按钮显示文本
    std::string displayText;
    char buf[64];
    snprintf(buf, sizeof(buf), "VK: %d", keyCode);

    if (listening)
        displayText = U8("按任意键(ESC清空)");
    else {
        displayText = GetSystemKeyName(keyCode);
        if (displayText == U8("未知按键"))
            displayText = buf;
    }

    // 点击按钮：切换监听状态
    if (ImGui::Button(displayText.c_str(), ImVec2(120, 0))) {
        listening = !listening;
    }

    if (listening)
    {
        // ESC 清空热键
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            keyCode = NULL;
            listening = false;
            changed = true;
        }
        // 遍历所有虚拟按键捕获
        else
        {
            for (UINT vk = 1; vk <= 0xFF; ++vk)
            {
                // 跳过无效按键
                if (vk >= 133 && vk <= 134) continue;

                if (GetAsyncKeyState(vk) & 0x8000)
                {
                    keyCode = vk;
                    listening = false;
                    changed = true;
                    break;
                }
            }
        }
    }

    // 保存当前控件状态
    storage->SetBool(id, listening);

    // 右侧标签
    ImGui::SameLine();
    ImGui::TextUnformatted(label);

    ImGui::PopID();
    return changed;
}
//滑块开关
bool ImGuiSwitch(const char* label, bool* v)
{
    ImGui::PushID(label);
    bool clicked = false;

    // 开关样式配置
    const float scale = 0.85f;// 开关整体缩放
    const float baseHeight = ImGui::GetFrameHeight();
    const float height = baseHeight * scale;
    const float width = height * 1.75f;
    const float radius = height * 0.50f;
    const ImVec2& styleSpacing = ImGui::GetStyle().ItemSpacing;

    // 独立动画
    ImGuiID id = ImGui::GetID(label);
    ImGuiStorage* storage = ImGui::GetStateStorage();
    float t = storage->GetFloat(id, 0.0f);

    // 使用 delta time，使动画与帧率无关
    ImGuiIO& io = ImGui::GetIO();
    float dt = ImMax(1.0f / 1000.0f, io.DeltaTime);
    const float animSpeedPerSec = 6.0f; // 每秒逼近速率（原每帧0.1 大致等价 ~6/s）
    t = *v ? ImMin(t + animSpeedPerSec * dt, 1.0f) : ImMax(t - animSpeedPerSec * dt, 0.0f);

    // 1. 计算文本尺寸 + 总交互区域（开关+文本+间距）
    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 totalSize = ImVec2(width + styleSpacing.x + textSize.x, baseHeight);

    // 2. 记录绘制前的光标位置（用于后续恢复布局）
    ImVec2 cursorBefore = ImGui::GetCursorPos();
    ImVec2 curScreenPos = ImGui::GetCursorScreenPos();

    // 3. 透明交互按钮（整行可点，自动推进光标到正确位置）
    ImGui::InvisibleButton(label, totalSize);
    if (ImGui::IsItemClicked()) {
        *v = !*v;
        clicked = true;
    }

    // 颜色混合
    auto BlendColors = [](ImU32 colA, ImU32 colB, float t) -> ImU32 {
        ImColor a(colA), b(colB);
        const float i = 1.0f - t;
        return IM_COL32(
            (int)(a.Value.x * 255 * i + b.Value.x * 255 * t),
            (int)(a.Value.y * 255 * i + b.Value.y * 255 * t),
            (int)(a.Value.z * 255 * i + b.Value.z * 255 * t),
            (int)(a.Value.w * 255 * i + b.Value.w * 255 * t)
        );
        };

    // 绘制开关
    ImDrawList* draw = ImGui::GetWindowDrawList();
    const float switchCenterY = curScreenPos.y + (baseHeight - height) * 0.5f;
    const ImU32 bg = BlendColors(IM_COL32(55, 55, 55, 255), IM_COL32(15, 150, 255, 255), t);
    const ImU32 handle = IM_COL32(255, 255, 255, 255);

    draw->AddRectFilled(ImVec2(curScreenPos.x, switchCenterY),
        ImVec2(curScreenPos.x + width, switchCenterY + height),
        bg, radius);

    const float handleRadius = radius * 0.75f;
    draw->AddCircleFilled(
        ImVec2(curScreenPos.x + radius + (width - height) * t, switchCenterY + radius),
        handleRadius - 1.5f,
        handle
    );

    // 标题
    const float textCenterY = curScreenPos.y + (baseHeight - textSize.y) * 0.5f;
    draw->AddText(ImVec2(curScreenPos.x + width + styleSpacing.x, textCenterY),
        ImGui::GetColorU32(ImGuiCol_Text),
        label);

    // 5. 强制把光标推进到正确位置（关键！解决后续控件布局错乱）
    ImGui::SetCursorPos(cursorBefore); // 先回退到原始位置
    ImGui::Dummy(totalSize);           // 用Dummy占位，让光标自动推进

    // 保存动画状态
    storage->SetFloat(id, t);
    ImGui::PopID();

    return clicked;
}
bool ImGuiModernSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f")
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems || !v)
        return false;

    ImGuiStyle& style = ImGui::GetStyle();
    ImDrawList* draw = ImGui::GetWindowDrawList();
    bool value_changed = false;

    // ===================== 样式配置 =====================
    const float track_height = 4.0f;
    const float track_radius = track_height * 0.5f;
    const float radius_normal = 6.0f;
    const float radius_hover = 9.0f;
    const float anim_speed_per_sec = 10.0f; // 每秒动画速率（用于圆圈放大/缩小）
    const float frame_height = ImGui::GetFrameHeight();
    const float slider_width = ImGui::CalcItemWidth();

    // 时间步
    ImGuiIO& io = ImGui::GetIO();
    float dt = ImMax(1.0f / 1000.0f, io.DeltaTime);

    // 布局与交互区域
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::InvisibleButton(label, ImVec2(slider_width, frame_height));
    const bool is_hovered = ImGui::IsItemHovered();
    const bool is_active = ImGui::IsItemActive();

    // 滑块坐标
    const float center_y = pos.y + frame_height * 0.5f;
    const float range = v_max - v_min;
    const float safe_range = ImMax(range, 1e-6f);

    // 存储与动画
    ImGuiID id = ImGui::GetID(label);
    ImGuiStorage* storage = ImGui::GetStateStorage();

    float currentValue = *v;
    float t = ImClamp((currentValue - v_min) / safe_range, 0.0f, 1.0f);
    float circle_x = pos.x + t * slider_width;

    // 圆圈 hover 动画（帧率无关）
    float anim_t = storage->GetFloat(id, 0.0f);
    ImVec2 mouse_pos = ImGui::GetMousePos();
    const float dx = mouse_pos.x - circle_x, dy = mouse_pos.y - center_y;
    const bool want_animate = (is_hovered && (dx * dx + dy * dy) < (radius_hover * radius_hover)) || is_active;
    anim_t = want_animate ? ImMin(anim_t + anim_speed_per_sec * dt, 1.0f) : ImMax(anim_t - anim_speed_per_sec * dt, 0.0f);
    const float current_radius = radius_normal + (radius_hover - radius_normal) * anim_t;

    // 拖动时加入阻尼（弹簧 + 阻尼）
    float smooth = storage->GetFloat(id ^ 0x100, currentValue);
    float vel = storage->GetFloat(id ^ 0x200, 0.0f);

    if (is_active)
    {
        const float mouse_t = ImClamp((ImGui::GetMousePos().x - pos.x) / slider_width, 0.0f, 1.0f);
        float target = v_min + mouse_t * range;

        // 弹簧阻尼参数（可调）
        const float K = 180.0f; // 刚度
        const float D = 30.0f;  // 阻尼
        vel += ((target - smooth) * K - D * vel) * dt;
        smooth += vel * dt;

        float newVal = ImClamp(smooth, v_min, v_max);
        if (fabs(newVal - *v) > 1e-5f) {
            *v = newVal;
            value_changed = true;
        }
    }
    else
    {
        // 非拖动时与外部值立即同步（避免外部值变化出现滞后）
        smooth = *v;
        vel = 0.0f;
    }

    // 绘制轨道与圆
    const float track_top = center_y - track_height * 0.5f;
    const float track_bot = center_y + track_height * 0.5f;
    const ImU32 col_bg = ImGui::GetColorU32(ImGuiCol_FrameBg);
    const ImU32 col_fill = ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
    const ImU32 col_circle = ImGui::GetColorU32(ImGuiCol_SliderGrab);

    // 背景轨道
    float filled_t = ImClamp((smooth - v_min) / safe_range, 0.0f, 1.0f);
    float filled_x = pos.x + filled_t * slider_width;
    draw->AddRectFilled(ImVec2(pos.x, track_top), ImVec2(pos.x + slider_width, track_bot), col_bg, track_radius);
    draw->AddRectFilled(ImVec2(pos.x, track_top), ImVec2(filled_x, track_bot), col_fill, track_radius);
    draw->AddCircleFilled(ImVec2(filled_x, center_y), current_radius, col_circle);

    // 拖动 / 悬停圆圈 → 显示数值
    if (want_animate)
    {
        ImGui::BeginTooltip();
        ImGui::Text(format, *v);
        ImGui::EndTooltip();
    }

    // 右侧标签
    ImGui::SameLine();
    ImGui::TextUnformatted(label);

    // 保存状态
    storage->SetFloat(id, anim_t);
    storage->SetFloat(id ^ 0x100, smooth);
    storage->SetFloat(id ^ 0x200, vel);

    return value_changed;
}


bool ImGuiModernSliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d")
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems || !v)
        return false;

    ImGuiStyle& style = ImGui::GetStyle();
    ImDrawList* draw = ImGui::GetWindowDrawList();
    bool value_changed = false;

    const float track_height = 4.0f;
    const float track_radius = track_height * 0.5f;
    const float radius_normal = 6.0f;
    const float radius_hover = 9.0f;
    const float anim_speed_per_sec = 10.0f;
    const float frame_height = ImGui::GetFrameHeight();
    const float slider_width = ImGui::CalcItemWidth();

    ImGuiIO& io = ImGui::GetIO();
    float dt = ImMax(1.0f / 1000.0f, io.DeltaTime);

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::InvisibleButton(label, ImVec2(slider_width, frame_height));
    const bool is_hovered = ImGui::IsItemHovered();
    const bool is_active = ImGui::IsItemActive();

    const float range = (float)(v_max - v_min);
    float currentValue = (float)(*v);
    const float t = ImClamp((currentValue - v_min) / ImMax(range, 1.0f), 0.0f, 1.0f);
    const float cx = pos.x + t * slider_width;
    const float cy = pos.y + frame_height * 0.5f;

    ImGuiID id = ImGui::GetID(label);
    ImGuiStorage* storage = ImGui::GetStateStorage();

    float anim_t = storage->GetFloat(id, 0.0f);
    ImVec2 mouse_pos = ImGui::GetMousePos();
    const float dx = mouse_pos.x - cx, dy = mouse_pos.y - cy;
    const bool want_animate = (is_hovered && (dx * dx + dy * dy) < (radius_hover * radius_hover)) || is_active;
    anim_t = want_animate ? ImMin(anim_t + anim_speed_per_sec * dt, 1.0f) : ImMax(anim_t - anim_speed_per_sec * dt, 0.0f);
    const float current_radius = radius_normal + (radius_hover - radius_normal) * anim_t;

    // 阻尼存储
    float smooth = storage->GetFloat(id ^ 0x100, currentValue);
    float vel = storage->GetFloat(id ^ 0x200, 0.0f);

    if (is_active)
    {
        const float mouse_t = ImClamp((ImGui::GetMousePos().x - pos.x) / slider_width, 0.0f, 1.0f);
        float target = v_min + mouse_t * range;

        const float K = 180.0f;
        const float D = 30.0f;
        vel += ((target - smooth) * K - D * vel) * dt;
        smooth += vel * dt;

        float newVal = ImClamp(smooth, (float)v_min, (float)v_max);
        int newInt = (int)std::round(newVal);
        if (newInt != *v) {
            *v = newInt;
            value_changed = true;
        }
    }
    else
    {
        smooth = (float)*v;
        vel = 0.0f;
    }

    const float track_top = cy - track_height * 0.5f;
    const float track_bot = cy + track_height * 0.5f;
    const ImU32 col_bg = ImGui::GetColorU32(ImGuiCol_FrameBg);
    const ImU32 col_fill = ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
    const ImU32 col_circle = ImGui::GetColorU32(ImGuiCol_SliderGrab);

    float filled_t = ImClamp((smooth - v_min) / ImMax(range, 1.0f), 0.0f, 1.0f);
    float filled_x = pos.x + filled_t * slider_width;
    draw->AddRectFilled(ImVec2(pos.x, track_top), ImVec2(pos.x + slider_width, track_bot), col_bg, track_radius);
    draw->AddRectFilled(ImVec2(pos.x, track_top), ImVec2(filled_x, track_bot), col_fill, track_radius);
    draw->AddCircleFilled(ImVec2(filled_x, cy), current_radius, col_circle);

    if (want_animate)
    {
        ImGui::BeginTooltip();
        ImGui::Text(format, *v);
        ImGui::EndTooltip();
    }

    ImGui::SameLine();
    ImGui::TextUnformatted(label);

    storage->SetFloat(id, anim_t);
    storage->SetFloat(id ^ 0x100, smooth);
    storage->SetFloat(id ^ 0x200, vel);

    return value_changed;
}





void Draw_Menu() {
    static int tab = 0;

    ImGui::SetNextWindowBgAlpha(0.8f); // 设置窗口背景透明度
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x/2-(650/2), ImGui::GetIO().DisplaySize.y/2-(450/2)), ImGuiCond_Once);  // 设置窗口初始位置（居中）
    ImGui::SetNextWindowSize(ImVec2(650, 450), ImGuiCond_Once); // 设置窗口初始大小

	ImGui::Begin(U8("TL - C++ [Home-显示/隐藏][Alt+End - 退出辅助]"), NULL, 2);//0默认，1隐藏标题，2禁止调整大小
    /*
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x + (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - ImGui::CalcTextSize(U8("TL - C++ [Home-显示/隐藏][Alt+End - 退出辅助]")).x) * 0.5f);// 标题居中
    ImGui::Text(U8("TL - C++ [Home-显示/隐藏][Alt+End - 退出辅助]"));
    ImGui::Separator(); //分割线
    */

    // ===== 顶部 =====
    ImGuiColorBar(0.3f);
    ImGui::Text(U8("他们都不认可你，偏偏你却最争气！"));
    ImGui::SameLine();
    if (本人数据::外绘)ImGui::Text(U8(" FPS: %.2f"), ImGui::GetIO().Framerate);
    else ImGui::Text(U8(" FPS: %.2f(跟随游戏)"), ImGui::GetIO().Framerate);
    ImGuiColorBar(0.3f);
    //ImGui::Separator(); //分割线

    // ===== 左侧菜单 =====
    ImGui::BeginChild(U8("menu"), ImVec2(130, 0), true);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 12));
    /*
    if (ImGui::Button(U8("显示设置"), ImVec2(-FLT_MIN, 45)))tab = 0;
    if (ImGui::Button(U8("物资设置"), ImVec2(-FLT_MIN, 45)))tab = 1;
    if (ImGui::Button(U8("自瞄设置"), ImVec2(-FLT_MIN, 45)))tab = 2;
    if (ImGui::Button(U8("内存功能"), ImVec2(-FLT_MIN, 45)))tab = 3;
    if (ImGui::Button(U8("功能设置"), ImVec2(-FLT_MIN, 45)))tab = 4;
    if (ImGui::Button(U8("用户信息"), ImVec2(-FLT_MIN, 45)))tab = 5;
    */
    std::vector<const char*> tabs = { U8("显示设置"), U8("物资设置"), U8("自瞄设置"), U8("内存功能"), U8("功能设置"), U8("用户信息") };
    static int selIdx = 0;
    const int mapToTab[] = { 0, 1, 2, 3, 4, 5 };
    for (int i = 0; i < 6; ++i) if (tab == mapToTab[i]) { selIdx = i; break; }
    if (ImGuiSlidingCapsuleSelector("left_tabs", tabs, selIdx, ImVec2(-FLT_MIN, 45.0f)))
        tab = mapToTab[selIdx];


    ImGui::PopStyleVar(2);
    ImGuiColorBar(0.3f);
    if (ImGui::Button(U8("保存配置"), ImVec2(-FLT_MIN, 30))) { PlaySoundA("C:\\Windows\\Media\\Windows Background.wav", NULL, SND_FILENAME | SND_ASYNC); 保存配置(); }
    if (ImGui::Button(U8("安全退出"), ImVec2(-FLT_MIN, 30)) || (GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_END) & 0x8000)) ImGui::OpenPopup(U8("关闭窗口"));


    if (ImGui::BeginPopupModal(U8("关闭窗口"),nullptr,2)) {
        static bool 弹出 = false;
        if (!弹出) {
            PlaySoundA("C:\\Windows\\Media\\Windows Background.wav", NULL, SND_FILENAME | SND_ASYNC);
			弹出 = true;
        }
        ImGui::Text(U8("你确定要退出辅助？"));
        float avail_width = ImGui::GetContentRegionAvail().x;
        float button_width = (avail_width - 4) * 0.5f;
        if (ImGui::Button(U8("确定"), ImVec2(button_width, 30)))本人数据::安全退出 = true;
        ImGui::SameLine();
        if (ImGui::Button(U8("取消"), ImVec2(button_width, 30))) { ImGui::CloseCurrentPopup(); 弹出 = false;}

        ImGui::EndPopup();
    }



    ImGui::EndChild();
    ImGui::SameLine();

    // ===== 右侧功能区 =====
    ImGui::BeginChild(U8("panel"), ImVec2(0, 0), true);

    //显示
    if (tab == 0){
        ImGui::Text(U8("显示设置"));
        ImGuiColorBar(0.3f);
        //ImGui::Separator();
		ImGui::Spacing();

        ImGui::Checkbox(U8("绘制方框"), &显示::绘制方框);ImGui::SameLine();
        ImGui::Checkbox(U8("绘制信息"), &显示::绘制信息);ImGui::SameLine();
        ImGui::Checkbox(U8("傻逼队友"), &显示::绘制队友);ImGui::SameLine();
        ImGui::Checkbox(U8("显示人数"), &显示::显示人数);ImGui::SameLine();
        ImGui::Checkbox(U8("绘制射线"), &显示::绘制射线);

        ImGui::Checkbox(U8("显示头部"), &显示::显示头部);ImGui::SameLine();
        ImGui::Checkbox(U8("显示骨骼"), &显示::显示骨骼);ImGui::SameLine();
        ImGui::Checkbox(U8("显示人机"), &显示::显示人机);ImGui::SameLine();
        ImGui::Checkbox(U8("显示载具"), &显示::显示载具);ImGui::SameLine();
        ImGui::Checkbox(U8("被瞄提醒"), &显示::被瞄提醒);

        ImGui::Checkbox(U8("开启提示"), &显示::开启提示); ImGui::SameLine();
        ImGui::Checkbox(U8("绘制准星"), &显示::绘制准星); 
        if (本人数据::外绘) { ImGui::SameLine(); ImGuiSwitch(U8("主播模式"), &显示::主播模式); }

        ImGui::Text(U8("绘制信息：蓝色名字的为手机端玩家，红色名字的为悬赏目标，绿色名字的\n为掉线玩家"));
        ImGui::Text(U8("开启提示：开启提示里面的快捷键只有在打开开启提示时才生效"));
        if (本人数据::外绘) ImGui::Text(U8("主播模式：开启后可防止截图和录屏软件录制到辅助界面"));

    }
    if (tab == 1) {
        ImGui::Text(U8("物资设置(可能影响性能)"));
        ImGuiColorBar(0.3f);
        //ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox(U8("显示物资"), &物资::显示物资);ImGui::SameLine();
        ImGui::Checkbox(U8("物资堆叠"), &物资::物资堆叠); ImGui::SameLine();
        ImGuiSwitch(U8("皮肤美化"), &美化::开启); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("前往游戏根目录的 TL-C++_MH.ini 文件修改配置文件\n修改后重启生效！"));
        ImGuiColorBar(0.3f);
        //1步枪 2冲锋枪 3射手步枪 4狙击枪 5机枪 6霰弹枪 7手枪 8特殊武器 9暗器 10常用装备 11常用身份 12常用药品 13常用倍镜 14常用配件 15常用芯片 16常用投掷 17常用近战 18常用子弹 19信号枪
        ImGui::Checkbox(U8("步枪枪械"), &物资::步枪);ImGui::SameLine();
        ImGui::Checkbox(U8("冲锋枪械"), &物资::冲锋枪);ImGui::SameLine();
        ImGui::Checkbox(U8("射手步枪"), &物资::射手步枪);ImGui::SameLine();
        ImGui::Checkbox(U8("狙击枪械"), &物资::狙击枪);ImGui::SameLine();
        ImGui::Checkbox(U8("机枪枪械"), &物资::机枪);
        ImGui::Checkbox(U8("霰弹枪械"), &物资::霰弹枪);ImGui::SameLine();
        ImGui::Checkbox(U8("手枪枪械"), &物资::手枪);ImGui::SameLine();
        ImGui::Checkbox(U8("特殊武器"), &物资::特殊武器);ImGui::SameLine();
        ImGui::Checkbox(U8("暗器枪械"), &物资::暗器);ImGui::SameLine();
        ImGui::Checkbox(U8("常用装备"), &物资::常用装备);
        ImGui::Checkbox(U8("常用身份"), &物资::常用身份);ImGui::SameLine();
        ImGui::Checkbox(U8("常用药品"), &物资::常用药品);ImGui::SameLine();
        ImGui::Checkbox(U8("常用倍镜"), &物资::常用倍镜);ImGui::SameLine();
        ImGui::Checkbox(U8("常用配件"), &物资::常用配件);ImGui::SameLine();
        ImGui::Checkbox(U8("常用芯片"), &物资::常用芯片);
        ImGui::Checkbox(U8("常用投掷"), &物资::常用投掷);ImGui::SameLine();
        ImGui::Checkbox(U8("常用近战"), &物资::常用近战);ImGui::SameLine();
        ImGui::Checkbox(U8("常用子弹"), &物资::常用子弹);

        ImGui::Text(U8("Tips：信号枪默认显示，土炮炮为常用投掷，紫金红葫芦为常用药品"));
    }
    //自瞄
    if (tab == 2) {
        ImGui::Text(U8("自瞄设置[对人机无效]"));
        ImGuiColorBar(0.3f);
        //ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox(U8("开启自瞄"), &自瞄::开启自瞄); ImGui::SameLine();
        ImGui::Checkbox(U8("显示范围"), &自瞄::显示范围); ImGui::SameLine();
        ImGui::Checkbox(U8("空手不瞄"), &自瞄::空手不瞄); ImGui::SameLine();
        ImGui::Checkbox(U8("瞄准位置"), &自瞄::瞄准位置); ImGui::SameLine();
        ImGui::Checkbox(U8("子弹追踪"), &自瞄::子弹追踪); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("目标必须在自瞄范围内！！！"));

        ImGui::Text(U8("自瞄算法"));
        ImGui::RadioButton(U8("鼠标自瞄"), &自瞄::自瞄算法, 0); ImGui::SameLine();
        ImGui::RadioButton(U8("内存自瞄"), &自瞄::自瞄算法, 1);
        ImGui::Text(U8("自瞄位置"));
        if (自瞄::自瞄算法 == 0) {
        ImGui::RadioButton(U8("头顶"), &自瞄::鼠标自瞄::自瞄位置, 0); ImGui::SameLine();
        ImGui::RadioButton(U8("头部"), &自瞄::鼠标自瞄::自瞄位置, 1); ImGui::SameLine();
        ImGui::RadioButton(U8("脖子"), &自瞄::鼠标自瞄::自瞄位置, 2); ImGui::SameLine();
        ImGui::RadioButton(U8("身子"), &自瞄::鼠标自瞄::自瞄位置, 3);
        }
        if (自瞄::自瞄算法 == 1) {
            ImGui::RadioButton(U8("头骨"), &自瞄::内存自瞄::自瞄位置, 0); ImGui::SameLine();
            ImGui::RadioButton(U8("头部"), &自瞄::内存自瞄::自瞄位置, 1); ImGui::SameLine();
            ImGui::RadioButton(U8("脖子"), &自瞄::内存自瞄::自瞄位置, 2); ImGui::SameLine();
            ImGui::RadioButton(U8("身子"), &自瞄::内存自瞄::自瞄位置, 3);
            if (自瞄::内存自瞄::自瞄位置 == 0) { 
                ImGui::SameLine();
                ImGuiSwitch(U8("开启预判"), &自瞄::开启预判); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("自行判断预判精准度再确定是否开启！"));
            }
        }


        ImGui::Text(U8("热键"));
		ImGuiHotKey(U8(""), 自瞄::自瞄热键);
        ImGui::Text(U8("快捷热键(防止自定义热键无效)"));
        ImGui::RadioButton(U8("左键##1"), &自瞄::自瞄热键, VK_LBUTTON);ImGui::SameLine();
        ImGui::RadioButton(U8("右键##1"), &自瞄::自瞄热键, VK_RBUTTON);ImGui::SameLine();
        ImGui::RadioButton(U8("Shift键##1"), &自瞄::自瞄热键, VK_SHIFT);ImGui::SameLine();
        ImGui::RadioButton(U8("Ctrl键##1"), &自瞄::自瞄热键, VK_CONTROL);ImGui::SameLine();
        ImGui::RadioButton(U8("前侧键##1"), &自瞄::自瞄热键, VK_XBUTTON2);ImGui::SameLine();
        ImGui::RadioButton(U8("后侧键##1"), &自瞄::自瞄热键, VK_XBUTTON1);


        if (自瞄::自瞄算法 == 0) ImGui::SliderFloat(U8("自瞄速度##1"), &自瞄::鼠标自瞄::自瞄速度, 0, 15,U8("%.1f"));
        if (自瞄::自瞄算法 == 1) ImGui::SliderFloat(U8("自瞄速度"), &自瞄::内存自瞄::自瞄速度, 0, 20, U8("%.1f"));

        ImGui::SliderInt(U8("自瞄范围"), &自瞄::自瞄范围, 0, 2000);

    }
    //内存
    if (tab == 3){  
        ImGui::Text(U8("内存功能(有几率封号！)"));
        ImGuiColorBar(0.3f);
        //ImGui::Separator();
        ImGui::Spacing();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), U8("稳定功能"));
        ImGui::Checkbox(U8("无限子弹"), &内存::无限子弹);ImGui::SameLine();
        ImGui::Checkbox(U8("子弹爆射"), &内存::子弹爆射);ImGui::SameLine();if (ImGui::IsItemHovered()) ImGui::SetTooltip(U8("有bug"));
        ImGui::Checkbox(U8("全枪自动"), &内存::全枪自动);ImGui::SameLine();
        ImGui::Checkbox(U8("全枪聚点"), &内存::全枪聚点);

        ImGui::Checkbox(U8("加特不热"), &内存::加特不热);ImGui::SameLine();
        ImGui::Checkbox(U8("人物高跳"), &内存::人物高跳);ImGui::SameLine();
        ImGui::Checkbox(U8("广角视野"), &内存::广角视野);ImGui::SameLine();
        ImGui::Checkbox(U8("主播无后"), &内存::主播无后);

        ImGui::Checkbox(U8("超级无后"), &内存::超级无后);ImGui::SameLine();
        ImGui::Checkbox(U8("无视缺氧"), &内存::无视缺氧);ImGui::SameLine();
        ImGui::Checkbox(U8("载具锁油"), &内存::载具锁油);ImGui::SameLine();
		ImGui::Checkbox(U8("人机变大"), &内存::人机变大);if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("记得调整大小"));

        ImGui::Checkbox(U8("无视雪球"), &内存::无视雪球);ImGui::SameLine();
        ImGui::Checkbox(U8("无视火焰"), &内存::无视火焰);ImGui::SameLine();
        ImGui::Checkbox(U8("人物旋转"), &内存::人物旋转);ImGui::SameLine();
        ImGui::Checkbox(U8("子弹瞬击"), &内存::子弹瞬击);if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("记得调整大小"));

        ImGui::Checkbox(U8("落地无僵"), &内存::落地无僵);  if (!内存::吸取人机)ImGui::SameLine();
        ImGui::Checkbox(U8("吸取人机"), &内存::吸取人机);
        if (内存::吸取人机) { 
            ImGui::SameLine(); 
            ImGuiSwitch(U8("热键吸取"), &内存::热键吸取); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("长按热键吸取人机"));
            if (内存::热键吸取) {
                ImGui::SameLine();
                ImGuiHotKey(U8("吸取人机热键"), 内存::吸取人机热键);
            }
        }
		if (!内存::意念上车 && !内存::热键吸取)ImGui::SameLine();//防止界面被挤乱
        ImGui::Checkbox(U8("意念上车"), &内存::意念上车); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("请不要在车上使用！！\n如无效请查看载具位置是否可用"));
        if (内存::意念上车) {
            ImGui::SameLine();
            ImGuiHotKey(U8("意念上车热键"), 内存::意念上车热键);
            ImGuiModernSliderInt(U8("意念上车位置"), &内存::意念上车位置, 0, 5); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("0为驾驶位，其次往后，如果载具没有这个位置或者被占用则无法上车"));
        }
        

        ImGui::SliderInt(U8("人机大小"), &内存::人机大小, 1, 100);
        ImGui::SliderFloat(U8("广角大小"), &内存::广角大小, 0, 15, U8("%.1f"));
        ImGui::SliderFloat(U8("高跳大小"), &内存::高跳大小, 0, 100, U8("%.0f"));
        ImGui::SliderFloat(U8("瞬击值"), &内存::瞬击值, 0, 15, U8("%.0f"));

        ImGuiColorBar(0.3f);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), U8("奔放(人品)功能 基本上秒封(看人品)！"));
        ImGui::Checkbox(U8("改碰撞体"), &内存::改碰撞体);ImGui::SameLine();
        ImGui::Checkbox(U8("超级加速"), &内存::超级加速);ImGui::SameLine();
        ImGui::Checkbox(U8("魔法子弹"), &内存::魔法子弹);if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("前提打开子弹追踪,能量武器无法使用！\n用法如同追踪，见人就秒，代价是拉闸指数爆表\n如果你使用的是内存自瞄，必须打开自瞄且包括子弹追踪才有效果")); ImGui::SameLine();
        ImGui::Checkbox(U8("自由飞天"), &内存::自由飞天); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("WASD上下左右，空格上升，C下降"));
        ImGui::Checkbox(U8("意念拳人"), &内存::意念拳人); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("以极快的速度拳击距离自身5m内的敌人")); ImGui::SameLine();
        ImGui::Checkbox(U8("射击间隔"), &内存::射击间隔); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("记得调整大小"));

        ImGui::SliderFloat(U8("飞天速度"), &内存::飞天速度, 0, 200, U8("%.1fM"));
        ImGui::SliderFloat(U8("超级加速值"), &内存::超级加速值, 0, 100, U8("%.1f"));
        ImGui::SliderFloat(U8("射击间隔##1"), &内存::射击间隔值, 0.01, 0.5, U8("%.2f秒/发")); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("值越小，射击间隔越短，射速越快(正常射速0.1s/发)\n涉及到稳定性，低于0.05就容易封号，所以值不调太离谱应该不会封\n调整完必须重新打开该功能！"));

        ImGui::Checkbox(U8("标点传送"), &内存::标点传送);if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("按住热键传送到标点位置"));ImGui::SameLine();
        ImGuiHotKey(U8("标点传送热键"), 内存::标点传送热键); ImGui::SameLine();
        ImGuiSwitch(U8("渐进模式"), &内存::渐进模式); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("逐渐传送到标点位置"));
        if (内存::标点传送 && 内存::渐进模式) {
            ImGui::SliderFloat(U8("传送频率"), &内存::传送频率, 0, 10, U8("%.1fms"));
            ImGui::SliderFloat(U8("单次距离"), &内存::单次距离, 0, 30, U8("%.1fM"));
        }

        ImGui::Checkbox(U8("传送敌人"), &内存::传送敌人);if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("按住热键传送到自瞄范围内离准星最近的敌人背后"));ImGui::SameLine();
        ImGuiHotKey(U8("传送敌人热键"), 内存::传送敌人热键);ImGui::SameLine();
        ImGuiSwitch(U8("回弹模式"), &内存::回弹模式);if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("按下热键传送到敌人背后，松手传送回来"));
        ImGui::Checkbox(U8("意念炸车"), &内存::意念炸车); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("车辆名称变红后按下热键即可炸车 少量的炸一下还是可以的 不要炸太多 服务器会给你飞踢了"));
        ImGui::SameLine();
        ImGuiHotKey(U8("意念炸车热键"), 内存::意念炸车热键);


    }
    if (tab == 4) {;
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8("功能设置"));
        ImGui::Spacing();
        ImGui::Text(U8("信息类型"));
        ImGui::RadioButton(U8("美观信息##1"), &显示::信息类型, 0);ImGui::SameLine();
        ImGui::RadioButton(U8("详细信息##1"), &显示::信息类型, 1);
        ImGui::Text(U8("方框类型"));
        ImGui::RadioButton(U8("圆角方框"), &显示::方框类型, 0);ImGui::SameLine();
        ImGui::RadioButton(U8("立体方框"), &显示::方框类型, 1);
        ImGui::Text(U8("射线位置"));
        ImGui::RadioButton(U8("顶部"), &显示::射线位置, 0);ImGui::SameLine();
        ImGui::RadioButton(U8("中部"), &显示::射线位置, 1);ImGui::SameLine();
        ImGui::RadioButton(U8("底部"), &显示::射线位置, 2);
        ImGui::Text(U8("载具类型"));
        ImGui::RadioButton(U8("详细信息"), &显示::载具类型, 0);ImGui::SameLine();
        ImGui::RadioButton(U8("简洁信息"), &显示::载具类型, 1);ImGui::SameLine();
        ImGui::RadioButton(U8("美观信息"), &显示::载具类型, 2);
        if (ImGui::Button(U8("颜色配置菜单"))) tab = 6;
        ImGui::SliderFloat(U8("开启提示菜单X"), &本人数据::开启提示位置x, 0, 3840, U8("%.0f"));
        ImGui::SliderFloat(U8("开启提示菜单Y"), &本人数据::开启提示位置y, 0, 2160, U8("%.0f"));
        ImGui::SliderFloat(U8("开启提示菜单透明度"), &本人数据::开启提示透明度, 0, 1, U8("%.2f"));
        ImGuiSwitch(U8("FPS面板"), &显示::FPS面板);ImGui::SameLine();
        ImGuiSwitch(U8("限制FPS"), &本人数据::限制FPS); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("限制帧率"));
        if (!本人数据::外绘) ImGuiModernSliderFloat(U8("限制FPS"), &本人数据::FPS值, 1, 360, U8("FPS:%.0f"));
        ImGuiSwitch(U8("落雪特效"), &本人数据::落雪特效); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("渣机勿开！！"));
    }
    if (tab == 5) {
        time_t now = time(nullptr);
        struct tm local;
        localtime_s(&local, &now);
        ImGui::Text(U8("用户信息"));
        ImGuiColorBar(0.3f);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), U8("当前时间：%d年%d月%d日%d时%d分%d秒"), local.tm_year + 1900, local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), U8("在线时长：%s"), 取程序运行时间_文本().c_str());
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), U8("到期时间：%s"), 时间_计算相差时间(本人数据::到期时间).c_str());
        ImGuiColorBar(0.3f);
        //ImGuiSwitch(U8("开发者选项"), &本人数据::Debug); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("不知道的东西不要瞎jb点！"));
        if (ImGui::Button(U8("显示控制台"))) ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL); ImGui::SameLine();
        if (ImGui::Button(U8("隐藏控制台"))) ShowWindow(GetConsoleWindow(), SW_HIDE);
        if (ImGui::Button(U8("用户文档")))system("start https://share.note.youdao.com/s/Z61WCBeC"); ImGui::SameLine();
        if (ImGui::Button(U8("加入QQ群")))system("start https://qm.qq.com/q/8hELhrFDuo"); ImGui::SameLine();
        if (ImGui::Button(U8("前往购卡")))system("start https://buy.jry0.com/shop/TLnb666");

    }
    if (tab == 6) {
        if (ImGui::Button(U8(" < 返回 "))) tab = 4;
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x + (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - ImGui::CalcTextSize(U8("颜色配置面板")).x) * 0.5f);
        ImGui::Text(U8("颜色配置面板"));
        ImGuiColorBar(0.3f);
        ImGui_ColorEditImU32_Beauty(U8("方框颜色"), &颜色::方框颜色, ImVec2(40, 16));
        ImGui_ColorEditImU32_Beauty(U8("射线颜色"), &颜色::射线颜色, ImVec2(40, 16));
        ImGui_ColorEditImU32_Beauty(U8("骨骼颜色"), &颜色::骨骼颜色, ImVec2(40, 16));
        ImGui_ColorEditImU32_Beauty(U8("自瞄范围"), &颜色::自瞄范围, ImVec2(40, 16));
        ImGui_ColorEditImU32_Beauty(U8("人机颜色"), &颜色::人机颜色, ImVec2(40, 16));
        ImGui_ColorEditImU32_Beauty(U8("载具背景"), &颜色::载具背景, ImVec2(40, 16)); if (ImGui::IsItemHovered())ImGui::SetTooltip(U8("载具的美观信息的背景颜色"));
        if (ImGui::Button(U8("恢复默认配色"), ImVec2(-FLT_MIN, 45))) {
            颜色::方框颜色 = IM_COL32(0, 255, 255, 255);
            颜色::射线颜色 = IM_COL32(255, 255, 255, 255);
            颜色::自瞄范围 = IM_COL32(255, 255, 255, 255);
            颜色::骨骼颜色 = IM_COL32(0, 255, 255, 255);
            颜色::人机颜色 = IM_COL32(255, 0, 255, 255);
            颜色::载具背景 = IM_COL32(255, 0, 255, 255);
        }
    }
    ImGui::EndChild();
    ImGui::End();
    if (本人数据::Debug)Debug_Menu();

}


void 功能 (const char* 功能名称, bool 功能状态) {
    if (本人数据::开启提示透明度 == 0) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", 功能名称);
    else ImGui::Text("%s", 功能名称);
    ImGui::SameLine();
    if (功能状态 == true) ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), U8("[开启]"));
    else ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), U8("[关闭]"));
}

//开启提示窗口
void MiniMenu(bool* p_open) {
    // 窗口基础属性
    ImGui::SetNextWindowBgAlpha(本人数据::开启提示透明度); // 设置窗口背景透明度

    const ImVec2 windowSize(150.0f, 345.0f);      // 窗口尺寸，每加一个键加25
    const float edgePadding = 12.0f;              // 与屏幕边缘的间距（可调）
    const float animSpeed = 0.12f;                // 动画速度（0..1），越大越快

    // 屏幕尺寸
    ImGuiIO& io = ImGui::GetIO();
    const float screenW = io.DisplaySize.x;
    const float screenH = io.DisplaySize.y;

    // 时间步
    float dt = ImMax(1.0f / 1000.0f, io.DeltaTime);

    // 静态动画状态，跨帧保存
    static float animX = 本人数据::开启提示位置x;
    static float animY = 本人数据::开启提示位置y;
    static bool firstInit = true;

    // 计算目标位置（由滑条决定），并限制到屏幕内保留边距
    float desiredX = 本人数据::开启提示位置x;
    float desiredY = 本人数据::开启提示位置y;
    float clampedX = ImClamp(desiredX, edgePadding, screenW - windowSize.x - edgePadding);
    float clampedY = ImClamp(desiredY, edgePadding, screenH - windowSize.y - edgePadding);

    // 依据 p_open 控制可见性：true 则滑入，false 则退回屏幕左侧
    bool visible = (p_open == nullptr) ? true : (*p_open != false);
    float hiddenX = -windowSize.x - edgePadding;
    float targetX = visible ? clampedX : hiddenX;
    float targetY = clampedY;

    // 首次初始化时让窗口从隐藏位置滑入
    if (firstInit) {
        animX = hiddenX;
        animY = targetY;
        firstInit = false;
    }

    // 帧率无关的平滑插值（阻尼）
    const float animSpeedPerSec = 8.0f; // 每秒逼近速率（可调）
    animX += (targetX - animX) * animSpeedPerSec * dt;
    animY += (targetY - animY) * animSpeedPerSec * dt;

    // 将动画计算的位置应用到窗口
    ImGui::SetNextWindowPos(ImVec2(animX, animY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);


    // 将 p_open 传入 ImGui::Begin，使其成为控制窗口显示/关闭的标志
    ImGui::Begin(U8("开启提示"), p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    ImGuiWindowFlags lockFlag =
        ImGuiWindowFlags_NoMove |   // 不能鼠标拖动
        ImGuiWindowFlags_NoResize | // 禁止缩放
        ImGuiWindowFlags_NoCollapse;// 禁止折叠

    float textWidth = ImGui::CalcTextSize(U8("开启提示")).x;
    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x
        + (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - textWidth) * 0.5f);

    if (本人数据::开启提示透明度 == 0) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), U8("开启提示"));
    else ImGui::Text(U8("开启提示"));

    ImGuiColorBar(0.3f);
    功能(U8("   ~  显示物资"), 物资::显示物资);
    ImGui::Separator();
    功能(U8("  F1 绘制信息"), 显示::绘制信息);
    ImGui::Separator();
    功能(U8("  F2 傻逼队友"), 显示::绘制队友);
    ImGui::Separator();
    功能(U8("  F3 开启自瞄"), 自瞄::开启自瞄);
    ImGui::Separator();
    功能(U8("  F4 显示载具"), 显示::显示载具);
    ImGui::Separator();
    功能(U8("  F5 显示人机"), 显示::显示人机);
    if (本人数据::外绘) {
        ImGui::Separator();
        功能(U8("  F6 主播模式"), 显示::主播模式);
    }
    ImGui::Separator();
    功能(U8("  F7 子弹追踪"), 自瞄::子弹追踪);
    ImGui::Separator();
    功能(U8("  F8 魔法子弹"), 内存::魔法子弹);
    ImGui::Separator();
    功能(U8("  F9 人物高跳"), 内存::人物高跳);
    ImGui::Separator();
    功能(U8(" F10 自由飞天"), 内存::自由飞天);

    ImGuiColorBar(0.3f);

    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x
        + (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - ImGui::CalcTextSize(U8("Home 显/隐")).x) * 0.5f);
    if (本人数据::开启提示透明度 == 0) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), U8("Home 显/隐"));
    else ImGui::Text(U8("Home 显/隐"));

    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x
        + (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - ImGui::CalcTextSize(U8("Alt+End 安全退出")).x) * 0.5f);
    if (本人数据::开启提示透明度 == 0) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), U8("Alt+End 安全退出"));
    else ImGui::Text(U8("Alt+End 安全退出"));

    ImGui::End();

    if (*p_open) {
        功能开关(物资::显示物资, VK_OEM_3);
        功能开关(显示::绘制信息, VK_F1);
        功能开关(显示::绘制队友, VK_F2);
        功能开关(自瞄::开启自瞄, VK_F3);
        功能开关(显示::显示载具, VK_F4);
        功能开关(显示::显示人机, VK_F5);
        功能开关(显示::主播模式, VK_F6);
        功能开关(自瞄::子弹追踪, VK_F7);
        功能开关(内存::魔法子弹, VK_F8);
        功能开关(内存::人物高跳, VK_F9);
        功能开关(内存::自由飞天, VK_F10);
    }
}

void Debug_Menu() {
    ImGui::SetNextWindowPos(ImVec2(200, 200), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(700, 560), ImGuiCond_Once);
    ImGui::Begin(U8("Debug"), NULL, 0);

    // 密码输入与验证
    static char passwordInput[128] = "";
    static bool authenticated = false;
    const char* correctPassword = "password";

    ImGui::Text(U8("调试面板"));
    ImGui::Separator();
    ImGui::InputText(U8("密码##debug_pwd"), passwordInput, sizeof(passwordInput), ImGuiInputTextFlags_Password);
    ImGui::SameLine();
    if (ImGui::Button(U8("验证密码"))) {
        authenticated = (std::strcmp(passwordInput, correctPassword) == 0);
    }
    ImGui::SameLine();
    if (ImGui::Button(U8("登出"))) {
        authenticated = false;
        passwordInput[0] = '\0';
    }

    ImGui::Spacing();

    if (!authenticated) {
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), U8("未验证或密码错误"));
        ImGui::End();
        return;
    }

    // 帮助函数：渲染一行可右键复制的文本（格式化单个值）
    auto CopyableLineFmt = [](const char* fmt, unsigned long long val) {
        char buf[256];
        std::snprintf(buf, sizeof(buf), fmt, val);
        ImGui::Selectable(buf, false, 0);
        if (ImGui::BeginPopupContextItem(nullptr)) { // 使用 item id 作为 popup id
            if (ImGui::MenuItem(U8("复制"))) {
                ImGui::SetClipboardText(buf);
            }
            ImGui::EndPopup();
        }
        };
    // 帮助函数：渲染一行可右键复制的纯文本
    auto CopyableLineStr = [](const char* text) {
        ImGui::Selectable(text, false, 0);
        if (ImGui::BeginPopupContextItem(nullptr)) {
            if (ImGui::MenuItem(U8("复制"))) {
                ImGui::SetClipboardText(text);
            }
            ImGui::EndPopup();
        }
        };
    ImGuiSwitch(U8("显示自己"), &显示::显示自己);
    // 先显示 特征码取地址 输出（放在偏移上面）
    ImGui::BeginChild(U8("feature_block"), ImVec2(0, 200), true, 0);
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), U8(" 特征码地址输出"));
    ImGui::Spacing();
    // 使用 helper 渲染可复制行
    CopyableLineFmt(U8("世界地址: GameAssembly.dll + 0x%llX"), (unsigned long long)数据::世界地址);
    CopyableLineFmt(U8("主播无后: GameAssembly.dll + 0x%llX"), (unsigned long long)数据::主播无后);
    CopyableLineFmt(U8("超级无后: GameAssembly.dll + 0x%llX"), (unsigned long long)数据::超级无后);
    CopyableLineFmt(U8("无视缺氧: GameAssembly.dll + 0x%llX"), (unsigned long long)数据::无视缺氧);
    CopyableLineFmt(U8("载具锁油: GameAssembly.dll + 0x%llX"), (unsigned long long)数据::载具锁油);
    CopyableLineFmt(U8("子弹瞬击: GameAssembly.dll + 0x%llX"), (unsigned long long)数据::子弹加速);
    CopyableLineFmt(U8("无视火焰: GameAssembly.dll + 0x%llX"), (unsigned long long)数据::无视火焰);
    CopyableLineFmt(U8("无视雪球: GameAssembly.dll + 0x%llX"), (unsigned long long)数据::无视雪球);
    CopyableLineFmt(U8("落地无僵: GameAssembly.dll + 0x%llX"), (unsigned long long)数据::落地无僵);
    CopyableLineFmt(U8("枪械间隔: GameAssembly.dll + 0x%llX"), (unsigned long long)数据::枪械间隔);
    ImGui::Separator();
    CopyableLineFmt(U8("本人地址: 0x%llX"), (unsigned long long)本人数据::本人地址);
    ImGui::EndChild();

    ImGui::Separator();

    // 再显示 PrintAllOffsets 的内容（偏移列表），每行可右键复制
    ImGui::BeginChild(U8("offsets_block"), ImVec2(0, 0), true, 0);
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), U8("           地址输出"));
    ImGui::Text(" ");

    CopyableLineStr(U8("[开局模块]"));
    CopyableLineFmt(U8("[开局模块] 世界地址：0x%llX"), (unsigned long long)Start_Game::Uworld);
    CopyableLineFmt(U8("[开局模块] 世界偏移：0x%llX"), (unsigned long long)Start_Game::GameData);
    CopyableLineFmt(U8("[开局模块] 人物偏移：0x%llX"), (unsigned long long)Start_Game::StartGame);
    CopyableLineFmt(U8("[战斗世界] 物品偏移：0x%llX"), (unsigned long long)Start_Game::BattleWorld);
    CopyableLineFmt(U8("[开局模块] 角色AI管理器：0x%llX"), (unsigned long long)Start_Game::RoleAIManager);
    CopyableLineFmt(U8("[开局模块] 战斗角色逻辑：0x%llX"), (unsigned long long)Start_Game::BattleRoleLogic);
    CopyableLineFmt(U8("[开局模块] 全部载具：0x%llX"), (unsigned long long)Start_Game::AllCar);
    CopyableLineFmt(U8("[战斗世界] 物品管理器：0x%llX"), (unsigned long long)Start_Game::ItemManager);
    CopyableLineFmt(U8("[战斗世界] 物品偏移：0x%llX"), (unsigned long long)Start_Game::ItemManager1);
    ImGui::Text(" ");

    CopyableLineStr(U8("[角色]"));
    CopyableLineFmt(U8("[角色] 昵称：0x%llX"), (unsigned long long)BattleRoleLogic::NickName);
    CopyableLineFmt(U8("[角色] 血量：0x%llX"), (unsigned long long)BattleRoleLogic::HP);
    CopyableLineFmt(U8("[角色] 倒地血量：0x%llX"), (unsigned long long)BattleRoleLogic::WeakValue);
    CopyableLineFmt(U8("[角色] 编号：0x%llX"), (unsigned long long)BattleRoleLogic::TeamNum);
    CopyableLineFmt(U8("[角色] 坐标：0x%llX"), (unsigned long long)BattleRoleLogic::Pos);
    CopyableLineFmt(U8("[角色] 标点坐标：0x%llX"), (unsigned long long)BattleRoleLogic::SpotPoint);
    CopyableLineFmt(U8("[角色] 手持：0x%llX"), (unsigned long long)BattleRoleLogic::Weapon);
    CopyableLineFmt(U8("[角色] 杀敌：0x%llX"), (unsigned long long)BattleRoleLogic::KillRoleNum);
    CopyableLineFmt(U8("[角色] ID：0x%llX"), (unsigned long long)BattleRoleLogic::RoleID);
    CopyableLineFmt(U8("[角色] 公共偏移：0x%llX"), (unsigned long long)BattleRoleLogic::roleLogicClient);
    CopyableLineFmt(U8("[角色] Buff控制：0x%llX"), (unsigned long long)BattleRoleLogic::RoleBuffControl);
    CopyableLineFmt(U8("[角色] 角色大小：0x%llX"), (unsigned long long)BattleRoleLogic::RoleSize);
    CopyableLineFmt(U8("[角色] 是否踩球：0x%llX"), (unsigned long long)BattleRoleLogic::UserCircusBallNet);
    CopyableLineFmt(U8("[角色] 玩家平台：0x%llX"), (unsigned long long)BattleRoleLogic::PlayerPlatform);
    CopyableLineFmt(U8("[角色] 头部装备：0x%llX"), (unsigned long long)BattleRoleLogic::HeadEquipPart);
    CopyableLineFmt(U8("[角色] 身体装备：0x%llX"), (unsigned long long)BattleRoleLogic::BodyEquipPart);
    CopyableLineFmt(U8("[角色] 背包装备：0x%llX"), (unsigned long long)BattleRoleLogic::PackEquip);
    CopyableLineFmt(U8("[角色] 功能服装备：0x%llX"), (unsigned long long)BattleRoleLogic::FunctionalGarmentEquipPart);
    CopyableLineFmt(U8("[角色] 是否在线：0x%llX"), (unsigned long long)BattleRoleLogic::isOnline);
    ImGui::Text(" ");
    CopyableLineFmt(U8("[角色] roleNetClient：0x%llX"), (unsigned long long)BattleRoleLogic::roleNetClient);
    CopyableLineFmt(U8("[角色] RoleClient：0x%llX"), (unsigned long long)BattleRole::RoleClient);
    CopyableLineFmt(U8("[角色] MyRoleControl：0x%llX"), (unsigned long long)BattleRole::MyRoleControl);
    ImGui::Text(" ");
    CopyableLineFmt(U8("[角色] 手持武器：0x%llX"), (unsigned long long)BattleRole::UserWeapon);
    CopyableLineFmt(U8("[角色] 玩家朝向：0x%llX"), (unsigned long long)BattleRole::lastRotaY);
    CopyableLineFmt(U8("[角色] 是否显示：0x%llX"), (unsigned long long)BattleRole::IsEventRoleShow);
    CopyableLineFmt(U8("[角色] 是否倒地：0x%llX"), (unsigned long long)BattleRole::tempIsWeak);
    CopyableLineFmt(U8("[角色] 是否死亡：0x%llX"), (unsigned long long)BattleRole::hideNoNetRole);
    CopyableLineFmt(U8("[角色] 玩家标识：0x%llX"), (unsigned long long)BattleRole::playerId);
    ImGui::Text(" ");

    CopyableLineFmt(U8("[角色Buff] 跳跃高度：0x%llX"), (unsigned long long)RoleBuffControl::JumpNum);
    CopyableLineFmt(U8("[角色Buff] 行走速度：0x%llX"), (unsigned long long)RoleBuffControl::WalkNum);
    CopyableLineFmt(U8("[角色Buff] 镜头距离：0x%llX"), (unsigned long long)RoleBuffControl::CameraRatio);
    ImGui::Text(" ");

    CopyableLineFmt(U8("[载具] 血量：0x%llX"), (unsigned long long)AllCar::HP);
    CopyableLineFmt(U8("[载具] 名称：0x%llX"), (unsigned long long)AllCar::Name);
    CopyableLineFmt(U8("[载具] 镜像：0x%llX"), (unsigned long long)AllCar::Mirror);
    CopyableLineFmt(U8("[载具] 坐标：0x%llX"), (unsigned long long)AllCar::Position);
    CopyableLineFmt(U8("[载具] 油量：0x%llX"), (unsigned long long)AllCar::Consumption);
    CopyableLineFmt(U8("[载具] 载具网络客户端：0x%llX"), (unsigned long long)AllCar::carNetClient);
    CopyableLineFmt(U8("[载具] ID：0x%llX"), (unsigned long long)AllCar::_carId);
    ImGui::Text(" ");

    CopyableLineFmt(U8("[人机] 血量：0x%llX"), (unsigned long long)RoleAIManager::HP);
    CopyableLineFmt(U8("[人机] 名称：0x%llX"), (unsigned long long)RoleAIManager::Name);
    CopyableLineFmt(U8("[人机] 坐标：0x%llX"), (unsigned long long)RoleAIManager::Position);
    CopyableLineFmt(U8("[人机] 大小：0x%llX"), (unsigned long long)RoleAIManager::RoleSize);
    ImGui::Text(" ");

    CopyableLineFmt(U8("[物品] 编号：0x%llX"), (unsigned long long)AbsPickItemNet::AutoId);
    CopyableLineFmt(U8("[物品] 英文名：0x%llX"), (unsigned long long)AbsPickItemNet::ItemSign);
    CopyableLineFmt(U8("[物品] 物品ID：0x%llX"), (unsigned long long)AbsPickItemNet::ItemId);
    CopyableLineFmt(U8("[物品] 物品类型：0x%llX"), (unsigned long long)AbsPickItemNet::ItemType);
    CopyableLineFmt(U8("[物品] 物品名称：0x%llX"), (unsigned long long)AbsPickItemNet::ItemName);
    CopyableLineFmt(U8("[物品] 子弹数量：0x%llX"), (unsigned long long)AbsPickItemNet::BulletNum);
    CopyableLineFmt(U8("[物品] 物品等级：0x%llX"), (unsigned long long)AbsPickItemNet::ItemLevel);
    CopyableLineFmt(U8("[物品] 坐标：0x%llX"), (unsigned long long)AbsPickItemNet::SyncPoint);
    CopyableLineFmt(U8("[物品] 拾取角色ID：0x%llX"), (unsigned long long)AbsPickItemNet::PickRoleId);
    CopyableLineFmt(U8("[物品] 当前数值：0x%llX"), (unsigned long long)AbsPickItemNet::NowValue);
    CopyableLineFmt(U8("[物品] 皮肤标识：0x%llX"), (unsigned long long)AbsPickItemNet::skinSign);
    CopyableLineFmt(U8("[物品] 射击标识：0x%llX"), (unsigned long long)AbsPickItemNet::ShootSign);
    ImGui::Text(" ");

    CopyableLineFmt(U8("[相机] 旋转X：0x%llX"), (unsigned long long)Camera_Controller::MyCameraRotationX);
    CopyableLineFmt(U8("[相机] 旋转Y：0x%llX"), (unsigned long long)Camera_Controller::MyCameraRotationY);
    CopyableLineFmt(U8("[相机] 相机坐标：0x%llX"), (unsigned long long)Camera_Controller::MyCameraTran);
    CopyableLineFmt(U8("[相机] 地址：0x%llX"), (unsigned long long)Camera_Controller::g_pCamObj);

    ImGui::EndChild();
    ImGui::End();
}


