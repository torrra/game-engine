#pragma once

#include <string>

#include "TypesECS.h"

namespace engine
{
	class Entity
	{
	private:

		// Special handle values
		enum EHandleUtils : EntityHandle
		{
			INVALID_HANDLE = 0xFFFFFFFFFFFFFFFF,

			// get 32 lower bits
			INDEX_MASK = 0x00000000FFFFFFFF,

			// get 32 higher bits
			UID_MASK = 0xFFFFFFFF00000000
		};


		// Makes it easier to know which components an entity has
		enum EComponentFlags : uint64
		{
			NO_COMPONENTS = 0,
			TRANSFORM = 1
		};

		friend class SceneGraph;

	public:

			 Entity(const std::string& name, EntityHandle handle, EntityHandle parent);
			 Entity(void) = delete;
			 Entity(const Entity&) = default;

		// Is this entity a valid object?
		// true: this object is a valid entity instance
		// false: this object is 'dead' and subject to overwrite
		bool IsValid(void) const;

		// Are this entity and its components included when updating the scene?
		void Activate(bool activeState);

		// Display this entity's information:
		// name, handle, parent, status flags, component flags
		void Print(void) const;

		// Does this entity have a component of a given type?
		template<CValidComponent TComponentType>
		bool HasComponent(void) const;

		EntityHandle GetHandle(void) const;
		EntityHandle GetParent(void) const;

		std::string  GetName(void) const;
		void		 SetName(const std::string& name);

		Entity& operator=(const Entity& rhs);

	private:

		// Permanently set object up for destruction
		void Invalidate(void);

		// Get EComponentFlag corresponding to the component type
		// NO_COMPONENT by default, must be manually instantiated for
		// each component type in their respective headers
		template<CValidComponent TComponentType>
		static constexpr EComponentFlags GetComponentFlag();

		std::string			m_name;
		EntityHandle		m_handle;
		EntityHandle		m_parent = INVALID_HANDLE;

		// status modifiers that affect behavior (inactive, invalid)
		uint64				m_statusFlags = ecs::NONE;

		// what component this entity hass
		uint64				m_components = NO_COMPONENTS;

	};


	template<CValidComponent TComponentType>
	inline constexpr Entity::EComponentFlags Entity::GetComponentFlag()
	{
		return NO_COMPONENTS;
	}


	template<CValidComponent TComponentType>
	inline bool Entity::HasComponent(void) const
	{
		return m_components & GetComponentFlag<TComponentType>();
	}


}