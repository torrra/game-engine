#include "CoreTypes.h"

#include "core/components/Script.h"
#include "core/systems/ScriptSystem.h"

#include "serialization/TextSerializer.h"

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
        uint32 index = static_cast<uint32>(m_scriptObjects.size());
        m_scriptObjects.emplace_back(m_owner, type, index);
    }

    void Script::RemoveScriptObject(uint32 index)
    {
        if (index < m_scriptObjects.size())
        {
            ScriptObject& script = m_scriptObjects[index];
            script.Unregister();
        }
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

        uint64 scriptObjectCount = m_scriptObjects.size();

        for (const ScriptObject& object : m_scriptObjects)
        {
            if (!object.IsValid())
                --scriptObjectCount;
        }

        text::Serialize(output, "count", scriptObjectCount);
        output << "\n   ";

        for (const ScriptObject& object : m_scriptObjects)
        {
            if (!object.IsValid())
                continue;

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

            AddScriptObject(objName);
        }

        MOVE_TEXT_CURSOR(text, end);
        return text::DeserializeInteger(text, m_flags);
    }

    const std::vector<ScriptObject>& Script::GetScripts(void) const
    {
        return m_scriptObjects;
    }
}