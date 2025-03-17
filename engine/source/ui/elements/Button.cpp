#include "ui/elements/Button.h"
#include "utility/Colors.h"
#include "resource/ResourceManager.h"
#include "resource/font/Font.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

#undef new
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

engine::Button::Button(const char* text, math::Vector2f const& position, math::Vector2f const& size)
	: m_text(text), m_rounding(0.0f), m_padding(0)
{
	// Transform
	SetPosition(position);
	SetScale(size);
	
	// Default colors
	SetColor(DARK_GRAY, 1.0f );
	SetHoveredColor(GRAY, 1.0f);
	SetPressedColor(LIGHT_GRAY, 1.0f);
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
	bool pressed, hovered, held = false;
	
	pressed = ImGui::ButtonBehavior(box, id, &hovered, &held);

	uint32 color = m_regularColor;
	if (pressed)
	{
		m_state = EButtonState::BUTTON_PRESSED;
		color = m_pressedColor;
	}
	else if (hovered)
	{
		m_state = EButtonState::BUTTON_HOVERED;
		color = m_hoveredColor;
	}
	else
	{
		m_state = EButtonState::BUTTON_UP;
		color = m_regularColor;
	}
	
	window->DrawList->AddRectFilled(box.Min, box.Max, color, m_rounding);
	
	ImGui::RenderNavCursor(box, id);
	ImGui::RenderFrame(box.Min, box.Max, color, true, m_rounding);
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
	m_regularColor = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

void engine::Button::SetHoveredColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_hoveredColor = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

void engine::Button::SetPressedColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_pressedColor = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

void engine::Button::SetRounding(f32 rounding)
{
	m_rounding = rounding;
}

void engine::Button::SetPadding(math::Vector2<uint16> paddingPx)
{
	m_padding = paddingPx;
}

engine::EButtonState engine::Button::GetState(void) const noexcept
{
	return m_state;
}

bool engine::Button::IsPressed(void) const noexcept
{
	return m_state == EButtonState::BUTTON_PRESSED;
}

void engine::Button::CalcSize(void)
{
	ImGui::PushFont(ImGui::GetFont());
	math::Vector2f textSize = ImGui::CalcTextSize(m_text.c_str());
	ImGui::PopFont();
	
	m_transform.m_sizePx = 
	{
		textSize.GetX() + static_cast<f32>(2 * m_padding.GetX()),
		textSize.GetY() + static_cast<f32>(2 * m_padding.GetY())
	};
}
