#include "ui/components/Component.h"

#include <engine/ui/UIComponent.h>

void editor::ComponentProperty::RenderSection(void)
{
    
    if (ui::CollapseSection(m_componentName.c_str()))
    {
        ui::StartSection(m_componentName.c_str());
        SectionContent();
        ui::EndSection();
    }

}

void editor::ComponentProperty::SectionContent(void)
{}

void editor::ComponentProperty::SetName(const char* name)
{
    m_componentName = name;
}
