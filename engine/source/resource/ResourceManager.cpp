#include "resource/ResourceManager.h"
#include "resource/shader/Shader.h"

std::mutex               engine::ResourceManager::m_mutex;
engine::ResourceManager* engine::ResourceManager::m_instance = nullptr;

void engine::ResourceManager::LoadShader(
		const char* shaderProgramName,
		const char* vertShader, 
		const char* fragShader, bool isVertAbsolute, bool isFragAbsolute)
{
	if (HasResource(shaderProgramName))
	{
		std::printf("Shader name already exists. Failed to create new shader program '%s'\n", 
			shaderProgramName);
		
		return;
	}

	ShaderProgram* newShader = new ShaderProgram();
    m_mutex.lock();
    GetInstance()->m_resources[shaderProgramName] = ResourceContainer(newShader, shaderProgramName);
    m_mutex.unlock();

    newShader->m_vertexShader = vertShader;
    newShader->m_fragShader = fragShader;
    newShader->CreateProgram(isVertAbsolute, isFragAbsolute);
}

void engine::ResourceManager::Unload(std::string const& fileName)
{
    std::lock_guard lock(m_mutex);

	// Check resource exists
	if (!HasResource(fileName))
		return;

    printf("Unloaded %s\n", fileName.c_str());
    GetInstance()->m_resources.erase(fileName);
}

void engine::ResourceManager::UnloadAll(void)
{
    m_mutex.lock();
    GetInstance()->m_resources.clear();
    m_mutex.unlock();
}

void engine::ResourceManager::ShutDown(void)
{
	if (!GetInstance()->m_resources.empty())
		GetInstance()->UnloadAll();

	delete m_instance;
    m_instance = nullptr;
}

engine::ResourceManager* engine::ResourceManager::GetInstance(void)
{
	if (!m_instance)
		m_instance = new ResourceManager();

	return m_instance;
}

bool engine::ResourceManager::HasResource(std::string const& fileName)
{
	return GetInstance()->m_resources.contains(fileName);
}
