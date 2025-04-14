#include "ResourceRefDecl.h"
#include "resource/ResourceManager.h"

namespace engine
{
    template<typename TResourceType>
    inline void ResourceRef<TResourceType>::DecrementRefCount(void)
    {
        if (m_controlBlock && m_controlBlock->RemoveRef())
            ResourceManager::Unload(m_controlBlock->GetKey());

    }
}