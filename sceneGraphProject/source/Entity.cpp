#include <cstdio>

#include "Entity.h"

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

	EntityHandle Entity::GetHandle(void) const
	{
		return m_handle;
	}

	EntityHandle Entity::GetParent(void) const
	{
		return m_parent;
	}

	std::string Entity::GetName(void) const
	{
		return m_name;
	}

	void Entity::SetName(const std::string& name)
	{
		m_name = name;
	}

	bool Entity::IsValid() const
	{
		return !(m_statusFlags & ecs::INVALID_OBJECT);
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