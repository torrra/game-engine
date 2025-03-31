#include "viewport/Viewport.h"
#include "thread/ThreadManager.h"
#include "core/SceneGraph.h"
#include "window/WindowLib.h"

#include <math/Vector2.hpp>

#include <glad/glad.h>



#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

void engine::Viewport::Update(FrameBuffer& fbo)
{
	static const ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse;

	ImGui::Begin("Viewport", nullptr, flags);
	
	math::Vector2f regionAvail = ImGui::GetContentRegionAvail();
	math::Vector2f position = ImGui::GetCursorScreenPos();
	
	glViewport(0, 0, (int32) regionAvail.GetX(), (int32) regionAvail.GetY());
	fbo.RescaleFBO((int32) regionAvail.GetX(), (int32) regionAvail.GetY());

	ImGui::GetWindowDrawList()->AddImage(
		fbo.GetFrameTexture(),
		position,
		position + regionAvail,
		math::Vector2f(0.F, 1.F),
		math::Vector2f(1.F, 0.F)
	);

	ImGui::End();
}

engine::ViewportV2::ViewportV2(FrameBuffer const& fbo)
	: m_fbo(fbo), m_prevSize(0), m_render(true)
{
}

void engine::ViewportV2::Update(void)
{
	static const ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoBackground;

	ImGui::Begin("ViewportV2", nullptr, flags);

	math::Vector2f regionAvail = ImGui::GetContentRegionAvail();
	math::Vector2i sizePx(
		static_cast<int32>(regionAvail.GetX()),
		static_cast<int32>(regionAvail.GetY())
	);
	glViewport(0, 0, sizePx.GetX(), sizePx.GetY());

	//if (m_prevSize != sizePx)
	//{
		m_fbo.RescaleFBO(sizePx.GetX(), sizePx.GetY());
		m_prevSize = sizePx;
		//ImGui::GetWindowDrawList()->AddImage(
		//	m_fbo.GetFrameTexture(),
		//	position,
		//	position + regionAvail,
		//	math::Vector2f(0.F, 1.F),
		//	math::Vector2f(1.F, 0.F)
		//);

	//}

	math::Vector2f position = ImGui::GetCursorScreenPos();
	ImGui::Image(
		m_fbo.GetFrameTexture(),
		regionAvail,
		math::Vector2f(0.F, 1.F),
		math::Vector2f(1.F, 0.F)
	);
	
	ImGui::End();
}

void engine::ViewportV2::RenderScene(SceneGraph* sceneGraph, math::Vector4f bgColor)
{

	m_fbo.Bind();

	wnd::ClearWindow(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
	ThreadManager::RenderScene(sceneGraph);

	m_fbo.UnBind();
}