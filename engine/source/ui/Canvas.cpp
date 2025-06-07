#include "ui/Canvas.h"
#include "ui/elements/Label.h"
#include "engine/Engine.h"

#undef new
#include <imgui/imgui.h>

#include "utility/MemoryCheck.h"

#include <functional>

engine::Canvas::Canvas(math::Vector2f const& windowSize, math::Vector4f const& color)
    : m_size(windowSize), m_prevSize(windowSize), m_uidCounter(0), m_isFirstUpdate(true)
{
    SetColor(color[0], color[1], color[2], color[3]);
}

void engine::Canvas::Render(math::Vector2f const& position, math::Vector2f const& size)
{
    // Re-introduce window position
    (void) position;

    std::lock_guard lock(m_canvasMutex);

    // Make canvas uninteractable & allow inputs to pass through
    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNavInputs |
        ImGuiWindowFlags_NoDocking;

    // Window needs to be different if the scene is rendered is
    if (engine::Engine::GetEngine()->HasEditor())
    {
        
        ImGui::Begin("Simulation view", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
        ImGui::PushStyleColor(ImGuiCol_ChildBg, m_bgColor);
        
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
}

void engine::Canvas::RemoveElement(UIElement* element)
{
    // Check if an element is null
    if (!element)
        return;

    std::lock_guard lock(m_canvasMutex);

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
    std::lock_guard lock(m_canvasMutex);

    for (UIElement* element : m_elements)
    {
        delete element;
    }

    m_elements.clear();
}

void engine::Canvas::Clear(void)
{
    std::lock_guard lock(m_canvasMutex);

    // Remove all elements
    for (UIElement* element : m_elements)
    {
        delete element;
    }
    
    m_elements.clear();
}

engine::Label* engine::Canvas::AddLabel(const char* text, math::Vector2f const& position)
{
    std::lock_guard lock(m_canvasMutex);

    m_elements.push_back(new Label(text, position));

    UIElement* element = m_elements[m_elements.size() - 1];
    element->SetUID(++m_uidCounter);

    return dynamic_cast<Label*>(element);
}

engine::Image* engine::Canvas::AddImage(
    const char* fileName, 
    math::Vector2f const& position)
{
    std::lock_guard lock(m_canvasMutex);

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
    std::lock_guard lock(m_canvasMutex);

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
    std::lock_guard lock(m_canvasMutex);

    m_elements.push_back(new ProgressBar(position, size, range));

    UIElement* element = m_elements[m_elements.size() - 1];
    element->SetUID(++m_uidCounter);

    return dynamic_cast<ProgressBar*>(element);
}

engine::Rectangle* engine::Canvas::AddRectangle(
    math::Vector2f const& position, 
    math::Vector2f const& size)
{
    std::lock_guard lock(m_canvasMutex);

    m_elements.push_back(new Rectangle(position, size));
    
    UIElement* element = m_elements[m_elements.size() - 1];
    element->SetUID(++m_uidCounter);

    return dynamic_cast<Rectangle*>(element);
}

void engine::Canvas::SetColor(f32 red, f32 green, f32 blue, f32 alpha)
{
    std::lock_guard lock(m_canvasMutex);

    // Store color as int to reduce memory usage
    m_bgColor = ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha});
}

math::Vector2f engine::Canvas::GetSize(void) const
{
    return m_size;
}

void engine::Canvas::RescaleCanvas(void)
{
    m_size = ImGui::GetContentRegionAvail();

    // Prevent canvas invalid aspect ratio
    if (m_isFirstUpdate)
    {
        m_prevSize = m_size;
        m_isFirstUpdate = false;
    }

    // Screen size has not changed
    if (m_size == m_prevSize)
        return;

    f32 regionRatio = m_size.GetX() / m_prevSize.GetX();

    // Update scale for all UI elements
    for (UIElement* element : m_elements)
    {
        if (element->m_autoScale)
            element->AutoScale(regionRatio);
    }

    // Update previous screen size
    m_prevSize = m_size;
}
