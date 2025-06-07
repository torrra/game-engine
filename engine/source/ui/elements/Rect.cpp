#include "ui/elements/Rect.h"
#include "utility/Colors.h"

#undef new
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

engine::Rectangle::Rectangle(math::Vector2f const& position, math::Vector2f const& size)
    : m_rounding(0.0f)
{
    // Set transform
    SetPosition(position);
    SetScale(size);
    
    SetColor(WHITE, 1.0f);
}

void engine::Rectangle::Render(void)
{
    std::lock_guard lock(m_mutex);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    const ImGuiID& id = window->GetID(m_uid); // TODO: check, this may cause later issues

    ImRect box(
        window->Pos + m_transform.m_position,
        window->Pos + m_transform.m_position + m_transform.m_sizePx
    );

    if (window->SkipItems || !ImGui::ItemAdd(box, id))
        return;

    window->DrawList->AddRectFilled(box.Min, box.Max, m_color, m_rounding);
}

void engine::Rectangle::SetColor(f32 red, f32 green, f32 blue, f32 alpha)
{
    std::lock_guard lock(m_mutex);
    m_color = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

void engine::Rectangle::SetRounding(f32 rounding)
{
    std::lock_guard lock(m_mutex);
    m_rounding = rounding;
}
