#include "physics/rigidbody/RigidBodyDynamic.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Core

#include "core/SceneGraph.h"

#pragma endregion

#include <iostream>

engine::RigidBodyDynamic::RigidBodyDynamic(EntityHandle owner, SceneGraph* scene)
{
    m_rigidBodyImpl = new RigidBodyDynamicImpl();
    m_owner			= owner;
    m_currentScene	= scene;
}

engine::Transform& engine::RigidBodyDynamic::CheckEntityTransform(void)
{
    if (Transform* temp = m_currentScene->GetComponent<engine::Transform>(m_owner))
    {
        return *temp;
    }

    return *m_currentScene->CreateComponent<engine::Transform>(m_owner);
}

void engine::RigidBodyDynamic::CreateDynamicBoxRigidBody(void)
{
    m_material = new Material(0.5f, 0.5f, 0.6f);

    m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
                                            *PhysicsEngine::Get().GetImpl().m_physics,
                                            ToPxTransform(CheckEntityTransform()),
                                            physx::PxBoxGeometry(0.5f, 0.5f, 0.5f), 
                                            *m_material->GetImpl().m_material, 1.0f);

    // Set the gravity by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
        false);

    // Add the rigid body to the physics scene
    PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
}

void engine::RigidBodyDynamic::CreateDynamicSphereRigidBody(void)
{
    m_material = new Material(0.5f, 0.5f, 0.6f);

    m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
                                            *PhysicsEngine::Get().GetImpl().m_physics,
                                            ToPxTransform(CheckEntityTransform()),
                                            physx::PxSphereGeometry(0.5f),
                                            *m_material->GetImpl().m_material, 1.0f);

    // Set the gravity by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
        false);

    // Add the rigid body to the physics scene
    PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
}

void engine::RigidBodyDynamic::CreateDynamicCapsuleRigidBody(void)
{
    m_material = new Material(0.5f, 0.5f, 0.6f);

    m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
                                            *PhysicsEngine::Get().GetImpl().m_physics,
                                            ToPxTransform(CheckEntityTransform()),
                                            physx::PxCapsuleGeometry(0.25f, 0.25f),
                                            *m_material->GetImpl().m_material, 1.0f);

    physx::PxTransform currentPose = m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose();
    physx::PxQuat rotation(physx::PxHalfPi, physx::PxVec3(0, 0, 1));

    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        shapes->setLocalPose(physx::PxTransform(physx::PxVec3(0, 0, 0), -rotation * currentPose.q));
    }

    // Set the gravity by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
        false);

    // Add the rigid body to the physics scene
    PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
}

void engine::RigidBodyDynamic::UpdateEntity(EntityHandle inEntityHandle)
{
    physx::PxTransform transformTemp = m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose();

    *m_currentScene->GetComponent<Transform>(inEntityHandle) = ToTransform(transformTemp);
}

void engine::RigidBodyDynamic::UpdateRigidBody(const Transform& inEntityTransform)
{
    m_rigidBodyImpl->m_rigidBodyDynamic->setGlobalPose(ToPxTransform(inEntityTransform));
}

void engine::RigidBodyDynamic::RigidBodyDynamicCleanUp(void)
{
    PX_RELEASE(m_material->GetImpl().m_material);
    delete m_material;
    PX_RELEASE(m_rigidBodyImpl->m_rigidBodyDynamic);
}

engine::RigidBodyDynamic::~RigidBodyDynamic(void)
{
    RigidBodyDynamicCleanUp();

    delete m_rigidBodyImpl;
    m_rigidBodyImpl = nullptr;
}

bool engine::RigidBodyDynamic::GetIsGravityDisabled(void) const
{
    return m_rigidBodyImpl->m_rigidBodyDynamic->getActorFlags().isSet(
                physx::PxActorFlag::eDISABLE_GRAVITY);
}

math::Vector3f engine::RigidBodyDynamic::GetBoxHalfExtents(void) const
{
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        if (shapes->getGeometry().getType() == physx::PxGeometryType::eBOX)
        {
            const physx::PxGeometry* geometry = &shapes->getGeometry();
            const physx::PxBoxGeometry& boxGeometry = static_cast<const physx::PxBoxGeometry&>(*geometry);
            return math::Vector3f(boxGeometry.halfExtents.x, boxGeometry.halfExtents.y,
                                  boxGeometry.halfExtents.z);
        }
        std::cerr << "Invalid geometry type" << " : ";
        return math::Vector3f(EErrorGeometryType_Invalid);
    }
    std::cerr << "Invalid shapes" << " : ";
    return math::Vector3f(EErrorGeometryType_Invalid);
}

f32 engine::RigidBodyDynamic::GetSphereRadius(void) const
{
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        if (shapes->getGeometry().getType() == physx::PxGeometryType::eSPHERE)
        {
            const physx::PxGeometry* geometry = &shapes->getGeometry();
            const physx::PxSphereGeometry& sphereGeometry = static_cast<const physx::PxSphereGeometry&>(*geometry);
            return sphereGeometry.radius;
        }
        std::cerr << "Invalid geometry type" << " : ";
        return EErrorGeometryType_Invalid;
    }
    std::cerr << "Invalid shapes" << " : ";
    return EErrorGeometryType_Invalid;
}

math::Vector2f engine::RigidBodyDynamic::GetCapsuleRadius(void) const
{
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        if (shapes->getGeometry().getType() == physx::PxGeometryType::eBOX)
        {
            const physx::PxGeometry* geometry = &shapes->getGeometry();
            const physx::PxCapsuleGeometry capsuleGeometry = static_cast<const physx::PxCapsuleGeometry&>(*geometry);
            return math::Vector2f(capsuleGeometry.radius, capsuleGeometry.halfHeight);
        }
        std::cerr << "Invalid geometry type" << " : ";
        return math::Vector2f(EErrorGeometryType_Invalid);
    }
    std::cerr << "Invalid shapes" << " : ";
    return math::Vector2f(EErrorGeometryType_Invalid);
}

void engine::RigidBodyDynamic::SetGravityDisabled(bool inIsGravityDisabled)
{
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, 
        inIsGravityDisabled);
}

void engine::RigidBodyDynamic::SetBoxHalfExtents(math::Vector3f inHalfExtents) const
{
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        if (shapes->getGeometry().getType() == physx::PxGeometryType::eBOX)
        {
            shapes->setGeometry(physx::PxBoxGeometry(inHalfExtents.GetX(), inHalfExtents.GetY(),
                                inHalfExtents.GetZ()));
        }
        std::cerr << "Invalid geometry type" << std::endl;
        return;
    }
    std::cerr << "Invalid shapes" << std::endl;
    return;
}

engine::RigidBodyDynamic* engine::RigidBodyDynamicFactory::Create(SceneGraph* scene, 
                                                                  EntityHandle owner,
                                                                  const EGeometryType& inGeometry)
{
    if (RigidBodyDynamic* temp = new RigidBodyDynamic(owner, scene))
    {
        switch (inGeometry)
        {
        case EGeometryType::BOX :
            temp->CreateDynamicBoxRigidBody();
            return temp;
            break;
        case EGeometryType::SPHERE:
            temp->CreateDynamicSphereRigidBody();
            return temp;
            break;
        case EGeometryType::CAPSULE:
            temp->CreateDynamicCapsuleRigidBody();
            return temp;
            break;
        default:
            std::cout << "Geometry type not supported" << std::endl;
            break;
        }
    }
    return nullptr;
}
