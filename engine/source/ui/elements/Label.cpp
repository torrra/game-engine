#include "ui/elements/Label.h"
#include "resource/ResourceManager.h"
#include "resource/font/Font.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <cstdarg>

engine::Label::Label(const char* text)
	: m_text(text), m_fontScale(1.0f), m_font(nullptr), m_textBoxWidth(FLT_MAX)
{
	m_textColor = {1.0f, 1.0f, 1.0f, 1.0f};
}

void engine::Label::Render(void)
{
	ImGui::SetCursorPos(m_transform.m_position);

	if (m_font)
	{
		ImGui::PushFont(m_font);
		m_font->Scale = m_fontScale;
	}

	ImGui::PushTextWrapPos(m_transform.m_position.GetX() + m_textBoxWidth);
	ImGui::TextColored(m_textColor, m_text.c_str());
	ImGui::PopTextWrapPos();

	if (m_font)
	{
		ImGui::PopFont();
		m_font->Scale = 1.0f;
	}

}

void engine::Label::SetTextColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_textColor = {red, green, blue, alpha};
}

void engine::Label::SetFont(const char* fontName, f32 fontSize)
{
	m_font = ResourceManager::GetResource<Font>(fontName)->GetFont();

	if (m_font)
		m_fontScale = fontSize / m_font->FontSize;
	else
		printf("'SetFont()' failed, font '%s' could not be found. Make sure the font file is loaded first.\n", fontName);
}

void engine::Label::WrapText(f32 maxWidthPx)
{
	m_textBoxWidth = maxWidthPx;
}
