#pragma once

#include <string>

#include "engine/core/TypesECS.h"

namespace engine
{
    // Class meant to represent an instance of a user-defined lua object
    // does not store any script data, only the name and owner of the instance
    class ScriptObject
    {
    public:

        ScriptObject(void) = delete;
        ScriptObject(EntityHandle entity, const std::string& type);
        ScriptObject(const ScriptObject&) = default;
        ~ScriptObject(void) = default;

        const std::string& GetType(void) const;

        // Create lua instance
        void Register(void) const;

    private:

        // TODO: change for script resource when ready
        std::string		m_type;
        EntityHandle	m_entity;
    };
}