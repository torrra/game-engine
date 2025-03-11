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
	: m_fileName(fileName), m_keepAspectRatio(false)
{
	m_imageData = ResourceManager::GetResource<Texture>(m_fileName.c_str());
}

engine::Image::~Image(void)
{
	m_fileName.clear();
}

ENGINE_API void engine::Image::Render(void)
{
	if (!m_imageData)
		return;

	math::Vector2f size(m_transform.m_scale);
	
	if (m_keepAspectRatio)
	{
		size = math::Vector2f(
			m_transform.m_scale.GetX(),
			m_transform.m_scale.GetX() * m_imageData->GetAspectRatio()
		);
	}

	ImGui::SetCursorPos(m_transform.m_position);
	ImGui::Image(m_imageData->GetTexture(), size, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
}

ENGINE_API void engine::Image::KeepAspectRatio(bool value)
{
	m_keepAspectRatio = value;
}
