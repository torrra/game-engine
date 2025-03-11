#include "ui/elements/Image.h"
#include "resource/ResourceManager.h"
#include "resource/texture/Texture.h"

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


engine::Image::Image(const char* fileName)
	: m_fileName(fileName), m_keepAspectRatio(false), m_tint(1.0f), m_borderColor(0.0f)
{
	m_data = ResourceManager::GetResource<Texture>(m_fileName.c_str());
}

engine::Image::~Image(void)
{
	m_fileName.clear();
}

void engine::Image::Render(void)
{
	if (!m_data)
		return;

	math::Vector2f size(m_transform.m_scale);
	
	if (m_keepAspectRatio)
	{
		size = math::Vector2f(
			m_transform.m_scale.GetX(),
			m_transform.m_scale.GetX() * m_data->GetAspectRatio()
		);
	}

	ImGui::SetCursorPos(m_transform.m_position);
	ImGui::Image(m_data->GetTexture(), size, ImVec2(0, 0), ImVec2(1, 1), m_tint, m_borderColor);
}

void engine::Image::ConstantAspectRatio(bool value)
{
	m_keepAspectRatio = value;
}

void engine::Image::SetBorderColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_borderColor = {red, green, blue, alpha};
}

void engine::Image::SetImageTint(f32 red, f32 green, f32 blue, f32 alpha)
{
	m_tint = {red, green, blue, alpha};
}
