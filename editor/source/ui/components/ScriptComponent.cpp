#include "ui/components/ScriptComponent.h"

#include <engine/ui/UIComponent.h>

editor::ScriptComponent::ScriptComponent(void)
{
    // TODO: add script name
    SetName("Script");
}

editor::ScriptComponent::~ScriptComponent(void)
{}

void editor::ScriptComponent::SectionContent(void)
{
    ui::Text("e");
}
