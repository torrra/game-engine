#include "resource/model/Mesh.h"
#include "resource/model/Buffer.h"
#include "resource/texture/Texture.h"
#include "resource/ResourceManager.h"
#include "thread/ThreadManager.h"

#include <assimp/mesh.h>
#include <assimp/vector3.h>
#include <assimp/scene.h>

#include <glad/glad.h>

engine::Mesh::Mesh(void)
{
    constexpr uint64 pathNum = sizeof(m_maps) / sizeof(m_maps[0]);
    m_texturePaths = new std::string[pathNum];
}

void engine::Mesh::ProcessMesh(const void* mesh)
{
    const aiMesh* meshImpl = reinterpret_cast<const aiMesh*>(mesh);

    if (meshImpl->HasNormals())
        m_metaData.m_hasNormals = true;

    if (meshImpl->HasTangentsAndBitangents())
        m_metaData.m_hasTangents = true;

    if (meshImpl->mTextureCoords[0])
        m_metaData.m_hasUV = true;

    if (meshImpl->mColors[0])
        m_metaData.m_hasVertexColors = true;

    ProcessVertices(meshImpl);

    // Indices
    for (uint32 i = 0; i < meshImpl->mNumFaces; ++i)
    {
        aiFace face = meshImpl->mFaces[i];

        for (uint32 j = 0; j < face.mNumIndices; ++j)
        {
            m_indices.push_back(face.mIndices[j]);
        }
    }
}

void engine::Mesh::DeleteMesh(void)
{
    m_vbo.DeleteData();
    m_ebo.DeleteData();
    glDeleteVertexArrays(1, &m_vao);
}

void engine::Mesh::SetupGraphics(void)
{
    glCreateVertexArrays(1, &m_vao);
    CreateVBO();
    CreateEBO();

    uint32 stride = SetAttributes();

    glVertexArrayElementBuffer(m_vao, m_ebo.GetBufferID());
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo.GetBufferID(), 0, stride);

    ImportTexturesFromMaterial();
    PostLoad();
}

uint32 engine::Mesh::SetAttributes(void)
{
    uint32 relativeOffset = 0;

    // Position
    SetAttribute(0, 3, relativeOffset);
    relativeOffset += sizeof(aiVector3D);

    // Normal
    if (m_metaData.m_hasNormals)
    {
        SetAttribute(1, 3, relativeOffset);
        relativeOffset += sizeof(aiVector3D);
    }
    if (m_metaData.m_hasTangents)
    {
        // Tangent
        SetAttribute(2, 3, relativeOffset);
        relativeOffset += sizeof(aiVector3D);

        // BiTangent
        SetAttribute(3, 3, relativeOffset);
        relativeOffset += sizeof(aiVector3D);
    }
    // TexCoord
    if (m_metaData.m_hasUV)
    {
        SetAttribute(4, 2, relativeOffset);
        relativeOffset += sizeof(aiVector2D);
    }

    if (m_metaData.m_hasVertexColors)
    {
        SetAttribute(5, 4, relativeOffset);
        relativeOffset += sizeof(aiColor4D);
    }

    return relativeOffset;
}

void engine::Mesh::SetAttribute(uint32 index, int32 size, uint32 relativeOffset) const
{
    glEnableVertexArrayAttrib(m_vao, index);
    glVertexArrayAttribFormat(m_vao, index, size, GL_FLOAT, GL_FALSE, relativeOffset);
    glVertexArrayAttribBinding(m_vao, index, 0);
}

void engine::Mesh::CreateVBO(void)
{
    m_vbo.Init();

    // Size
    uint64 size = m_vertices.size() * sizeof(f32)/* * sizeof(Vertex)*/;

    // Set data
    m_vbo.SetData(m_vertices.data(), size);

}

void engine::Mesh::CreateEBO(void)
{
    m_ebo.Init();

    // Size
    uint64 size = m_indices.size() * sizeof(uint32);

    // Set data
    m_ebo.SetData(m_indices.data(), size);
}

void engine::Mesh::PostLoad(void)
{
    m_indexCount = static_cast<uint32>(m_indices.size());

    m_indices.clear();
    m_vertices.clear();

    m_indices.shrink_to_fit();
    m_vertices.shrink_to_fit();
}

void engine::Mesh::ProcessVertices(const aiMesh* mesh)
{
    constexpr uint64 sizeVec3 = sizeof(aiVector3D) / sizeof(f32);
    constexpr uint64 sizeVec2 = sizeof(aiVector2D) / sizeof(f32);
    constexpr uint64 sizeColor = sizeof(aiColor4D) / sizeof(f32);

    for (uint32 i = 0; i < mesh->mNumVertices; ++i)
    {
        // Position
        aiVector3D& vertex = mesh->mVertices[i];
        m_vertices.insert(m_vertices.end(), &vertex.x, &vertex.x + sizeVec3);

        // Normal
        if (m_metaData.m_hasNormals)
        {
            aiVector3D& normal = mesh->mNormals[i];
            m_vertices.insert(m_vertices.end(), &normal.x, &normal.x + sizeVec3);
        }
        // Tangent / biTangents
        if (m_metaData.m_hasTangents)
        {
            aiVector3D& tangent = mesh->mTangents[i];
            aiVector3D& biTangent = mesh->mBitangents[i];
            m_vertices.insert(m_vertices.end(), &tangent.x, &tangent.x + sizeVec3);
            m_vertices.insert(m_vertices.end(), &biTangent.x, &biTangent.x + sizeVec3);
        }
        // TexCoords
        if (mesh->mTextureCoords[0])
        {
            aiVector3D& uv = mesh->mTextureCoords[0][i];
            m_vertices.insert(m_vertices.end(), &uv.x, &uv.x + sizeVec2);
        }
        if (mesh->mColors[0])
        {
            aiColor4D& color = mesh->mColors[0][i];
            m_vertices.insert(m_vertices.end(), &color.r, &color.r + sizeColor);
        }
    }
}

void engine::Mesh::ProcessMaterial(const void* material, const std::string&/* name*/)
{
     const aiMaterial* matImpl = reinterpret_cast<const aiMaterial*>(material);

     matImpl->Get(AI_MATKEY_COLOR_DIFFUSE,
                  *reinterpret_cast<aiColor3D*>(&m_material.m_diffuse));

     matImpl->Get(AI_MATKEY_COLOR_AMBIENT,
                           *reinterpret_cast<aiColor3D*>(&m_material.m_ambient));

     matImpl->Get(AI_MATKEY_COLOR_SPECULAR,
                           *reinterpret_cast<aiColor3D*>(&m_material.m_specular));
     aiString texturePath;

     if (matImpl->GetTextureCount(aiTextureType_DIFFUSE) > 0)
     {
         matImpl->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
         m_texturePaths[DIFFUSE] = texturePath.C_Str();
         texturePath.Clear();
     }

     if (matImpl->GetTextureCount(aiTextureType_NORMALS) > 0)
     {
         matImpl->GetTexture(aiTextureType_NORMALS, 0, &texturePath);
         m_texturePaths[NORMAL] = texturePath.C_Str();
         texturePath.Clear();
     }

     if (matImpl->GetTextureCount(aiTextureType_SPECULAR) > 0)
     {
         matImpl->GetTexture(aiTextureType_NORMALS, 0, &texturePath);
         m_texturePaths[SPECULAR] = texturePath.C_Str();
         texturePath.Clear();
     }

     if (matImpl->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
     {
         matImpl->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &texturePath);
         m_texturePaths[ROUGHNESS] = texturePath.C_Str();
         texturePath.Clear();
     }

     if (matImpl->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
     {
         matImpl->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &texturePath);
         m_texturePaths[AMBIENT_OCCLUSION] = texturePath.C_Str();
         texturePath.Clear();
     }

}

void engine::Mesh::ImportTexturesFromMaterial()
{
    constexpr uint32 pathNum = static_cast<uint32>(sizeof(m_maps) / sizeof(m_maps[0]));

    for (uint64 pathIndex = 0; pathIndex < pathNum; ++pathIndex)
    {
        if (m_texturePaths[pathIndex].empty())
            continue;

        ResourceManager::Load<Texture>(m_texturePaths[pathIndex]);
        m_maps[pathIndex] = ResourceManager::GetResource<Texture>(m_texturePaths[pathIndex]);

    }

    delete[] m_texturePaths;
    m_texturePaths = nullptr;

}

uint32 engine::Mesh::GetVertexArrayID(void) const
{
    return m_vao;
}

uint32 engine::Mesh::GetIndexCount(void) const
{
    return m_indexCount;
}

void engine::Mesh::UseTextureMaps(void) const
{
    constexpr uint32 mapArraySize = sizeof(m_maps) / sizeof(Texture*);

    for (uint32 textureID = 0; textureID < mapArraySize; ++textureID)
    {
        if (m_maps[textureID])
            m_maps[textureID]->UseTexture(textureID);
        else
            Texture::RemoveTexture(textureID);
    }
}

void engine::Mesh::Draw(void) const
{
    UseTextureMaps();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
