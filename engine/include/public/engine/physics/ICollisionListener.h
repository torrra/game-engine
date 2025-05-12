#pragma once

#pragma region Engine

#include "engine/CoreTypes.h"

#pragma endregion

#pragma region Standard

#include <type_traits>

#pragma endregion

namespace engine
{
    namespace collision
    {
        enum class ECollisionGroup : uint32
        {
            NONE_COLLISION = 0,
            CHARACTER_COLLISION = 1 << 0,
            PROJECTILE_COLLISION = 1 << 1,
            ENVIRONMENT_COLLISION = 1 << 2,
            TRIGGER_COLLISION = 1 << 3,
            DEFAULT_COLLISION = 1 << 4

        }; // !Enum ECollisionGroup

        inline ECollisionGroup operator|(ECollisionGroup inLhs, ECollisionGroup inRhs) 
        { 
            return static_cast<ECollisionGroup>(
                static_cast<uint32>(inLhs) | static_cast<uint32>(inRhs));
        }

        inline uint32 GetCollisionMask(ECollisionGroup inGroup)
        {
            switch (inGroup)
            {
            case engine::collision::ECollisionGroup::NONE_COLLISION:
                return 0;

            case engine::collision::ECollisionGroup::CHARACTER_COLLISION:
                return static_cast<uint32>(
                    ECollisionGroup::CHARACTER_COLLISION |
                    ECollisionGroup::PROJECTILE_COLLISION |
                    ECollisionGroup::ENVIRONMENT_COLLISION |
                    ECollisionGroup::TRIGGER_COLLISION);

            case engine::collision::ECollisionGroup::PROJECTILE_COLLISION:
                return static_cast<uint32>(
                    ECollisionGroup::CHARACTER_COLLISION |
                    ECollisionGroup::ENVIRONMENT_COLLISION);

            case engine::collision::ECollisionGroup::ENVIRONMENT_COLLISION:
                return static_cast<uint32>(
                    ECollisionGroup::CHARACTER_COLLISION |
                    ECollisionGroup::PROJECTILE_COLLISION);

            case engine::collision::ECollisionGroup::TRIGGER_COLLISION:
                return static_cast<uint32>(
                    ECollisionGroup::CHARACTER_COLLISION);

            case engine::collision::ECollisionGroup::DEFAULT_COLLISION:
                return static_cast<uint32>(
                    ECollisionGroup::CHARACTER_COLLISION |
                    ECollisionGroup::PROJECTILE_COLLISION |
                    ECollisionGroup::ENVIRONMENT_COLLISION |
                    ECollisionGroup::TRIGGER_COLLISION);

            default:
                return 0;
            }
        }      
    } // !Namespace collision

    class ICollisionListener
    {
    public :

        /// Destructor
        virtual ~ICollisionListener() = default;

        /// Functions
        virtual void OnCollisionEnter(EntityHandle inOther) = 0;
        virtual void OnCollisionExit(EntityHandle inOther) = 0;
        virtual void OnCollisionStay(EntityHandle inOther) = 0;

        virtual void OnTriggerEnter(EntityHandle inOther) = 0;
        virtual void OnTriggerExit(EntityHandle inOther) = 0;
        virtual void OnTriggerStay(EntityHandle inOther) = 0;        

    }; // !Class ICollisionListener
} // !Namespace engine