#pragma once

#include "engine/CoreTypes.h"
#include <string>

namespace engine
{
    class ResourceContainer
    {
    public:

        ResourceContainer(void) = delete;
        ResourceContainer(void* resource, const std::string* key);
        ~ResourceContainer(void) = default;

        void AddRef(void);
        bool RemoveRef(void);

        void* GetResource(void);
        const std::string& GetKey(void);

    private:

        void*               m_resource;
        const std::string*  m_key;
        int32               m_refCount = 0;

    };
}