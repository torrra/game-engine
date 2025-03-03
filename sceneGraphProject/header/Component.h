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

		Component(EntityHandle owner);

		virtual void Update(void) = 0;


		bool IsValid(void) const;
		bool IsActive(void) const;

		void Activate(bool activeState);
		void Invalidate(void);

	protected:

		EntityHandle	m_owner;
		uint64			m_flags = 0;
	};

	template <CValidComponent TComponentType>
	struct UpdateAfterParent
	{
		static constexpr bool m_value = false;
	};

}