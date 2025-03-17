#include "ui/Canvas.h"
#include "ui/elements/Label.h"

#undef new
#include <imgui/imgui.h>

#include "utility/MemoryCheck.h"

#include <functional>

engine::Canvas::Canvas(math::Vector2f const& windowSize, math::Vector4f const& color)
	: m_size(windowSize), m_prevSize(windowSize), m_uidCounter(0)
{
	SetColor(color[0], color[1], color[2], color[3]);
}

void engine::Canvas::Render(void)
{
	// Make canvas uninteractable & allow inputs to pass through
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
	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_bgColor);
	ImGui::Begin("Canvas", nullptr, flags);

	// Scale all UI elements to be consistent with screen size
	RescaleCanvas();

	// Update all UI elements on this canvas
	for (UIElement* element : m_elements)
	{
		element->Render();
	}

	ImGui::End();
	ImGui::PopStyleColor();
}

void engine::Canvas::Clear(void)
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
	element->SetUID(++m_uidCounter);

	return dynamic_cast<Label*>(element);
}

engine::Image* engine::Canvas::AddImage(const char* fileName)
{
	m_elements.push_back(new Image(fileName));

	UIElement* element = m_elements[(int32) m_elements.size() - 1];
	element->SetUID(++m_uidCounter);

	return dynamic_cast<Image*>(element);
}

engine::Button* engine::Canvas::AddButton(const char* text, std::function<void(void)> function)
{
	m_elements.push_back(new Button(text, function));

	UIElement* element = m_elements[(int32) m_elements.size() - 1];
	element->SetUID(++m_uidCounter);
	
	return dynamic_cast<Button*>(element);
}

engine::ProgressBar* engine::Canvas::AddProgressBar(math::Vector2f const& position, math::Vector2f const& size, math::Vector2f const& range)
{
	m_elements.push_back(new ProgressBar(position, size, range));

	UIElement* element = m_elements[(int32) m_elements.size() - 1];
	element->SetUID(++m_uidCounter);

	return dynamic_cast<ProgressBar*>(element);
}

engine::Rectangle* engine::Canvas::AddRectangle(math::Vector2f const& position, math::Vector2f const& size)
{
	m_elements.push_back(new Rectangle(position, size));
	
	UIElement* element = m_elements[(int32) m_elements.size() - 1];
	element->SetUID(++m_uidCounter);

	return dynamic_cast<Rectangle*>(element);
}

void engine::Canvas::SetColor(f32 red, f32 green, f32 blue, f32 alpha)
{
	// Store color as int to reduce memory usage
	m_bgColor = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

void engine::Canvas::RescaleCanvas(void)
{
	
	m_size = ImGui::GetContentRegionAvail();

	// Screen size has not changed
	if (m_size == m_prevSize)
		return;

	const f32 regionRatio = m_size.GetX() / m_prevSize.GetX();

	// Update scale for all UI elements
	for (UIElement* element : m_elements)
	{
		if (element->m_autoScale)
			element->AutoScale(regionRatio);
	}

	// Update previous screen size
	m_prevSize = m_size;
}
