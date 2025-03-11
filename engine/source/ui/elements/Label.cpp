#include "ui/elements/Label.h"

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <cstdarg>

engine::Label::Label(const char* text)
	: m_text(text)
{
	m_textColor = {1.0f, 1.0f, 1.0f, 1.0f};
}

engine::Label::~Label(void)
{
}

ENGINE_API void engine::Label::Render(void)
{
	// TODO: imgui config
	//ImGui::SetCursorPos(uiUtils::Convert(m_transform.m_position));
	//ImGui::TextColored(uiUtils::Convert(m_textColor), m_text.c_str());
}
