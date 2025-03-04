#pragma once

#include <type_traits>
#include <unordered_map>

#include "TypesECS.h"
#include "Entity.h"

namespace engine
{
	class Component
	{
	private:

		friend class SceneGraph;

	public:

		ENGINE_API Component(EntityHandle owner);
		ENGINE_API ~Component(void) = default;

		virtual void Update(void) = 0;

		// Is this component a valid object?
		// true: this object is a valid component instance
		// false: this object is 'dead' and subject to overwrite
		ENGINE_API
		bool IsValid(void) const;

		// Is this component included when updating the scene?
		ENGINE_API
		bool IsActive(void) const;

		// Set the component's active status
		// true: will be included in update
		// false: will be excluded in update
		ENGINE_API
		void Activate(bool activeState);

		// Permanently set object up for destruction
		ENGINE_API
		void Invalidate(void);

	protected:

		// which entity in scene graph owns this component
		EntityHandle	m_owner;

		// status modifiers that affect behavior (inactive, invalid)
		uint64			m_flags = 0;
	};


	// Defines whether a type has objects that NEED to be updated after their parents
	// e.g. a transform component should be updated after its owner's transform.
	// False by default, must be manually instanciated and defined as true if needed
	template <CValidComponent TComponentType>
	struct UpdateAfterParent
	{
		static constexpr bool m_value = false;
	};

}