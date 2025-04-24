#pragma once

#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

#include "engine/EngineExport.h"
#include "TypesECS.h"
#include "Component.h"
#include "Entity.h"
#include "ComponentArray.h"

#include "components/Script.h"
#include "components/Transform.h"
#include "components/Camera.h"
#include "components/Renderer.h"

#include "engine/physics/rigidbody/RigidBodyDynamic.h"
#include "engine/physics/rigidbody/RigidBodyStatic.h"

namespace engine
{
    class SceneGraph
    {

    private:

        using HandleMap = std::unordered_map<EntityHandle, EntityHandle>;

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

        // Component arrays to be copied to render in parallel with a
        // game logic tick
        struct ComponentCache
        {
            CopyableComponentArray<Transform>			m_transformRenderCache;
            CopyableComponentArray<Camera>				m_cameraRenderCache;
        };


        using EntityNameMap = std::unordered_map<uint64, EntityHandle>;
        

    public:

        ENGINE_API SceneGraph(void) = default;
        ENGINE_API SceneGraph(const SceneGraph&) = delete;
        ENGINE_API SceneGraph(SceneGraph&&) noexcept = default;
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


        // Tick all components of a given type, provided that this typed
        // has its UpdateComponent trait set to true
        template <CValidComponent TComponentType, typename... TVariadicArgs>
        void UpdateComponents(TVariadicArgs&&... args);

        void StartAllScripts(void);
        void SyncTransformsPostPhysics(void);
        void SyncRigidbodiesPrePhysics(void);

        // Re-register all existing components after a lua state reset
        ENGINE_API
        void RegisterAllComponents(void);

        ENGINE_API
        void RegisterAllEntities(void);

        // Render all active renderers with all active cameras.
        // This function will use the arrays populated with CacheComponents(),
        // and not the active transform array
        ENGINE_API
        void RenderFromCache(void);

        // Copy all data from transform component array to a separate cache
        // This function is used after the game logic update, allowing to start the next
        // gameplay tick before rendering on the main thread
        ENGINE_API
        void CacheComponents(void);

        // Clear transforms and cameras in render cache
        ENGINE_API
        void ClearCache(void);

        // Serialize all valid entities and recalculate their handles
        // The handles are recalculated as invalid entities are filtered out,
        // potentially leaving empty spots to be filled by valid entities.
        // These new handles are only applied upon next deserialization to avoid breaking
        // existing references
        ENGINE_API
        void SerializeText(std::ofstream& file);

        // Read from a scene file
        ENGINE_API
        void DeserializeText(std::ifstream& file);

        ENGINE_API
        SceneGraph& operator=(const SceneGraph&) = default;

        ENGINE_API
        SceneGraph& operator=(SceneGraph&&) noexcept = default;

        // Output a int64 between LONG_MIN and LONG_MAX
        ENGINE_API
        static int64 RandomNumber(void);

    private:

        // Get the component array corresponding to a type
        template <CValidComponent TComponentType>
        ComponentArray<TComponentType>& GetComponentArray(void);

        // Get the component array corresponding to a type
        template <CValidComponent TComponentType>
        const ComponentArray<TComponentType>& GetComponentArray(void) const;

        // Group the index and uid bits together in a 64-bit handle
        EntityHandle MakeHandle(int32 index, int32 uid);
        int32 GetHandleVersion(EntityHandle handle);
        int32 GetHandleIndex(EntityHandle handle);

        // Internal ReparentEntity overload. Directly uses an entity's pointer
        void	ReparentEntity(Entity* toReparent, EntityHandle newParent);


        // Check if components need to be moved after reparenting an entity, and move them if so
        void MoveReparentedComponents(EntityHandle reparented, EntityHandle newParent);

        // Register all valid components of a type
        template <CValidComponent TComponentType>
        void RegisterComponents(void);

        // Serialize all components of a type
        template <CValidComponent TComponentType>
        void SerializeComponents(std::ostream& file, HandleMap& handles);

        template <CValidComponent TComponentType>
        void SerializeSingleComponent(std::ostream& file,
                                      const Entity& entity,
                                      HandleMap& handles) const;

        const char* DeserializeEntityText(const char* text, const char* end);

        template <typename... TVariadicArgs>
        void ReorderDeserializedTextArrays(TVariadicArgs&... args);

        template <CValidComponent TComponentType>
        void ReorderTextArray(Component::DeserializedArray<TComponentType>& array);


        // All transform components in the scene
        CopyableComponentArray<Transform>			m_sceneTransforms;

        // All script components in the scene
        ComponentArray<Script>				m_sceneScripts;

        // All cameras components in the scene
        CopyableComponentArray<Camera>				m_sceneCameras;

        // All renderer components in the scene
        ComponentArray<Renderer>			m_sceneRenderers;

        ComponentArray<RigidBodyDynamic>    m_sceneDynamicRigidBodies;

        ComponentArray<RigidBodyStatic>     m_sceneStaticRigidBodies;

         

        // All entities in the scene
        std::vector<Entity>					m_sceneEntities;

        ComponentCache						m_renderCache;

        // Random uint64 generator. We only need a unique instance
        static thread_local Random			m_randomNumGen;
    };




    template <CValidComponent TComponentType, typename... TVariadicArgs>
    inline void SceneGraph::UpdateComponents(TVariadicArgs&&... args)
    {
        if constexpr (!UpdateComponent<TComponentType>::m_value)
            return;

        ComponentArray<TComponentType>& array = GetComponentArray<TComponentType>();

        for (TComponentType& component : array)
        {
            if (component.IsValid() && component.IsActive())
                component.Update(std::forward<TVariadicArgs>(args)...);
        }
    }

    template<>
    inline ComponentArray<Transform>& SceneGraph::GetComponentArray<Transform>(void)
    {
        return m_sceneTransforms;
    }


    template<>
    inline ComponentArray<Camera>& SceneGraph::GetComponentArray<Camera>(void)
    {
        return m_sceneCameras;
    }

    template<>
    inline ComponentArray<Renderer>& SceneGraph::GetComponentArray<Renderer>(void)
    {
        return m_sceneRenderers;
    }

    template<>
    inline ComponentArray<RigidBodyDynamic>& SceneGraph::GetComponentArray<RigidBodyDynamic>(void)
    {
        return m_sceneDynamicRigidBodies;
    }

    template<>
    inline ComponentArray<RigidBodyStatic>& SceneGraph::GetComponentArray<RigidBodyStatic>(void)
    {
        return m_sceneStaticRigidBodies;
    }


    template<> inline
    const ComponentArray<Transform>& SceneGraph::GetComponentArray<Transform>(void) const
    {
        return m_sceneTransforms;
    }


    template<> inline
    const ComponentArray<Camera>& SceneGraph::GetComponentArray<Camera>(void) const
    {
        return m_sceneCameras;
    }

    template<> inline
    const ComponentArray<Renderer>& SceneGraph::GetComponentArray<Renderer>(void) const
    {
        return m_sceneRenderers;
    }

    template<> inline
    const ComponentArray<RigidBodyDynamic>& SceneGraph::GetComponentArray<RigidBodyDynamic>(void) const
    {
        return m_sceneDynamicRigidBodies;
    }
    
    template<> inline
    const ComponentArray<RigidBodyStatic>& SceneGraph::GetComponentArray<RigidBodyStatic>(void) const
    {
        return m_sceneStaticRigidBodies;
    }

    template<CValidComponent TComponentType>
    inline void SceneGraph::RegisterComponents(void)
    {
        ComponentArray<TComponentType>& array = GetComponentArray<TComponentType>();

        for (TComponentType& component : array)
        {
            if (component.IsValid())
                component.Register();
        }

    }

    template<>
    inline ComponentArray<Script>& SceneGraph::GetComponentArray<Script>(void)
    {
        return m_sceneScripts;
    }

    template<> inline
    const ComponentArray<Script>& SceneGraph::GetComponentArray<Script>(void) const
    {
        return m_sceneScripts;
    }



    template<CValidComponent TComponentType>
    inline TComponentType* SceneGraph::GetComponent(EntityHandle ownerEntity)
    {
        ComponentArray<TComponentType>& array = GetComponentArray<TComponentType>();

        return array.GetComponent(ownerEntity);
    }


    template<CValidComponent TComponentType>
    inline void SceneGraph::SerializeComponents(std::ostream& file, HandleMap& handles)
    {
        ComponentArray<TComponentType>& array = GetComponentArray<TComponentType>();

        for (const TComponentType& component : array)
        {
            uint64 index = array.GetComponentIndex(component.GetOwner());
            component.SerializeText(file, handles[component.GetOwner()], index);
        }
    }

    template<CValidComponent TComponentType>
    inline void SceneGraph::SerializeSingleComponent(std::ostream& file,
                                                     const Entity& entity,
                                                     HandleMap& handles) const
    {
        const ComponentArray<TComponentType>& array = GetComponentArray<TComponentType>();

        if (const TComponentType* component = array.GetComponent(entity.m_handle))
        {
            uint64 index = array.GetComponentIndex(entity.m_handle);
            component->SerializeText(file, handles[entity.m_handle], index);
        }

    }

    template<typename ...TVariadicArgs>
    inline void SceneGraph::ReorderDeserializedTextArrays(TVariadicArgs& ...args)
    {
        (ReorderTextArray(args), ...);
    }

    template<CValidComponent TComponentType>
    inline void SceneGraph::ReorderTextArray(Component::DeserializedArray<TComponentType>& array)
    {
        using CompIndex = Component::IndexedComponent<TComponentType>;

        if constexpr (UpdateAfterParent<TComponentType>::m_value)
        {
            std::sort(array.begin(), array.end(),
            [](const CompIndex& lhs, const CompIndex& rhs) -> bool {return lhs.first < rhs.first; });
        }

        ComponentArray<TComponentType>& compArray = GetComponentArray<TComponentType>();

        for (CompIndex& component : array)
        {
            component.second.m_currentScene = this;
            compArray.AddDeserializedComponent(std::move(component.second));
        }
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

        TComponentType* newComponent = array.CreateComponent(ownerEntity, ownerPtr->m_handle, this);

        newComponent->Register();
        return newComponent;
    }

}