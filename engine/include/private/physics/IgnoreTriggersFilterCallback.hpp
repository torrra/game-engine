#pragma once

#include <physx/PxPhysicsAPI.h>

class IgnoreTriggersFilterCallback : public physx::PxQueryFilterCallback
{
public:
    virtual physx::PxQueryHitType::Enum preFilter(
        [[maybe_unused]] const physx::PxFilterData& filterData,
        [[maybe_unused]] const physx::PxShape* shape,
        [[maybe_unused]] const physx::PxRigidActor* actor,
        [[maybe_unused]] physx::PxHitFlags& queryFlags) override
    {
        if (shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
        {
            return physx::PxQueryHitType::eNONE;  // IGNORE
        }

        return physx::PxQueryHitType::eBLOCK;  // KEEP
    }

    virtual physx::PxQueryHitType::Enum postFilter(
        [[maybe_unused]] const physx::PxFilterData& filterData, 
        [[maybe_unused]] const physx::PxQueryHit& hit,
        [[maybe_unused]] const physx::PxShape* shape,
        [[maybe_unused]] const physx::PxRigidActor* actor) override
    {
        return physx::PxQueryHitType::eBLOCK;
    }
};
