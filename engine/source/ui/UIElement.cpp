#include "ui/UIElement.h"

#undef new
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#define INVALID_UNIQUE_ID -1

engine::UIElement::UIElement(void)
    : m_transform(), m_uid(INVALID_UNIQUE_ID), m_autoScale(true)
{
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

void engine::UIElement::AutoScale(f32 regionRatio)
{
    // Resize via change in screen width
    m_transform.m_position *= regionRatio;
    m_transform.m_sizePx *= regionRatio;
}

std::mutex& engine::UIElement::GetMutex(void)
{
    return m_mutex;
}

