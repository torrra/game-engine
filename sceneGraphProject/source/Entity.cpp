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

		return (*this);
	}

	void Entity::Invalidate(void)
	{
		m_statusFlags |= ecs::INVALID_OBJECT;
	}

	bool Entity::IsValid() const
	{
		return !(m_statusFlags & ecs::INVALID_OBJECT);
	}

	void Entity::Activate(bool activeState)
	{
		if (activeState)
			m_statusFlags &= (~ecs::EFlags::INACTIVE_OBJECT);

		else
			m_statusFlags |= ecs::EFlags::INACTIVE_OBJECT;
	}

	void Entity::Print(void)
	{
		printf("\n[Entity]\nname: %s\nhandle: %zu\nparent: %zu\nflags: %zu\ncomponents: \
%zu\n\n", m_name.c_str(), m_handle, m_parent, m_statusFlags, m_components);
	}
}