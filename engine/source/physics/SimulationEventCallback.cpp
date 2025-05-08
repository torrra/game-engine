#include "physics/SimulationEventCallback.h"

#pragma region Physics

#include "engine/physics/ICollisionListener.h"

#pragma endregion

void engine::SimulationEventCallback::onContact(const physx::PxContactPairHeader& inPairHeader, const physx::PxContactPair* inPairs, physx::PxU32 inNbPairs)
{
    ICollisionListener* listenerA = static_cast<ICollisionListener*>(inPairHeader.actors[0]->userData);
    ICollisionListener* listenerB = static_cast<ICollisionListener*>(inPairHeader.actors[1]->userData);

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
        auto* triggerListener = static_cast<ICollisionListener*>(pair.triggerActor->userData);
        auto* otherListener = static_cast<ICollisionListener*>(pair.otherActor->userData);

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
