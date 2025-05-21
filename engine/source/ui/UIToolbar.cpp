#include "ui/UIToolbar.h"
#include "ui/IconsLucide.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

ui::ToolbarButton::ToolbarButton(const char* displayLabel)
    : m_displayLabel(displayLabel), m_value(false)
{}

ui::Toolbar::Toolbar(const char* name)
    : m_name(name), m_orientation(TOOLBAR_VERTICAL)
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

    // 1. We request auto-sizing on one axis
    // Note however this will only affect the toolbar when NOT docked.
    math::Vector2f requested_size = (m_orientation == TOOLBAR_HORIZONTAL) ? math::Vector2f(-1.0f, 0.0f) : math::Vector2f(0.0f, -1.0f);
    ImGui::SetNextWindowSize(requested_size);

    // 2. Specific docking options for toolbars.
    // Currently they add some constraint we ideally wouldn't want, but this is simplifying our first implementation
    ImGuiWindowClass window_class;
    window_class.DockingAllowUnclassed = true;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoCloseButton;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_HiddenTabBar; // ImGuiDockNodeFlags_NoTabBar // FIXME: Will need a working Undock widget for _NoTabBar to work
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingSplit;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;

    if (m_orientation == TOOLBAR_HORIZONTAL)
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeY;
    else
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeX;
    ImGui::SetNextWindowClass(&window_class);

    const float font_size = ImGui::GetFontSize();
    const math::Vector2f icon_size(math::Floor(font_size * 1.7f), math::Floor(font_size * 1.7f));
    ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

    // Overwrite node size
    ImGuiDockNode* node = ImGui::GetWindowDockNode();
    if (node)
    {
        // Overwrite size of the node
        ImGuiStyle& style = ImGui::GetStyle();
        const EToolbarOrientation toolbar_axis_perp = (EToolbarOrientation) (m_orientation ^ 1);
        const float TOOLBAR_SIZE_WHEN_DOCKED = style.WindowPadding[toolbar_axis_perp] * 2.0f + icon_size[toolbar_axis_perp];
        node->WantLockSizeOnce = true;
        node->Size[toolbar_axis_perp] = node->SizeRef[toolbar_axis_perp] = TOOLBAR_SIZE_WHEN_DOCKED;

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


        if ((int64) buttonIndex == m_selected)
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
        else
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_Button));

        m_buttons[buttonIndex].m_value = ImGui::Button(m_buttons[buttonIndex].m_displayLabel.c_str(), icon_size);
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
