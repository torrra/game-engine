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

engine::TriangleMesh::TriangleMesh(EntityHandle inOwner, class SceneGraph* inScene)
{
    m_triangleMeshImpl = new TriangleMeshImpl();

    m_owner = inOwner;
    m_currentScene = inScene;

    m_model = m_currentScene->GetComponent<engine::Renderer>(m_owner)->GetModel();
    if (m_model == nullptr)
    {
        return;
    }
}
void engine::TriangleMesh::physicsTest(void)
{
    physx::PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = static_cast<physx::PxU32>(
                                m_model->GetStaticMeshes()[0].GetIndexCount());
    meshDesc.points.stride = sizeof(math::Vector3f);
    meshDesc.points.data = m_model->GetStaticMeshes()[0].GetVertices();

    meshDesc.triangles.count = static_cast<physx::PxU32>(
                                    m_model->GetStaticMeshes()[0].GetIndexCount() / 3);
    meshDesc.triangles.stride = 3 * sizeof(unsigned int);
    meshDesc.triangles.data = m_model->GetStaticMeshes()[0].GetIndices();

    physx::PxTolerancesScale scale;
    physx::PxCookingParams params(scale);

    physx::PxDefaultMemoryOutputStream writeBuffer;
    physx::PxTriangleMeshCookingResult::Enum result;

    bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
    if (!status)
        return;

    physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

    m_triangleMeshImpl->m_triangleMesh = 
        PhysicsEngine::Get().GetImpl().m_physics->createTriangleMesh(readBuffer);

    physx::PxMaterial* material = PhysicsEngine::Get().GetImpl().m_physics->createMaterial(0.5f, 
                                                                                           0.5f, 
                                                                                           0.6f);

    physx::PxRigidStatic* actor = 
        physx::PxCreateStatic(*PhysicsEngine::Get().GetImpl().m_physics,
                ToPxTransform(m_currentScene->GetComponent<engine::Transform>(
                    m_owner)->GetTransform()), physx::PxTriangleMeshGeometry(
                        m_triangleMeshImpl->m_triangleMesh), *material);

    PhysicsEngine::Get().GetImpl().m_scene->addActor(*actor);
}
