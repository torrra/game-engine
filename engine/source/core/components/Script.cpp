#include "CoreTypes.h"

#include "core/components/Script.h"
#include "core/systems/ScriptSystem.h"

#include <iostream>

namespace engine
{
    void Script::Start(void)
    {
        ScriptSystem::StartScript(m_owner);
    }

    void Script::Update(f32 deltaTime)
    {
        ScriptSystem::UpdateScript(m_owner, deltaTime);
    }

    void Script::Register(void)
    {
        ScriptSystem::RegisterNewComponent("_NewScriptComponent", m_owner);

        for (const ScriptObject& object : m_scriptObjects)
            object.Register();
    }

    void Script::AddScriptObject(const std::string& type)
    {
        std::string formattedType = type;

        for (char& character : formattedType)
        {
            if (character >= 'A' && character <= 'Z')
                character += 32;
        }

        m_scriptObjects.emplace_back(m_owner, formattedType);
    }
}