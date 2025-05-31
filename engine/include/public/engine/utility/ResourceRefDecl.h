#pragma once

#include "engine/CoreTypes.h"
#include "ResourceContainer.h"
#include <concepts>


namespace engine
{
    template <typename TResourceType>
    class ResourceRef
    {  

    public:

        ResourceRef(void) = default;
        ResourceRef(ResourceContainer* controlBlock, TResourceType* rawVal);
        ResourceRef(const ResourceRef& other);
        ResourceRef(ResourceRef&&) noexcept;
        ~ResourceRef(void);

        ResourceRef& operator=(const ResourceRef& rhs);
        ResourceRef& operator=(ResourceRef&& rhs) noexcept;

        template <std::derived_from<TResourceType> TOtherType>
        ResourceRef& operator=(const ResourceRef<TOtherType>& rhs);

        template <std::derived_from<TResourceType> TOtherType>
        ResourceRef& operator=(ResourceRef<TOtherType>&& rhs) noexcept;

        const TResourceType& operator*(void) const;
        const TResourceType* operator->(void) const;

        bool operator==(const ResourceRef& rhs) const;
        bool operator!=(const ResourceRef& rhs) const;

        bool operator==(const TResourceType* rhs) const;
        bool operator!=(const TResourceType* rhs) const;

        operator bool(void) const;
        
    protected:

        void DecrementRefCount(void);

        TResourceType* m_raw = nullptr;
        ResourceContainer* m_controlBlock = nullptr;
       
    }; 

    template <typename TResourceType>
    class EditableRef : public ResourceRef<TResourceType>
    {
    public:

        using ResourceRef<TResourceType>::ResourceRef;

        template <std::derived_from<TResourceType> TOtherType>
        EditableRef& operator=(const EditableRef<TOtherType>& rhs);

        template <std::derived_from<TResourceType> TOtherType>
        EditableRef& operator=(EditableRef<TOtherType>&& rhs) noexcept;

        TResourceType& operator*(void);
        TResourceType* operator->(void);

    };

    template<typename TResourceType>
    inline TResourceType& EditableRef<TResourceType>::operator*(void)
    {
        return *ResourceRef<TResourceType>::m_raw;
    }

    template<typename TResourceType>
    inline TResourceType* EditableRef<TResourceType>::operator->(void)
    {
        return ResourceRef<TResourceType>::m_raw;
    }


    template<typename TResourceType> inline
    ResourceRef<TResourceType>::ResourceRef(ResourceContainer* controlBlock,
                                            TResourceType* rawVal)
        : m_controlBlock(controlBlock), m_raw(rawVal)
    {
        if (m_controlBlock)
            m_controlBlock->AddRef();
    }

    template<typename TResourceType> inline
    ResourceRef<TResourceType>::ResourceRef(const ResourceRef& other)
       : m_controlBlock(other.m_controlBlock), m_raw(other.m_raw)
    {
        if (m_controlBlock)
            m_controlBlock->AddRef();
    }

    template<typename TResourceType>
    inline ResourceRef<TResourceType>::ResourceRef(ResourceRef&& rhs) noexcept
    {
        m_controlBlock = rhs.m_controlBlock;
        m_raw = rhs.m_raw;

        rhs.m_controlBlock = nullptr;
        rhs.m_raw = nullptr;
    }

    template<typename TResourceType> inline
    ResourceRef<TResourceType>::~ResourceRef(void)
    {
        if (m_controlBlock)
            m_controlBlock->RemoveRef();
    }

    template<typename TResourceType> inline
    ResourceRef<TResourceType>& ResourceRef<TResourceType>::operator=(
                                                    const ResourceRef<TResourceType>& rhs)
    {
        DecrementRefCount();

        m_controlBlock = rhs.m_controlBlock;
        m_raw = rhs.m_raw;

        if (rhs.m_controlBlock)
            rhs.m_controlBlock->AddRef();

        return *this;
    }


    template<typename TResourceType> inline
    ResourceRef<TResourceType>& ResourceRef<TResourceType>::operator=(
                                                     ResourceRef<TResourceType>&& rhs) noexcept
    {
        DecrementRefCount();

        m_controlBlock = rhs.m_controlBlock;
        m_raw = rhs.m_raw;

        rhs.m_controlBlock = nullptr;
        rhs.m_raw = nullptr;

        return *this;
    }


    template<typename TResourceType>
    inline const TResourceType& ResourceRef<TResourceType>::operator*(void) const
    {
        return *m_raw;
    }

    template<typename TResourceType>
    inline const TResourceType* ResourceRef<TResourceType>::operator->(void) const
    {
        return m_raw;
    }

    template<typename TResourceType>
    inline bool ResourceRef<TResourceType>::operator==(const ResourceRef& rhs) const
    {
        return m_raw == rhs.m_raw;
    }

    template<typename TResourceType>
    inline bool ResourceRef<TResourceType>::operator!=(const ResourceRef& rhs) const
    {
        return !(*this == rhs);
    }

    template<typename TResourceType>
    inline bool ResourceRef<TResourceType>::operator==(const TResourceType* rhs) const
    {
        return m_raw == rhs;
    }

    template<typename TResourceType>
    inline bool ResourceRef<TResourceType>::operator!=(const TResourceType* rhs) const
    {
        return !(*this == rhs);
    }

    template<typename TResourceType>
    inline ResourceRef<TResourceType>::operator bool(void) const
    {
        return m_raw != nullptr;
    }

    template<typename TResourceType>
    template <std::derived_from<TResourceType> TOtherType> inline
    ResourceRef<TResourceType>&
    ResourceRef<TResourceType>::operator=(const ResourceRef<TOtherType>& rhs)
    {
        DecrementRefCount();

        m_controlBlock = rhs.m_controlBlock;
        m_raw = rhs.m_raw;

        if (rhs.m_controlBlock)
            rhs.m_controlBlock->AddRef();

        return *this;
    }

    template<typename TResourceType>
    template <std::derived_from<TResourceType> TOtherType> inline
        ResourceRef<TResourceType>&
        ResourceRef<TResourceType>::operator=(ResourceRef<TOtherType>&& rhs) noexcept
    {
        DecrementRefCount();

        m_controlBlock = rhs.m_controlBlock;
        m_raw = rhs.m_raw;

        rhs.m_controlBlock = nullptr;
        rhs.m_raw = nullptr;

        return *this;
    }

    template<typename TResourceType>
    template <std::derived_from<TResourceType> TOtherType> inline
        EditableRef<TResourceType>&
        EditableRef<TResourceType>::operator=(const EditableRef<TOtherType>& rhs)
    {
        ResourceRef<TResourceType>::DecrementRefCount();

        memccpy(this, &rhs, sizeof(*this));

        if (ResourceRef<TResourceType>::m_controlBlock)
            ResourceRef<TResourceType>::m_controlBlock->AddRef();

        return *this;
    }

    template<typename TResourceType>
    template <std::derived_from<TResourceType> TOtherType> inline
        EditableRef<TResourceType>&
        EditableRef<TResourceType>::operator=(EditableRef<TOtherType>&& rhs) noexcept
    {
       ResourceRef<TResourceType>::DecrementRefCount();

        memcpy(this, &rhs, sizeof(*this));
        memset(&rhs, 0, sizeof(rhs));

        return *this;
    }
}