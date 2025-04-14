#pragma once

#include "Component.h"

namespace engine
{
    template <CValidComponent TComponentType>
    class ComponentArray
    {
    public:


        ComponentArray(void) = default;
        ComponentArray(const ComponentArray&) = default;
        ~ComponentArray(void) = default;

        // Check if a component should be moved to the back of the array. The component will
        // only be moved if the component type requires children to be updated after parents,
        // and if the child is found before the parent.
        // false: component has not been found or has not been moved
        // true: component was found and moved to the back of the array
        bool     MoveReparentedComponent(EntityHandle owner, EntityHandle parent);

        // Force component to move to the back of the array. Only check if the component
        // exists
        // false: component has not been found
        // true: component was found and moved
        bool      MoveReparentedComponent(EntityHandle owner);

        // Permanently et a component up for destruction
        void      InvalidateComponent(EntityHandle owner);

        // Create a new component
        TComponentType* CreateComponent(EntityHandle owner, EntityHandle parent,
                                        class SceneGraph* scene);

        TComponentType*         GetComponent(EntityHandle owner);
        const TComponentType*   GetComponent(EntityHandle owner) const;

        // Check if an entity owns a component in this array
        bool  HasComponent(EntityHandle entity) const;


        // Get component's location in array from its owner's handle
        uint64 GetComponentIndex(EntityHandle owner) const;


        void AddDeserializedComponent(const TComponentType& component);


        // Standard functions necessary to use ranged for loops.
        // Cannot use PascalCase for these two functions as they must
        // match a precise syntax. Their return types are STL vector
        // iterators

        auto begin(void);
        auto end(void);

        ComponentArray& operator=(const ComponentArray&) = default;

    private:

        // Create a component without checking if the object's parent is
        // before its new memory location
        TComponentType* ForceCreateComponent(EntityHandle owner, class SceneGraph* scene);

        std::unordered_map<EntityHandle, uint64>    m_entityIndexMap;
        std::vector<TComponentType>                    m_components;
    };


    template<CValidComponent TComponentType>
    inline bool ComponentArray<TComponentType>::MoveReparentedComponent(EntityHandle owner,
                                                                        EntityHandle parent)
    {
        // do not do anything if the contained type is not sensitive to
        // the memory layout (if parents don't need to be updated before children)
        if constexpr (!UpdateAfterParent<TComponentType>::m_value)
        {
            printf("not moving component");
            return false;
        }

        else
        {
            if ((!m_entityIndexMap.contains(owner)))
                return false;

            if (!m_entityIndexMap.contains(parent) ||
                m_entityIndexMap[parent] < m_entityIndexMap[owner])
            {
                printf("parent is already before child\n");
                return false;
            }

            uint64                newIndex = m_components.size();
            TComponentType& toMove = m_components[m_entityIndexMap[owner]];

            m_components.emplace_back(toMove);

            toMove.Invalidate();
            m_entityIndexMap[owner] = newIndex;

            printf("yea we moving the component\n");
            return true;
        }

    }

    template<CValidComponent TComponentType>
    inline bool ComponentArray<TComponentType>::MoveReparentedComponent(EntityHandle owner)
    {    
        if (!m_entityIndexMap.contains(owner))
            return false;

        uint64            newIndex = m_components.size();
        TComponentType& toMove = m_components[m_entityIndexMap[owner]];

        // copy component to move in the back of the array and invalidate old version
        m_components.emplace_back(toMove);

        toMove.Unregister();
        toMove.Invalidate();
        m_entityIndexMap[owner] = newIndex;

        printf("yea we moving the component\n");
        return true;
    }

    template<CValidComponent TComponentType>
    inline void ComponentArray<TComponentType>::InvalidateComponent(EntityHandle owner)
    {
        if (!m_entityIndexMap.contains(owner))
            return;

        // set component to be written over
        TComponentType& component = m_components[m_entityIndexMap[owner]];
        component.Unregister();
        component.Invalidate();

        // erase mapping to owner entity
        m_entityIndexMap.erase(owner);
    }

    template<CValidComponent TComponentType> inline 
    TComponentType* ComponentArray<TComponentType>::CreateComponent(EntityHandle owner,
                                                                    EntityHandle parent,
                                                                    class SceneGraph* scene)
    {    
        // Creates new component with no regard for ordering if new component does not
        // need to be updated after parent
        if constexpr (!UpdateAfterParent<TComponentType>::m_value)
            return ForceCreateComponent(owner, scene);

        else
        {
            // parentIndex is 0 by default so that it is always considered 'before'
            // the new component when the latter has no actual parent, makes it so it
            // fails later 'parentIndex > newIndex' check and does not stop overwrite
            EntityHandle    parentIndex = 0;

            if (m_entityIndexMap.contains(parent))
                parentIndex = m_entityIndexMap[parent];

            EntityHandle size = static_cast<EntityHandle>(m_components.size());

            for (EntityHandle newIndex = 0; newIndex < size; ++newIndex)
            {
                TComponentType& currentComponent = m_components[newIndex];

                // Do not overwrite component if it is valid or before parent
                if (currentComponent.IsValid() || parentIndex > newIndex)
                    continue;

                printf("[Component array]: filling invalid slot\n");

                currentComponent = TComponentType(owner, scene);
                m_entityIndexMap[owner] = newIndex;
                return &currentComponent;
            }

            printf("[Component array]: creating new slot\n");
            m_entityIndexMap[owner] = m_components.size();
            return &m_components.emplace_back(owner, scene);
        }
    }

    template<CValidComponent TComponentType>
    inline TComponentType* ComponentArray<TComponentType>::GetComponent(EntityHandle owner)
    {
        if (!m_entityIndexMap.contains(owner))
            return nullptr;

        return &m_components[m_entityIndexMap[owner]];
    }

    template<CValidComponent TComponentType> inline
    const TComponentType* ComponentArray<TComponentType>::GetComponent(EntityHandle owner) const
    {
        if (!m_entityIndexMap.contains(owner))
            return nullptr;

        return &m_components[m_entityIndexMap.at(owner)];
    }

    template<CValidComponent TComponentType>
    inline bool ComponentArray<TComponentType>::HasComponent(EntityHandle entity) const
    {
        return m_entityIndexMap.contains(entity);
    }

    template<CValidComponent TComponentType>
    inline uint64 ComponentArray<TComponentType>::GetComponentIndex(EntityHandle owner) const
    {
        if (m_entityIndexMap.contains(owner))
            return m_entityIndexMap.at(owner);
        else
            return static_cast<uint64>(-1);
    }

    template<CValidComponent TComponentType>
    inline void ComponentArray<TComponentType>::AddDeserializedComponent(const TComponentType& component)
    {
        m_entityIndexMap[component.GetOwner()] = m_components.size();
        m_components.push_back(component);
    }

    template<CValidComponent TComponentType>
    inline auto ComponentArray<TComponentType>::begin(void)
    {
        return m_components.begin();
    }

    template<CValidComponent TComponentType>
    inline auto ComponentArray<TComponentType>::end(void)
    {
        return m_components.end();
    }

    template<CValidComponent TComponentType> inline
    TComponentType* ComponentArray<TComponentType>::ForceCreateComponent(EntityHandle owner,
                                                                    class SceneGraph* scene)
    {
        EntityHandle size = static_cast<EntityHandle>(m_components.size());

        // write over invalid component if possible
        for (EntityHandle newIndex = 0; newIndex < size; ++newIndex)
        {
            TComponentType& currentComponent = m_components[newIndex];

            if (currentComponent.IsValid())
                continue;

            printf("[Component array]: filling invalid slot\n");

            currentComponent = TComponentType(owner, scene);
            m_entityIndexMap[owner] = newIndex;
            return &currentComponent;
        }

        printf("[Component array]: creating new slot\n");

        // create new component if no invalid component was found
        m_entityIndexMap[owner] = m_components.size();
        return &m_components.emplace_back(owner, scene);
    }
}