#include "resource/ResourceManager.h"

std::mutex engine::ResourceManager::m_mutex;
engine::ResourceManager* engine::ResourceManager::m_instance = nullptr;

void engine::ResourceManager::Unload(std::string const& fileName)
{
	// Check resource exists
	if (!HasResource(fileName))
		return;

	delete GetInstance()->m_resources[fileName];
}

void engine::ResourceManager::UnloadAll(void)
{
	for (auto& resource : GetInstance()->m_resources)
	{
		delete resource.second;
	}
}

void engine::ResourceManager::CloseResourceManager(void)
{
	if (!GetInstance()->m_resources.empty())
		GetInstance()->UnloadAll();

	delete m_instance;
}

engine::ResourceManager* engine::ResourceManager::GetInstance(void)
{
	if (!m_instance)
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		if (!m_instance)
			m_instance = new ResourceManager();
	}

	return m_instance;
}

bool engine::ResourceManager::HasResource(std::string const& fileName)
{
	return GetInstance()->m_resources.contains(fileName);
}
