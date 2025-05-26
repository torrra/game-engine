#include "resource/material/MeshMaterial.h"
#include "resource/texture/Texture.h"
#include "resource/ResourceManager.h"
#include "serialization/TextSerializer.h"
#include "thread/ThreadManager.h"
#include "utility/ResourceRef.h"

#include <fstream>

#include "glad/glad.h"

constexpr const char g_materialExtension[] = ".mmat";

engine::MeshMaterial::MeshMaterial(const MeshMaterial& other)
{
    m_materialSSBO.Init();
    m_data = other.m_data;
    memcpy(m_textureMaps, other.m_textureMaps, sizeof(m_textureMaps));
    m_filePath = other.m_filePath;
    m_filePath.replace_filename("Copy-") += other.m_filePath.filename();

    SerializeText();
}

engine::MeshMaterial::~MeshMaterial(void)
{
    m_materialSSBO.DeleteData();
}

void engine::MeshMaterial::InitBuffer(void)
{
    m_materialSSBO.Init();
    m_materialSSBO.SetData(&m_data, sizeof(m_data));
}

bool engine::MeshMaterial::LoadResource(const char* filename)
{
   m_filePath = filename;

    std::ifstream input(filename, std::ios::in | std::ios::binary);

    if (!input)
        return false;

    DeserializeText(input);
    return true;
}
    

void engine::MeshMaterial::Use(uint32 index) const
{
    constexpr uint32 mapArraySize = sizeof(m_textureMaps) / sizeof(m_textureMaps[0]);

    for (uint32 textureID = 0; textureID < mapArraySize; ++textureID)
    {
        if (m_textureMaps[textureID])
            m_textureMaps[textureID]->UseTexture(textureID);
        else
            Texture::RemoveTexture(textureID);
    }

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_materialSSBO.GetBufferID());
}

void engine::MeshMaterial::SetAmbient(const math::Vector3f& value)
{
    m_data.m_ambient = value;
    glNamedBufferSubData(m_materialSSBO.GetBufferID(), offsetof(BufferData, BufferData::m_ambient),
                         sizeof(math::Vector3f), &m_data.m_ambient);
}

void engine::MeshMaterial::SetDiffuse(const math::Vector3f& value)
{
    m_data.m_diffuse = value;
    glNamedBufferSubData(m_materialSSBO.GetBufferID(), offsetof(BufferData, BufferData::m_diffuse),
        sizeof(m_data.m_diffuse), &m_data.m_diffuse);
}

void engine::MeshMaterial::SetSpecular(const math::Vector3f& value)
{
    m_data.m_specular = value;
    glNamedBufferSubData(m_materialSSBO.GetBufferID(), offsetof(BufferData, BufferData::m_specular),
        sizeof(m_data.m_specular), &m_data.m_specular);
}

void engine::MeshMaterial::SetEmissive(const math::Vector3f& value)
{
    m_data.m_emissive = value;
    glNamedBufferSubData(m_materialSSBO.GetBufferID(), offsetof(BufferData, BufferData::m_emissive),
        sizeof(m_data.m_emissive), &m_data.m_emissive);
}


void engine::MeshMaterial::SetDiffuseMap(ResourceRef<Texture>&& texture)
{
    m_textureMaps[DIFFUSE] = std::forward<ResourceRef<Texture>>(texture);
}

void engine::MeshMaterial::SetNormalMap(ResourceRef<Texture>&& texture)
{
    m_textureMaps[NORMAL] = std::forward<ResourceRef<Texture>>(texture);
}

void engine::MeshMaterial::SetSpecularMap(ResourceRef<Texture>&& texture)
{
    m_textureMaps[SPECULAR] = std::forward<ResourceRef<Texture>>(texture);
}

void engine::MeshMaterial::SetDiffuseRoughnessMap(ResourceRef<Texture>&& texture)
{
    m_textureMaps[ROUGHNESS] = std::forward<ResourceRef<Texture>>(texture);
}

void engine::MeshMaterial::SetAmbientOcclusionMap(ResourceRef<Texture>&& texture)
{
    m_textureMaps[AMBIENT_OCCLUSION] = std::forward<ResourceRef<Texture>>(texture);
}

void engine::MeshMaterial::SetShininess(f32 value)
{
    m_data.m_shininess = value;
    glNamedBufferSubData(m_materialSSBO.GetBufferID(), offsetof(BufferData, BufferData::m_shininess),
        sizeof(m_data.m_shininess), &m_data.m_shininess);
}

void engine::MeshMaterial::SetRefractionIndex(f32 value)
{
    m_data.m_refractionIndex = value;
    glNamedBufferSubData(m_materialSSBO.GetBufferID(), offsetof(BufferData, BufferData::m_refractionIndex),
        sizeof(m_data.m_refractionIndex), &m_data.m_refractionIndex);
}

const math::Vector3f& engine::MeshMaterial::GetAmbient(void) const
{
    return m_data.m_ambient;
}

const math::Vector3f& engine::MeshMaterial::GetDiffuse(void) const
{
    return m_data.m_diffuse;
}

const math::Vector3f& engine::MeshMaterial::GeSpecular(void) const
{
    return m_data.m_specular;
}

const math::Vector3f& engine::MeshMaterial::GetEmissive(void) const
{
    return m_data.m_emissive;
}

f32 engine::MeshMaterial::GetShininess(void) const
{
    return m_data.m_shininess;
}

f32 engine::MeshMaterial::GetRefractionIndex(void) const
{
    return m_data.m_refractionIndex;
}

const engine::ResourceRef<engine::Texture>& engine::MeshMaterial::GetDiffuseMap(void) const
{
    return m_textureMaps[DIFFUSE];
}

const engine::ResourceRef<engine::Texture>& engine::MeshMaterial::GetNormalMap(void) const
{
    return m_textureMaps[DIFFUSE];
}

const engine::ResourceRef<engine::Texture>& engine::MeshMaterial::GetSpecularMap(void) const
{
    return m_textureMaps[SPECULAR];
}

const engine::ResourceRef<engine::Texture>& engine::MeshMaterial::GetDiffuseRoughnessMap(void) const
{
    return m_textureMaps[ROUGHNESS];
}

const engine::ResourceRef<engine::Texture>& engine::MeshMaterial::GetAmbientOcclusionMap(void) const
{
    return m_textureMaps[AMBIENT_OCCLUSION];
}

void engine::MeshMaterial::SerializeText(void) const
{
    std::ofstream output(m_filePath, std::ios::out | std::ios::trunc);

    output << "[" << m_filePath.filename() << "]\n    ";
    text::Serialize(output, "ambient", m_data.m_ambient);
    output << "\n    ";
    text::Serialize(output, "diffuse", m_data.m_diffuse);
    output << "\n    ";
    text::Serialize(output, "specular", m_data.m_specular);
    output << "\n    ";
    text::Serialize(output, "emissive", m_data.m_emissive);
    output << "\n    ";
    text::Serialize(output, "shininess", m_data.m_shininess);
    output << "\n    ";
    text::Serialize(output, "refractionIndex", m_data.m_refractionIndex);
    output << "\n    ";

    SerializeTexturePaths(output);

}

void engine::MeshMaterial::DeserializeText(std::ifstream& input)
{
    const char* start;
    const char* end;
    const char* text = text::LoadFileData(input, start, end);

    MOVE_TEXT_CURSOR_FREE(text, start, end);
    text = text::DeserializeVector(text, m_data.m_ambient);

    MOVE_TEXT_CURSOR_FREE(text, start, end);
    text = text::DeserializeVector(text, m_data.m_diffuse);

    MOVE_TEXT_CURSOR_FREE(text, start, end);
    text = text::DeserializeVector(text, m_data.m_specular);

    MOVE_TEXT_CURSOR_FREE(text, start, end);
    text = text::DeserializeVector(text, m_data.m_emissive);

    MOVE_TEXT_CURSOR_FREE(text, start, end);
    text = text::DeserializeReal(text, m_data.m_shininess);

    MOVE_TEXT_CURSOR_FREE(text, start, end);
    text = text::DeserializeReal(text, m_data.m_refractionIndex);

    MOVE_TEXT_CURSOR_FREE(text, start, end);
    text = DeserializeTextureMaps(text, end);

    text::UnloadFileData(start);
}

void engine::MeshMaterial::MoveFile(const std::filesystem::path& newPath)
{
    std::filesystem::path name = m_filePath.filename();
    m_filePath = newPath;
    m_filePath += name;
}

const std::filesystem::path& engine::MeshMaterial::GetFilePath(void) const
{
    return m_filePath;
}

engine::EditableRef<engine::MeshMaterial> engine::MeshMaterial::CreateMaterial(const char* path)
{
    std::string pathStr = path;

    MeshMaterial mat;
    mat.m_filePath = path;

    mat.InitBuffer();
    mat.SerializeText();

    ResourceManager::CreateFromData<MeshMaterial>(pathStr, std::move(mat));
    return ResourceManager::GetEditableResource<MeshMaterial>(pathStr);
}

void engine::MeshMaterial::SerializeTexturePaths(std::ostream& output) const
{
    text::Serialize(output, "hasDiffuseMap", (bool)m_textureMaps[DIFFUSE]);

    if (m_textureMaps[DIFFUSE])
    {
        output << "\n    ";
        if (const std::string* key = ResourceManager::FindKeyByVal(m_textureMaps[DIFFUSE]))
            text::Serialize(output, "diffuseMap", *key);
    }


    output << "\n    ";
    text::Serialize(output, "hasNormalMap", (bool)m_textureMaps[NORMAL]);

    if (m_textureMaps[NORMAL])
    {
        output << "\n    ";
        if (const std::string* key = ResourceManager::FindKeyByVal(m_textureMaps[NORMAL]))
            text::Serialize(output, "normalMap", *key);
    }

    output << "\n    ";
    text::Serialize(output, "hasSpecularMap", (bool)m_textureMaps[SPECULAR]);

    if (m_textureMaps[SPECULAR])
    {
        output << "\n    ";
        if (const std::string* key = ResourceManager::FindKeyByVal(m_textureMaps[SPECULAR]))
            text::Serialize(output, "specularMap", *key);
    }

    output << "\n    ";
    text::Serialize(output, "hasAmbientOcclusionMap", (bool)m_textureMaps[AMBIENT_OCCLUSION]);

    if (m_textureMaps[AMBIENT_OCCLUSION])
    {
        output << "\n    ";
        if (const std::string* key = ResourceManager::FindKeyByVal(m_textureMaps[AMBIENT_OCCLUSION]))
            text::Serialize(output, "ambientOcclusionMap", *key);
    }

    output << "\n    ";
    text::Serialize(output, "hasDiffuseRoughnessMap", (bool)m_textureMaps[ROUGHNESS]);

    if (m_textureMaps[ROUGHNESS])
    {
        output << "\n    ";
        if (const std::string* key = ResourceManager::FindKeyByVal(m_textureMaps[ROUGHNESS]))
            text::Serialize(output, "diffuseRoughnessMap", *key);
    }
}

const char* engine::MeshMaterial::DeserializeTextureMaps(const char* text, const char* end)
{
    constexpr uint64 pathCount = sizeof(m_textureMaps) / sizeof(m_textureMaps[1]);

    std::string* paths = new std::string[pathCount];

    for (uint64 pathIndex = 0; pathIndex < pathCount; ++pathIndex)
        text = DeserializePath(text, end, paths[pathIndex]);


    ThreadManager::AddTask<ThreadManager::ETaskType::GRAPHICS>([this, paths]()
        {
            for (uint64 pathIndex = 0; pathIndex < pathCount; ++pathIndex)
            {
                if (paths[pathIndex].empty())
                    continue;

                ResourceManager::Load<Texture>(paths[pathIndex]);
                m_textureMaps[pathIndex] =
                ResourceManager::GetResource<Texture>(paths[pathIndex]);
            }

            delete[] paths;
            InitBuffer();
        });

    return text;
}

const char* engine::MeshMaterial::DeserializePath(const char* text, const char* end, std::string& path)
{
    uint8 hasMap = false;

    text = text::DeserializeInteger(text, hasMap);

    if (hasMap)
        text = text::DeserializeString(text, end, path);

    MOVE_TEXT_CURSOR(text, end);

    return text;
}

