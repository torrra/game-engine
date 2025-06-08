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

    if (!GetCollisionListener(data0, listenerA, ownerA))
        return;

    if (!GetCollisionListener(data1, listenerB, ownerB))
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

    if (!GetCollisionListener(data0, listenerA, ownerA))
        return;

    if (!GetCollisionListener(data1, listenerB, ownerB))
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

bool engine::SimulationEventCallback::GetCollisionListener(RigidBodyData* inData,
                                                           ICollisionListener*& outListener,
                                                           EntityHandle& outOwner)
{

    SceneGraph* scene = Engine::GetEngine()->GetGraph();

    ComponentArray<RigidBodyDynamic>& dynamicRigidBodies =
    scene->GetComponentArray<RigidBodyDynamic>();

    ComponentArray<RigidBodyStatic>& staticRigidBodies =
    scene->GetComponentArray<RigidBodyStatic>();

    ComponentArray<TriangleMesh>& triangleMeshes =
    scene->GetComponentArray<TriangleMesh>();

    RigidBodyDynamic* dynamicRb = nullptr;
    RigidBodyStatic* staticRb = nullptr;
    TriangleMesh* triangleMesh = nullptr;

    switch (inData->m_type)
    {
    case EShapeType::DYNAMIC:

        if (inData->m_index >= dynamicRigidBodies.GetSize())
            return false;

        dynamicRb = &*(dynamicRigidBodies.begin() + inData->m_index);

        if (!dynamicRb->IsActive() || !dynamicRb->IsValid())
            return false;

       outOwner = dynamicRb->GetOwner();
       outListener = dynamic_cast<ICollisionListener*>(dynamicRb);
       return true;

    case EShapeType::STATIC:

        if (inData->m_index >= staticRigidBodies.GetSize())
            return false;

        staticRb = &*(staticRigidBodies.begin() + inData->m_index);

        if (!staticRb->IsActive() || !staticRb->IsValid())
            return false;

        outOwner = staticRb->GetOwner();
        outListener = dynamic_cast<ICollisionListener*>(staticRb);
        return true;

    case EShapeType::TRIANGLE:

        if (inData->m_index >= triangleMeshes.GetSize())
            return false;

         triangleMesh = &*(triangleMeshes.begin() + inData->m_index);

         if (!triangleMesh->IsActive() || !triangleMesh->IsValid())
             return false;

         outOwner = triangleMesh->GetOwner();
         outListener = dynamic_cast<ICollisionListener*>(triangleMesh);
         return true;

    default:
        return false;
    }
}
