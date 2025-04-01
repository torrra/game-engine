#include "ui/elements/ProgressBar.h"

#include <math/Arithmetic.hpp>

#undef new
#include <imgui/imgui.h>

#define NO_TEXT "\0"

engine::ProgressBar::ProgressBar(math::Vector2f const& position, math::Vector2f const& size, math::Vector2f const& range)
    : m_range(range)
{
    SetPosition(position);
    SetScale(size);

    // Default value
    m_value = range[1];

    // Default colors
    SetBgColor(DARK_GRAY, 1.0f);
    SetFillColor(LIGHT_GRAY, 1.0f);
}

void engine::ProgressBar::Render(void)
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, m_bgColor);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, m_fillColor);
    
    ImGui::SetCursorPos(m_transform.m_position);
    ImGui::ProgressBar(m_value, m_range, NO_TEXT);

    ImGui::PopStyleColor(2);
}

void engine::ProgressBar::SetValue(f32 value)
{
    // Limit value to progress bar range
    m_value = math::Clamp(value, m_range[0], m_range[1]);
}

void engine::ProgressBar::SetBgColor(f32 red, f32 green, f32 blue, f32 alpha)
{
    m_bgColor = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

void engine::ProgressBar::SetFillColor(f32 red, f32 green, f32 blue, f32 alpha)
{
    m_fillColor = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}
