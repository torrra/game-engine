#include "ui/components/Component.h"
#include <engine/core/SceneGraph.h>
#include <engine/ui/UIComponent.h>

void editor::BaseComponent::RenderSection(engine::SceneGraph* graph)
{
    
    if (ui::CollapseSection(m_componentName.c_str(), m_isClosed))
    {
        ui::StartSection(m_componentName.c_str());
        SectionContent();
        ui::EndSection();
    }

    if (m_isClosed)
    {
        // TODO: consider remove component
        printf("Removing component... %s\n", m_componentName.c_str());
    }
}

void editor::BaseComponent::SectionContent(void)
{}

bool editor::BaseComponent::InputField(const char* uid, f32* value, f32 increment, f32 width, f32 xOffset)
{
    ui::SameLine(xOffset);
    ui::ItemWidth(width);
    return ui::InputBox(uid, value, increment);
}

void editor::BaseComponent::SetName(const char* name)
{
    m_componentName = name;
}
