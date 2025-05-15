#include "physics/rigidbody/RigidBodyDynamic.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Core

#include "core/SceneGraph.h"
#include "core/ComponentArray.hpp"

#pragma endregion

#pragma region engine

#include "engine/ConsoleLog.hpp"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"

#pragma endregion

#include "serialization/TextSerializer.h"

engine::RigidBodyDynamic::RigidBodyDynamic(EntityHandle inOwner, SceneGraph* inScene)
{
    // Initialize the rigidbody implementation struct
    m_rigidBodyImpl = new RigidBodyDynamicImpl();
    //m_data = new RigidBodyData();
    // Set the owner and the current scene
    m_owner			= inOwner;
    m_currentScene	= inScene;
}

engine::Transform& engine::RigidBodyDynamic::CheckEntityTransform(void)
{
    // Check if the entity has a transform
    if (Transform* temp = m_currentScene->GetComponent<engine::Transform>(m_owner))
    {
        // If true take the entity transform
        PrintLog(InfoPreset(), "Found transform component on entity.");
        return *temp;
    }

    // If not create a new transform to the entity
    PrintLog(WarningPreset(), "No transform component found on entity create new one.");
    return *m_currentScene->CreateComponent<engine::Transform>(m_owner);
}

void engine::RigidBodyDynamic::CreateDynamicBoxRigidBody(void)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic == nullptr)
    {
        // Create a new material with default values
        m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

        // Create a new dynamic rigid body with box geometry and default values
        m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
            *PhysicsEngine::Get().GetImpl().m_physics,
            ToPxTransform(CheckEntityTransform()),
            physx::PxBoxGeometry(m_halfExtents.GetX(),
                m_halfExtents.GetY(),
                m_halfExtents.GetZ()),
            *m_materialImpl->GetImpl().m_material, 1.0f);

        // Set the gravity by default
        m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
            m_isGravityDisabled);
        // Set the visualization of the rigid body to true by default
        m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

        SetTrigger(m_isTrigger);
        SetXAxisLock(m_isXLock);
        SetYAxisLock(m_isYLock);
        SetZAxisLock(m_isZLock);

        m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
        m_data.m_type = EShapeType::DYNAMIC;
        void** dataPtr = reinterpret_cast<void**>(&m_data);
        m_rigidBodyImpl->m_rigidBodyDynamic->userData = *dataPtr;

        SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));

        // Add the rigid body to the physics scene
        PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
        m_shape = EGeometryType::BOX;
    }
}

void engine::RigidBodyDynamic::CreateDynamicSphereRigidBody(void)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic == nullptr)
    {
        // Create a new material with default values
        m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

        // Create a new dynamic rigid body with sphere geometry and default values
        m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
            *PhysicsEngine::Get().GetImpl().m_physics,
            ToPxTransform(CheckEntityTransform()),
            physx::PxSphereGeometry(m_radius),
            *m_materialImpl->GetImpl().m_material, 1.0f);

        // Set the gravity by default
        m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
            m_isGravityDisabled);

        // Set the visualization of the rigid body to true by default
        m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

        SetTrigger(m_isTrigger);
        SetXAxisLock(m_isXLock);
        SetYAxisLock(m_isYLock);
        SetZAxisLock(m_isZLock);

        m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
        m_data.m_type = EShapeType::DYNAMIC;
        void** dataPtr = reinterpret_cast<void**>(&m_data);
        m_rigidBodyImpl->m_rigidBodyDynamic->userData = *dataPtr;

        SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));

        // Add the rigid body to the physics scene
        PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
        m_shape = EGeometryType::SPHERE;
    }
}

void engine::RigidBodyDynamic::CreateDynamicCapsuleRigidBody(void)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic == nullptr)
    {
        // Create a new material with default values
        m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

        // Create a new dynamic rigid body with capsule geometry and default values
        m_rigidBodyImpl->m_rigidBodyDynamic = physx::PxCreateDynamic(
            *PhysicsEngine::Get().GetImpl().m_physics,
            ToPxTransform(CheckEntityTransform()),
            physx::PxCapsuleGeometry(m_capsuleFormat.GetX(),
                m_capsuleFormat.GetY()),
            *m_materialImpl->GetImpl().m_material, 1.0f);

        SetCapsuleBaseOrientation();

        // Set the gravity by default
        m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
            m_isGravityDisabled);

        // Set the visualization of the rigid body to true by default
        m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

        SetTrigger(m_isTrigger);
        SetXAxisLock(m_isXLock);
        SetYAxisLock(m_isYLock);
        SetZAxisLock(m_isZLock);

        m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
        m_data.m_type = EShapeType::DYNAMIC;
        void** dataPtr = reinterpret_cast<void**>(&m_data);
        m_rigidBodyImpl->m_rigidBodyDynamic->userData = *dataPtr;

        SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));

        // Add the rigid body to the physics scene
        PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyImpl->m_rigidBodyDynamic);
        m_shape = EGeometryType::CAPSULE;
    }
}

void engine::RigidBodyDynamic::UpdateEntity()
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
    {
        // Update the entity transform in regard to the rigid body for exemple,
        // if the rigid body is under gravity then the rigid body fall so the entity fall
        Transform* transform = m_currentScene->GetComponent<Transform>(m_owner);

        Transform updatedTransform = ToTransform(m_rigidBodyImpl->m_rigidBodyDynamic->getGlobalPose());

        transform->CopyPosition(updatedTransform);
        transform->CopyRotation(updatedTransform);
    }
}

void engine::RigidBodyDynamic::UpdateRigidBody()
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
    {
        Transform worldTransform;

        Transform& entityTransform = *m_currentScene->GetComponent<Transform>(m_owner);

        worldTransform.SetPosition(Transform::ToWorldPosition(entityTransform));
        worldTransform.SetRotation(Transform::ToWorldRotation(entityTransform));

        // Update the transform of the rigid body in regard to the entity
        m_rigidBodyImpl->m_rigidBodyDynamic->setGlobalPose(ToPxTransform(worldTransform));
    }
}

void engine::RigidBodyDynamic::RigidBodyDynamicCleanUp(void)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
    {
        // Delete the pointer
        delete m_materialImpl;
        m_materialImpl = nullptr;
        // Release the rigid body
        PX_RELEASE(m_rigidBodyImpl->m_rigidBodyDynamic);

        // Delete the pointer to the implementation structure
        delete m_rigidBodyImpl;
        m_rigidBodyImpl = nullptr;
    }
}

void engine::RigidBodyDynamic::SerializeText(std::ostream& output, EntityHandle owner, uint64 index) const
{
    output << "[RigidBodyDynamic]\n     ";

    if constexpr (UpdateAfterParent<RigidBodyDynamic>::m_value)
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
    if (m_shape == EGeometryType::BOX)
    {
        text::Serialize(output, "box half extents", m_halfExtents);
        output << "\n     ";
    }
    else if (m_shape == EGeometryType::SPHERE)
    {
        text::Serialize(output, "sphere radius", m_radius);
        output << "\n     ";
    }
    else if (m_shape == EGeometryType::CAPSULE)
    {
        text::Serialize(output, "capsule format", m_capsuleFormat);
        output << "\n     ";
    }
    text::Serialize(output, "gravity", static_cast<uint32>(m_isGravityDisabled));
    output << "\n     ";
    text::Serialize(output, "trigger", static_cast<uint32>(m_isTrigger));
    output << "\n     ";
    text::Serialize(output, "lock X axis", static_cast<uint32>(m_isXLock));
    output << "\n     ";
    text::Serialize(output, "lock Y axis", static_cast<uint32>(m_isYLock));
    output << "\n     ";
    text::Serialize(output, "lock Z axis", static_cast<uint32>(m_isZLock));
    output << "\n     ";
    text::Serialize(output, "flags", m_flags);
    output << '\n';
}

const char* engine::RigidBodyDynamic::DeserializeText(const char* text, const char* end)
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

    if (m_shape == EGeometryType::BOX)
    {
        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeVector(text, m_halfExtents);
    }
    else if (m_shape == EGeometryType::SPHERE)
    {
        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeReal(text, m_radius);
    }
    else if (m_shape == EGeometryType::CAPSULE)
    {
        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeVector(text, m_capsuleFormat);
    }

    MOVE_TEXT_CURSOR(text, end);
    uint32 isGravityDisabled = 1;
    text = text::DeserializeInteger(text, isGravityDisabled);
    m_isGravityDisabled = static_cast<bool>(isGravityDisabled);
    
    MOVE_TEXT_CURSOR(text, end);
    uint32 isTrigger = 1;
    text = text::DeserializeInteger(text, isTrigger);
    m_isTrigger = static_cast<bool>(isTrigger);

    MOVE_TEXT_CURSOR(text, end);
    uint32 isXLock = 1;
    text = text::DeserializeInteger(text, isXLock);
    m_isXLock = static_cast<bool>(isXLock);
    MOVE_TEXT_CURSOR(text, end);
    uint32 isYLock = 1;
    text = text::DeserializeInteger(text, isYLock);
    m_isYLock = static_cast<bool>(isYLock);
    MOVE_TEXT_CURSOR(text, end);
    uint32 isZLock = 1;
    text = text::DeserializeInteger(text, isZLock);
    m_isZLock = static_cast<bool>(isZLock);

    MOVE_TEXT_CURSOR(text, end);
    return text::DeserializeInteger(text, m_flags);
}

bool engine::RigidBodyDynamic::IsGravityDisabled(void)
{
    // Retrieve the gravity state
    return m_isGravityDisabled;
}

math::Vector3f engine::RigidBodyDynamic::GetBoxHalfExtents(void)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
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

                m_halfExtents = math::Vector3f(boxGeometry.halfExtents.x, boxGeometry.halfExtents.y,
                    boxGeometry.halfExtents.z);

                return math::Vector3f(boxGeometry.halfExtents.x, boxGeometry.halfExtents.y,
                    boxGeometry.halfExtents.z);
            }
            PrintLog(ErrorPreset(), "Invalid geometry type : type is not box");
            return math::Vector3f(EErrorGeometryType_Invalid);
        }
    }
    PrintLog(ErrorPreset(), "Invalid shapes");
    return math::Vector3f(EErrorGeometryType_Invalid);
}

f32 engine::RigidBodyDynamic::GetSphereRadius(void)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
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

                m_radius = sphereGeometry.radius;

                return sphereGeometry.radius;
            }
            PrintLog(ErrorPreset(), "Get sphere radius : Invalid geometry type : type is not sphere");
            return EErrorGeometryType_Invalid;
        }
    }
    PrintLog(ErrorPreset(), ("Invalid shapes"));
    return EErrorGeometryType_Invalid;
}

math::Vector2f engine::RigidBodyDynamic::GetCapsuleFormat(void)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
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

                m_capsuleFormat = math::Vector2f(capsuleGeometry.radius, capsuleGeometry.halfHeight);

                return math::Vector2f(capsuleGeometry.radius, capsuleGeometry.halfHeight);
            }
            PrintLog(ErrorPreset(), "Get capsule format : Invalid geometry type : type is not capsule");
            return math::Vector2f(EErrorGeometryType_Invalid);
        }
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return math::Vector2f(EErrorGeometryType_Invalid);
}

math::Vector3f engine::RigidBodyDynamic::GetLinearVelocity(void) const
{
    return ToVector3f(m_rigidBodyImpl->m_rigidBodyDynamic->getLinearVelocity());
}

math::Vector3f engine::RigidBodyDynamic::GetAngularVelocity(void) const
{
    return ToVector3f(m_rigidBodyImpl->m_rigidBodyDynamic->getAngularVelocity());
}

bool engine::RigidBodyDynamic::GetIsTrigger(void) const
{
    return m_isTrigger;
}

bool engine::RigidBodyDynamic::GetIsXAxisLock(void) const
{
    return m_isXLock;
}

bool engine::RigidBodyDynamic::GetIsYAxisLock(void) const
{
    return m_isYLock;
}

bool engine::RigidBodyDynamic::GetIsZAxisLock(void) const
{
    return m_isZLock;
}

void engine::RigidBodyDynamic::SetGravityDisabled(bool inIsGravityDisabled)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
    {
        // Set the gravity disabled or enabled state
        // State by default is enabled
        m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY,
            inIsGravityDisabled);
        m_isGravityDisabled = inIsGravityDisabled;

        PrintLog(SuccessPreset(), "Successfully set gravity.");
    }
}

void engine::RigidBodyDynamic::SetBoxHalfExtents(math::Vector3f inHalfExtents)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
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
                m_halfExtents = math::Vector3f(inHalfExtents);
                PrintLog(SuccessPreset(), "Successfully set box half extents.");
                return;
            }
            PrintLog(ErrorPreset(), "Set box half extents : Invalid geometry type : type is not box.");
            return;
        }
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return;
}

void engine::RigidBodyDynamic::SetSphereRadius(f32 inRadius)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
    {
        // Set the sphere radius by using the shape of rigid body to access the good geometry
        physx::PxShape* shapes = nullptr;
        m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shapes, 1);
        if (shapes)
        {
            if (shapes->getGeometry().getType() == physx::PxGeometryType::eSPHERE)
            {
                shapes->setGeometry(physx::PxSphereGeometry(inRadius));
                m_radius = inRadius;
                PrintLog(SuccessPreset(), "Successfully set sphere radius.");
                return;
            }
            PrintLog(ErrorPreset(), "Set sphere radius : Invalid geometry type : type is not sphere.");
            return;
        }
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return;
}

void engine::RigidBodyDynamic::SetCapsuleFormat(f32 inRadius, f32 inHalfHeight)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
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
                m_capsuleFormat = math::Vector2f(inRadius, inHalfHeight);
                PrintLog(SuccessPreset(), "Successfully set capsule format.");
                return;
            }
            PrintLog(ErrorPreset(), "Set capsule : Invalid geometry type : type is not capsule.");
            return;
        }
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return;
}

void engine::RigidBodyDynamic::SetDebugVisualization(bool inIsDebugVisualization)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
    {
        m_rigidBodyImpl->m_rigidBodyDynamic->setActorFlag(physx::PxActorFlag::eVISUALIZATION,
            inIsDebugVisualization);
    }
}

void engine::RigidBodyDynamic::SetCollisionGroupAndMask(uint32 inCollisionGroup, uint32 inCollisionMask)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
    {
        physx::PxFilterData filterData;
        filterData.word0 = inCollisionGroup;
        filterData.word1 = inCollisionMask;

        physx::PxShape* shape = nullptr;
        m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shape, 1);
        if (shape)
        {
            shape->setSimulationFilterData(filterData);
        }
    }
}

void engine::RigidBodyDynamic::SetCapsuleBaseOrientation(void)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
    {
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
    }
}

void engine::RigidBodyDynamic::SetCollisionGroup(collision::ECollisionGroup inCollisionGroup)
{
    m_collisionGroup = inCollisionGroup;
}

void engine::RigidBodyDynamic::SetTrigger(bool inIsTrigger)
{
    if (m_rigidBodyImpl != nullptr && m_rigidBodyImpl->m_rigidBodyDynamic != nullptr)
    {
        physx::PxShape* shape = nullptr;
        m_rigidBodyImpl->m_rigidBodyDynamic->getShapes(&shape, 1);
        if (shape)
        {
            if (inIsTrigger)
            {
                shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
                shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
                m_collisionGroup = collision::ECollisionGroup::TRIGGER_COLLISION;
                m_isTrigger = true;
            }
            else
            {
                shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
                shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
                m_collisionGroup = collision::ECollisionGroup::ENVIRONMENT_COLLISION;
                m_isTrigger = false;
            }
        }
    }
}

void engine::RigidBodyDynamic::SetLinearVelocity(math::Vector3f inLinearVelocity, bool inAutoWake)
{
    m_rigidBodyImpl->m_rigidBodyDynamic->setLinearVelocity(ToPxVec3(inLinearVelocity), inAutoWake);
}

void engine::RigidBodyDynamic::SetAngularVelocity(math::Vector3f inAngularVelocity, bool inAutoWake)
{
    m_rigidBodyImpl->m_rigidBodyDynamic->setAngularVelocity(ToPxVec3(inAngularVelocity), inAutoWake);
}

void engine::RigidBodyDynamic::SetXAxisLock(bool inAxisLock)
{
    m_rigidBodyImpl->m_rigidBodyDynamic->setRigidDynamicLockFlag(
        physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, inAxisLock);
    m_isXLock = inAxisLock;
}

void engine::RigidBodyDynamic::SetYAxisLock(bool inAxisLock)
{
    m_rigidBodyImpl->m_rigidBodyDynamic->setRigidDynamicLockFlag(
        physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, inAxisLock);
    m_isYLock = inAxisLock;
}

void engine::RigidBodyDynamic::SetZAxisLock(bool inAxisLock)
{
    m_rigidBodyImpl->m_rigidBodyDynamic->setRigidDynamicLockFlag(
        physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, inAxisLock);
    m_isZLock = inAxisLock;
}

void engine::RigidBodyDynamic::OnCollisionEnter(EntityHandle inOther)
{
    if (RigidBodyDynamic* rbDynamic = m_currentScene->GetComponent<RigidBodyDynamic>(inOther))
        PrintLog(SuccessPreset(), "[Collision] enter between : " + std::to_string(m_data.m_index) +
                                  " with dynamic " + std::to_string(rbDynamic->m_data.m_index));

    else if (RigidBodyStatic* rbStatic = m_currentScene->GetComponent<RigidBodyStatic>(inOther))
        PrintLog(SuccessPreset(), "[Collision] enter between : " + std::to_string(m_data.m_index) + 
                                  " with static " + std::to_string(rbStatic->m_data.m_index));

    else
        PrintLog(ErrorPreset(), "[Collision] enter between : " + std::to_string(m_data.m_index) + 
                                " with wrong entity");
}

void engine::RigidBodyDynamic::OnCollisionExit(EntityHandle inOther)
{
    if (RigidBodyDynamic* rbDynamic = m_currentScene->GetComponent<RigidBodyDynamic>(inOther))
        PrintLog(SuccessPreset(), "[Collision] exit between : " + std::to_string(m_data.m_index) +
            " with dynamic " + std::to_string(rbDynamic->m_data.m_index));

    else if (RigidBodyStatic* rbStatic = m_currentScene->GetComponent<RigidBodyStatic>(inOther))
        PrintLog(SuccessPreset(), "[Collision] exit between : " + std::to_string(m_data.m_index) +
            " with static " + std::to_string(rbStatic->m_data.m_index));

    else
        PrintLog(ErrorPreset(), "[Collision] exit between : " + std::to_string(m_data.m_index) +
            " with wrong entity");
}

void engine::RigidBodyDynamic::OnTriggerEnter(EntityHandle inOther)
{
    if (RigidBodyDynamic* rbDynamic = m_currentScene->GetComponent<RigidBodyDynamic>(inOther))
        PrintLog(SuccessPreset(), "[Trigger] enter between : " + std::to_string(m_data.m_index) +
            " with dynamic " + std::to_string(rbDynamic->m_data.m_index));

    else if (RigidBodyStatic* rbStatic = m_currentScene->GetComponent<RigidBodyStatic>(inOther))
        PrintLog(SuccessPreset(), "[Trigger] enter between : " + std::to_string(m_data.m_index) +
            " with static " + std::to_string(rbStatic->m_data.m_index));

    else
        PrintLog(ErrorPreset(), "[Trigger] enter between : " + std::to_string(m_data.m_index) +
            " with wrong entity");
}

void engine::RigidBodyDynamic::OnTriggerExit(EntityHandle inOther)
{
    if (RigidBodyDynamic* rbDynamic = m_currentScene->GetComponent<RigidBodyDynamic>(inOther))
        PrintLog(SuccessPreset(), "[Trigger] exit between : " + std::to_string(m_data.m_index) +
            " with dynamic " + std::to_string(rbDynamic->m_data.m_index));

    else if (RigidBodyStatic* rbStatic = m_currentScene->GetComponent<RigidBodyStatic>(inOther))
        PrintLog(SuccessPreset(), "[Trigger] exit between : " + std::to_string(m_data.m_index) +
            " with static " + std::to_string(rbStatic->m_data.m_index));

    else
        PrintLog(ErrorPreset(), "[Trigger] exit between : " + std::to_string(m_data.m_index) +
            " with wrong entity");
}

void engine::RigidBodyDynamic::AddForce(const math::Vector3f& inForce, EForceMode inForceMode, bool inAutoWake)
{
    switch (inForceMode)
    {
    case EForceMode::FORCE:
        m_rigidBodyImpl->m_rigidBodyDynamic->addForce(ToPxVec3(inForce), physx::PxForceMode::eFORCE,
                                                      inAutoWake);
        break;
    case EForceMode::IMPULSE:
        m_rigidBodyImpl->m_rigidBodyDynamic->addForce(ToPxVec3(inForce), physx::PxForceMode::eIMPULSE,
                                                      inAutoWake);
        break;
    case EForceMode::VELOCITY_CHANGE:
        m_rigidBodyImpl->m_rigidBodyDynamic->addForce(ToPxVec3(inForce), physx::PxForceMode::eVELOCITY_CHANGE,
                                                      inAutoWake);
        break;
    case EForceMode::ACCELERATION:
        m_rigidBodyImpl->m_rigidBodyDynamic->addForce(ToPxVec3(inForce), physx::PxForceMode::eACCELERATION,
                                                      inAutoWake);
        break;
    default:
        PrintLog(ErrorPreset(), "Invalid force mode, use force mode by default.");
        m_rigidBodyImpl->m_rigidBodyDynamic->addForce(ToPxVec3(inForce), physx::PxForceMode::eFORCE,
                                                      inAutoWake);
        break;
    }
}

void engine::RigidBodyDynamic::AddTorque(const math::Vector3f& inTorque, EForceMode inForceMode, bool inAutoWake)
{
    switch (inForceMode)
    {
    case EForceMode::FORCE:
        m_rigidBodyImpl->m_rigidBodyDynamic->addForce(ToPxVec3(inTorque), physx::PxForceMode::eFORCE,
            inAutoWake);
        break;
    case EForceMode::IMPULSE:
        m_rigidBodyImpl->m_rigidBodyDynamic->addForce(ToPxVec3(inTorque), physx::PxForceMode::eIMPULSE,
            inAutoWake);
        break;
    case EForceMode::VELOCITY_CHANGE:
        m_rigidBodyImpl->m_rigidBodyDynamic->addForce(ToPxVec3(inTorque), physx::PxForceMode::eVELOCITY_CHANGE,
            inAutoWake);
        break;
    case EForceMode::ACCELERATION:
        m_rigidBodyImpl->m_rigidBodyDynamic->addForce(ToPxVec3(inTorque), physx::PxForceMode::eACCELERATION,
            inAutoWake);
        break;
    default:
        PrintLog(ErrorPreset(), "Invalid force mode, use force mode by default.");
        m_rigidBodyImpl->m_rigidBodyDynamic->addForce(ToPxVec3(inTorque), physx::PxForceMode::eFORCE,
            inAutoWake);
        break;
    }
}

void engine::RigidBodyDynamic::SwitchShape(const EGeometryType& inGeometry)
{
    if (m_rigidBodyImpl == nullptr)
        m_rigidBodyImpl = new RigidBodyDynamicImpl();

    switch (inGeometry)
    {
    case EGeometryType::BOX:
        CreateDynamicBoxRigidBody();
        break;
    case EGeometryType::SPHERE:
        CreateDynamicSphereRigidBody();
        break;
    case EGeometryType::CAPSULE:
        CreateDynamicCapsuleRigidBody();
        break;
    default:
        PrintLog(ErrorPreset(), "Invalid geometry type, create box by default.");
        CreateDynamicBoxRigidBody();
        break;
    }
}

engine::RigidBodyDynamic* engine::RigidBodyDynamicFactory::CreateDynamic(SceneGraph* inScene, 
                                                                         EntityHandle inOwner,
                                                                         const EGeometryType& inGeometry)
{
    // Create dynamic rigid body in regard to the geometry and give it an owner and a scene
    if (RigidBodyDynamic* temp = inScene->CreateComponent<RigidBodyDynamic>(inOwner))
    {
        temp->SwitchShape(inGeometry);
        return temp;
    }
    PrintLog(ErrorPreset(), "Failed to create dynamic rigid body.");
    return nullptr;
}
