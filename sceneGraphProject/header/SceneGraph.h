#pragma once

#include <random>
#include <vector>
#include <string>

#include "TypesECS.h"
#include "Component.h"
#include "Entity.h"
#include "ComponentArray.h"
#include "Transform.h"

namespace engine
{
	class SceneGraph
	{

	private:

		class Random
		{
		public:

			Random(void) = delete;
			Random(Random&&) = delete;
			Random(std::random_device randomDevice);

			uint64 Generate();

		private:
			std::mt19937_64							m_generator;
			std::uniform_int_distribution<uint64>	m_distribution;
		};

	public:

		EntityHandle CreateEntity(const std::string& name,
								  EntityHandle parent = Entity::INVALID_HANDLE);

		template <CValidComponent TComponentType>
		TComponentType* CreateComponent(EntityHandle ownerEntity);

		template <CValidComponent TComponentType>
		TComponentType* GetComponent(EntityHandle ownerEntity);

		Entity* GetEntity(EntityHandle& handle);

		Entity* GetEntityUnchecked(EntityHandle handle);

		Entity* GetEntity(const std::string& name);

		void	DestroyEntity(EntityHandle entity);

		std::vector<EntityHandle> GetChildren(EntityHandle parent);
		std::vector<EntityHandle> GetChildrenAllLevels(EntityHandle parent);

		

		static uint64 RandomNumber(void);

	//private:

		template <CValidComponent TComponentType>
		ComponentArray<TComponentType>& GetComponentArray(void);


		EntityHandle MakeHandle(EntityHandle index, EntityHandle uid);
		EntityHandle ReparentEntity(Entity& toReparent, EntityHandle newParent, 
								    uint64 basePos);

		void		 ReparentEntity(Entity& toReparent, EntityHandle newParent);

		EntityHandle MoveEntityToBack(Entity& toMove, EntityHandle newParent);
		void AddChildrenToArray(std::vector<EntityHandle>& array, EntityHandle parent);

		ComponentArray<Transform>			m_sceneTransforms;
		std::vector<Entity>					m_sceneEntities;

		static Random						m_randomNumGen;
	};




	template<>
	inline ComponentArray<Transform>& SceneGraph::GetComponentArray<Transform>(void)
	{
		return m_sceneTransforms;
	}


	template<CValidComponent TComponentType>
	inline TComponentType* SceneGraph::GetComponent(EntityHandle ownerEntity)
	{
		ComponentArray<TComponentType>& array = GetComponentArray<TComponentType>();

		Entity* ownerPtr = GetEntity(ownerEntity);

		if (!ownerPtr)
			return nullptr;

		if (ownerPtr->HasComponent<TComponentType>())
			return array.GetComponent(ownerEntity);

		return nullptr;
	}


	template<CValidComponent TComponentType>
	inline TComponentType* SceneGraph::CreateComponent(EntityHandle ownerEntity)
	{
		Entity* ownerPtr = GetEntity(ownerEntity);

		if (!ownerPtr)
			return nullptr;

		if (ownerPtr->HasComponent<TComponentType>())
			return GetComponent<TComponentType>(ownerEntity);

		ComponentArray<TComponentType>& array = GetComponentArray<TComponentType>();

		ownerPtr->m_components |= Entity::GetComponentFlag<TComponentType>();
		return array.CreateComponent(ownerEntity, ownerPtr->m_parent);
	}

}