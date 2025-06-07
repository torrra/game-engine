#include "ui/elements/Label.h"

#include "resource/ResourceManager.h"
#include "resource/font/Font.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

#undef new
#include <imgui/imgui.h>

engine::Label::Label(const char* text, math::Vector2f const& position)
    : m_text(text), m_fontScale(1.0f), m_font(nullptr), m_textBoxWidth(FLT_MAX)
{
    SetPosition(position);
    SetTextColor(WHITE, 1.0f);
}

void engine::Label::Render(void)
{
    std::lock_guard lock(m_mutex);
    ImGui::SetCursorPos(m_transform.m_position);

    if (m_font)
    {
        ImGui::PushFont(m_font);
        m_font->Scale = m_fontScale;
    }
    else
    {
        ImGui::PushFont(ImGui::GetFont());

    }

    ImGui::PushStyleColor(ImGuiCol_Text, m_textColor);
    ImGui::PushTextWrapPos(m_transform.m_position.GetX() + m_textBoxWidth);
    ImGui::Text(m_text.c_str());
    ImGui::PopTextWrapPos();
    ImGui::PopStyleColor();

    if (m_font || !m_font)
    {
        ImGui::PopFont();

        if (m_font)
            m_font->Scale = 1.0f;
    }
}

void engine::Label::SetTextColor(f32 red, f32 green, f32 blue, f32 alpha)
{
    std::lock_guard lock(m_mutex);
    m_textColor = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

void engine::Label::SetFont(const char* fontName, f32 fontSize)
{
    std::lock_guard lock(m_mutex);
    m_font = ResourceManager::GetResource<Font>(fontName)->GetFont();

    if (m_font &&
        m_font->FontSize >= 1.0f)
        m_fontScale = fontSize / m_font->FontSize;
    else
        // TODO: potentially add logging
        std::printf("'SetFont()' failed, font '%s' could not be found or invalid font size. Make sure the font file is loaded first.\n", fontName);
}

void engine::Label::WrapText(f32 maxWidthPx)
{
    std::lock_guard lock(m_mutex);
    m_textBoxWidth = maxWidthPx;
}
