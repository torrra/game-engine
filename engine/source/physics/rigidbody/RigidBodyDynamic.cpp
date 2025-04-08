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

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"

#pragma endregion

engine::RigidBodyDynamic::RigidBodyDynamic(EntityHandle owner, SceneGraph* scene)
{
    // Initialize the rigidbody implementation struct
    m_rigidBodyImpl = new RigidBodyDynamicImpl();
    // Set the owner and the current scene
    m_owner			= owner;
    m_currentScene	= scene;
}

engine::Transform& engine::RigidBodyDynamic::CheckEntityTransform(void)
{
    // Check if the entity has a transform
    if (Transform* temp = m_currentScene->GetComponent<engine::Transform>(m_owner))
    {
        // If true take the entity transform
        PrintLog(SuccessPreset(), "Found transform component on entity.");
        return *temp;
    }

    // If not create a new transform to the entity
    PrintLog(WarningPreset(), "No transform component found on entity create new one.");
    return *m_currentScene->CreateComponent<engine::Transform>(m_owner);
}

void engine::RigidBodyDynamic::CreateDynamicBoxRigidBody(void)
{
    // Create a new material with default values
    m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

    // Create a new dynamic rigid body with box geometry and default values
    m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
                                            *PhysicsEngine::Get().GetImpl().m_physics,
                                            ToPxTransform(CheckEntityTransform()),
                                            physx::PxBoxGeometry(0.5f, 0.5f, 0.5f), 
                                            *m_materialImpl->GetImpl().m_material, 1.0f);

    // Set the gravity by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
        false);

    // Set the visualization of the rigid body to false by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);

    // Add the rigid body to the physics scene
    PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);

    PrintLog(SuccessPreset(), "Created dynamic box rigid body.");
}

void engine::RigidBodyDynamic::CreateDynamicSphereRigidBody(void)
{
    // Create a new material with default values
    m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

    // Create a new dynamic rigid body with sphere geometry and default values
    m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
                                            *PhysicsEngine::Get().GetImpl().m_physics,
                                            ToPxTransform(CheckEntityTransform()),
                                            physx::PxSphereGeometry(0.5f),
                                            *m_materialImpl->GetImpl().m_material, 1.0f);

    // Set the gravity by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
        false);

    // Set the visualization of the rigid body to false by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);

    // Add the rigid body to the physics scene
    PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);

    PrintLog(SuccessPreset(), "Created dynamic sphere rigid body.");
}

void engine::RigidBodyDynamic::CreateDynamicCapsuleRigidBody(void)
{
    // Create a new material with default values
    m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

    // Create a new dynamic rigid body with capsule geometry and default values
    m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
                                            *PhysicsEngine::Get().GetImpl().m_physics,
                                            ToPxTransform(CheckEntityTransform()),
                                            physx::PxCapsuleGeometry(0.25f, 0.25f),
                                            *m_materialImpl->GetImpl().m_material, 1.0f);

    // Rotate the intiale position of the capsule to be at the vertical by default
    // By using the local pose to not rotate the entity attach to it
    physx::PxTransform currentPose = m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose();
    physx::PxQuat rotation(physx::PxHalfPi, physx::PxVec3(0, 0, 1));

    // Update the local pose via the shape
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        shapes->setLocalPose(physx::PxTransform(physx::PxVec3(0, 0, 0), -rotation * currentPose.q)
                            );
    }

    // Set the gravity by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
        false);

    // Set the visualization of the rigid body to false by default
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);

    // Add the rigid body to the physics scene
    PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);

    PrintLog(SuccessPreset(), "Created dynamic capsule rigid body.");
}

void engine::RigidBodyDynamic::UpdateEntity(EntityHandle inEntityHandle)
{
    // Update the entity transform in regard to the rigid body for exemple,
    // if the rigid body is under gravity then the rigid body fall so the entity fall
    *m_currentScene->GetComponent<Transform>(inEntityHandle) = 
                            ToTransform(m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose());
}

void engine::RigidBodyDynamic::UpdateRigidBody(EntityHandle inEntityHandle)
{
    // Update the transform of the rigid body in regard to the entity
    m_rigidBodyImpl->m_rigidBodyDynamic->setGlobalPose(ToPxTransform(
        *m_currentScene->GetComponent<Transform>(inEntityHandle)));
}

void engine::RigidBodyDynamic::RigidBodyDynamicCleanUp(void)
{
    // Release the material
    PX_RELEASE(m_materialImpl->GetImpl().m_material);
    // Delete the pointer
    delete m_materialImpl;
    // Release the rigid body
    PX_RELEASE(m_rigidBodyImpl->m_rigidBodyDynamic);
}

engine::RigidBodyDynamic::~RigidBodyDynamic(void)
{
    // Release all rigid body resources
    RigidBodyDynamicCleanUp();

    // Delete the pointer to the implementation structure
    delete m_rigidBodyImpl;
    m_rigidBodyImpl = nullptr;

    PrintLog(SuccessPreset(), "Cleaned up dynamic rigid body.");
}

bool engine::RigidBodyDynamic::GetIsGravityDisabled(void) const
{
    // Retrieve the gravity state
    return m_rigidBodyImpl->m_rigidBodyDynamic->getActorFlags().isSet(
                physx::PxActorFlag::eDISABLE_GRAVITY);
}

math::Vector3f engine::RigidBodyDynamic::GetBoxHalfExtents(void) const
{
    // Retrieve the box half extents by getting the shape of the rigid body to access
    // the good geometry and retrive the good information about the box
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        if (shapes->getGeometry().getType() == physx::PxGeometryType::eBOX)
        {
            const physx::PxGeometry* geometry = &shapes->getGeometry();
            const physx::PxBoxGeometry& boxGeometry = 
                                    static_cast<const physx::PxBoxGeometry&>(*geometry);

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
    // Retrieve the sphere radius by getting the shape of the rigid body to access
    // the good geometry and retrive the good information about the sphere
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        if (shapes->getGeometry().getType() == physx::PxGeometryType::eSPHERE)
        {
            const physx::PxGeometry* geometry = &shapes->getGeometry();
            const physx::PxSphereGeometry& sphereGeometry = 
                                    static_cast<const physx::PxSphereGeometry&>(*geometry);

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
    // Retrieve the capsule half height and radius by getting the shape of the rigid body to 
    // access the good geometry and retrive the good information about the capsule
    physx::PxShape* shapes = nullptr;
    m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
    if (shapes)
    {
        if (shapes->getGeometry().getType() == physx::PxGeometryType::eCAPSULE)
        {
            const physx::PxGeometry* geometry = &shapes->getGeometry();
            const physx::PxCapsuleGeometry capsuleGeometry = 
                                        static_cast<const physx::PxCapsuleGeometry&>(*geometry);

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
    // Set the gravity disabled or enabled state
    // State by default is enabled
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, 
                                                      inIsGravityDisabled);

    PrintLog(SuccessPreset(), "Successfully set gravity.");
}

void engine::RigidBodyDynamic::SetBoxHalfExtents(math::Vector3f inHalfExtents) const
{
    // Set the box half extents by using the shape of rigid body to access the good geometry
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
    // Set the sphere radius by using the shape of rigid body to access the good geometry
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
    // Set the capsule half height and radius by using the shape of rigid body to access the 
    // good geometry
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

void engine::RigidBodyDynamic::SetDebugVisualization(bool inIsDebugVisualization)
{
    m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, 
                                                      inIsDebugVisualization);
}

engine::RigidBodyDynamic* engine::RigidBodyDynamicFactory::CreateDynamic(SceneGraph* scene, 
                                                                         EntityHandle owner,
                                                                         const EGeometryType& inGeometry)
{
    // Create dynamic rigid body in regard to the geometry and give it an owner and a scene
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
            PrintLog(ErrorPreset(), "Invalid geometry type, create box by default.");
            temp->CreateDynamicBoxRigidBody();
            return temp;
            break;
        }
    }
    PrintLog(ErrorPreset(), "Failed to create dynamic rigid body.");
    return nullptr;
}
