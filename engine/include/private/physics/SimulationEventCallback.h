#pragma once

#pragma region PhysX

#include <physx/PxPhysicsAPI.h>

#pragma endregion

#pragma region engine

#include "engine/core/TypesECS.h"
#include "physics/ICollisionListener.h"

#pragma endregion

namespace engine
{
    class SimulationEventCallback : public physx::PxSimulationEventCallback
    {
    private :

        void onContact(const physx::PxContactPairHeader& inPairHeader,
                       const physx::PxContactPair* inPairs,
                       physx::PxU32 inNbPairs) override;

        void onTrigger(physx::PxTriggerPair* inPairs,
                       physx::PxU32 inNbPairs) override;

        void onAdvance(const physx::PxRigidBody* const* bodyBuffer,
                       const physx::PxTransform* poseBuffer,
                       const physx::PxU32 count) override;

        void onConstraintBreak(physx::PxConstraintInfo* constraints,
                               physx::PxU32 count) override;

        void onWake(physx::PxActor** actors,
                    physx::PxU32 count) override;

        void onSleep(physx::PxActor** actors,
                     physx::PxU32 count) override;



        bool GetCollisionListener(struct RigidBodyData* inData,
                                 ICollisionListener*& outListener,
                                 EntityHandle& outOwner);

    }; // !Class SimulationEventCallback
} // !Namespace engine