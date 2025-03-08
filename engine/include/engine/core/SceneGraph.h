#pragma once

#include <random>
#include <vector>
#include <string>

#include "engine/EngineExport.h"
#include "TypesECS.h"
#include "Component.h"
#include "Entity.h"
#include "ComponentArray.h"
#include "components/Transform.h"

namespace engine
{
	class SceneGraph
	{

	private:

		// Random uint64 generator.
		class Random
		{
		public:

			Random(void) = delete;
			Random(Random&&) = delete;
			Random(std::random_device randomDevice);

			int64 Generate();

		private:
			std::mt19937_64							m_generator;
			std::uniform_int_distribution<int64>	m_distribution;
		};

	public:

		ENGINE_API SceneGraph(void) = default;
		ENGINE_API SceneGraph(const SceneGraph&) = default;
		ENGINE_API ~SceneGraph(void) = default;

		// Create a new entity in the current scene.
		// Returns a unique handle that is used to retrieve the handle from its
		// memory location, and is used to check the entity's existence/validity.
		ENGINE_API	
		EntityHandle CreateEntity(const std::string& name,
								  EntityHandle parent = Entity::INVALID_HANDLE);

		// Create a component to attach to an existing entity.
		// An entity can only have one component of each type
		// NOTE: Pointers are not guaranteed to persist through the
		// program's lifetime. GetComponent() should be called frequently to
		// ensure the reference is up-to-date
		template <CValidComponent TComponentType>
		TComponentType* CreateComponent(EntityHandle ownerEntity);

		// Get an existing component from an entity.
		// Returns nullptr if:
		// - the entity does not own such a component
		// - the entity does not exist or is invalid
		// NOTE: Pointers are not guaranteed to persist through the
		// program's lifetime. This function should be called frequently to
		// ensure the reference is up-to-date
		template <CValidComponent TComponentType>
		TComponentType* GetComponent(EntityHandle ownerEntity);

		// Get a raw pointer to the entity from its handle.
		// Returns nullptr if:
		// - the entity does not exist
		// - the entity exists but was flagged as invalid
		// NOTE: Pointers are not guaranteed to persist through the
		// program's lifetime. This function should be called frequently to
		// ensure the reference is up-to-date
		ENGINE_API
		Entity* GetEntity(EntityHandle handle);

		// Get the first entity encountered with that name. May not be the highest
		// object displayed in the hierarchy. Entity must have a unique name to 
		// guarantee the entity you're getting is the right one
		// NOTE: Pointers are not guaranteed to persist through the
		// program's lifetime. This function should be called frequently to
		// ensure the reference is up-to-date
		ENGINE_API
		Entity* GetEntity(const std::string& name);

		// Get the entity's name with its parents names in front of it,
		// separated with a dot.
		ENGINE_API
		std::string GetFullEntityName(EntityHandle entity);

		// Get the handles of every entity that is a parent/grandparent/etc of
		// an entity.The first element in the returned array is the entity's direct
		// parent, and the last one is the one on top of the hierarchy
		ENGINE_API
		std::vector<EntityHandle> GetAllParents(EntityHandle entity);


		// Permanently set an entity for destruction/overwrite.
		ENGINE_API
		void	DestroyEntity(EntityHandle entity);

		// Give a new parent to an entity
		ENGINE_API
		void	ReparentEntity(EntityHandle toReparent, EntityHandle newParent);

		// Get an entity's direct children (no grandchildren, etc)
		ENGINE_API
		std::vector<EntityHandle> GetChildren(EntityHandle parent);

		// Get all of entity's children, grandchildren, etc
		ENGINE_API
		std::vector<EntityHandle> GetChildrenAllLevels(EntityHandle parent);

		ENGINE_API
		SceneGraph& operator=(const SceneGraph&) = default;

		// Output a uint64 between 0 and ULONG_MAX
		ENGINE_API
		static uint64 RandomNumber(void);

	private:

		// Get the component array corresponding to a type
		template <CValidComponent TComponentType>
		ComponentArray<TComponentType>& GetComponentArray(void);

		// Group the index and uid bits together in a 64-bit handle
		EntityHandle MakeHandle(EntityHandle index, EntityHandle uid);

		// Internal ReparentEntity overload. Direclty uses an entity's pointer
		void	ReparentEntity(Entity* toReparent, EntityHandle newParent);

		// All transform components in the scene
		ComponentArray<Transform>			m_sceneTransforms;

		// All entities in tge scene
		std::vector<Entity>					m_sceneEntities;

		// Random uint64 generator. We only need a unique instance
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

		return array.GetComponent(ownerEntity);
	}


	template<CValidComponent TComponentType>
	inline TComponentType* SceneGraph::CreateComponent(EntityHandle ownerEntity)
	{
		Entity* ownerPtr = GetEntity(ownerEntity);

		if (!ownerPtr)
			return nullptr;

		// return existing component instead
		if (ownerPtr->HasComponent<TComponentType>())
			return GetComponent<TComponentType>(ownerEntity);

		ComponentArray<TComponentType>& array = GetComponentArray<TComponentType>();

		// set owner flag to tell that it owns a component of this type
		ownerPtr->m_components |= Entity::GetComponentFlag<TComponentType>();
		return array.CreateComponent(ownerEntity, ownerPtr->m_parent);
	}

}

// short, convenient namespace alias
namespace mustang_2024_moteur_gpm_2027_gpm_2027_projet_moteur = engine;