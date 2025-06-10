#include "resource/ResourceManager.h"
#include "resource/shader/Shader.h"
#include "resource/model/Model.h"
#include "resource/animation/Animation.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::mutex               engine::ResourceManager::m_mutex;
engine::ResourceManager* engine::ResourceManager::m_instance = nullptr;


namespace engine::importer
{
    // global variable to avoid exposing assimp to
    // Model.h by having it as a class member.
    // Creating an importer is an expensive operation,
    // so we keep one per thread
    thread_local Assimp::Importer g_resourceManagerImporter;
}


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

std::string engine::ResourceManager::LoadShaderFromFrag(const std::string& fragShader)
{
    constexpr const char* defaultVertexShader = ".\\shaders\\ModelTextured.vert";

    uint64 startOffset = fragShader.find_last_of('\\') + 1;
    uint64 endOffset = fragShader.find_last_of('.');

    if (startOffset < endOffset)
    {
        std::string fragName = fragShader.substr(startOffset);
        std::string programName = "program" + fragName.substr(0, endOffset);

        engine::ResourceManager::LoadShader(programName.c_str(),
            defaultVertexShader,
            fragShader.c_str(), true);

        return programName;
    }

    return {};
}

engine::ResourceManager::EAsyncResourceType engine::ResourceManager::LoadAsyncResource(
                                                               std::string const& fileName)
{
    std::string resourcePath;

    resourcePath = Engine::GetEngine()->GetProjectDir().string();
    resourcePath.push_back('\\');
    resourcePath += fileName;
    
    const aiScene* scene = importer::g_resourceManagerImporter.ReadFile(resourcePath,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData
    );


    if (!scene)
        return EAsyncResourceType::INVALID;

    if (scene->HasMeshes())
    {
        CreateFromData<Model>(fileName, scene, fileName);
        return EAsyncResourceType::MODEL;
    }
    else if (scene->HasAnimations())
    {
        CreateFromData<Animation>(fileName, scene, 0);
        return EAsyncResourceType::ANIMATION;
    }
    
    return EAsyncResourceType::INVALID;

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
