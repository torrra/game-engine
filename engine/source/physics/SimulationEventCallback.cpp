#include "physics/SimulationEventCallback.h"

#include "engine/Engine.h"
#include "engine/core/SceneGraph.h"
#include "engine/physics/geometry/Geometry.hpp"

void engine::SimulationEventCallback::onContact(const physx::PxContactPairHeader& inPairHeader, const physx::PxContactPair* inPairs, physx::PxU32 inNbPairs)
{
    RigidBodyData* data0 = reinterpret_cast<RigidBodyData*>(&inPairHeader.actors[0]->userData);
    RigidBodyData* data1 = reinterpret_cast<RigidBodyData*>(&inPairHeader.actors[1]->userData);

    ICollisionListener* listenerA = nullptr;
    ICollisionListener* listenerB = nullptr;

    EntityHandle ownerA = Entity::INVALID_HANDLE;
    EntityHandle ownerB = Entity::INVALID_HANDLE;

    if (data0->m_type == EShapeType::DYNAMIC)
    {
        RigidBodyDynamic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + data0->m_index);

        ownerA = rigidbody.GetOwner();
        listenerA = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else
    {
        RigidBodyStatic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyStatic>().begin() + data0->m_index);

        ownerA = rigidbody.GetOwner();
        listenerA = dynamic_cast<ICollisionListener*>(&rigidbody);
    }

    if (data1->m_type == EShapeType::DYNAMIC)
    {
        RigidBodyDynamic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + data1->m_index);

        ownerB = rigidbody.GetOwner();
        listenerB = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else
    {
        RigidBodyStatic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyStatic>().begin() + data1->m_index);

        ownerB = rigidbody.GetOwner();
        listenerB = dynamic_cast<ICollisionListener*>(&rigidbody);
    }

    if (!listenerA || !listenerB)
        return;

    for (physx::PxU32 pairIndex = 0; pairIndex < inNbPairs; ++pairIndex)
    {
        const physx::PxContactPair& contactPair = inPairs[pairIndex];

        if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            listenerA->OnCollisionEnter(ownerB);
            listenerB->OnCollisionEnter(ownerA);
        }

        if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            listenerA->OnCollisionStay(ownerB);
            listenerB->OnCollisionStay(ownerA);
        }

        if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            listenerA->OnCollisionExit(ownerB);
            listenerB->OnCollisionExit(ownerA);
        }
    }
}

void engine::SimulationEventCallback::onTrigger(physx::PxTriggerPair* inPairs, physx::PxU32 inNbPairs)
{
    RigidBodyData* data0 = reinterpret_cast<RigidBodyData*>(&inPairs->triggerActor->userData);
    RigidBodyData* data1 = reinterpret_cast<RigidBodyData*>(&inPairs->otherActor->userData);

    ICollisionListener* listenerA = nullptr;
    ICollisionListener* listenerB = nullptr;

    EntityHandle ownerA = Entity::INVALID_HANDLE;
    EntityHandle ownerB = Entity::INVALID_HANDLE;

    if (data0->m_type == EShapeType::DYNAMIC)
    {
        RigidBodyDynamic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + data0->m_index);

        ownerA = rigidbody.GetOwner();
        listenerA = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else
    {
        RigidBodyStatic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyStatic>().begin() + data0->m_index);

        ownerA = rigidbody.GetOwner();
        listenerA = dynamic_cast<ICollisionListener*>(&rigidbody);
    }

    if (data1->m_type == EShapeType::DYNAMIC)
    {
        RigidBodyDynamic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + data1->m_index);

        ownerB = rigidbody.GetOwner();
        listenerB = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else
    {
        RigidBodyStatic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyStatic>().begin() + data1->m_index);

        ownerB = rigidbody.GetOwner();
        listenerB = dynamic_cast<ICollisionListener*>(&rigidbody);
    }

    if (!listenerA || !listenerB)
        return;

    for (physx::PxU32 pairIndex = 0; pairIndex < inNbPairs; ++pairIndex)
    {
        const physx::PxTriggerPair& pair = inPairs[pairIndex];

        // Récupère les objets de l'utilisateur
        ICollisionListener* triggerListener = listenerA;
        ICollisionListener* otherListener = listenerB;

        if (!triggerListener || !otherListener)
            continue;

        if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            listenerA->OnTriggerEnter(ownerB);
        }
        if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            listenerA->OnTriggerExit(ownerB);
        }
        if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            listenerA->OnTriggerStay(ownerB);
        }
    }
}

void engine::SimulationEventCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{
    (void)bodyBuffer;
    (void)poseBuffer;
    (void)count;
}

void engine::SimulationEventCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
    (void)constraints;
    (void)count;
}

void engine::SimulationEventCallback::onWake(physx::PxActor** actors, physx::PxU32 count)
{
    (void)actors;
    (void)count;
}

void engine::SimulationEventCallback::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
    (void)actors;
    (void)count;
}