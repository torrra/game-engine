#include "engine/physics/rigidbody/RigidBodyStatic.h"

#pragma region Internal

#include "physics/InternalPhysXStruct.hpp"
#include "physics/InternalPhyxConversion.hpp"

#pragma endregion

#pragma region Core

#include "core/SceneGraph.h"

#pragma endregion

#pragma region Engine

#include "engine/ConsoleLog.hpp"

#pragma endregion

#pragma region Physics

#include "engine/physics/PhysicsEngine.h"

#pragma endregion

#pragma region Serialization

#include "serialization/TextSerializer.h"

#pragma endregion

#pragma region System

#include "core/systems/ScriptSystem.h"

#pragma endregion

/// <summary>
/// TODO :  Clean and comment
///         Comment on git
///         Do the same for the other rigid bodies
/// </summary>
/// <param name="inOwner"></param>
/// <param name="inScene"></param>

engine::RigidBodyStatic::RigidBodyStatic(EntityHandle inOwner, SceneGraph* inScene) :
    m_data{}
{
	m_rigidBodyStaticImpl	= new RigidBodyStaticImpl();

	m_owner					= inOwner;
	m_currentScene			= inScene;
}

void engine::RigidBodyStatic::Invalidate(void)
{
    Component::Invalidate();
    RigidBodyStaticCleanUp();
}

math::Vector3f engine::RigidBodyStatic::GetBoxHalfExtents(void)
{
    // Retrieve the box half extents by getting the shape of the rigid body to access
    // the good geometry and retrive the good information about the box
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
        physx::PxShape* shapes = nullptr;
        m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
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
            PrintLog(WarningPreset(), "Invalid geometry type : type is not box");
            return math::Vector3f(0.f);
        }
    }
    PrintLog(ErrorPreset(), "Invalid shapes");
    return math::Vector3f(0.f);
}

f32 engine::RigidBodyStatic::GetSphereRadius(void)
{
    // Retrieve the sphere radius by getting the shape of the rigid body to access
    // the good geometry and retrive the good information about the sphere
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
        physx::PxShape* shapes = nullptr;
        m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
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
            PrintLog(WarningPreset(), "Get sphere radius : Invalid geometry type : type is not sphere");
            return 0.f;
        }
    }
    PrintLog(ErrorPreset(), ("Invalid shapes"));
    return 0.f;
}

math::Vector2f engine::RigidBodyStatic::GetCapsuleFormat(void)
{
    // Retrieve the capsule half height and radius by getting the shape of the rigid body to 
    // access the good geometry and retrive the good information about the capsule
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
        physx::PxShape* shapes = nullptr;
        m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
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
            PrintLog(WarningPreset(), "Get capsule format : Invalid geometry type : type is not capsule");
            return math::Vector2f(0.f);
        }
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return math::Vector2f(0.f);
}

bool engine::RigidBodyStatic::GetIsTrigger(void) const
{
    return m_isTrigger;
}

void engine::RigidBodyStatic::SetBoxHalfExtents(const math::Vector3f& inHalfExtents)
{
    // Set the box half extents by using the shape of rigid body to access the good geometry
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
        physx::PxShape* shapes = nullptr;
        m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
        if (shapes)
        {
            if (shapes->getGeometry().getType() == physx::PxGeometryType::eBOX)
            {
                shapes->setGeometry(physx::PxBoxGeometry(inHalfExtents.GetX(), inHalfExtents.GetY(),
                    inHalfExtents.GetZ()));
                m_halfExtents = math::Vector3f(inHalfExtents);
                return;
            }
            PrintLog(WarningPreset(), "Set box half extents : Invalid geometry type : type is not box.");
            return;
        }
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return;
}

void engine::RigidBodyStatic::SetSphereRadius(f32 inRadius)
{
    // Set the sphere radius by using the shape of rigid body to access the good geometry
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
        physx::PxShape* shapes = nullptr;
        m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
        if (shapes)
        {
            if (shapes->getGeometry().getType() == physx::PxGeometryType::eSPHERE)
            {
                shapes->setGeometry(physx::PxSphereGeometry(inRadius));
                m_radius = inRadius;
                return;
            }
            PrintLog(WarningPreset(), "Set sphere radius : Invalid geometry type : type is not sphere.");
            return;
        }
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return;
}

void engine::RigidBodyStatic::SetCapsuleFormat(f32 inRadius, f32 inHalfHeight)
{
    // Set the capsule half height and radius by using the shape of rigid body to access the 
    // good geometry
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
        physx::PxShape* shapes = nullptr;
        m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
        if (shapes)
        {
            if (shapes->getGeometry().getType() == physx::PxGeometryType::eCAPSULE)
            {
                shapes->setGeometry(physx::PxCapsuleGeometry(inRadius, inHalfHeight));
                m_capsuleFormat = math::Vector2f(inRadius, inHalfHeight);
                return;
            }
            PrintLog(WarningPreset(), "Set capsule : Invalid geometry type : type is not capsule.");
            return;
        }
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return;
}

void engine::RigidBodyStatic::UpdateEntity(void)
{
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr &&
        m_rigidBodyShape != EGeometryType::PLANE)
    {
        // Update the entity transform in regard to the rigid body
        Transform* transform = m_currentScene->GetComponent<Transform>(m_owner);

        Transform updatedTransform = ToTransform(m_rigidBodyStaticImpl->m_rigidBodyStatic->getGlobalPose());

        transform->CopyPosition(updatedTransform);
        transform->CopyRotation(updatedTransform);
    }
}

void engine::RigidBodyStatic::UpdateRigidBody(void)
{
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr && 
        m_rigidBodyShape != EGeometryType::PLANE)
    {
        Transform worldTransform;

        Transform& entityTransform = *m_currentScene->GetComponent<Transform>(m_owner);

        worldTransform.SetPosition(Transform::ToWorldPosition(entityTransform));
        worldTransform.SetRotation(Transform::ToWorldRotation(entityTransform));

        // Update the transform of the rigid body in regard to the entity
        m_rigidBodyStaticImpl->m_rigidBodyStatic->setGlobalPose(ToPxTransform(worldTransform));
    }
}

void engine::RigidBodyStatic::RigidBodyStaticCleanUp(void)
{
    // Delete the pointer to the implementation structure
    delete m_materialImpl;
    m_materialImpl = nullptr;

    if (m_rigidBodyStaticImpl != nullptr)
    {
        if (m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
        {
            // Release the rigid body
            PX_RELEASE(m_rigidBodyStaticImpl->m_rigidBodyStatic);
        }
            
        delete m_rigidBodyStaticImpl;
        m_rigidBodyStaticImpl = nullptr;
    }
}

void engine::RigidBodyStatic::SerializeText(std::ostream& output, EntityHandle owner, uint64 index) const
{
    output << "[RigidBodyStatic]\n     ";

    if constexpr (UpdateAfterParent<RigidBodyStatic>::m_value)
    {
        text::Serialize(output, "index", index);
        output << "\n     ";
    }

    text::Serialize(output, "owner", owner);
    output << "\n     ";
    text::Serialize(output, "type", m_rigidBodyType);
    output << "\n     ";
    text::Serialize(output, "shape", m_rigidBodyShape);
    output << "\n     ";
    text::Serialize(output, "collision group", static_cast<uint32>(m_collisionGroup));
    output << "\n     ";

    if (m_rigidBodyShape == EGeometryType::BOX)
    {
        text::Serialize(output, "box half extents", m_halfExtents);
        output << "\n     ";
    }
    else if (m_rigidBodyShape == EGeometryType::SPHERE)
    {
        text::Serialize(output, "sphere radius", m_radius);
        output << "\n     ";
    }
    else if (m_rigidBodyShape == EGeometryType::CAPSULE)
    {
        text::Serialize(output, "capsule format", m_capsuleFormat);
        output << "\n     ";
    }
    text::Serialize(output, "trigger", m_isTrigger);
    output << "\n     ";
    text::Serialize(output, "flags", m_flags);
    output << '\n';
}

const char* engine::RigidBodyStatic::DeserializeText(const char* text, const char* end)
{
    MOVE_TEXT_CURSOR(text, end);
    text = text::DeserializeInteger(text, m_owner);

    MOVE_TEXT_CURSOR(text, end);
    text = text::DeserializeInteger(text, m_rigidBodyType);

    MOVE_TEXT_CURSOR(text, end);
    text = text::DeserializeInteger(text, m_rigidBodyShape);
    
    MOVE_TEXT_CURSOR(text, end);
    uint32 collisionGroup = 0;
    text = text::DeserializeInteger(text, collisionGroup);
    m_collisionGroup = static_cast<collision::ECollisionGroup>(collisionGroup);

    if (m_rigidBodyShape == EGeometryType::BOX)
    {
        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeVector(text, m_halfExtents);
    }
    else if (m_rigidBodyShape == EGeometryType::SPHERE)
    {
        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeReal(text, m_radius);
    }
    else if (m_rigidBodyShape == EGeometryType::CAPSULE)
    {
        MOVE_TEXT_CURSOR(text, end);
        text = text::DeserializeVector(text, m_capsuleFormat);
    }

    MOVE_TEXT_CURSOR(text, end);
    uint32 isTrigger = 1;
    text = text::DeserializeInteger(text, isTrigger);
    m_isTrigger = static_cast<bool>(isTrigger);

    MOVE_TEXT_CURSOR(text, end);
    return text::DeserializeInteger(text, m_flags);
}

engine::Transform& engine::RigidBodyStatic::CheckEntityTransform(void)
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

void engine::RigidBodyStatic::SetCollisionGroupAndMask(uint32 inCollisionGroup, uint32 inCollisionMask)
{
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
        physx::PxFilterData filterData;
        filterData.word0 = inCollisionGroup;
        filterData.word1 = inCollisionMask;

        physx::PxShape* shape = nullptr;
        m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shape, 1);
        if (shape)
        {
            shape->setSimulationFilterData(filterData);
        }
    }
}

void engine::RigidBodyStatic::SetCapsuleBaseOrientation(void)
{
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
        // Rotate the intiale position of the capsule to be at the vertical by default
        // By using the local pose to not rotate the entity attach to it
        physx::PxTransform currentPose = m_rigidBodyStaticImpl->m_rigidBodyStatic->getGlobalPose();
        physx::PxQuat rotation(physx::PxHalfPi, physx::PxVec3(0, 0, 1));

        // Update the local pose via the shape
        physx::PxShape* shapes = nullptr;
        m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
        if (shapes)
        {
            shapes->setLocalPose(physx::PxTransform(physx::PxVec3(0, 0, 0), -rotation * currentPose.q)
            );
        }
    }
}

void engine::RigidBodyStatic::CreateStaticBoxRigidBody(void)
{
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic == nullptr)
    {
        // Create a new material with default values
        m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

        // Create a new static rigid body with box geometry and default values
        m_rigidBodyStaticImpl->m_rigidBodyStatic = physx::PxCreateStatic(
            *PhysicsEngine::Get().GetImpl().m_physics,
            ToPxTransform(CheckEntityTransform()),
            physx::PxBoxGeometry(m_halfExtents.GetX(), m_halfExtents.GetY(), m_halfExtents.GetZ()),
            *m_materialImpl->GetImpl().m_material);

        // Set the visualization of the rigid body to true by default
        m_rigidBodyStaticImpl->m_rigidBodyStatic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

        SetTrigger(m_isTrigger);

        m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
        m_data.m_type = EShapeType::STATIC;
        void** dataPtr = reinterpret_cast<void**>(&m_data);
        m_rigidBodyStaticImpl->m_rigidBodyStatic->userData = *dataPtr;

        SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));

        // Add the rigid body to the physics scene
        PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyStaticImpl->m_rigidBodyStatic);
        m_rigidBodyShape = EGeometryType::BOX;
    }
}

void engine::RigidBodyStatic::CreateStaticSphereRigidBody(void)
{
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic == nullptr)
    {
        // Create a new material with default values
        m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

        // Create a new static rigid body with sphere geometry and default values
        m_rigidBodyStaticImpl->m_rigidBodyStatic = physx::PxCreateStatic(
            *PhysicsEngine::Get().GetImpl().m_physics,
            ToPxTransform(CheckEntityTransform()),
            physx::PxSphereGeometry(m_radius),
            *m_materialImpl->GetImpl().m_material);

        // Set the visualization of the rigid body to true by default
        m_rigidBodyStaticImpl->m_rigidBodyStatic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

        SetTrigger(m_isTrigger);

        m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
        m_data.m_type = EShapeType::STATIC;
        void** dataPtr = reinterpret_cast<void**>(&m_data);
        m_rigidBodyStaticImpl->m_rigidBodyStatic->userData = *dataPtr;

        SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));

        // Add the rigid body to the physics scene
        PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyStaticImpl->m_rigidBodyStatic);
        m_rigidBodyShape = EGeometryType::SPHERE;
    }
}

void engine::RigidBodyStatic::CreateStaticCapsuleRigidBody(void)
{
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic == nullptr)
    {
        // Create a new material with default values
        m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

        // Create a new static rigid body with capsule geometry and default values
        m_rigidBodyStaticImpl->m_rigidBodyStatic = physx::PxCreateStatic(
            *PhysicsEngine::Get().GetImpl().m_physics,
            ToPxTransform(CheckEntityTransform()),
            physx::PxCapsuleGeometry(m_capsuleFormat.GetX(), m_capsuleFormat.GetY()),
            *m_materialImpl->GetImpl().m_material);

        SetCapsuleBaseOrientation();

        // Set the visualization of the rigid body to true by default
        m_rigidBodyStaticImpl->m_rigidBodyStatic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

        SetTrigger(m_isTrigger);

        m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
        m_data.m_type = EShapeType::STATIC;
        void** dataPtr = reinterpret_cast<void**>(&m_data);
        m_rigidBodyStaticImpl->m_rigidBodyStatic->userData = *dataPtr;

        SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));

        // Add the rigid body to the physics scene
        PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyStaticImpl->m_rigidBodyStatic);
        m_rigidBodyShape = EGeometryType::CAPSULE;
    }
}

void engine::RigidBodyStatic::CreateStaticPlaneRigidBody(void)
{
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic == nullptr)
    {
        // Create a new material with default values
        m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

        // Create a new static rigid body with plane geometry and default values
        m_rigidBodyStaticImpl->m_rigidBodyStatic = physx::PxCreatePlane(
            *PhysicsEngine::Get().GetImpl().m_physics,
            physx::PxPlane(physx::PxVec3(0.f, 1.f, 0.f), 0.f),
            *m_materialImpl->GetImpl().m_material);

        // Set the visualization of the rigid body to true by default
        m_rigidBodyStaticImpl->m_rigidBodyStatic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

        m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
        m_data.m_type = EShapeType::STATIC;
        void** dataPtr = reinterpret_cast<void**>(&m_data);
        m_rigidBodyStaticImpl->m_rigidBodyStatic->userData = *dataPtr;

        SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));

        PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyStaticImpl->m_rigidBodyStatic);
        m_rigidBodyShape = EGeometryType::PLANE;
    }
}

void engine::RigidBodyStatic::SetDebugVisualization(bool inIsDebugVisualization)
{
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
        m_rigidBodyStaticImpl->m_rigidBodyStatic->setActorFlag(physx::PxActorFlag::eVISUALIZATION,
            inIsDebugVisualization);
    }
}

void engine::RigidBodyStatic::SetCollisionGroup(collision::ECollisionGroup inCollisionGroup)
{
    m_collisionGroup = inCollisionGroup;
}

void engine::RigidBodyStatic::SetTrigger(bool inIsTrigger)
{
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
        physx::PxShape* shape = nullptr;
        m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shape, 1);
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
                shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
                shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
                m_collisionGroup = collision::ECollisionGroup::ENVIRONMENT_COLLISION;
                m_isTrigger = false;
            }
        }
    }
}

void engine::RigidBodyStatic::Register(void)
{
    ScriptSystem::RegisterNewComponent("_NewRigidBodyStaticComponent", m_owner);
}

void engine::RigidBodyStatic::Unregister(void)
{
    ScriptSystem::UnregisterComponent("_RemoveRigidBodyStaticComponent", m_owner);
}

void engine::RigidBodyStatic::SwitchShape(const EGeometryType& inGeometry)
{
    if (m_rigidBodyStaticImpl == nullptr)
        m_rigidBodyStaticImpl = new RigidBodyStaticImpl();

    switch (inGeometry)
    {
    case EGeometryType::BOX:
        CreateStaticBoxRigidBody();
        break;
    case EGeometryType::SPHERE:
        CreateStaticSphereRigidBody();
        break;
    case EGeometryType::CAPSULE:
        CreateStaticCapsuleRigidBody();
        break;
    case EGeometryType::PLANE:
        CreateStaticPlaneRigidBody();
        break;
    default:
        PrintLog(ErrorPreset(), "Invalid geometry type");
        break;
    }
}

engine::RigidBodyStatic* engine::RigidBodyStaticFactory::CreateStatic(SceneGraph* inScene, 
                                                                      EntityHandle inOwner, 
                                                                      const EGeometryType& inGeometry)
{
    // Create static rigid body in regard to the geometry and give it an owner and a scene
    if (RigidBodyStatic* temp = inScene->CreateComponent<RigidBodyStatic>(inOwner))
    {       
        temp->SwitchShape(inGeometry);
        return temp;
    }
    PrintLog(ErrorPreset(), "Failed to create static rigid body.");
    return nullptr;
}
