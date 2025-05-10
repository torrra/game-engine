#include "physics/SimulationEventCallback.h"

#include "engine/Engine.h"
#include "engine/core/SceneGraph.h"
#include "engine/physics/geometry/Geometry.hpp"

void engine::SimulationEventCallback::onContact(const physx::PxContactPairHeader& inPairHeader, const physx::PxContactPair* inPairs, physx::PxU32 inNbPairs)
{
    //uint32 type = GetShapeType(reinterpret_cast<uint64>(inPairHeader.actors[0]->userData));
    //uint32 index = GetRigidBodyIndex(reinterpret_cast<uint64>(inPairHeader.actors[0]->userData));
    //uint32 type2 = GetShapeType(reinterpret_cast<uint64>(inPairHeader.actors[1]->userData));
    //uint32 index2 = GetRigidBodyIndex(reinterpret_cast<uint64>(inPairHeader.actors[1]->userData));

    auto* data0 = reinterpret_cast<RigidBodyData*>(inPairHeader.actors[0]->userData);
    auto* data1 = reinterpret_cast<RigidBodyData*>(inPairHeader.actors[1]->userData);

    ICollisionListener* listenerA = nullptr;
    ICollisionListener* listenerB = nullptr;

    switch (data0->m_type)
    {
    case EShapeType::DYNAMIC:
        listenerA =  dynamic_cast<ICollisionListener*>(&(*(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + data0->m_index)));
        break;
    case EShapeType::STATIC:
        listenerA = dynamic_cast<ICollisionListener*>(&(*(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyStatic>().begin() + data0->m_index)));
        break;
    default:
        break;
    }
    switch (data1->m_type)
    {
    case EShapeType::DYNAMIC:
        listenerB = dynamic_cast<ICollisionListener*>(&(*(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyDynamic>().begin() + data1->m_index)));
        break;
    case EShapeType::STATIC:
        listenerB = dynamic_cast<ICollisionListener*>(&(*(Engine::GetEngine()->GetGraph()->GetComponentArray<RigidBodyStatic>().begin() + data1->m_index)));
        break;
    default:
        break;
    }

    if (!listenerA || !listenerB)
        return;

    for (physx::PxU32 pairIndex = 0; pairIndex < inNbPairs; ++pairIndex)
    {
        const physx::PxContactPair& contactPair = inPairs[pairIndex];

        if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            listenerA->OnCollisionEnter(inPairHeader.actors[1]->userData);
            listenerB->OnCollisionEnter(inPairHeader.actors[0]->userData);
        }

        if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            listenerA->OnCollisionStay(inPairHeader.actors[1]->userData);
            listenerB->OnCollisionStay(inPairHeader.actors[0]->userData);
        }

        if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            listenerA->OnCollisionExit(inPairHeader.actors[1]->userData);
            listenerB->OnCollisionExit(inPairHeader.actors[0]->userData);
        }
    }
}

void engine::SimulationEventCallback::onTrigger(physx::PxTriggerPair* inPairs, physx::PxU32 inNbPairs)
{
    for (physx::PxU32 pairIndex = 0; pairIndex < inNbPairs; ++pairIndex)
    {
        const physx::PxTriggerPair& pair = inPairs[pairIndex];

        // Récupère les objets de l'utilisateur
        ICollisionListener* triggerListener = static_cast<ICollisionListener*>(pair.triggerActor->userData);
        ICollisionListener* otherListener = static_cast<ICollisionListener*>(pair.otherActor->userData);

        if (!triggerListener || !otherListener) continue;

        if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            triggerListener->OnTriggerEnter(pair.otherActor->userData);
        }
        if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            triggerListener->OnTriggerExit(pair.otherActor->userData);
        }
        if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            triggerListener->OnTriggerStay(pair.otherActor->userData);
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