#include "ui/UIStyle.h"

#undef new
#include <imgui/imgui.h>

uint32 ui::GetColor(EColor colorID, f32 alpha)
{
    return ImGui::GetColorU32(colorID, alpha);
}

f32 ui::GetFontSize(void)
{
    return ImGui::GetFontSize();
}

math::Vector2f ui::GetTextSize(const char* text)
{
    return ImGui::CalcTextSize(text);
}

void ui::SetWindowFontScale(f32 scale)
{
    ImGui::SetWindowFontScale(scale);
}