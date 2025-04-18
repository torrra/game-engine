#include "ui/components/Component.h"

#include <engine/ui/UIComponent.h>

void editor::BaseComponent::RenderSection(void)
{
    
    if (ui::CollapseSection(m_componentName.c_str()))
    {
        ui::StartSection(m_componentName.c_str());
        SectionContent();
        ui::EndSection();
    }
}

void editor::BaseComponent::SectionContent(void)
{}

void editor::BaseComponent::SetName(const char* name)
{
    m_componentName = name;
}

bool editor::BaseComponent::InputField(const char* uid, f32* value, f32 increment, f32 width, f32 xOffset)
{
    ui::SameLine(xOffset);
    ui::ItemWidth(width);
    return ui::InputBox(uid, value, increment);
}