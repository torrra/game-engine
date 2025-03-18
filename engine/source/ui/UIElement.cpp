#include "ui/UIElement.h"

engine::UIElement::UIElement(void)
	: m_transform()
{
}

math::Vector2f engine::UIElement::GetPosition(void) const noexcept
{
	return m_transform.m_position;
}

math::Vector2f engine::UIElement::GetScale(void) const noexcept
{
	return m_transform.m_scale;
}

void engine::UIElement::SetPosition(math::Vector2f const& position)
{
	m_transform.m_position = position;
}

void engine::UIElement::SetScale(math::Vector2f const& scale)
{
	m_transform.m_scale = scale;
}
