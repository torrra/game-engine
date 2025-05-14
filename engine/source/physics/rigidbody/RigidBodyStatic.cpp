#include "engine/physics/rigidbody/RigidBodyStatic.h"

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

#include "serialization/TextSerializer.h"

engine::RigidBodyStatic::RigidBodyStatic(EntityHandle inOwner, SceneGraph* inScene)
{
	m_rigidBodyStaticImpl	= new RigidBodyStaticImpl();
	m_owner					= inOwner;
	m_currentScene			= inScene;
}

math::Vector3f engine::RigidBodyStatic::GetBoxHalfExtents(void)
{
    // Retrieve the box half extents by getting the shape of the rigid body to access
    // the good geometry and retrive the good information about the box
    physx::PxShape* shapes = nullptr;
    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
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
        PrintLog(ErrorPreset(), "Invalid geometry type : type is not box");
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
    physx::PxShape* shapes = nullptr;

    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
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
        PrintLog(ErrorPreset(), "Get sphere radius : Invalid geometry type : type is not sphere");
        return 0.f;
    }
    }
    PrintLog(ErrorPreset(), ("Invalid shapes"));
    return 0.f;
}

math::Vector2f engine::RigidBodyStatic::GetCapsuleFormat(void) const
{
    // Retrieve the capsule half height and radius by getting the shape of the rigid body to 
    // access the good geometry and retrive the good information about the capsule
    physx::PxShape* shapes = nullptr;

    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
    m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
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
        return math::Vector2f(0.f);
    }
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return math::Vector2f(0.f);
}

const char* engine::RigidBodyStatic::GetGeometryName(void) const
{
    switch (m_shape)
    {
    case EGeometryType::BOX :
        return "Box";
    case EGeometryType::SPHERE:
        return "Sphere";
    case EGeometryType::CAPSULE:
        return "Capsule";
    case EGeometryType::PLANE:
        return "Plane";
    }
    return "";
}

void engine::RigidBodyStatic::SetBoxHalfExtents(math::Vector3f inHalfExtents)
{
    // Set the box half extents by using the shape of rigid body to access the good geometry
    physx::PxShape* shapes = nullptr;

    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
    m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
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

void engine::RigidBodyStatic::SetSphereRadius(f32 inRadius)
{
    // Set the sphere radius by using the shape of rigid body to access the good geometry
    physx::PxShape* shapes = nullptr;

    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
    m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
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

void engine::RigidBodyStatic::SetCapsuleFormat(f32 inRadius, f32 inHalfHeight) const
{
    // Set the capsule half height and radius by using the shape of rigid body to access the 
    // good geometry
    physx::PxShape* shapes = nullptr;

    if (m_rigidBodyStaticImpl != nullptr && m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
    {
    m_rigidBodyStaticImpl->m_rigidBodyStatic->getShapes(&shapes, 1);
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
    }
    PrintLog(ErrorPreset(), "Invalid shape");
    return;
}

void engine::RigidBodyStatic::UpdateEntity(void)
{
    if (m_rigidBodyStaticImpl != nullptr)
    {
        if (m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr &&
            m_currentScene->GetEntity(m_owner)->HasComponent<Transform>())
        {
    // Update the entity transform in regard to the rigid body
    Transform* transform = m_currentScene->GetComponent<Transform>(m_owner);

    Transform updatedTransform = ToTransform(m_rigidBodyStaticImpl->m_rigidBodyStatic->getGlobalPose());

    transform->CopyPosition(updatedTransform);
    transform->CopyRotation(updatedTransform);
}
    }
}

void engine::RigidBodyStatic::UpdateRigidBody(void)
{
    if (m_rigidBodyStaticImpl != nullptr)
    {
        if (m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr &&
            m_currentScene->GetEntity(m_owner)->HasComponent<Transform>())
        {
    Transform worldTransform;

    Transform& entityTransform = *m_currentScene->GetComponent<Transform>(m_owner);

    worldTransform.SetPosition(Transform::ToWorldPosition(entityTransform));
    worldTransform.SetRotation(Transform::ToWorldRotation(entityTransform));

    // Update the transform of the rigid body in regard to the entity
    m_rigidBodyStaticImpl->m_rigidBodyStatic->setGlobalPose(ToPxTransform(worldTransform));
}
    }
}

void engine::RigidBodyStatic::RigidBodyStaticCleanUp(void)
{
    if (m_rigidBodyStaticImpl != nullptr)
    {
        if (m_rigidBodyStaticImpl->m_rigidBodyStatic != nullptr)
        {
    // Delete the pointer to the implementation structure
    delete m_materialImpl;
    m_materialImpl = nullptr;
    // Release the rigid body
	PX_RELEASE(m_rigidBodyStaticImpl->m_rigidBodyStatic);

    delete m_rigidBodyStaticImpl;
    m_rigidBodyStaticImpl = nullptr;
}
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
    text::Serialize(output, "flags", m_flags);
    output << '\n';
}

const char* engine::RigidBodyStatic::DeserializeText(const char* text, const char* end)
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

void engine::RigidBodyStatic::CreateStaticBoxRigidBody(void)
{
    // Create a new material with default values
    m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

    if (m_rigidBodyStaticImpl != nullptr)
    {
        if (m_rigidBodyStaticImpl->m_rigidBodyStatic == nullptr)
        {
    // Create a new static rigid body with box geometry and default values
    m_rigidBodyStaticImpl->m_rigidBodyStatic = physx::PxCreateStatic(
        *PhysicsEngine::Get().GetImpl().m_physics,
        ToPxTransform(CheckEntityTransform()),
                physx::PxBoxGeometry(m_halfExtents.GetX(), m_halfExtents.GetY(), m_halfExtents.GetZ()),
        *m_materialImpl->GetImpl().m_material);

    // Set the visualization of the rigid body to false by default
    m_rigidBodyStaticImpl->m_rigidBodyStatic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);    

    m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
    m_data.m_type = EShapeType::STATIC;
    void** dataPtr = reinterpret_cast<void**>(&m_data);
    m_rigidBodyStaticImpl->m_rigidBodyStatic->userData = *dataPtr;

    SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));
    
    // Add the rigid body to the physics scene
    PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyStaticImpl->m_rigidBodyStatic);
    m_shape = EGeometryType::BOX;
}
    }
}

void engine::RigidBodyStatic::CreateStaticSphereRigidBody(void)
{
    // Create a new material with default values
    m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

    // Create a new static rigid body with sphere geometry and default values
    m_rigidBodyStaticImpl->m_rigidBodyStatic = physx::PxCreateStatic(
        *PhysicsEngine::Get().GetImpl().m_physics,
        ToPxTransform(CheckEntityTransform()),
        physx::PxSphereGeometry(0.5f), 
        *m_materialImpl->GetImpl().m_material);

    // Set the visualization of the rigid body to false by default
    m_rigidBodyStaticImpl->m_rigidBodyStatic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);

    m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
    m_data.m_type = EShapeType::STATIC;
    void** dataPtr = reinterpret_cast<void**>(&m_data);
    m_rigidBodyStaticImpl->m_rigidBodyStatic->userData = *dataPtr;

    SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));

    // Add the rigid body to the physics scene
    PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyStaticImpl->m_rigidBodyStatic);
    m_shape = EGeometryType::SPHERE;
}

void engine::RigidBodyStatic::CreateStaticCapsuleRigidBody(void)
{
    // Create a new material with default values
    m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

    // Create a new static rigid body with capsule geometry and default values
    m_rigidBodyStaticImpl->m_rigidBodyStatic = physx::PxCreateStatic(
        *PhysicsEngine::Get().GetImpl().m_physics,
        ToPxTransform(CheckEntityTransform()),
        physx::PxCapsuleGeometry(0.25f, 0.5f), 
        *m_materialImpl->GetImpl().m_material);

    // Set the visualization of the rigid body to false by default
    m_rigidBodyStaticImpl->m_rigidBodyStatic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);

    m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
    m_data.m_type = EShapeType::STATIC;
    void** dataPtr = reinterpret_cast<void**>(&m_data);
    m_rigidBodyStaticImpl->m_rigidBodyStatic->userData = *dataPtr;

    SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));

    // Add the rigid body to the physics scene
    PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyStaticImpl->m_rigidBodyStatic);
    m_shape = EGeometryType::CAPSULE;
}

void engine::RigidBodyStatic::CreateStaticPlaneRigidBody(void)
{
    // Create a new material with default values
    m_materialImpl = new Material(0.5f, 0.5f, 0.6f);

    // Create a new static rigid body with plane geometry and default values
    m_rigidBodyStaticImpl->m_rigidBodyStatic = physx::PxCreatePlane(
        *PhysicsEngine::Get().GetImpl().m_physics,
        physx::PxPlane(physx::PxVec3(0.f, 1.f, 0.f), 0.f),
        *m_materialImpl->GetImpl().m_material);

    // Set the visualization of the rigid body to false by default
    m_rigidBodyStaticImpl->m_rigidBodyStatic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);

    m_data.m_index = static_cast<uint32>(m_currentScene->GetThisIndex(this));
    m_data.m_type = EShapeType::STATIC;
    void** dataPtr = reinterpret_cast<void**>(&m_data);
    m_rigidBodyStaticImpl->m_rigidBodyStatic->userData = *dataPtr;

    SetCollisionGroupAndMask(static_cast<uint32>(m_collisionGroup), collision::GetCollisionMask(m_collisionGroup));

    PhysicsEngine::Get().GetImpl().m_scene->addActor(*m_rigidBodyStaticImpl->m_rigidBodyStatic);
    m_shape = EGeometryType::PLANE;
}

void engine::RigidBodyStatic::SetDebugVisualization(bool inIsDebugVisualization)
{
    m_rigidBodyStaticImpl->m_rigidBodyStatic->setActorFlag(physx::PxActorFlag::eVISUALIZATION, 
                                                           inIsDebugVisualization);
}

void engine::RigidBodyStatic::SetCollisionGroup(collision::ECollisionGroup inCollisionGroup)
{
    m_collisionGroup = inCollisionGroup;
}

void engine::RigidBodyStatic::SetTrigger(bool inIsTrigger)
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
        }
        else
        {
            shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
            shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
            m_collisionGroup = collision::ECollisionGroup::ENVIRONMENT_COLLISION;
        }
    }
}

void engine::RigidBodyStatic::SwitchShape(const EGeometryType& inGeometry)
{
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

void engine::RigidBodyStatic::OnCollisionEnter(EntityHandle inOther)
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

void engine::RigidBodyStatic::OnCollisionExit(EntityHandle inOther)
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

void engine::RigidBodyStatic::OnTriggerEnter(EntityHandle inOther)
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

void engine::RigidBodyStatic::OnTriggerExit(EntityHandle inOther)
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
