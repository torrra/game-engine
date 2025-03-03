#pragma once

#include <string>

#include "TypesECS.h"

namespace engine
{
	class Entity
	{
	private:

		enum EHandleUtils : EntityHandle
		{
			INVALID_HANDLE = 0xFFFFFFFFFFFFFFFF,

			// get 32 lower bits
			INDEX_MASK = 0x00000000FFFFFFFF,

			// get 32 higher bits
			UID_MASK = 0xFFFFFFFF00000000
		};

		enum EComponentFlags : uint64
		{
			NO_COMPONENTS = 0,
			TRANSFORM = 1
		};

		friend class SceneGraph;

	public:

		Entity(const std::string& name, EntityHandle handle, EntityHandle parent);
		Entity(void) = delete;
		// note: copying an entity does NOT copy its parent index
		// a copy will be parentless by default
		Entity(const Entity&) = default;

		bool IsValid(void) const;
		void Activate(bool activeState);

		void Print(void);

		template<CValidComponent TComponentType>
		bool HasComponent(void);

	//private:


		Entity& operator=(const Entity& rhs);

		void Invalidate(void);

		template<CValidComponent TComponentType>
		static constexpr EComponentFlags GetComponentFlag();

		std::string			m_name;
		EntityHandle		m_handle;
		EntityHandle		m_parent = INVALID_HANDLE;
		uint64				m_statusFlags = ecs::NONE;
		uint64				m_components = NO_COMPONENTS;

	};

	template<CValidComponent TComponentType>
	inline constexpr Entity::EComponentFlags Entity::GetComponentFlag()
	{
		return NO_COMPONENTS;
	}


	template<CValidComponent TComponentType>
	inline bool Entity::HasComponent(void)
	{
		return m_components & GetComponentFlag<TComponentType>();
	}


}