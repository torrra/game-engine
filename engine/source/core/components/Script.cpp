#include "CoreTypes.h"

#include "core/components/Script.h"
#include "core/systems/ScriptSystem.h"

#include "serialization/TextSerializer.h"

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

    void Script::Unregister(void)
    {
        ScriptSystem::UnregisterComponent("_RemoveScriptComponent", m_owner);
    }

    void Script::AddScriptObject(const std::string& type)
    {
        /*std::string formattedType = type;

        for (char& character : formattedType)
        {
            if (character >= 'A' && character <= 'Z')
                character += 32;
        }

        m_scriptObjects.emplace_back(m_owner, formattedType);*/
        m_scriptObjects.emplace_back(m_owner, type);
    }

    void Script::SerializeText(std::ostream& output, EntityHandle owner, uint64 index) const
    {
        output << "[Script]\n   ";

        if constexpr (UpdateAfterParent<Script>::m_value)
        {
            text::Serialize(output, "index", index);
            output << "\n   ";
        }

        text::Serialize(output, "owner", owner);
        output << "\n   ";

        text::Serialize(output, "count", m_scriptObjects.size());
        output << "\n   ";

        for (const ScriptObject& object : m_scriptObjects)
        {
            text::Serialize(output, "scriptObject", object.GetType());
            output << "\n   ";
        }

        text::Serialize(output, "flags", m_flags);
        output << '\n';

    }

    const char* Script::DeserializeText(const char* text, const char* end)
    {
        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, m_owner);

        uint64 scriptObjectCount = 0;

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, scriptObjectCount);

        for (uint64 obj = 0; obj < scriptObjectCount; ++obj)
        {
            std::string objName;
            text = text::DeserializeString(text, end, objName);
            m_scriptObjects.emplace_back(m_owner, objName);
        }

        MOVE_TEXT_CURSOR(text, end);
        return text::DeserializeInteger(text, m_flags);
    }
}