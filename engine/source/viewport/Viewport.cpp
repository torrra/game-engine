#include "viewport/Viewport.h"
#include "thread/ThreadManager.h"
#include "core/SceneGraph.h"
#include "window/WindowLib.h"

#include <math/Vector2.hpp>

#include <glad/glad.h>

#undef new
#include <imgui/imgui.h>

engine::Viewport::Viewport(const char* title)
	: m_title(title), m_bgColor(0.1f, 0.1f, 0.1f, 0.1f)
{}

engine::Viewport::Viewport(const char* title, math::Vector4f bgColor)
	: m_title(title), m_bgColor(bgColor)
{}

void engine::Viewport::DrawViewport(void)
{
	// Window flags
	static const ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoSavedSettings;

	
	// TODO: add default docking node
	ImGui::SetNextWindowSize({960, 540}, ImGuiCond_Once);
	ImGui::Begin(m_title.c_str(), nullptr, flags);

	// Transform
	math::Vector2f position = ImGui::GetCursorScreenPos();
	math::Vector2f regionAvail = ImGui::GetContentRegionAvail();
	math::Vector2i sizePx
	(
		static_cast<int32>(regionAvail.GetX()),
		static_cast<int32>(regionAvail.GetY())
	);

	glViewport(0, 0, sizePx.GetX(), sizePx.GetY());
	m_fbo.RescaleFBO(sizePx.GetX(), sizePx.GetY());

	ImGui::Image(
		m_fbo.GetFrameTexture(),
		regionAvail,
		math::Vector2f(0.F, 1.F),
		math::Vector2f(1.F, 0.F)
	);
	
	ImGui::End();
}

void engine::Viewport::RenderToViewport(SceneGraph* sceneGraph)
{
	m_fbo.Bind();
	wnd::ClearWindow(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);
	ThreadManager::RenderScene(sceneGraph);
	m_fbo.Unbind();
}

void engine::Viewport::SetBgColor(math::Vector4f const& bgColor)
{
	m_bgColor = bgColor;
}
