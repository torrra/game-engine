#pragma once

#include "engine/CoreTypes.h"

namespace engine
{

    namespace ecs
    {
        // Define special cases that may affect behavior
        enum EFlags : uint64
        {
            NONE = 0,

            // Object is dead and can be written over
            INVALID_OBJECT = 1,

            // Object will not be updated
            INACTIVE_OBJECT = (uint64)(1 << 1),

            // Object persists through scenes and must be serialized
            // manually into the desired scene
            MULTI_SCENE_OBJECT = (uint64)(1 << 2),

            // Object will never be sezialized
            NON_SERIALIZABLE_OBJECT = (uint64)(1 << 3),
        };
    }


    // Unique Entity identifier, made up of 32 index bits (lower) and 32 UID bits (higher)
    // The index bits refer to the entity's position in memory, and the UID is meant to
    // differentiate between unique entities, as two entities could have the same memory
    // locations at different points in time.
    using EntityHandle = int64;

    // Component abstract class. All component types must inherit from it to be
    // considered valid components
    class Component;

    // Entity class. Represent game objects
    class Entity;

    // Ensure a component type inherits from Component class
    template <typename TComponentType>
    concept CValidComponent = std::derived_from<TComponentType, Component>;

}