#include "ui/elements/Rect.h"
#include "utility/Colors.h"

#undef new
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

engine::Rectangle::Rectangle(math::Vector2f const& pos, math::Vector2f const& sizePx)
{
	m_transform.m_position = pos;
	m_transform.m_sizePx = sizePx;
	m_color = ImGui::ColorConvertFloat4ToU32({GREEN, 1.0f});
	m_rounding = 0.0f;
}

void engine::Rectangle::Render(void)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	const ImGuiID& id = window->GetID((int32) m_uid); // TODO: check, this may cause later issues

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
	m_color = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

void engine::Rectangle::SetRounding(f32 rounding)
{
	m_rounding = rounding;
}
