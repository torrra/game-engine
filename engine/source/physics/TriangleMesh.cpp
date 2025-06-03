#include "engine/physics/TriangleMesh.h"

#pragma region Resource

#include "engine/resource/model/Model.h"

#pragma endregion

#pragma region Core

#include "core/SceneGraph.h"

#pragma endregion

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Serialization

#include "serialization/TextSerializer.h"

#pragma endregion

#include "engine/ConsoleLog.hpp"

engine::TriangleMesh::TriangleMesh(EntityHandle inOwner, class SceneGraph* inScene)
{
    // Initialize the triangle mesh implementation
    m_triangleMeshImpl  = new TriangleMeshImpl();

    m_data.m_index = 0;
    m_data.m_type = EShapeType::TRIANGLE;

    // Set the owner and the current scene
    m_owner             = inOwner;
    m_currentScene      = inScene;

    
}

void engine::TriangleMesh::CreateTriangleMesh(void)
{
    if (m_triangleMeshImpl == nullptr)
    {
        // Initialize the triangle mesh implementation
        m_triangleMeshImpl = new TriangleMeshImpl();

        m_data.m_index = 0;
        m_data.m_type = EShapeType::TRIANGLE;
    }
    // Get the model and return if failed
    if (engine::Renderer* renderer = m_currentScene->GetComponent<engine::Renderer>(m_owner))
        m_model = renderer->GetModel();

    else
        return;

    if (m_model == nullptr)
        return;

    /*
        Create the triangle mesh description
        points.count : The number of points
        points.stride : The stride of the points
        points.data : The data of the points
    */
    physx::PxTriangleMeshDesc meshDesc;
    if (m_model->CanRender() && !m_isDrawn)
    {
        m_triangleMeshImpl->m_triangleMeshDesc.points.count = 
            m_model->GetStaticMeshes()[0].GetIndexCount();
        m_triangleMeshImpl->m_triangleMeshDesc.points.stride = sizeof(math::Vector3f);
        m_triangleMeshImpl->m_triangleMeshDesc.points.data =
            m_model->GetStaticMeshes()[0].GetVertices();

        CookTriangleMesh();

        m_isDrawn = true;
    }
}

void engine::TriangleMesh::SerializeText(std::ostream& output, EntityHandle owner, uint64 index) const
{
    output << "[TriangleMesh]\n     ";

    if constexpr (UpdateAfterParent<TriangleMesh>::m_value)
    {
        text::Serialize(output, "index", index);
        output << "\n     ";
    }

    text::Serialize(output, "owner", owner);
    output << "\n     ";
    text::Serialize(output, "type", m_type);
    output << "\n     ";
    text::Serialize(output, "shape", m_shape);
    output << "\n     ";
    text::Serialize(output, "collision group", static_cast<uint32>(m_collisionGroup));
    output << "\n     ";
    text::Serialize(output, "flags", m_flags);
    output << '\n';
}

const char* engine::TriangleMesh::DeserializeText(const char* text, const char* end)
{
    MOVE_TEXT_CURSOR(text, end);
    text = text::DeserializeInteger(text, m_owner);

    MOVE_TEXT_CURSOR(text, end);
    text = text::DeserializeInteger(text, m_type);

    MOVE_TEXT_CURSOR(text, end);
    text = text::DeserializeInteger(text, m_shape);

    MOVE_TEXT_CURSOR(text, end);
    uint32 collisionGroup = 0;
    text = text::DeserializeInteger(text, collisionGroup);
    m_collisionGroup = static_cast<collision::ECollisionGroup>(collisionGroup);

    MOVE_TEXT_CURSOR(text, end);
    return text::DeserializeInteger(text, m_flags);
}

void engine::TriangleMesh::CleanUpTriangleMesh(void)
{   
    if (m_triangleMeshImpl != nullptr)
    {
        m_triangleMeshImpl->m_triangleMeshDesc.setToDefault();

        PhysicsEngine::Get().GetImpl().m_scene->removeActor(*m_triangleMeshImpl->m_actor);

        if (m_triangleMeshImpl->m_triangleMesh != nullptr)
        {
            // Release the triangle mesh
            PX_RELEASE(m_triangleMeshImpl->m_triangleMesh);
        }

        // Delete the triangle mesh implementation
        delete m_triangleMeshImpl;
        m_triangleMeshImpl = nullptr;
    }
}

void engine::TriangleMesh::UpdateEntity(void)
{
    if (m_triangleMeshImpl->m_actor != nullptr)
    {
        // Update the entity transform in regard to the rigid body
        if (Transform* transform = m_currentScene->GetComponent<Transform>(m_owner))
        {
            Transform updatedTransform = ToTransform(m_triangleMeshImpl->m_actor->getGlobalPose());

            transform->CopyPosition(updatedTransform);
            transform->CopyRotation(updatedTransform);
        }
    }
}

void engine::TriangleMesh::UpdateTriangleMesh(void)
{
    if (m_triangleMeshImpl->m_actor != nullptr)
    {
        Transform worldTransform;
        if (Transform* entityTransform = m_currentScene->GetComponent<Transform>(m_owner))
        {
            worldTransform.SetPosition(Transform::ToWorldPosition(*entityTransform));
            worldTransform.SetRotation(Transform::ToWorldRotation(*entityTransform));

            // Update the transform of the rigid body in regard to the entity
            m_triangleMeshImpl->m_actor->setGlobalPose(ToPxTransform(worldTransform));
        }
    }
}

void engine::TriangleMesh::CookTriangleMesh(void)
{
    /*
        Set the cooking parameters
        scale : The scale of the physics
    */
    physx::PxCookingParams params(PhysicsEngine::Get().GetImpl().m_physics->getTolerancesScale());

    // Default implementation of a memory write stream
    physx::PxDefaultMemoryOutputStream writeBuffer;
    // Result from triangle mesh cooking
    physx::PxTriangleMeshCookingResult::Enum result;

    // Cook the triangle mesh and return if failed
    bool status = PxCookTriangleMesh(params, m_triangleMeshImpl->m_triangleMeshDesc, writeBuffer, 
                                     &result);
    if (!status)
        return;

    // Default implementation of a memory read stream
    physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

    // Create the triangle mesh
    m_triangleMeshImpl->m_triangleMesh =
        PhysicsEngine::Get().GetImpl().m_physics->createTriangleMesh(readBuffer);

    // Create the material
    physx::PxMaterial* material = PhysicsEngine::Get().GetImpl().m_physics->createMaterial(0.5f,
                                                                                           0.5f,
                                                                                           0.6f);
    if (m_currentScene->GetEntity(m_owner)->HasComponent<engine::Transform>())
    {
        // Create the static rigid body
        m_triangleMeshImpl->m_actor =
            physx::PxCreateStatic(*PhysicsEngine::Get().GetImpl().m_physics,
                ToPxTransform(m_currentScene->GetComponent<engine::Transform>(
                    m_owner)->GetTransform()), physx::PxTriangleMeshGeometry(
                        m_triangleMeshImpl->m_triangleMesh), *material);
    }

    if (m_triangleMeshImpl->m_actor != nullptr)
    {
        // Set the visualization of the rigid body to false by default
        m_triangleMeshImpl->m_actor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

        m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
        m_data.m_type = EShapeType::TRIANGLE;
        void** dataPtr = reinterpret_cast<void**>(&m_data);
        m_triangleMeshImpl->m_actor->userData = *dataPtr;

        SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup),
            collision::GetCollisionMask(m_collisionGroup));

        // Add the actor to the physics scene
        PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_triangleMeshImpl->m_actor);
        m_shape = EGeometryType::TRIANGLE_MESH;
    }
}

void engine::TriangleMesh::SetCollisionGroupAndMask(uint32 inCollisionGroup, uint32 inCollisionMask)
{
    physx::PxFilterData filterData;
    filterData.word0 = inCollisionGroup;
    filterData.word1 = inCollisionMask;

    physx::PxShape* shape = nullptr;
    reinterpret_cast<physx::PxRigidBody*>(m_triangleMeshImpl->m_actor)->getShapes(&shape, 1);
    if (shape)
    {
        shape->setSimulationFilterData(filterData);
    }
}

engine::TriangleMeshImpl* engine::TriangleMesh::GetTriangleMeshImpl(void)
{
    return m_triangleMeshImpl;
}

engine::TriangleMesh* engine::TriangleMeshStaticFactory::CreateStatic(SceneGraph* inScene, EntityHandle inOwner)
{
    // Create static rigid body in regard to the geometry and give it an owner and a scene
    if (TriangleMesh* temp = inScene->CreateComponent<TriangleMesh>(inOwner))
    {
        temp->CreateTriangleMesh();
        return temp;
    }
    PrintLog(ErrorPreset(), "Failed to create triangle mesh.");
    return nullptr;
}
