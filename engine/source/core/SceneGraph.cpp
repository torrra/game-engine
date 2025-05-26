#include "core/SceneGraph.h"
#include "core/SceneGraph.h"
#include "core/Entity.h"
#include "core/systems/ScriptSystem.h"

#include "serialization/TextSerializer.h"
#include "thread/ThreadManager.h"
#include "InternalOpenGLError.hpp"
#include "resource/model/Buffer.h"
#include "physics/Raycast.h"

namespace engine
{
    thread_local SceneGraph::Random	SceneGraph::m_randomNumGen = Random(std::random_device());

    void SceneGraph::StartAllScripts(void)
    {
        for (Script& script : m_sceneScripts)
        {
            if (script.IsValid())
                script.Start();
        }
    }

    void SceneGraph::SyncTransformsPostPhysics(void)
    {
        for (RigidBodyDynamic& rigidbody : m_sceneDynamicRigidBodies)
        {
            Entity* entity = GetEntity(rigidbody.GetOwner());

            if (entity && entity->IsActive())
                rigidbody.UpdateEntity();
        }

        for (RigidBodyStatic& rigidbody : m_sceneStaticRigidBodies)
        {
            Entity* entity = GetEntity(rigidbody.GetOwner());

            if (entity && entity->IsActive())
                rigidbody.UpdateEntity();
        }

        for (TriangleMesh& mesh : m_sceneTriangleMeshes)
        {
            Entity* entity = GetEntity(mesh.GetOwner());

            if (entity && entity->IsActive())
                mesh.UpdateEntity();
        }
    }

    void SceneGraph::SyncRigidbodiesPrePhysics(void)
    {
        for (RigidBodyDynamic& rigidbody : m_sceneDynamicRigidBodies)
        {
            Entity* entity = GetEntity(rigidbody.GetOwner());

            if (entity && entity->IsActive())
                rigidbody.UpdateRigidBody();
        }

        for (RigidBodyStatic& rigidbody : m_sceneStaticRigidBodies)
        {
            Entity* entity = GetEntity(rigidbody.GetOwner());

            if (entity && entity->IsActive())
                rigidbody.UpdateRigidBody();
        }

        for (TriangleMesh& mesh : m_sceneTriangleMeshes)
        {
            Entity* entity = GetEntity(mesh.GetOwner());

            if (entity && entity->IsActive())
                mesh.UpdateTriangleMesh();
        }
    }

    void SceneGraph::RegisterAllComponents(void)
    {
        RegisterComponents<Transform>();
        RegisterComponents<Script>();
        RegisterComponents<Camera>();
        RegisterComponents<RigidBodyDynamic>();
        RegisterComponents<RigidBodyStatic>();
        RegisterComponents<TriangleMesh>();
        RegisterComponents<AudioPlayer>();
        RegisterComponents<NavigationPoint>();
        RegisterComponents<LightSource>();
    }

    void SceneGraph::RegisterAllEntities(void)
    {
        for (Entity& entity : m_sceneEntities)
        {
            if (entity.IsValid())
                ScriptSystem::RegisterNewEntity(entity.m_handle, GetFullEntityName(entity.m_handle));
        }
    }

    int64 SceneGraph::RandomNumber(void)
    {
        return m_randomNumGen.Generate();
    }

    EntityHandle SceneGraph::CreateEntity(const std::string& name, EntityHandle parent)
    {
        int32 newIndex = 0;

        int32 entityCount = static_cast<int32>(m_sceneEntities.size());

        for (; newIndex < entityCount; ++newIndex)
        {
            Entity& currentEntity = m_sceneEntities[newIndex];

            if (!currentEntity.IsValid())
            {
                EntityHandle newHandle = MakeHandle(newIndex, GetHandleVersion(currentEntity.m_handle) + 1);

                // write over dead entity to avoid reallocation
                currentEntity = Entity(name, newHandle, parent);
                ScriptSystem::RegisterNewEntity(newHandle, GetFullEntityName(newHandle));
                return currentEntity.m_handle;
            }
        }

        // create entity in new slot in array
        EntityHandle newHandle = MakeHandle(newIndex, 0);

        if (newHandle != Entity::INVALID_HANDLE)
        {
            m_sceneEntities.emplace_back(name, newHandle, parent);
            ScriptSystem::RegisterNewEntity(newHandle, GetFullEntityName(newHandle));
        }

        // TODO: log error?
        //else
            //printf("[Scene graph]: no entity created\n");

        return newHandle;
    }


    Entity* SceneGraph::GetEntity(EntityHandle handle)
    {
        if (handle == Entity::INVALID_HANDLE)
            return nullptr;

        int32 index = GetHandleIndex(handle);

        // out of bounds
        if (index >= static_cast<int32>(m_sceneEntities.size()))
            return nullptr;

        Entity& entity = m_sceneEntities[index];

        // requested object was written over and no longer exists
        if (handle != entity.m_handle)
            return nullptr;

        // object is dead and will be written over
        if (!entity.IsValid())
            return nullptr;

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
            m_sceneCameras.InvalidateComponent(entity);
            m_sceneRenderers.InvalidateComponent(entity);
            m_sceneScripts.InvalidateComponent(entity);
            m_sceneDynamicRigidBodies.InvalidateComponent(entity);
            m_sceneStaticRigidBodies.InvalidateComponent(entity);
            m_sceneTriangleMeshes.InvalidateComponent(entity);
            m_sceneAudioPlayer.InvalidateComponent(entity);
            m_sceneNavigationPoints.InvalidateComponent(entity);
            m_sceneLights.InvalidateComponent(entity);

            ScriptSystem::UnregisterEntity(entity);
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
        // we have to look through the entire array each time because child entities
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


    void SceneGraph::RenderFromCache(void)
    {
        for (Camera& camera : m_renderCache.m_cameraRenderCache)
        {
            if (!camera.IsValid() || !camera.IsActive())
                continue;

            math::Matrix4f viewProjection = camera.ViewProjection();

            RenderFromCacheSingleCamera(viewProjection);
        }
    }

    Transform* SceneGraph::GetCachedTransform(EntityHandle owner)
    {
        return m_renderCache.m_transformRenderCache.GetComponent(owner);
    }

    void SceneGraph::UpdateSceneLights(Buffer& omniBuffer,
                                       Buffer& directionalBuffer, Buffer& spotBuffer)
    {

        UpdateLightCount(omniBuffer, directionalBuffer, spotBuffer);

        uint32 omniBufferOffset = sizeof(GLuint);
        uint32 directionalBufferOffset = sizeof(GLuint);
        uint32 spotBufferOffset = sizeof(GLuint);

        for (const LightSource& light : m_renderCache.m_lightRenderCache)
        {
            if (!light.IsValid() || !light.IsActive())
                continue;
            
            switch (light.GetType())
            {
            case LightSource::ELightType::OMNIDIRECTIONAL:
                light.AddOmniToBuffer(omniBuffer, omniBufferOffset);
                omniBufferOffset += LightSource::GetOmniSize();
                break;

            case LightSource::ELightType::DIRECTIONAL:
                light.AddDirectionalToBuffer(directionalBuffer, directionalBufferOffset);
                directionalBufferOffset += LightSource::GetDirectionalSize();
                break;

            case LightSource::ELightType::SPOTLIGHT:
                light.AddSpotToBuffer(spotBuffer, spotBufferOffset);
                spotBufferOffset += LightSource::GetSpotSize();
                break;

            default:
                break;
            }
            
        }
    }

    const ComponentArray<LightSource>& SceneGraph::GetCachedLights(void) const
    {
        return m_renderCache.m_lightRenderCache;
    }

    void SceneGraph::RenderFromCacheSingleCamera(const math::Matrix4f& viewProjection)
    {
        for (Renderer& renderer : m_sceneRenderers)
        {
            if (!renderer.IsValid() || !renderer.IsActive())
                continue;

            renderer.Render(viewProjection, m_renderCache.m_transformRenderCache);
            OpenGLError();
        }
    }

    void SceneGraph::CacheComponents(void)
    {
        m_renderCache.m_cameraRenderCache = m_sceneCameras;
        m_renderCache.m_transformRenderCache = m_sceneTransforms;
        m_renderCache.m_lightRenderCache = m_sceneLights;
    }

    void SceneGraph::ClearCache(void)
    {
        m_renderCache.m_cameraRenderCache = CopyableComponentArray<Camera>();
        m_renderCache.m_transformRenderCache = CopyableComponentArray<Transform>();
    }

    EntityHandle SceneGraph::MakeHandle(int32 index, int32 uid)
    {
        // if either half is over 32 bits, the handle is invalid
        if (index >= LONG_MAX || uid >= LONG_MAX)
            return Entity::INVALID_HANDLE;

        if (index <= LONG_MIN || uid <= LONG_MIN)
            return Entity::INVALID_HANDLE;

        return static_cast<EntityHandle>(index) | (static_cast<EntityHandle>(uid) << 32);
    }

    int32 SceneGraph::GetHandleVersion(EntityHandle handle)
    {
        return static_cast<int32>((handle & Entity::UID_MASK) >> 32);
    }

    int32 SceneGraph::GetHandleIndex(EntityHandle handle)
    {
        return static_cast<int32>(handle & Entity::INDEX_MASK);
    }

    void SceneGraph::ReparentEntity(Entity* toReparent, EntityHandle newParent)
    {
        // Entity should not be its own parent
        if (newParent == toReparent->m_handle)
            return;

        toReparent->m_parent = newParent;

        if (newParent == Entity::INVALID_HANDLE)
            return;

        int32 parentIndex = GetHandleIndex(newParent);
        int32 toReparentIndex = GetHandleIndex(toReparent->m_handle);

        if (toReparentIndex < parentIndex)
            MoveReparentedComponents(toReparent->m_handle, newParent);			
    }

    void SceneGraph::MoveReparentedComponents(EntityHandle reparented, EntityHandle newParent)
    {
        m_sceneTransforms.MoveReparentedComponent(reparented, newParent);
        m_sceneScripts.MoveReparentedComponent(reparented, newParent);
        m_sceneCameras.MoveReparentedComponent(reparented, newParent);
        m_sceneScripts.MoveReparentedComponent(reparented, newParent);
        m_sceneDynamicRigidBodies.MoveReparentedComponent(reparented, newParent);
        m_sceneStaticRigidBodies.MoveReparentedComponent(reparented, newParent);
        m_sceneTriangleMeshes.MoveReparentedComponent(reparented, newParent);
        m_sceneAudioPlayer.MoveReparentedComponent(reparented, newParent);
        m_sceneNavigationPoints.MoveReparentedComponent(reparented, newParent);
        m_sceneLights.MoveReparentedComponent(reparented, newParent);

        std::vector<EntityHandle> allChildren = GetChildrenAllLevels(reparented);

        for (EntityHandle child : allChildren)
        {
           m_sceneTransforms.MoveReparentedComponent(child);
           m_sceneScripts.MoveReparentedComponent(child);
           m_sceneCameras.MoveReparentedComponent(child);
           m_sceneRenderers.MoveReparentedComponent(child);
           m_sceneDynamicRigidBodies.MoveReparentedComponent(child);
           m_sceneStaticRigidBodies.MoveReparentedComponent(child);
           m_sceneTriangleMeshes.MoveReparentedComponent(child);
           m_sceneAudioPlayer.MoveReparentedComponent(child);
           m_sceneNavigationPoints.MoveReparentedComponent(child);
           m_sceneLights.MoveReparentedComponent(child);
        }
    }

    SceneGraph::Random::Random(std::random_device randomDevice)
        : m_generator(randomDevice()), m_distribution(LONG_MIN, LONG_MAX)
    {

    }

    int64 SceneGraph::Random::Generate()
    {
        return m_distribution(m_generator);
    }
    
    const char* SceneGraph::DeserializeEntityText(const char* text, const char* end)
    {    
        return m_sceneEntities.emplace_back().DeserializeText(text, end);
    }

    void SceneGraph::UpdateLightCount(Buffer& omniBuffer, Buffer& directionalBuffer,
                                      Buffer& spotBuffer)
    {

        uint32 omniCount = 0;
        uint32 directionalCount = 0;
        uint32 spotCount = 0;

        for (const LightSource& light : m_renderCache.m_lightRenderCache)
        {
            if (!light.IsValid() || !light.IsActive())
                continue;

            switch (light.GetType())
            {
            case LightSource::ELightType::OMNIDIRECTIONAL:
                ++omniCount;
                break;

            case LightSource::ELightType::DIRECTIONAL:
                ++directionalCount;
                break;

            case LightSource::ELightType::SPOTLIGHT:
                ++spotCount;
                break;

            default:
                break;
            }
        }

        uint32 omniBufSize = LightSource::GetOmniSize() * omniCount;
        omniBuffer.SetData(0, omniBufSize + sizeof(uint32));
        omniBuffer.SetData(&omniCount, sizeof(uint32), 0);

        uint32 dirBufSize = LightSource::GetDirectionalSize() * directionalCount;
        directionalBuffer.SetData(0, dirBufSize + sizeof(uint32));
        directionalBuffer.SetData(&directionalCount, sizeof(uint32), 0);

        uint32 spotBufSize = LightSource::GetSpotSize() * spotCount;
        spotBuffer.SetData(0, sizeof(uint32) + spotBufSize);
        spotBuffer.SetData(&spotCount, sizeof(uint32), 0);
    }

    void SceneGraph::CleanRigidBodies(void)
    {
        Raycast::CleanupRays();
        for (RigidBodyDynamic& rbDynamic : m_sceneDynamicRigidBodies)
        {
            rbDynamic.RigidBodyDynamicCleanUp();
        }
        for (RigidBodyStatic& rbStatic : m_sceneStaticRigidBodies)
        {
            rbStatic.RigidBodyStaticCleanUp();
        }
        for (TriangleMesh& triangleMesh : m_sceneTriangleMeshes)
        {
            triangleMesh.CleanUpTriangleMesh();
        }
        m_sceneDynamicRigidBodies = ComponentArray<RigidBodyDynamic>();
        m_sceneStaticRigidBodies = ComponentArray<RigidBodyStatic>();
        m_sceneTriangleMeshes = ComponentArray<TriangleMesh>();
    }


    void SceneGraph::SerializeText(std::ofstream& file)
    {
        std::vector<Entity> validEntities;
        HandleMap			handles;

        int32 index = 0;
        for (const Entity& entity : m_sceneEntities)
        {
            if (!entity.IsValid())
                continue;

            EntityHandle newHandle = MakeHandle(index, GetHandleVersion(entity.m_handle));

            handles[entity.m_handle] = newHandle;
            validEntities.push_back(entity);
            ++index;
        }

        for (Entity& entity : validEntities)
        {
            entity.m_handle = handles[entity.m_handle];

            auto parentHandle = handles.find(entity.m_parent);

            if (parentHandle != handles.end())
                entity.m_parent = parentHandle->second;
            else
                entity.m_parent = -1;

            entity.SerializeText(file);
            SerializeSingleComponent<Transform>(file, entity, handles);
            SerializeSingleComponent<Camera>(file, entity, handles);
            SerializeSingleComponent<Renderer>(file, entity, handles);
            SerializeSingleComponent<Script>(file, entity, handles);
            SerializeSingleComponent<RigidBodyDynamic>(file, entity, handles);
            SerializeSingleComponent<RigidBodyStatic>(file, entity, handles);
            SerializeSingleComponent<TriangleMesh>(file, entity, handles);
            SerializeSingleComponent<AudioPlayer>(file, entity, handles);
            SerializeSingleComponent<NavigationPoint>(file, entity, handles);
            SerializeSingleComponent<LightSource>(file, entity, handles);
        }
    }

    void SceneGraph::DeserializeText(std::ifstream& file)
    {
        Component::DeserializedArray<Transform> transforms;
        Component::DeserializedArray<Camera>	cameras;
        Component::DeserializedArray<Renderer>	renderers;
        Component::DeserializedArray<Script>	scripts;
        Component::DeserializedArray<RigidBodyDynamic>	dynamicRigidBodies;
        Component::DeserializedArray<RigidBodyStatic>	staticRigidBodies;
        Component::DeserializedArray<TriangleMesh>	triangleMeshes;
        Component::DeserializedArray<AudioPlayer>	audioPlayers;
        Component::DeserializedArray<NavigationPoint> navigationPoint;
        Component::DeserializedArray<LightSource>	lights;

        const char* start;
        const char* end;
        const char* data = text::LoadFileData(file, start, end);

        while (start != end)
        {
            if (memcmp(start, "[Entity]", 8) == 0)
                start = DeserializeEntityText(start, end);

            else if (memcmp(start, "[Transform]", 11) == 0)
                start = Component::DeserializeComponentText(transforms, start, end);

            else if (memcmp(start, "[Camera]", 8) == 0)
                start = Component::DeserializeComponentText(cameras, start, end);

            else if (memcmp(start, "[Renderer]", 10) == 0)
                start = Component::DeserializeComponentText(renderers, start, end);

            else if (memcmp(start, "[Script]", 8) == 0)
                start = Component::DeserializeComponentText(scripts, start, end);

            else if (memcmp(start, "[RigidBodyDynamic]", 17) == 0)
                start = Component::DeserializeComponentText(dynamicRigidBodies, start, end);

            else if (memcmp(start, "[RigidBodyStatic]", 16) == 0)
                start = Component::DeserializeComponentText(staticRigidBodies, start, end);

            else if (memcmp(start, "[TriangleMesh]", 14) == 0)
                start = Component::DeserializeComponentText(triangleMeshes, start, end);

            else if (memcmp(start, "[AudioPlayer]", 13) == 0)
                start = Component::DeserializeComponentText(audioPlayers, start, end);
            
            else if (memcmp(start, "[NavigationPoint]", 17) == 0)
                start = Component::DeserializeComponentText(navigationPoint, start, end);


            else if (memcmp(start, "[LightSource]", 13) == 0)
                start = Component::DeserializeComponentText(lights, start, end);

            start = text::GetNewLine(start, end);
        }

        ReorderDeserializedTextArrays(transforms, cameras, renderers, scripts, dynamicRigidBodies, 
                                      staticRigidBodies, triangleMeshes, audioPlayers, 
                                      navigationPoint, lights);
        text::UnloadFileData(data);
        for (RigidBodyDynamic& rbDynamic : m_sceneDynamicRigidBodies)
        {
            rbDynamic.SwitchShape(static_cast<EGeometryType>(rbDynamic.m_rigidBodyShape));
        }
        for (RigidBodyStatic& rbStatic : m_sceneStaticRigidBodies)
        {
            rbStatic.SwitchShape(static_cast<EGeometryType>(rbStatic.m_rigidBodyShape));
        }
    }

}