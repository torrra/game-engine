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

		Entity* GetEntity(EntityHandle& handle);

		Entity* GetEntityUnchecked(EntityHandle handle);

		Entity* GetEntity(const std::string& name);


		static uint64 RandomNumber(void);

	//private:

		template <CValidComponent TComponentType>
		ComponentArray<TComponentType>& GetComponentArray(void);


		EntityHandle MakeHandle(EntityHandle index, EntityHandle uid);
		EntityHandle ReparentEntity(Entity& toReparent, EntityHandle newParent, 
								    uint64 basePos = 0);

		EntityHandle MoveEntityToBack(Entity& toMove, EntityHandle newParent);

		ComponentArray<Transform>			m_sceneTransforms;
		std::vector<Entity>					m_sceneEntities;

		static Random						m_randomNumGen;
	};




	template<>
	inline ComponentArray<Transform>& SceneGraph::GetComponentArray<Transform>(void)
	{
		return m_sceneTransforms;
	}

}