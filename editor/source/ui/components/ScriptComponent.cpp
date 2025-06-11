#include "ui/components/ScriptComponent.h"
#include <engine/ui/UIComponent.h>
#include <engine/core/components/Script.h>
#include <engine/core/SceneGraph.h>

editor::ScriptComponent::ScriptComponent(std::string const& scriptName)
{
    SetName(scriptName.c_str());
    SetType(SCRIPT);
}

void editor::ScriptComponent::SetScript(uint32 script)
{
    m_script = script;
}

void editor::ScriptComponent::SectionContent(void)
{
    ui::Text("User script");
}

void editor::ScriptComponent::RenderSection(engine::SceneGraph* graph,
                                            engine::EntityHandle const& handle)
{
    BaseComponent::RenderSection(graph, handle);

    if (!m_isClosed)
        return;

    if (engine::Script* script = graph->GetComponent<engine::Script>(handle))
    {
        script->RemoveScriptObject(m_script);
    }
    
}
