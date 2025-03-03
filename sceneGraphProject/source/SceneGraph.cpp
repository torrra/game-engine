#include "SceneGraph.h"
#include "Entity.h"

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

		for (; newIndex < m_sceneEntities.size(); ++newIndex)
		{
			Entity& currentEntity = m_sceneEntities[newIndex];

			if (!currentEntity.IsValid())
			{
				printf("[Scene graph]: filling invalid slot\n");

				currentEntity = Entity(name, MakeHandle(newIndex, newUID), parent);

				return currentEntity.m_handle;
			}
		}

		printf("[Scene graph]: creating new slot\n");

		EntityHandle newHandle = MakeHandle(newIndex, newUID);

		if (newHandle != Entity::INVALID_HANDLE)
			m_sceneEntities.emplace_back(name, newHandle, parent);
		else
			printf("[Scene graph]: no entity created\n");

		return newHandle;
	}

	Entity* SceneGraph::GetEntity(EntityHandle& handle)
	{
		Entity* foundEntity = GetEntityUnchecked(handle);

		if (foundEntity)
			return foundEntity;

		EntityHandle uid = handle & Entity::UID_MASK;

		for (Entity& currentEntity : m_sceneEntities)
		{
			if (!currentEntity.IsValid())
				continue;

			if (uid == (currentEntity.m_handle & Entity::UID_MASK))
			{
				handle = currentEntity.m_handle;
				printf("[Scene graph]: found entity elsewhere, handle updated\n");
				return &currentEntity;
			}
		}

		printf("[Scene graph]: entity not found\n");
		return nullptr;
	}

	Entity* SceneGraph::GetEntityUnchecked(EntityHandle handle)
	{
		if (handle == Entity::INVALID_HANDLE)
			return nullptr;

		EntityHandle index = (handle & Entity::INDEX_MASK);

		if (index >= m_sceneEntities.size())
			return nullptr;

		Entity& entity = m_sceneEntities[index];
		EntityHandle uid = (handle & Entity::UID_MASK);

		if (uid != (entity.m_handle & Entity::UID_MASK))
			return nullptr;

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

	void SceneGraph::DestroyEntity(EntityHandle entity)
	{
		if (Entity* entityPtr = GetEntity(entity))
		{
			if (entityPtr->m_statusFlags & Entity::TRANSFORM)
				m_sceneTransforms.InvalidateComponent(entity);

			entityPtr->Invalidate();
		}
	}

	std::vector<EntityHandle> SceneGraph::GetChildren(EntityHandle parent)
	{
		std::vector<EntityHandle> children;

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
		std::vector<EntityHandle> children = GetChildren(parent);	

		uint64 index = 0;

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
		if (index >= ULONG_MAX || uid >= ULONG_MAX)
			return Entity::INVALID_HANDLE;

		index |= (uid << 32);
		return index;
	}



	EntityHandle SceneGraph::ReparentEntity(Entity& toReparent, EntityHandle newParent, uint64 basePos)
	{
		if (newParent == Entity::INVALID_HANDLE || newParent == toReparent.m_handle)
			return toReparent.m_handle;

		EntityHandle parentIndex = (newParent & Entity::INDEX_MASK);
		EntityHandle toReparentIndex = (toReparent.m_handle & Entity::INDEX_MASK);

		if (toReparentIndex >= parentIndex)
		{
			printf("[Scene graph]: reparent complete with no layout changes\n");

			return toReparent.m_handle;
		}
		else
		{
			printf("[Scene graph]: moving child to back of array\n");

			m_sceneTransforms.MoveReparentedComponent(toReparent.m_handle);

			return toReparent.m_handle;
		}

		toReparent.m_parent = newParent;
	}

	void SceneGraph::ReparentEntity(Entity& toReparent, EntityHandle newParent)
	{
		if (newParent == Entity::INVALID_HANDLE || newParent == toReparent.m_handle)
			return;

		EntityHandle parentIndex = (newParent & Entity::INDEX_MASK);
		EntityHandle toReparentIndex = (toReparent.m_handle & Entity::INDEX_MASK);

		if (toReparentIndex < parentIndex)
		{
			printf("[Scene graph]: moving child to back of array\n");
			m_sceneTransforms.MoveReparentedComponent(toReparent.m_handle);

			std::vector<EntityHandle> allChildren = GetChildrenAllLevels(toReparent.m_handle);

			for (EntityHandle child : allChildren)
			{
				m_sceneTransforms.MoveReparentedComponent(child);
			}
		}

		else
			printf("[Scene graph]: reparent complete with no layout changes\n");

		toReparent.m_parent = newParent;
	}

	EntityHandle SceneGraph::MoveEntityToBack(Entity& toMove, EntityHandle newParent)
	{
		toMove.Invalidate();

		EntityHandle uid = toMove.m_handle & Entity::UID_MASK;
		EntityHandle newHandle = MakeHandle(m_sceneEntities.size(), uid);

		Entity& reparentedEntity = m_sceneEntities.emplace_back(toMove);
		reparentedEntity.m_parent = newParent;
		reparentedEntity.m_handle = newHandle;
		reparentedEntity.m_statusFlags &= (~ecs::INVALID_OBJECT);

		return newHandle;
	}

	void SceneGraph::AddChildrenToArray(std::vector<EntityHandle>& array, EntityHandle parent)
	{
		for (Entity& entity : m_sceneEntities)
		{
			if (!entity.IsValid())
				continue;

			if (entity.m_parent == parent)
				array.push_back(entity.m_handle);
		}
	}

	SceneGraph::Random::Random(std::random_device randomDevice)
		: m_generator(randomDevice()), m_distribution(0, ULONG_MAX)
	{

	}

	uint64 SceneGraph::Random::Generate()
	{
		return m_distribution(m_generator);
	}

}