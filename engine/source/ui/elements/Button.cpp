#include "ui/elements/Button.h"
#include "utility/Colors.h"
#include "resource/ResourceManager.h"
#include "resource/font/Font.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

#undef new
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

engine::Button::Button(const char* text, std::function<void(void)> function)
	: m_text(text), m_edgeRounding(0.0f), m_padding(0), m_action(function)
{
	m_regularColor = ImGui::GetColorU32({RED, 1.0f});
	m_hoveredColor = ImGui::GetColorU32({GREEN, 1.0f});
	m_pressedColor = ImGui::GetColorU32({BLUE, 1.0f});


}

void engine::Button::Render(void)
{
	// Condition to avoid negative or null button size
	if (m_transform.m_sizePx.GetX() + m_transform.m_sizePx.GetY() <= 0.0f)
		CalcSize();
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	const ImGuiID& id = window->GetID(m_text.c_str());
	
	ImRect box(
		window->Pos + m_transform.m_position,
		window->Pos + m_transform.m_position + m_transform.m_sizePx
	);
	
	if (window->SkipItems || !ImGui::ItemAdd(box, id))
		return;
	
	// Button state
	bool hovered, held = false;
	if (ImGui::ButtonBehavior(box, id, &hovered, &held))
		m_action();
	
	// Color
	uint32 color = m_regularColor;
	if (held && hovered)
		color = m_pressedColor;
	else if (hovered)
		color = m_hoveredColor;
	
	window->DrawList->AddRectFilled(box.Min, box.Max, color, m_edgeRounding);
	
	ImGui::RenderNavCursor(box, id);
	ImGui::RenderFrame(box.Min, box.Max, color, true, m_edgeRounding);
	ImGui::RenderTextClipped(
		box.Min + math::Vector2f((float) m_padding.GetX(), (float) m_padding.GetY()),
		box.Max - math::Vector2f((float) m_padding.GetX(), (float) m_padding.GetY()),
		m_text.c_str(), 0, nullptr, ImGui::GetCurrentContext()->Style.ButtonTextAlign,
		&box
	);
}

void engine::Button::SetText(const char* text)
{
	m_text = text;

	CalcSize();
}

void engine::Button::SetColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_regularColor = ImGui::GetColorU32({red, green, blue, alpha});
}

void engine::Button::SetHoveredColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_hoveredColor = ImGui::GetColorU32({red, green, blue, alpha});
}

void engine::Button::SetPressedColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_pressedColor = ImGui::GetColorU32({red, green, blue, alpha});
}

void engine::Button::SetRounding(f32 rounding)
{
	m_edgeRounding = rounding;
}

void engine::Button::SetPadding(math::Vector2<uint16> paddingPx)
{
	m_padding = paddingPx;
}

void engine::Button::CalcSize(void)
{
	ImGui::PushFont(ImGui::GetFont());
	math::Vector2f textSize = ImGui::CalcTextSize(m_text.c_str());
	ImGui::PopFont();
	
	m_transform.m_sizePx = 
	{
		textSize.GetX() + (float) (2 * m_padding.GetX()),
		textSize.GetY() + (float) (2 * m_padding.GetY())
	};
}

//math::Vector2f engine::Button::CalcTextSize(uint16 fontSize)
//{
//	ImFont* font = nullptr;
//	ImGui::GetDefaultFont()->
//	if (fontName[0] != '\0')
//		font = engine::ResourceManager::GetResource<Font>(fontName)->GetFont();
//	else
//		font = ImGui::GetIO().Fonts[0].Fonts[0];
//
//	return font->CalcTextSizeA(fontSize, FLT_MAX, -1.0f, text);
//}