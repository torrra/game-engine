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

engine::TriangleMesh::TriangleMesh(EntityHandle inOwner, class SceneGraph* inScene)
{
    // Initialize the triangle mesh implementation
    m_triangleMeshImpl  = new TriangleMeshImpl();

    // Set the owner and the current scene
    m_owner             = inOwner;
    m_currentScene      = inScene;

    // Get the model and return if failed
    if (engine::Renderer* renderer = m_currentScene->GetComponent<engine::Renderer>(m_owner))
        m_model = renderer->GetModel();

    else
        return;

    if (m_model == nullptr)
        return;
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
    m_triangleMeshImpl->m_triangleMeshDesc.setToDefault();

    PhysicsEngine::Get().GetImpl().m_scene->removeActor(*m_triangleMeshImpl->m_actor);
    // Release the triangle mesh
    PX_RELEASE(m_triangleMeshImpl->m_triangleMesh);

    // Delete the triangle mesh implementation
    delete m_triangleMeshImpl;
    m_triangleMeshImpl = nullptr;
}

void engine::TriangleMesh::CreateTriangleMesh(void)
{
    /*
        Create the triangle mesh description
        points.count : The number of points
        points.stride : The stride of the points
        points.data : The data of the points
    */
    physx::PxTriangleMeshDesc meshDesc;
    m_triangleMeshImpl->m_triangleMeshDesc.points.count = static_cast<physx::PxU32>(
                                m_model->GetStaticMeshes()[0].GetIndexCount());
    m_triangleMeshImpl->m_triangleMeshDesc.points.stride = sizeof(math::Vector3f);
    m_triangleMeshImpl->m_triangleMeshDesc.points.data = 
                                m_model->GetStaticMeshes()[0].GetVertices();

    /*
        triangles.count : The number of triangles
        triangles.stride : The stride of the triangles
        triangles.data : The data of the triangles
    */
    m_triangleMeshImpl->m_triangleMeshDesc.triangles.count = static_cast<physx::PxU32>(
                                m_model->GetStaticMeshes()[0].GetIndexCount() / 3);
    m_triangleMeshImpl->m_triangleMeshDesc.triangles.stride = 3 * sizeof(uint32);
    m_triangleMeshImpl->m_triangleMeshDesc.triangles.data = 
                                m_model->GetStaticMeshes()[0].GetIndices();

    CookTriangleMesh();
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
    // Create the static rigid body
    physx::PxRigidStatic* actor =
        physx::PxCreateStatic(*PhysicsEngine::Get().GetImpl().m_physics,
            ToPxTransform(m_currentScene->GetComponent<engine::Transform>(
                m_owner)->GetTransform()), physx::PxTriangleMeshGeometry(
                    m_triangleMeshImpl->m_triangleMesh), *material);

    // Add the actor to the physics scene
    PhysicsEngine::Get().GetImpl().m_scene->addActor(*actor);
}

engine::TriangleMeshImpl* engine::TriangleMesh::GetTriangleMeshImpl(void)
{
    return m_triangleMeshImpl;
}

