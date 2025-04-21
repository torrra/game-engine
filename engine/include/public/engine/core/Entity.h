#pragma once

#include <string>

#include "TypesECS.h"
#include "engine/EngineExport.h"

namespace engine
{
    class Entity
    {
    public:
        // Special handle values
        enum EHandleUtils : EntityHandle
        {
            INVALID_HANDLE = 0xFFFFFFFFFFFFFFFF,

            // get 32 lower bits
            INDEX_MASK = 0x00000000FFFFFFFF,

            // get 32 higher bits
            UID_MASK = 0xFFFFFFFF00000000
        };

    private:
        // Makes it easier to know which components an entity has
        enum EComponentFlags : uint64
        {
            NO_COMPONENTS = 0,
            TRANSFORM = 1,
            SCRIPT = (uint64) (1 << 1),
            RENDERER = (uint64) (1 << 2),
            RIGIDBODY_DYNAMIC = (uint64) (1 << 3),
            RIGIDBODY_STATIC = (uint64) (1 << 4),
            CAMERA = (uint64) (1 << 5)
        };

        friend class SceneGraph;

    public:

        Entity(void) = default;
        ENGINE_API Entity(const std::string& name, EntityHandle handle, EntityHandle parent);
        ENGINE_API Entity(const Entity&) = default;

        // Is this entity a valid object?
        // true: this object is a valid entity instance
        // false: this object is 'dead' and subject to overwrite
        ENGINE_API
        bool IsValid(void) const;

        // Is this entity's components updated each tick?
        ENGINE_API
        bool IsActive(void) const;

        // Are this entity and its components included when updating the scene?
        ENGINE_API
        void Activate(bool activeState);

        // Display this entity's information:
        // name, handle, parent, status flags, component flags
        ENGINE_API
        void Print(void) const;

        // Does this entity have a component of a given type?
        template<CValidComponent TComponentType>
        bool HasComponent(void) const;

        ENGINE_API
        bool HasParent(void) const;

        ENGINE_API
        EntityHandle GetHandle(void) const;

        ENGINE_API
        EntityHandle GetParent(void) const;

        ENGINE_API
        const std::string&  GetName(void) const;

        ENGINE_API
        void         SetName(const std::string& name);

        ENGINE_API
        void        SerializeText(std::ofstream& file) const;

        ENGINE_API
        const char* DeserializeText(const char* text, const char* end);

        // Permanently set object up for destruction
        ENGINE_API
        void Invalidate(void);

        ENGINE_API
        Entity& operator=(const Entity& rhs);

    private:

        // Get EComponentFlag corresponding to the component type
        // NO_COMPONENT by default, must be manually instantiated for
        // each component type in their respective headers
        template<CValidComponent TComponentType>
        static constexpr EComponentFlags GetComponentFlag();

        std::string         m_name;
        EntityHandle        m_handle = INVALID_HANDLE;
        EntityHandle        m_parent = INVALID_HANDLE;

        // status modifiers that affect behavior (inactive, invalid)
        uint64              m_statusFlags = ecs::NONE;

        // what component this entity has
        uint64              m_components = NO_COMPONENTS;

    };


    template<CValidComponent TComponentType>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag()
    {
        return NO_COMPONENTS;
    }


    template<CValidComponent TComponentType>
    inline bool Entity::HasComponent(void) const
    {
        return m_components & GetComponentFlag<TComponentType>();
    }


}