#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include <string>

namespace engine
{
    class ResourceContainer
    {
    public:

        ENGINE_API ResourceContainer(void) = default;
        ENGINE_API ResourceContainer(class IResource* resource, const std::string& key);
        ENGINE_API ResourceContainer(ResourceContainer&& other) noexcept;
        ENGINE_API ~ResourceContainer(void);

        ENGINE_API void AddRef(void);
        ENGINE_API bool RemoveRef(void);

        ENGINE_API class IResource* GetResource(void);
        ENGINE_API const class IResource* GetResource(void) const;
        ENGINE_API const std::string& GetKey(void);

        ENGINE_API ResourceContainer& operator=(ResourceContainer&&) noexcept;

    private:

        class IResource*    m_resource = nullptr;
        std::string         m_key;
        int32               m_refCount = 0;

    };
}