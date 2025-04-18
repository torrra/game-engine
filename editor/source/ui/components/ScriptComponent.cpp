#include "ui/components/ScriptComponent.h"
#include <engine/ui/UIComponent.h>

editor::ScriptComponent::ScriptComponent(std::string const& scriptName)
    : m_script(nullptr)
{
    SetName(scriptName.c_str());
}

editor::ScriptComponent::~ScriptComponent(void)
{
    m_script = nullptr;
}

void editor::ScriptComponent::SetScript(engine::ScriptObject* script)
{
    m_script = script;
}

void editor::ScriptComponent::SectionContent(void)
{
    ui::Text("Script data... WIP");
}
