#include "ui/Canvas.h"
#include "ui/elements/Label.h"

#undef new
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "utility/MemoryCheck.h"

#include <iostream>
#include <functional>

engine::Canvas::Canvas(math::Vector2f const& windowSize, math::Vector4f const& color)
	: m_size(windowSize), m_prevSize(windowSize), m_color(color), m_uid(0)
{
}

ENGINE_API void engine::Canvas::Render(void)
{
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
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

	RescaleCanvas();

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

	UIElement* element = m_elements[(int32) m_elements.size() - 1];
	element->SetUID(++m_uid);

	return dynamic_cast<Label*>(element);
}

engine::Image* engine::Canvas::AddImage(const char* fileName)
{
	m_elements.push_back(new Image(fileName));

	UIElement* element = m_elements[(int32) m_elements.size() - 1];
	element->SetUID(++m_uid);

	return dynamic_cast<Image*>(element);
}

engine::Button* engine::Canvas::AddButton(const char* text, std::function<void(void)> function)
{
	m_elements.push_back(new Button(text, function));

	UIElement* element = m_elements[(int32) m_elements.size() - 1];
	element->SetUID(++m_uid);
	
	return dynamic_cast<Button*>(element);
}

engine::ProgressBar* engine::Canvas::AddProgressBar(math::Vector2f const& position, math::Vector2f const& size, math::Vector2f const& range)
{
	m_elements.push_back(new ProgressBar(position, size, range));

	UIElement* element = m_elements[(int32) m_elements.size() - 1];
	element->SetUID(++m_uid);

	return dynamic_cast<ProgressBar*>(element);
}

engine::Rectangle* engine::Canvas::AddRectangle(math::Vector2f const& pos, math::Vector2f const& sizePx)
{
	m_elements.push_back(new Rectangle(pos, sizePx));
	
	UIElement* element = m_elements[(int32) m_elements.size() - 1];
	element->SetUID(++m_uid);

	return dynamic_cast<Rectangle*>(element);
}

void engine::Canvas::RescaleCanvas(void)
{
	m_size = ImGui::GetContentRegionAvail();

	if (m_size != m_prevSize)
	{
		const f32 regionRatio = m_size.GetX() / m_prevSize.GetX();

		for (UIElement* element : m_elements)
		{

			if (element->m_autoScale)
				element->AutoScale(regionRatio);
		}

		m_prevSize = m_size;
	}
}
