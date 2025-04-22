#include "ui/UIDraw.h"

#undef new
#include <imgui/imgui.h>

ui::DrawList::DrawList(ImDrawList* drawList)
{
    m_drawList = drawList;
}

void ui::DrawList::AddRectFilled(math::Vector2f const& min, math::Vector2f const& max, int32 color, f32 rounding)
{
    m_drawList->AddRectFilled(min, max, color, rounding);
}

void ui::DrawList::AddRectFilled(math::Vector2f const& min, math::Vector2f const& max, math::Vector4f const& color, f32 rounding)
{
    int32 colorI32 = ImGui::ColorConvertFloat4ToU32(color);
    m_drawList->AddRectFilled(min, max, colorI32, rounding);
}

math::Vector2f ui::DrawList::GetMin(void) const noexcept
{
    return m_drawList->GetClipRectMin();
}

math::Vector2f ui::DrawList::GetMax(void) const noexcept
{
    return m_drawList->GetClipRectMax();
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
