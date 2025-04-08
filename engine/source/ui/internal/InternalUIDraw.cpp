#include "ui/UIDraw.h"

#undef new
#include <imgui/imgui.h>

ui::DrawList::DrawList(ImDrawList* drawList)
{
    m_drawList = drawList;
}

void ui::DrawList::AddRectFilled(math::Vector2f min, math::Vector2f max, int32 color, f32 rounding)
{
    m_drawList->AddRectFilled(min, max, color, rounding);
}

void ui::DrawList::AddRectFilled(math::Vector2f min, math::Vector2f max, math::Vector4f color, f32 rounding)
{
    int32 colorI32 = ImGui::ColorConvertFloat4ToU32(color);
    m_drawList->AddRectFilled(min, max, colorI32, rounding);
}

ui::DrawList::operator ImDrawList*(void)
{
    return m_drawList;
}

ui::DrawList::operator ImDrawList* (void) const
{
    return m_drawList;
}

ui::DrawList ui::DrawList::operator=(ImDrawList* drawList)
{
    m_drawList = drawList;

    return *this;
}

ui::DrawList ui::GetForegroundDrawList(void)
{
    return ImGui::GetForegroundDrawList();
}

ui::DrawList ui::GetBackgroundDrawList(void)
{
    return ImGui::GetBackgroundDrawList();
}

ui::DrawList ui::GetWindowDrawList(void)
{
    return ImGui::GetWindowDrawList();
}
