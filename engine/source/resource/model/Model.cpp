#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "resource/model/Model.h"
#include "resource/model/Buffer.h"
#include "resource/model/Vertex.h"

#include "engine/thread/ThreadManager.h"
#include "engine/resource/ResourceManager.h"
#include "engine/resource/texture/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

engine::Model::~Model(void)
{
    for (DynamicMesh& mesh : m_dynamicMeshes)
        mesh.DeleteMesh();
    
    for (Mesh& mesh : m_staticMeshes)
        mesh.DeleteMesh();
}

bool engine::Model::LoadResource(const char* fileName)
{
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

bool engine::Model::IsDynamic(void) const
{
    return m_isDynamic;
}

const std::vector<engine::Mesh>& engine::Model::GetStaticMeshes(void) const
{
    return m_staticMeshes;
}

const std::vector<engine::DynamicMesh>& engine::Model::GetDynamicMeshes(void) const
{
    return m_dynamicMeshes;
}

void engine::Model::ProcessTextures(const void* scene)
{
    const aiScene* sceneImpl = reinterpret_cast<const aiScene*>(scene);

    for (uint32 textureIndex = 0; textureIndex < sceneImpl->mNumTextures; ++textureIndex)
    {
        aiTexture* texture = sceneImpl->mTextures[textureIndex];
        int32 size = (texture->mHeight) ? texture->mWidth * texture->mHeight : texture->mWidth;

        // aiTexture data will be deleted once we finish importing the model,
        // so we need our own copy to pass relevant data to our render thread
        void* data = malloc(size * sizeof(*texture->pcData));

        if (!data)
            continue;

        memcpy(data, texture->pcData, size);

        ThreadManager::AddTask<ThreadManager::ETaskType::GRAPHICS>(
            [data, width = texture->mWidth,
             height = texture->mWidth,
             name = std::string(texture->mFilename.C_Str())]
            {
                // Assimp texture data always uses argb8888 format, so we can pass 4 as
                // channel count
                ResourceManager::CreateFromData<Texture>(name, data, width, height, 4);
                free(data);
            }
        );
    }
}

void engine::Model::ProcessMeshes(const void* scene, const void* node)
{
    const aiScene*  sceneImpl = reinterpret_cast<const aiScene*>(scene);
    const aiNode*   nodeImpl = reinterpret_cast<const aiNode*>(node);

    for (uint32 meshIndex = 0; meshIndex < nodeImpl->mNumMeshes; ++meshIndex)
    {
        // Use a pointer to pick between static and dynamic mesh without needing
        // to initialize it right away like a reference
        Mesh* mesh;
        
        if (m_isDynamic)
            mesh = &m_dynamicMeshes.emplace_back();
        else
            mesh = &m_staticMeshes.emplace_back();

        mesh->ProcessMesh(sceneImpl->mMeshes[nodeImpl->mMeshes[meshIndex]]);
    }

    for (uint32 childIndex = 0; childIndex < nodeImpl->mNumChildren; ++childIndex)
        ProcessMeshes(scene, nodeImpl->mChildren[childIndex]);
}

void engine::Model::WorkerThreadLoad(const std::string& name)
{
    // Read file
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(name,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_CalcTangentSpace
    );

    // Error management
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::printf("Failed to load model '%s'. Error: %s\n", name.c_str(), importer.GetErrorString());
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

    ProcessTextures(scene);  
    ProcessMeshes(scene, scene->mRootNode);
    m_loadStatus |= LOADED;

    // Send OpenGL setup to render thread
    ThreadManager::AddTask<ThreadManager::ETaskType::GRAPHICS>(&Model::RenderThreadSetup, this);
}

void engine::Model::RenderThreadSetup(void)
{
    if (m_isDynamic)
    {
        for (DynamicMesh& mesh : m_dynamicMeshes)
            mesh.SetupBuffers();
    }
    else
    {
        for (Mesh& mesh : m_staticMeshes)
            mesh.SetupBuffers();
    }

    m_loadStatus |= GRAPHICS_CALLS_COMPLETE;
}
