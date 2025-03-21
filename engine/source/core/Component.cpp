#include "core/Component.h"
#include "core/SceneGraph.h"
#include "serialization/TextSerializer.h"

namespace engine
{
	Component::Component(EntityHandle owner, SceneGraph* scene)
		: m_owner(owner), m_currentScene(scene)
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

	EntityHandle Component::GetOwner(void) const
	{
		return m_owner;
	}

	void Component::DeserializeIndexedText(std::ifstream& input, uint64& index)
	{
		text::MoveCursorToVal(input);
		text::Deserialize(input, index);
		DeserializeText(input);
	}
}