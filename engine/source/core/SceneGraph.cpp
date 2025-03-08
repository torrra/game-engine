#include "core/SceneGraph.h"
#include "core/Entity.h"

#include "core/systems/ScriptSystem.h"

#include <iostream>

namespace engine
{
	SceneGraph::Random	SceneGraph::m_randomNumGen = Random(std::random_device());

	uint64 SceneGraph::RandomNumber(void)
	{
		return m_randomNumGen.Generate();
	}

	EntityHandle SceneGraph::CreateEntity(const std::string& name, EntityHandle parent)
	{
		EntityHandle newIndex = 0;
		EntityHandle newUID = RandomNumber();

		EntityHandle entityCount = static_cast<EntityHandle>(m_sceneEntities.size());

		for (; newIndex < entityCount; ++newIndex)
		{
			Entity& currentEntity = m_sceneEntities[newIndex];

			if (!currentEntity.IsValid())
			{
				printf("[Scene graph]: filling invalid slot\n");
				EntityHandle newHandle = MakeHandle(newIndex, newUID);

				// write over dead entity to avoid reallocation
				currentEntity = Entity(name, newHandle, parent);
				ScriptSystem::RegisterNewEntity(newHandle, GetFullEntityName(newHandle));
				return currentEntity.m_handle;
			}
		}

		printf("[Scene graph]: creating new slot\n");

		// create entity in new slot in array
		EntityHandle newHandle = MakeHandle(newIndex, newUID);

		if (newHandle != Entity::INVALID_HANDLE)
		{
			m_sceneEntities.emplace_back(name, newHandle, parent);
			ScriptSystem::RegisterNewEntity(newHandle, GetFullEntityName(newHandle));
		}

		// TODO: log error?
		else
			printf("[Scene graph]: no entity created\n");

		return newHandle;
	}


	Entity* SceneGraph::GetEntity(EntityHandle handle)
	{
		if (handle == Entity::INVALID_HANDLE)
			return nullptr;

		EntityHandle index = (handle & Entity::INDEX_MASK);

		// out of bounds
		if (index >= static_cast<EntityHandle>(m_sceneEntities.size()))
			return nullptr;

		Entity& entity = m_sceneEntities[index];
		EntityHandle uid = (handle & Entity::UID_MASK);

		// requested object was written over and no longer exists
		if (uid != (entity.m_handle & Entity::UID_MASK))
			return nullptr;

		// object is dead and will be written over
		if (!entity.IsValid())
			return nullptr;

		printf("[Scene graph]: found entity with matching handle\n");
		return &entity;
	}


	Entity* SceneGraph::GetEntity(const std::string& name)
	{
		for (Entity& currentEntity : m_sceneEntities)
		{
			if (currentEntity.IsValid() && currentEntity.m_name == name)
				return &currentEntity;
		}

		return nullptr;
	}

	std::string SceneGraph::GetFullEntityName(EntityHandle entity)
	{
		std::string name;
		Entity* entityPtr = GetEntity(entity);

		if (!entityPtr)
			return name;

		std::vector<EntityHandle> allParents = GetAllParents(entity);

		// iterate backwards to start from parent closest to root
		for (auto parentIt = allParents.rbegin(); parentIt != allParents.rend(); ++parentIt)
		{
			if (Entity* parent = GetEntity(*parentIt))
				name += parent->GetName() + '.';
		}

		return name + entityPtr->GetName();
	}

	std::vector<EntityHandle> SceneGraph::GetAllParents(EntityHandle entity)
	{
		std::vector<EntityHandle>	parents;

		if (Entity* entityPtr = GetEntity(entity))
		{
			EntityHandle currentParent = entityPtr->m_parent;

			while (currentParent != Entity::INVALID_HANDLE)
			{
				parents.push_back(currentParent);
				entityPtr = GetEntity(currentParent);

				// No need to keep iterating if parent is invalid
				if (!entityPtr)
					break;
				else
					currentParent = entityPtr->m_parent;
			}
		}

		return parents;
	}

	void SceneGraph::DestroyEntity(EntityHandle entity)
	{
		if (Entity* entityPtr = GetEntity(entity))
		{
			m_sceneTransforms.InvalidateComponent(entity);

			entityPtr->Invalidate();

			std::vector<EntityHandle> children = GetChildren(entity);

			// invalid entity's parent becomes children's parent so
			// that we don't delete children
			for (EntityHandle child : children)
				ReparentEntity(GetEntity(child), entityPtr->m_parent);
		}
	}

	void SceneGraph::ReparentEntity(EntityHandle toReparent, EntityHandle newParent)
	{
		if (Entity* toReparentPtr = GetEntity(toReparent))
			ReparentEntity(toReparentPtr, newParent);
	}

	std::vector<EntityHandle> SceneGraph::GetChildren(EntityHandle parent)
	{
		std::vector<EntityHandle> children;

		// only gather valid, direct children of the parent
		for (Entity& entity : m_sceneEntities)
		{
			if (!entity.IsValid())
				continue;

			if (entity.m_parent == parent)
				children.push_back(entity.m_handle);
		}

		return children;
	}

	std::vector<EntityHandle> SceneGraph::GetChildrenAllLevels(EntityHandle parent)
	{
		// get direct children so array is populated from the start.
		// this will allow us to run the loop with 0 as a starting index,
		// because 0 will be smaller than the array's size if there are children
		std::vector<EntityHandle> children = GetChildren(parent);	

		uint64 index = 0;

		// gather all children's children and their children, etc...
		// we have to look through the entire array each time becauuse child entities
		// are not guaranteed to be after parent in memory
		while(index < children.size())
		{
			for (Entity& entity : m_sceneEntities)
			{
				if (entity.m_parent == children[index])
					children.push_back(entity.m_handle);
			}

			++index;
		}

		return children;
	}

	EntityHandle SceneGraph::MakeHandle(EntityHandle index, EntityHandle uid)
	{
		// if either half is over 32 bits, the handle is invalid
		if (index >= ULONG_MAX || uid >= ULONG_MAX)
			return Entity::INVALID_HANDLE;

		index |= (uid << 32);
		return index;
	}

	void SceneGraph::ReparentEntity(Entity* toReparent, EntityHandle newParent)
	{
		// Entity should not be its own parent
		if (newParent == toReparent->m_handle)
			return;

		toReparent->m_parent = newParent;

		if (newParent == Entity::INVALID_HANDLE)
			return;

		EntityHandle parentIndex = (newParent & Entity::INDEX_MASK);
		EntityHandle toReparentIndex = (toReparent->m_handle & Entity::INDEX_MASK);

		if (toReparentIndex < parentIndex)
		{
			printf("[Scene graph]: moving child to back of array\n");

			// no need to move the reparented component's children if it hasn't moved itself
			bool transformMoved = m_sceneTransforms.MoveReparentedComponent(toReparent->m_handle, newParent);

			std::vector<EntityHandle> allChildren = GetChildrenAllLevels(toReparent->m_handle);

			for (EntityHandle child : allChildren)
			{
				if (transformMoved)
					m_sceneTransforms.MoveReparentedComponent(child);
			}
		}

		else
			printf("[Scene graph]: reparent complete with no layout changes\n");

	}

	SceneGraph::Random::Random(std::random_device randomDevice)
		: m_generator(randomDevice()), m_distribution(LONG_MIN, LONG_MAX)
	{

	}

	int64 SceneGraph::Random::Generate()
	{
		return m_distribution(m_generator);
	}

}