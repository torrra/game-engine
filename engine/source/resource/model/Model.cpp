#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "resource/model/Model.h"

#include "engine/thread/ThreadManager.h"
#include "engine/resource/ResourceManager.h"
#include "engine/resource/texture/Texture.h"
#include "engine/resource/animation/Animation.h"

#include "serialization/TextSerializer.h"
#include "ConsoleLog.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <string>
#include <fstream>


namespace engine::importer
{
    // global variable to avoid exposing assimp to
    // Model.h by having it as a class member.
    // Creating an importer is an expensive operation,
    // so we keep one per thread
    thread_local Assimp::Importer g_importer;
}

engine::Model::Model(const void* data, const std::string& name)
{
    ThreadManager::AddTask(&Model::ProcessImportedData, this, data, std::string(name));
}

engine::Model::~Model(void)
{
    while (!(m_loadStatus & FAILED) && !(m_loadStatus & LOADED));

    for (DynamicMesh& mesh : m_dynamicMeshes)
        mesh.DeleteMesh();
    
    for (Mesh& mesh : m_staticMeshes)
        mesh.DeleteMesh();
}

bool engine::Model::LoadResource(const char* fileName)
{
    if (!fileName || *fileName == '\0')
        return false;

    std::string nameWithoutDir = fileName;
    uint64 lastBackslash = nameWithoutDir.find_last_of('\\');
    uint64 lastSlash = nameWithoutDir.find_last_of('/');

    if (lastBackslash != std::string::npos && lastSlash != std::string::npos)
        m_modelName = nameWithoutDir.substr(math::Max(lastSlash, lastBackslash) + 1);
    
    else if (lastBackslash != std::string::npos)
        m_modelName = nameWithoutDir.substr(lastBackslash + 1);

    else if (lastSlash != std::string::npos)
        m_modelName = nameWithoutDir.substr(lastSlash + 1);

    else
        m_modelName = fileName;

    ThreadManager::AddTask(&Model::WorkerThreadLoad, this, std::string(fileName));
    return true;
}

bool engine::Model::CanRender(void) const
{
    return (m_loadStatus & LOADED) && (m_loadStatus & GRAPHICS_CALLS_COMPLETE);
}

bool engine::Model::HasFailedToLoad(void) const
{
    return m_loadStatus & FAILED;
}

bool engine::Model::IsLoaded(void) const
{
    return m_loadStatus & LOADED;
}

bool engine::Model::IsDynamic(void) const
{
    return m_isDynamic;
}

void engine::Model::Draw(void) const
{
    if (!CanRender())
        return;

    if (m_isDynamic)
    {
        for (uint32 meshIndex = 0; meshIndex < m_dynamicMeshes.size(); ++meshIndex)
        {
            m_dynamicMeshes[meshIndex].Draw(false);
        }
    }
    else
    {
        for (uint32 meshIndex = 0; meshIndex < m_staticMeshes.size(); ++meshIndex)
        {
            m_staticMeshes[meshIndex].Draw(false);
        }
    }
}

void engine::Model::Draw(const std::vector<ResourceRef<MeshMaterial>>& materials) const
{
    if (!CanRender())
        return;

    if (m_isDynamic)
    {
        for (uint32 meshIndex = 0; meshIndex < m_dynamicMeshes.size(); ++meshIndex)
        {
            bool useDefaultMat = true;

            if (meshIndex < static_cast<uint32>(materials.size()))
            {
                const ResourceRef<MeshMaterial>& currentMaterial = materials[meshIndex];

                if (currentMaterial)
                {
                    currentMaterial->Use(0);
                    useDefaultMat = false;
                }
            }

            m_dynamicMeshes[meshIndex].Draw(useDefaultMat);
        }

        // Unbind skinning matrix buffer
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, 0);
    }
    else
    {
        for (uint32 meshIndex = 0; meshIndex < m_staticMeshes.size(); ++meshIndex)
        {
            bool useDefaultMat = true;

            if (meshIndex < static_cast<uint32>(materials.size()))
            {
                const ResourceRef<MeshMaterial>& currentMaterial = materials[meshIndex];

                if (currentMaterial)
                {
                    currentMaterial->Use(0);
                    useDefaultMat = false;
                }
            }

            m_staticMeshes[meshIndex].Draw(useDefaultMat);
        }
    }

}

uint32 engine::Model::GetMeshCount(void) const
{
    if (m_isDynamic)
        return static_cast<uint32>(m_dynamicMeshes.size());

    else
        return static_cast<uint32>(m_staticMeshes.size());
}

std::string engine::Model::GetName(void) const
{
    return m_modelName;
}

uint64 engine::Model::GetBoneCount(void) const
{
    return m_boneCount;
}

const std::vector<engine::Mesh>& engine::Model::GetStaticMeshes(void) const
{
    return m_staticMeshes;
}

const std::vector<engine::DynamicMesh>& engine::Model::GetDynamicMeshes(void) const
{
    return m_dynamicMeshes;
}

void engine::Model::ProcessImportedData(const void* data, const std::string& name)
{
    const aiScene* scene = static_cast<const aiScene*>(data);

    // Error management
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::printf("Failed to load model '%s'. Error: %s\n", name.c_str(), importer::g_importer.GetErrorString());
        m_loadStatus |= FAILED;
        return;
    }

    for (uint32 numMesh = 0; numMesh < scene->mNumMeshes; ++numMesh)
    {
        if (scene->mMeshes[numMesh]->HasBones())
        {
            m_isDynamic = true;
            break;
        }
    }

    std::string dir = GetDirectory(name);

    ProcessTextures(scene);
    ProcessMeshes(scene, scene->mRootNode, dir);


    if (scene->HasAnimations())
        Animation::LoadExtraAnimations(scene);

    for (DynamicMesh& mesh : m_dynamicMeshes)
        m_boneCount += mesh.GetBoneCount();

    m_loadStatus |= LOADED;

    // Send OpenGL setup to render thread
    ThreadManager::AddTask<ThreadManager::ETaskType::GRAPHICS>(&Model::RenderThreadSetup, this);
    importer::g_importer.FreeScene();
}

void engine::Model::ProcessTextures(const void* scene)
{
    const aiScene* sceneImpl = reinterpret_cast<const aiScene*>(scene);

    for (uint32 textureIndex = 0; textureIndex < sceneImpl->mNumTextures; ++textureIndex)
    {
        aiTexture* texture = sceneImpl->mTextures[textureIndex];
        int32 size = (texture->mHeight) ? texture->mWidth * texture->mHeight : texture->mWidth;

        void* data;
        int32 height;
        int32 width;
        int32 channels;

        if (!texture->mHeight)
        {
            data = stbi_load_from_memory((unsigned char*)texture->pcData, size, &width, &height, &channels, 4);

            if (!data)
                continue;
        }

        // aiTexture data will be deleted once we finish importing the model,
        // so we need our own copy to pass relevant data to our render thread
        else
        {
            data = malloc(size * sizeof(*texture->pcData));

            if (!data)
                continue;

            memcpy(data, texture->pcData, size);
            width = texture->mWidth;
            height = texture->mHeight;
        }

        ThreadManager::AddTask<ThreadManager::ETaskType::GRAPHICS>(
            [data, width, height,
             name = std::string(texture->mFilename.C_Str()),
             isLoadedWithSTB = (bool) texture->mHeight]
            {
                // Assimp texture data always uses argb8888 format, so we can pass 4 as
                // channel count
                ResourceManager::CreateFromData<Texture>(name, data, width, height, 4);

                if (isLoadedWithSTB)
                    stbi_image_free(data);
                else
                    free(data);
            }
        );
    }
}

std::string engine::Model::GetDirectory(const std::string& modelPath)
{
    char separator;
    uint64 len = modelPath.size() - 1;

    if (modelPath.find('\\') != std::string::npos)
        separator = '\\';

    else if (modelPath.find('/') != std::string::npos)
        separator = '/';

    else
        return {};

    // find mesh dir
    while (modelPath[--len] != separator);

    return modelPath.substr(0, len + 1);
}

void engine::Model::ProcessMeshes(const void* scene, const void* node, const std::string& name)
{
    const aiScene*  sceneImpl = reinterpret_cast<const aiScene*>(scene);
    const aiNode*   nodeImpl = reinterpret_cast<const aiNode*>(node);


    std::vector<std::future<void>> meshThreads;
    meshThreads.reserve(nodeImpl->mNumMeshes);
    uint64 oldSize = 0;

    if (m_isDynamic)
    {
        oldSize = m_dynamicMeshes.size();
        m_dynamicMeshes.resize(oldSize + nodeImpl->mNumMeshes);
    }
    else
    {
        oldSize = m_staticMeshes.size();
        m_staticMeshes.resize(oldSize + nodeImpl->mNumMeshes);
    }

    for (uint32 meshIndex = 0; meshIndex < nodeImpl->mNumMeshes; ++meshIndex)
    {     
        meshThreads.emplace_back(ThreadManager::AddTaskWithResult(
          [nodeImpl, meshIndex, sceneImpl, &name, this, index = oldSize + meshIndex]()
           {

            // Use a pointer to pick between static and dynamic mesh without needing
            // to initialize it right away like a reference
            Mesh* mesh;

            if (m_isDynamic)
                mesh = &m_dynamicMeshes[index];
            else
                mesh = &m_staticMeshes[index];

            aiMesh* importedMesh = sceneImpl->mMeshes[nodeImpl->mMeshes[meshIndex]];
            mesh->ProcessMesh(importedMesh);
            mesh->ProcessMaterial(sceneImpl->mMaterials[importedMesh->mMaterialIndex], name);

            }));
        
        
    }

    for (std::future<void>& finishedMesh : meshThreads)
    {
        if (finishedMesh.valid())
            finishedMesh.get();
        else
            return;
    }

    for (uint32 childIndex = 0; childIndex < nodeImpl->mNumChildren; ++childIndex)
    {
        ProcessMeshes(scene, nodeImpl->mChildren[childIndex], name);

        if (ThreadManager::IsShutDown())
            return;
    }
}

void engine::Model::WorkerThreadLoad(const std::string& name)
{
    // Read file
    const aiScene* scene = importer::g_importer.ReadFile(name,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData 
    );

    ProcessImportedData(scene, name);
}

void engine::Model::RenderThreadSetup(void)
{
    if (m_isDynamic)
    {
        for (DynamicMesh& mesh : m_dynamicMeshes)
        {
            mesh.SetupGraphics();
            ThreadManager::AddTask([&mesh]() { mesh.RenderThreadSkeletonSetup(); });
        }
    }
    else
    {
        for (Mesh& mesh : m_staticMeshes)
            mesh.SetupGraphics();
    }

    m_loadStatus |= GRAPHICS_CALLS_COMPLETE;
}
