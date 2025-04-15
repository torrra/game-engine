#include "ui/InternalUIWindow.h"

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

engine::ui::UIWindowRect::UIWindowRect(math::Vector2f min, math::Vector2f max)
    : m_min(min), m_max(max)
{}

engine::ui::UIWindowRect::operator ImRect (void)
{
    return ImRect(m_min, m_max);
}

engine::ui::UIWindowRect::operator ImRect (void) const
{
    return ImRect(m_min, m_max);
}


void engine::ui::StartWindowRenderer(const char* title, int32 flags)
{
    ImGui::Begin(title, nullptr, flags);
}

void engine::ui::EndWindowRenderer(void)
{
    ImGui::End();
}

void engine::ui::SetWindowPos(math::Vector2f position, int32 flag)
{
    ImGui::SetNextWindowPos(position, flag);
}

void engine::ui::SetWindowSize(math::Vector2f size, int32 flag)
{
    ImGui::SetNextWindowSize(size, flag);
}

const engine::ui::UIWindowRect engine::ui::GetCurrentWindowRect(void)
{
    ImGuiWindow* wnd = ImGui::GetCurrentWindow();

    if (wnd)
    {
        const ImRect& rect = wnd->Rect();
        return UIWindowRect(rect.Min, rect.Max);
    }

    return UIWindowRect();
}

