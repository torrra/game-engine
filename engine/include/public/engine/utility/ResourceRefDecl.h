#include "ResourceContainer.h"

namespace engine
{
    template <typename TResourceType>
    class ResourceRef
    {
    public:

        ResourceRef(void) = delete;
        ResourceRef(ResourceContainer* controlBlock, const TResourceType* rawVal);
        ResourceRef(const ResourceRef& other);
        ResourceRef(ResourceRef&&) = default;
        ~ResourceRef(void);

        ResourceRef& operator=(const ResourceRef&) = delete;
        ResourceRef& operator==(const ResourceRef&&) = delete;

        const TResourceType& operator*(void);
        const TResourceType* operator->(void);

        bool operator==(const ResourceRef& rhs) const;
        bool operator!=(const ResourceRef& rhs) const;

        bool operator==(const TResourceType* rhs) const;
        bool operator!=(const TResourceType* rhs) const;

        operator bool(void) const;

    private:

        void DecrementRefCount(void);

        const TResourceType* m_raw;
        ResourceContainer* m_controlBlock;
    }; 

    template<typename TResourceType> inline
    ResourceRef<TResourceType>::ResourceRef(ResourceContainer* controlBlock,
                                            const TResourceType* rawVal)
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

    template<typename TResourceType> inline
    ResourceRef<TResourceType>::~ResourceRef(void)
    {
        if (m_controlBlock)
            m_controlBlock->RemoveRef();
    }

    template<typename TResourceType>
    inline const TResourceType& ResourceRef<TResourceType>::operator*(void)
    {
        return *m_raw;
    }

    template<typename TResourceType>
    inline const TResourceType* ResourceRef<TResourceType>::operator->(void)
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

   
}