#include <cstdio>

#include "core/Entity.h"
#include "core/systems/ScriptSystem.h"

#include "serialization/TextSerializer.h"

namespace engine
{
    Entity::Entity(const std::string& name, EntityHandle handle, EntityHandle parent)
        : m_name(name), m_handle(handle), m_parent(parent)
    {
    }

    Entity& Entity::operator=(const Entity& rhs)
    {
        m_name = rhs.m_name;
        m_statusFlags = rhs.m_statusFlags;
        m_components = rhs.m_components;
        m_handle = rhs.m_handle;
        m_parent = rhs.m_parent;

        return (*this);
    }

    void Entity::Invalidate(void)
    {
        m_statusFlags |= ecs::INVALID_OBJECT;
    }

    bool Entity::HasParent(void) const
    {
        return m_parent != INVALID_HANDLE;
    }

    EntityHandle Entity::GetHandle(void) const
    {
        return m_handle;
    }

    EntityHandle Entity::GetParent(void) const
    {
        return m_parent;
    }

    const std::string& Entity::GetName(void) const
    {
        return m_name;
    }

    void Entity::SetName(const std::string& name)
    {
        m_name = name;
        ScriptSystem::RegisterNewEntity(m_handle, name);
    }

    void Entity::SerializeText(std::ofstream& file) const
    {
        if (m_statusFlags & ecs::NON_SERIALIZABLE_OBJECT)
            return;

        file << "[Entity]\n   ";
        text::Serialize(file, "name", m_name);
        file << "\n   ";
        text::Serialize(file, "handle", m_handle);
        file << "\n   ";
        text::Serialize(file, "parent", m_parent);
        file << "\n   ";
        text::Serialize(file, "flags", m_statusFlags);
        file << "\n   ";
        text::Serialize(file, "components", m_components);
        file << '\n';
    }

    const char* Entity::DeserializeText(const char* text, const char* end)
    {
        text = text::DeserializeString(text, end, m_name);

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, m_handle);

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, m_parent);

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, m_statusFlags);

        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeInteger(text, m_components);
        return text;
    }

    bool Entity::IsValid() const
    {
        return !(m_statusFlags & ecs::INVALID_OBJECT);
    }

    bool Entity::IsActive(void) const
    {
        return !(m_statusFlags & ecs::INACTIVE_OBJECT);
    }

    void Entity::Activate(bool activeState)
    {
        // Reset inactive bit
        if (activeState)
            m_statusFlags &= (~ecs::EFlags::INACTIVE_OBJECT);

        // Toggle inactive bit
        else
            m_statusFlags |= ecs::EFlags::INACTIVE_OBJECT;
    }

    void Entity::Print(void) const
    {
        printf("\n[Entity]\nname: %s\nhandle: %zu\nparent: %zu\nflags: %zu\ncomponents: \
%zu\n\n", m_name.c_str(), m_handle, m_parent, m_statusFlags, m_components);
    }
}