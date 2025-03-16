#include "ui/UIElement.h"

#undef new
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

engine::UIElement::UIElement(void)
	: m_transform(), m_uid(-1), m_autoScale(true)
{
	printf("UIElement constructor\n");
}

math::Vector2f engine::UIElement::GetPosition(void) const noexcept
{
	return m_transform.m_position;
}

math::Vector2f engine::UIElement::GetScale(void) const noexcept
{
	return m_transform.m_sizePx;
}

void engine::UIElement::SetPosition(math::Vector2f const& position)
{
	m_transform.m_position = position;
}

void engine::UIElement::SetScale(math::Vector2f const& scale)
{
	m_transform.m_sizePx = scale;
}

void engine::UIElement::SetAutoScale(bool autoResize)
{
	m_autoScale = autoResize;
}

void engine::UIElement::SetUID(int32 const& uid)
{
	m_uid = uid;
}

void engine::UIElement::AutoScale(void)
{
	// TODO: use callback to prevent useless function calls
	static math::Vector2f prevContentRegion = ImGui::GetContentRegionAvail();
	math::Vector2f contentRegion = ImGui::GetContentRegionAvail();

	if (prevContentRegion != contentRegion)
	{
		math::Vector2f regionRatio = contentRegion / prevContentRegion;
		f32 aspectRatio = m_transform.m_sizePx.GetX() / m_transform.m_sizePx.GetY();

		// Scale
		m_transform.m_sizePx = 
		{
			m_transform.m_sizePx.GetY() * regionRatio.GetY(),
			(m_transform.m_sizePx.GetY() * regionRatio.GetY()) * aspectRatio
		};

		// Position
		m_transform.m_position *= regionRatio;

		prevContentRegion = contentRegion;
	}
}
