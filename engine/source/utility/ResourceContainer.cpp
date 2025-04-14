#include "engine/utility/ResourceContainer.h"

engine::ResourceContainer::ResourceContainer(void* resource, const std::string* key)
    : m_resource(m_resource), m_key(key)
{
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

void* engine::ResourceContainer::GetResource(void)
{
    return m_resource;
}

const std::string& engine::ResourceContainer::GetKey(void)
{
    return *m_key;
}
