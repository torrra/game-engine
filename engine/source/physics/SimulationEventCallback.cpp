#include "physics/SimulationEventCallback.h"

#include "engine/Engine.h"
#include "engine/core/SceneGraph.h"
#include "engine/physics/geometry/Geometry.hpp"
#include "core/systems/ScriptSystem.h"

void engine::SimulationEventCallback::onContact(const physx::PxContactPairHeader& inPairHeader, 
                                                const physx::PxContactPair* inPairs, 
                                                physx::PxU32 inNbPairs)
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
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + 
              data0->m_index);

        ownerA = rigidbody.GetOwner();
        listenerA = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else if (data0->m_type == EShapeType::STATIC)
    {
        RigidBodyStatic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyStatic>().begin() + 
              data0->m_index);

        ownerA = rigidbody.GetOwner();
        listenerA = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else if (data0->m_type == EShapeType::TRIANGLE)
    {
        TriangleMesh& triangleMesh =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<TriangleMesh>().begin() +
                data0->m_index);

        ownerA = triangleMesh.GetOwner();
        listenerA = dynamic_cast<ICollisionListener*>(&triangleMesh);
    }

    if (data1->m_type == EShapeType::DYNAMIC)
    {
        RigidBodyDynamic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + 
              data1->m_index);

        ownerB = rigidbody.GetOwner();
        listenerB = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else if (data1->m_type == EShapeType::STATIC)
    {
        RigidBodyStatic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyStatic>().begin() +
              data1->m_index);

        ownerB = rigidbody.GetOwner();
        listenerB = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else if (data1->m_type == EShapeType::TRIANGLE)
    {
        TriangleMesh& triangleMesh =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<TriangleMesh>().begin() +
                data1->m_index);

        ownerB = triangleMesh.GetOwner();
        listenerB = dynamic_cast<ICollisionListener*>(&triangleMesh);
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

            ScriptSystem::NotifyCollisionEnter(ownerA, ownerB);

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

            ScriptSystem::NotifyCollisionExit(ownerA, ownerB);
        }
    }
}

void engine::SimulationEventCallback::onTrigger(physx::PxTriggerPair* inPairs, 
                                                physx::PxU32 inNbPairs)
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
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + 
              data0->m_index);

        ownerA = rigidbody.GetOwner();
        listenerA = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else if (data0->m_type == EShapeType::STATIC)
    {
        RigidBodyStatic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyStatic>().begin() + 
              data0->m_index);

        ownerA = rigidbody.GetOwner();
        listenerA = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else if (data0->m_type == EShapeType::TRIANGLE)
    {
        TriangleMesh& triangleMesh =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<TriangleMesh>().begin() +
                data0->m_index);

        ownerA = triangleMesh.GetOwner();
        listenerA = dynamic_cast<ICollisionListener*>(&triangleMesh);
    }

    if (data1->m_type == EShapeType::DYNAMIC)
    {
        RigidBodyDynamic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() +
              data1->m_index);

        ownerB = rigidbody.GetOwner();
        listenerB = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else if (data1->m_type == EShapeType::STATIC)
    {
        RigidBodyStatic& rigidbody =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyStatic>().begin() +
              data1->m_index);

        ownerB = rigidbody.GetOwner();
        listenerB = dynamic_cast<ICollisionListener*>(&rigidbody);
    }
    else if (data1->m_type == EShapeType::TRIANGLE)
    {
        TriangleMesh& triangleMesh =
            *(Engine::GetEngine()->GetGraph()->GetComponentArray<TriangleMesh>().begin() +
                data1->m_index);

        ownerB = triangleMesh.GetOwner();
        listenerB = dynamic_cast<ICollisionListener*>(&triangleMesh);
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

            ScriptSystem::NotifyTriggerEnter(ownerA, ownerB);
        }
        if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            listenerA->OnTriggerExit(ownerB);

            ScriptSystem::NotifyTriggerExit(ownerA, ownerB);
        }
        //if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        //{
        //    listenerA->OnTriggerStay(ownerB);
        //}
    }
}

void engine::SimulationEventCallback::onAdvance(
    [[maybe_unused]] const physx::PxRigidBody* const* bodyBuffer, 
    [[maybe_unused]] const physx::PxTransform* poseBuffer, 
    [[maybe_unused]] const physx::PxU32 count)
{}

void engine::SimulationEventCallback::onConstraintBreak(
    [[maybe_unused]] physx::PxConstraintInfo* constraints, 
    [[maybe_unused]] physx::PxU32 count)
{}

void engine::SimulationEventCallback::onWake([[maybe_unused]] physx::PxActor** actors, 
    [[maybe_unused]] physx::PxU32 count)
{}

void engine::SimulationEventCallback::onSleep([[maybe_unused]] physx::PxActor** actors, 
                                              [[maybe_unused]] physx::PxU32 count)
{}