#include "ui/InternalUIDraw.h"


#undef new
#include <imgui/imgui.h>

engine::ui::DrawList::DrawList(ImDrawList* drawList)
{
    m_drawList = drawList;
}

void engine::ui::DrawList::AddRectFilled(math::Vector2f min, math::Vector2f max, int32 color, f32 rounding)
{
    m_drawList->AddRectFilled(min, max, color, rounding);
}

void engine::ui::DrawList::AddRectFilled(math::Vector2f min, math::Vector2f max, math::Vector4f color, f32 rounding)
{
    int32 colorI32 = ImGui::ColorConvertFloat4ToU32(color);
    m_drawList->AddRectFilled(min, max, colorI32, rounding);
}

engine::ui::DrawList::operator ImDrawList*(void)
{
    return m_drawList;
}

engine::ui::DrawList::operator ImDrawList* (void) const
{
    return m_drawList;
}

engine::ui::DrawList engine::ui::DrawList::operator=(ImDrawList* drawList)
{
    m_drawList = drawList;

    return *this;
}

engine::ui::DrawList engine::ui::GetForegroundDrawList(void)
{
    return ImGui::GetForegroundDrawList();
}

engine::ui::DrawList engine::ui::GetBackgroundDrawList(void)
{
    return ImGui::GetBackgroundDrawList();
}

engine::ui::DrawList engine::ui::GetWindowDrawList(void)
{
    return ImGui::GetWindowDrawList();
}
