#include "ui/UIToolbar.h"
#include "ui/IconsLucide.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

ui::ToolbarButton::ToolbarButton(const char* displayLabel)
    : m_displayLabel(displayLabel), m_value(false)
{}

ui::Toolbar::Toolbar(const char* name)
    : m_name(name), m_orientation(TOOLBAR_VERTICAL), m_windowPosition(-1.0f)
{
}

uint32 ui::Toolbar::AddButton(const char* displayLabel)
{
    m_buttons.push_back(ToolbarButton(displayLabel));

    return static_cast<uint32>(m_buttons.size()) - 1;
}

bool ui::Toolbar::GetButtonState(uint32 index)
{
    return m_buttons[index].m_value;
}

void ui::Toolbar::Render(void)
{
    // Get auto resizing axis
    math::Vector2f requestedSize = (m_orientation == TOOLBAR_HORIZONTAL) ? math::Vector2f(-1.0f, 0.0f) : math::Vector2f(0.0f, -1.0f);
    ImGui::SetNextWindowSize(requestedSize);

    // Set window properties
    ImGuiWindowClass windowClass;
    windowClass.DockingAllowUnclassed = true;
    
    windowClass.DockNodeFlagsOverrideSet =
        ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_HiddenTabBar | 
        ImGuiDockNodeFlags_NoDockingSplit | ImGuiDockNodeFlags_NoDockingOverMe | 
        ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingOverOther;

    if (m_orientation == TOOLBAR_HORIZONTAL)
        windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeY;
    else 
        windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeX;
    ImGui::SetNextWindowClass(&windowClass);

    const f32 iconSizeVal = math::Floor(ImGui::GetFontSize() * 1.7f);
    const math::Vector2f iconSize(iconSizeVal, iconSizeVal);
    ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
    // Overwrite node size
    ImGuiDockNode* node = ImGui::GetWindowDockNode();
    if (node)
    {
        // Overwrite size of the node
        ImGuiStyle& style = ImGui::GetStyle();
        const EToolbarOrientation toolbarAxis = (EToolbarOrientation) (m_orientation ^ 1);
        const f32 dockedToolbarSize = style.WindowPadding[toolbarAxis] * 2.0f + iconSize[toolbarAxis];
        node->WantLockSizeOnce = true;
        node->Size[toolbarAxis] = node->SizeRef[toolbarAxis] = dockedToolbarSize;

        if (node->ParentNode && node->ParentNode->SplitAxis != ImGuiAxis_None)
            m_orientation = (EToolbarOrientation) (node->ParentNode->SplitAxis ^ 1);
    }

    // Render toolbar content
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, math::Vector2f(5.0f, 5.0f));
    for (uint64 buttonIndex = 0; buttonIndex < m_buttons.size(); ++buttonIndex)
    {
        if (m_buttons.size() > 0 && m_orientation == TOOLBAR_HORIZONTAL)
            ImGui::SameLine();

        // Set button background color
        if ((int64) buttonIndex == m_selected)
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
        else
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_Button));

        m_buttons[buttonIndex].m_value = ImGui::Button(m_buttons[buttonIndex].m_displayLabel.c_str(), iconSize);
        ImGui::PopStyleColor();
        
    }
    ImGui::PopStyleVar(2);

    // Context-menu to change axis
    if (!node)
    {
        if (ImGui::BeginPopupContextWindow())
        {
            ImGui::TextUnformatted(m_name.c_str());
            ImGui::Separator();
            if (ImGui::MenuItem("Horizontal", "", (m_orientation == TOOLBAR_HORIZONTAL)))
                m_orientation = TOOLBAR_HORIZONTAL;
            if (ImGui::MenuItem("Vertical", "", (m_orientation == TOOLBAR_VERTICAL)))
                m_orientation = TOOLBAR_VERTICAL;
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void ui::Toolbar::SetSelected(int32 index)
{
    m_selected = index;
}

void ui::Toolbar::SetToolbarPosition(math::Vector2f const& position)
{
    m_windowPosition = position;
}
