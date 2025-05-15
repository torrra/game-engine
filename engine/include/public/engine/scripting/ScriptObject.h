#pragma once

#include <string>
#include "engine/EngineExport.h"
#include "engine/core/TypesECS.h"

namespace engine
{
    // Class meant to represent an instance of a user-defined lua object
    // does not store any script data, only the name and owner of the instance
    class ScriptObject
    {
    public:

        ScriptObject(void) = delete;
        ENGINE_API ScriptObject(EntityHandle entity, const std::string& type, uint32 index);
        ENGINE_API ScriptObject(ScriptObject&&) noexcept = default;
                   ScriptObject(const ScriptObject&) = delete;
        ENGINE_API ~ScriptObject(void) = default;

        ENGINE_API const std::string& GetType(void) const;

        // Create lua instance
        ENGINE_API void Register(void) const;

        ENGINE_API void Unregister(void);

        ENGINE_API bool IsValid(void) const;
        ENGINE_API uint32 GetIndex(void) const;
        ENGINE_API const std::string& GetName(void) const;

        ENGINE_API ScriptObject& operator=(ScriptObject&&) noexcept = default;
        ENGINE_API ScriptObject& operator=(const ScriptObject&) = delete;

    private:

        std::string		m_type;
        EntityHandle	m_entity;
        uint32          m_index;
        bool            m_isValid;
    };
}