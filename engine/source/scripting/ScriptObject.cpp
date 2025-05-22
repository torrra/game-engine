#include "scripting/ScriptObject.h"
#include "core/systems/ScriptSystem.h"

namespace engine
{
    ScriptObject::ScriptObject(EntityHandle entity, const std::string& filepath, uint32 index)
        : m_entity(entity), m_type(filepath), m_index(index), m_isValid(true)
    {
    }

    const std::string& ScriptObject::GetType(void) const
    {
        return m_type;
    }

    void ScriptObject::Register(void) const
    {
        ScriptSystem::RegisterNewScriptObject(m_type, m_entity);
    }

    void ScriptObject::Unregister(void)
    {
        ScriptSystem::UnregisterScriptObject(m_type, m_entity);
        m_isValid = false;
    }

    bool ScriptObject::IsValid(void) const
    {
        return m_isValid;
    }

    uint32 ScriptObject::GetIndex(void) const
    {
        return m_index;
    }

    const std::string& ScriptObject::GetName(void) const
    {
        return m_type;
    }
}