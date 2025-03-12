#include "ui/elements/Button.h"
#include "utility/Colors.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

engine::Button::Button(const char* text, std::function<void(void)> function)
	: m_text(text), m_regularColor(DARK_GRAY, 1.0f), m_hoveredColor(GRAY, 1.0f),
	  m_pressedColor(WHITE, 1.0f), m_edgeRounding(0.0f), m_action(function)
{
}

void engine::Button::Render(void)
{
	ImGui::PushStyleColor(ImGuiCol_Button, m_regularColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_hoveredColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_pressedColor);

	if (ImGui::Button(m_text.c_str()))
		m_action();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void engine::Button::SetText(const char* text)
{
	m_text = text;
}

void engine::Button::SetColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_regularColor = {red, green, blue, alpha};
}

void engine::Button::SetHoveredColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_hoveredColor = {red, green, blue, alpha};
}

void engine::Button::SetPressedColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_pressedColor = {red, green, blue, alpha};
}

void engine::Button::SetRounding(f32 rounding)
{
	m_edgeRounding = rounding;
}
