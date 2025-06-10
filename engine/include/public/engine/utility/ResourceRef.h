#pragma once
#include "ResourceRefDecl.h"
#include "engine/resource/ResourceManager.h"
#include "engine/thread/ThreadManager.h"

namespace engine
{
    template<typename TResourceType>
    inline void ResourceRef<TResourceType>::DecrementRefCount(void)
    {
        // HACK: unload on another thread in case this thread is already locking
        // the resource manager's mutex
        if (m_controlBlock && m_controlBlock->RemoveRef())
            ThreadManager::AddTask(&ResourceManager::Unload, std::string(m_controlBlock->GetKey()));
    }
}