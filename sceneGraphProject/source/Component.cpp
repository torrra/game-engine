#include "Component.h"

namespace engine
{
	Component::Component(EntityHandle owner)
		: m_owner(owner)
	{
	}

	bool Component::IsValid(void) const
	{
		return !(m_flags & ecs::INVALID_OBJECT);
	}

	bool Component::IsActive(void) const
	{
		return !(m_flags & ecs::INACTIVE_OBJECT);
	}

	void Component::Activate(bool activeState)
	{
		// Reset inactive bit
		if (activeState)
			m_flags &= (~ecs::EFlags::INACTIVE_OBJECT);

		// Toggle inactive bit
		else
			m_flags |= ecs::EFlags::INACTIVE_OBJECT;
	}

	void Component::Invalidate(void)
	{
		m_flags |= ecs::INVALID_OBJECT;
	}
}