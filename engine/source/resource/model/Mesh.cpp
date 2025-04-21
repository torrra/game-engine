#include "resource/model/Mesh.h"
#include "resource/model/Buffer.h"
#include "resource/texture/Texture.h"
#include "resource/ResourceManager.h"
#include "thread/ThreadManager.h"
#include "serialization/TextSerializer.h"

#include <assimp/mesh.h>
#include <assimp/vector3.h>
#include <assimp/scene.h>

#include <glad/glad.h>

engine::Mesh::Mesh(void)
{
    constexpr uint64 pathNum = sizeof(m_material.m_textureMaps) / sizeof(m_material.m_textureMaps[0]);
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

    m_indexCount = static_cast<uint32>(m_indices.size());
}

uint64 engine::Mesh::GetVertexStride(void)
{
    return sizeof(aiVector3D);
}

void engine::Mesh::DeleteMesh(void)
{
    m_positionVBO.DeleteData();
    m_attributesVBO.DeleteData();
    m_ebo.DeleteData();
    m_material.m_materialSSBO.DeleteData();
    glDeleteVertexArrays(1, &m_vao);
}

void engine::Mesh::SetupGraphics(void)
{
    glCreateVertexArrays(1, &m_vao);
    CreateVBO();
    CreateEBO();

    uint32 stride = SetAttributes();

    glVertexArrayElementBuffer(m_vao, m_ebo.GetBufferID());
    glVertexArrayVertexBuffer(m_vao, 0, m_positionVBO.GetBufferID(), 0, sizeof(aiVector3D));
    glVertexArrayVertexBuffer(m_vao, 1, m_attributesVBO.GetBufferID(), 0, stride);

    ImportTexturesFromMaterial();
    m_material.InitBuffer();

    PostLoad();
}

uint32 engine::Mesh::SetAttributes(void)
{
    uint32 relativeOffset = 0;

    // Position
    glEnableVertexArrayAttrib(m_vao, 0);
    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_vao, 0, 0);

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
    glVertexArrayAttribBinding(m_vao, index, 1);
}

void engine::Mesh::CreateVBO(void)
{
    m_positionVBO.Init();

    // Size
    uint64 posSize = m_vertices.size() * sizeof(f32);
    uint64 attribsSize = m_vertexAttributes.size() * sizeof(f32);

    // Set data
    m_positionVBO.SetData(m_vertices.data(), posSize);

    if (attribsSize)
    {
        m_attributesVBO.Init();
        m_attributesVBO.SetData(m_vertexAttributes.data(), attribsSize);
    }

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
    m_vertexAttributes.clear();
    m_vertexAttributes.shrink_to_fit();
}

void engine::Mesh::ProcessVertices(const void* mesh)
{
    const aiMesh* meshImpl = reinterpret_cast<const aiMesh*>(mesh);
    constexpr uint64 sizeVec3 = sizeof(aiVector3D) / sizeof(f32);
    constexpr uint64 sizeVec2 = sizeof(aiVector2D) / sizeof(f32);
    constexpr uint64 sizeColor = sizeof(aiColor4D) / sizeof(f32);

    for (uint32 vertexNum = 0; vertexNum < meshImpl->mNumVertices; ++vertexNum)
    {
        // Position
        aiVector3D& vertex = meshImpl->mVertices[vertexNum];
        m_vertices.insert(m_vertices.end(), &vertex.x, &vertex.x + sizeVec3);

        // Normal
        if (m_metaData.m_hasNormals)
        {
            aiVector3D& normal = meshImpl->mNormals[vertexNum];
            m_vertexAttributes.insert(m_vertexAttributes.end(), &normal.x, &normal.x + sizeVec3);
        }
        // Tangent / biTangents
        if (m_metaData.m_hasTangents)
        {
            aiVector3D& tangent = meshImpl->mTangents[vertexNum];
            aiVector3D& biTangent = meshImpl->mBitangents[vertexNum];
            m_vertexAttributes.insert(m_vertexAttributes.end(), &tangent.x, &tangent.x + sizeVec3);
            m_vertexAttributes.insert(m_vertexAttributes.end(), 
                                      &biTangent.x, &biTangent.x + sizeVec3);
        }
        // TexCoords
        if (meshImpl->mTextureCoords[0])
        {
            aiVector3D& uv = meshImpl->mTextureCoords[0][vertexNum];
            m_vertexAttributes.insert(m_vertexAttributes.end(), &uv.x, &uv.x + sizeVec2);
        }
        if (meshImpl->mColors[0])
        {
            aiColor4D& color = meshImpl->mColors[0][vertexNum];
            m_vertexAttributes.insert(m_vertexAttributes.end(), &color.r, &color.r + sizeColor);
        }
    }
}

void engine::Mesh::ProcessMaterial(const void* material, const std::string& dir)
{
     const aiMaterial* matImpl = reinterpret_cast<const aiMaterial*>(material);

     matImpl->Get(AI_MATKEY_COLOR_DIFFUSE,
                  *reinterpret_cast<aiColor3D*>(&m_material.m_data.m_diffuse));

     matImpl->Get(AI_MATKEY_COLOR_AMBIENT,
                  *reinterpret_cast<aiColor3D*>(&m_material.m_data.m_ambient));

     matImpl->Get(AI_MATKEY_COLOR_SPECULAR,
                  *reinterpret_cast<aiColor3D*>(&m_material.m_data.m_specular));

     matImpl->Get(AI_MATKEY_SHININESS, m_material.m_data.m_shininess);
     matImpl->Get(AI_MATKEY_REFRACTI, m_material.m_data.m_refractionIndex);

     if (matImpl->GetTextureCount(aiTextureType_DIFFUSE) > 0)
         StoreTexturePath(material, MeshMaterial::DIFFUSE, dir);

     if (matImpl->GetTextureCount(aiTextureType_NORMALS) > 0)
         StoreTexturePath(material, MeshMaterial::NORMAL, dir);

     if (matImpl->GetTextureCount(aiTextureType_SPECULAR) > 0)
         StoreTexturePath(material, MeshMaterial::SPECULAR, dir);

     if (matImpl->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
         StoreTexturePath(material, MeshMaterial::ROUGHNESS, dir);

     if (matImpl->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
         StoreTexturePath(material, MeshMaterial::AMBIENT_OCCLUSION, dir);
}

void engine::Mesh::ImportTexturesFromMaterial()
{
    constexpr uint32 pathNum = static_cast<uint32>(sizeof(m_material.m_textureMaps) /
                                                   sizeof(m_material.m_textureMaps[0]));

    for (uint64 pathIndex = 0; pathIndex < pathNum; ++pathIndex)
    {
        if (m_texturePaths[pathIndex].empty())
            continue;

        ResourceManager::Load<Texture>(m_texturePaths[pathIndex]);

        m_material.m_textureMaps[pathIndex] =
        ResourceManager::GetResource<Texture>(m_texturePaths[pathIndex]);

    }

    delete[] m_texturePaths;
    m_texturePaths = nullptr;

}

void engine::Mesh::StoreTexturePath(const void* material, MeshMaterial::EMapIndex index, const std::string& dir)
{
    const aiMaterial* matImpl = reinterpret_cast<const aiMaterial*>(material);
    aiTextureType type;
    aiString path;

    switch (index)
    {
    case MeshMaterial::DIFFUSE:
        type = aiTextureType_DIFFUSE;
        break;

    case MeshMaterial::NORMAL:
        type = aiTextureType_NORMALS;
        break;

    case MeshMaterial::SPECULAR:
        type = aiTextureType_SPECULAR;
        break;

    case MeshMaterial::ROUGHNESS:
        type = aiTextureType_DIFFUSE_ROUGHNESS;
        break;

    case MeshMaterial::AMBIENT_OCCLUSION:
        type = aiTextureType_AMBIENT_OCCLUSION;
        break;
    default:
        return;
    }

    // Store path to load later from render thread
    matImpl->GetTexture(type, 0, &path);
    m_texturePaths[index] = dir + path.C_Str();
}

uint32 engine::Mesh::GetVertexArrayID(void) const
{
    return m_vao;
}

uint32 engine::Mesh::GetIndexCount(void) const
{
    return m_indexCount;
}

engine::MeshMaterial& engine::Mesh::Material(void)
{
    return m_material;
}

const engine::MeshMaterial& engine::Mesh::GetMaterial(void) const
{
    return m_material;
}


const f32* engine::Mesh::GetVertices(void) const
{
    return m_vertices.data();
}

const uint32* engine::Mesh::GetIndices(void) const
{
    return m_indices.data();
}

void engine::Mesh::Draw(bool useMaterial) const
{
    if (useMaterial)
        m_material.Use(0);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}
