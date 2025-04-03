#include "physics/rigidbody/RigidBodyDynamic.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Core

#include "core/SceneGraph.h"

#pragma endregion

#pragma region engine

#include "engine/ConsoleLog.hpp"

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
        PrintLog(SuccessPreset(), "Found transform component on entity.");
        return *temp;
    }

    PrintLog(WarningPreset(), "No transform component found on entity create new one.");
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

    PrintLog(SuccessPreset(), "Created dynamic box rigid body.");
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

    PrintLog(SuccessPreset(), "Created dynamic sphere rigid body.");
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

    PrintLog(SuccessPreset(), "Created dynamic capsule rigid body.");
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

    PrintLog(SuccessPreset(), "Cleaned up dynamic rigid body.");
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
        PrintLog(ErrorPreset(), "Invalid geometry type : type is not box");
        return math::Vector3f(EErrorGeometryType_Invalid);
    }
    PrintLog(ErrorPreset(), "Invalid shapes");
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
        PrintLog(ErrorPreset(), "Get sphere radius : Invalid geometry type : type is not sphere");
        return EErrorGeometryType_Invalid;
    }
    PrintLog(ErrorPreset(), ("Invalid shapes"));
    return EErrorGeometryType_Invalid;
}

math::Vector2f engine::RigidBodyDynamic::GetCapsuleFormat(void) const
{
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        if (shapes->getGeometry().getType() == physx::PxGeometryType::eCAPSULE)
        {
            const physx::PxGeometry* geometry = &shapes->getGeometry();
            const physx::PxCapsuleGeometry capsuleGeometry = static_cast<const physx::PxCapsuleGeometry&>(*geometry);
            return math::Vector2f(capsuleGeometry.radius, capsuleGeometry.halfHeight);
        }
        PrintLog(ErrorPreset(), "Get capsule format : Invalid geometry type : type is not capsule");
        return math::Vector2f(EErrorGeometryType_Invalid);
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return math::Vector2f(EErrorGeometryType_Invalid);
}

void engine::RigidBodyDynamic::SetGravityDisabled(bool inIsGravityDisabled)
{
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, 
        inIsGravityDisabled);

    PrintLog(SuccessPreset(), "Successfully set gravity.");
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
            PrintLog(SuccessPreset(), "Successfully set box half extents.");
            return;
        }
        PrintLog(ErrorPreset(), "Set box half extents : Invalid geometry type : type is not box.");
        return;
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return;
}

void engine::RigidBodyDynamic::SetSphereRadius(f32 inRadius) const
{
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        if (shapes->getGeometry().getType() == physx::PxGeometryType::eSPHERE)
        {
            shapes->setGeometry(physx::PxSphereGeometry(inRadius));
            PrintLog(SuccessPreset(), "Successfully set sphere radius.");
            return;
        }
        PrintLog(ErrorPreset(), "Set sphere radius : Invalid geometry type : type is not sphere.");
        return;
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return;
}

void engine::RigidBodyDynamic::SetCapsuleFormat(f32 inRadius, f32 inHalfHeight) const
{
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        if (shapes->getGeometry().getType() == physx::PxGeometryType::eCAPSULE)
        {
            shapes->setGeometry(physx::PxCapsuleGeometry(inRadius, inHalfHeight));
            PrintLog(SuccessPreset(), "Successfully set capsule format.");
            return;
        }
        PrintLog(ErrorPreset(), "Set capsule : Invalid geometry type : type is not capsule.");
        return;
    }
    PrintLog(ErrorPreset(), "Invalid shape");
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
            PrintLog(ErrorPreset(), "Invalid geometry type");
            break;
        }
    }
    PrintLog(ErrorPreset(), "Failed to create dynamic rigid body.");
    return nullptr;
}
