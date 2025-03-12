#include "ui/Canvas.h"
#include "ui/elements/Label.h"

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "utility/MemoryCheck.h"

#include <iostream>
#include <functional>

engine::Canvas::Canvas(math::Vector4<float> color)
	: m_color(color)
{
}

ENGINE_API void engine::Canvas::Render(void)
{
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		/*ImGuiWindowFlags_NoBackground |*/ // TODO: re-add later
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_color);
	ImGui::Begin("Canvas", nullptr, flags);

	for (UIElement* element : m_elements)
	{
		element->Render();
	}

	ImGui::End();
	ImGui::PopStyleColor();
}

ENGINE_API void engine::Canvas::Clear(void)
{
	for (UIElement* element : m_elements)
	{
		delete element;
	}

	m_elements.shrink_to_fit();
}

engine::Label* engine::Canvas::AddLabel(const char* text)
{
	m_elements.push_back(new Label(text));

	return dynamic_cast<Label*>(m_elements[(int) m_elements.size() - 1]);
}

engine::Image* engine::Canvas::AddImage(const char* fileName)
{
	m_elements.push_back(new Image(fileName));

	return dynamic_cast<Image*>(m_elements[(int) m_elements.size() - 1]);
}

engine::Button* engine::Canvas::AddButton(const char* text, std::function<void(void)> function)
{
	m_elements.push_back(new Button(text, function));

	return dynamic_cast<Button*>(m_elements[(int) m_elements.size() - 1]);
}

engine::ProgressBar* engine::Canvas::AddProgressBar(void)
{
	m_elements.push_back(new ProgressBar());

	return dynamic_cast<ProgressBar*>(m_elements[(int) m_elements.size() - 1]);
}
