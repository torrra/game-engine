#include "engine/utility/ResourceContainer.h"
#include "engine/resource/Resource.h"

engine::ResourceContainer::ResourceContainer(engine::IResource* resource, const std::string& key)
    : m_resource(resource), m_key(key)
{
}

engine::ResourceContainer::ResourceContainer(ResourceContainer&& other) noexcept
{
    m_key = std::move(other.m_key);
    m_refCount = other.m_refCount;
    m_resource = other.m_resource;

    other.m_resource = nullptr;
}

engine::ResourceContainer::~ResourceContainer(void)
{
    delete m_resource;
}

void engine::ResourceContainer::AddRef(void)
{
    ++m_refCount;
}

bool engine::ResourceContainer::RemoveRef(void)
{
    if (--m_refCount <= 0)
        return true;

    return false;
}

engine::IResource* engine::ResourceContainer::GetResource(void)
{
    return m_resource;
}

const engine::IResource* engine::ResourceContainer::GetResource(void) const
{
    return m_resource;
}

const std::string& engine::ResourceContainer::GetKey(void)
{
    return m_key;
}

engine::ResourceContainer& engine::ResourceContainer::operator=(ResourceContainer&& rhs) noexcept
{
    m_key = std::move(rhs.m_key);
    m_refCount = rhs.m_refCount;
    m_resource = rhs.m_resource;

    rhs.m_resource = nullptr;
    return *this;
}
