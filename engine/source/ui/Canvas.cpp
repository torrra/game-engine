#include "ui/Canvas.h"
#include "ui/elements/Label.h"
#include "engine/Engine.h"

#undef new
#include <imgui/imgui.h>

#include "utility/MemoryCheck.h"

#include <functional>

engine::Canvas::Canvas(math::Vector2f const& windowSize, math::Vector4f const& color)
    : m_size(windowSize), m_prevSize(windowSize), m_uidCounter(0)
{
    SetColor(color[0], color[1], color[2], color[3]);
}

void engine::Canvas::Render(math::Vector2f const& position, math::Vector2f const& size)
{
    // Make canvas uninteractable & allow inputs to pass through
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNavInputs |
        ImGuiWindowFlags_NoDocking;

    position;
    flags;
    if (engine::Engine::GetEngine()->HasEditor())
    {
        
        ImGui::Begin("Simulation view", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
        ImGui::PushStyleColor(ImGuiCol_ChildBg,m_bgColor);
        
        ImGui::BeginChild("Canvas", size, ImGuiChildFlags_NavFlattened, flags);
        
        ImGui::SetWindowSize(size);

        // Scale all UI elements to be consistent with screen size
        RescaleCanvas();

        // Update all UI elements on this canvas
        for (UIElement* element : m_elements)
        {
            element->Render();
        }

        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::End();
    }
    else
    {
        printf("No Editor\n");
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, {1.0f, 0.0f, 0.0f, 1.0f});
        
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

    //if (!engine::Engine::GetEngine()->HasEditor())
    //{
    //}
    //else
    //{
    //    ImGui::SetNextWindowPos(ImGui::GetWindowContentRegionMin());
    //    ImGui::SetNextWindowSize(ImGui::GetWindowSize());
    //}
    //
}

void engine::Canvas::RemoveElement(UIElement* element)
{
    // Check if an element is null
    if (!element)
        return;

    // Check element is in array
    for (int elementIndex = 0; elementIndex < m_elements.size(); ++elementIndex)
    {
        if (m_elements[elementIndex]->m_uid == element->m_uid)
        {
            // Delete element
            delete element;
            m_elements.erase(m_elements.begin() + elementIndex);

            return;
        }
    }

    std::printf("Warning: failed to delete UI element\n");
}

void engine::Canvas::RemoveAllEntities(void)
{
    for (UIElement* element : m_elements)
    {
        delete element;
    }

    m_elements.clear();
}

void engine::Canvas::Clear(void)
{
    // Remove all elements
    for (UIElement* element : m_elements)
    {
        delete element;
    }
    
    m_elements.clear();
}

engine::Label* engine::Canvas::AddLabel(const char* text, math::Vector2f const& position)
{
    m_elements.push_back(new Label(text, position));

    UIElement* element = m_elements[m_elements.size() - 1];
    element->SetUID(++m_uidCounter);

    return dynamic_cast<Label*>(element);
}

engine::Image* engine::Canvas::AddImage(
    const char* fileName, 
    math::Vector2f const& position)
{
    m_elements.push_back(new Image(fileName, position));

    UIElement* element = m_elements[m_elements.size() - 1];
    element->SetUID(++m_uidCounter);

    Image* image = dynamic_cast<Image*>(element);
    
    // Error handling
    if (!image->IsDataValid())
    {
        // TODO: add logging
        std::printf("Failed to create & add image to UI. Error: image '%s' could not be found.\n", fileName);
        RemoveElement(image);
    }

    return image;
}

engine::Button* engine::Canvas::AddButton(
    const char* text, 
    math::Vector2f const& position, 
    math::Vector2f const& size)
{
    m_elements.push_back(new Button(text, position, size));

    UIElement* element = m_elements[m_elements.size() - 1];
    element->SetUID(++m_uidCounter);
    
    return dynamic_cast<Button*>(element);
}

engine::ProgressBar* engine::Canvas::AddProgressBar(
    math::Vector2f const& position, 
    math::Vector2f const& size, 
    math::Vector2f const& range)
{
    m_elements.push_back(new ProgressBar(position, size, range));

    UIElement* element = m_elements[m_elements.size() - 1];
    element->SetUID(++m_uidCounter);

    return dynamic_cast<ProgressBar*>(element);
}

engine::Rectangle* engine::Canvas::AddRectangle(
    math::Vector2f const& position, 
    math::Vector2f const& size)
{
    m_elements.push_back(new Rectangle(position, size));
    
    UIElement* element = m_elements[m_elements.size() - 1];
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
