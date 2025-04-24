#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "engine/resource/model/Buffer.h"
#include "engine/resource/Resource.h"

#include <math/Vector3.hpp>

#include <filesystem>
#include <iostream>

namespace engine
{
    class MeshMaterial : public IResource
    {
    private:

        enum EMapIndex
        {
            DIFFUSE,
            NORMAL,
            SPECULAR,
            ROUGHNESS,
            AMBIENT_OCCLUSION
        };

        struct BufferData
        {
            math::Vector3f          m_ambient{ 0.f };
            math::Vector3f          m_diffuse{ 0.f };
            math::Vector3f          m_specular{ 0.f };
            math::Vector3f          m_emissive{ 0.f };

            f32                     m_shininess = 1.f;
            f32                     m_refractionIndex = 1.f;
        };


    public:


        ENGINE_API MeshMaterial(void) = default;
        ENGINE_API MeshMaterial(const MeshMaterial& other);
        ENGINE_API MeshMaterial(MeshMaterial&& other) noexcept = default;
        ENGINE_API ~MeshMaterial(void);

        ENGINE_API void InitBuffer(void);
        ENGINE_API bool LoadResource(const char* filename) override;

        ENGINE_API void Use(uint32 index) const;

        ENGINE_API void SetAmbient(const math::Vector3f& value);
        ENGINE_API void SetDiffuse(const math::Vector3f& value);
        ENGINE_API void SetSpecular(const math::Vector3f& value);
        ENGINE_API void SetEmissive(const math::Vector3f& value);

        ENGINE_API void SetDiffuseMap(const class Texture* texture);
        ENGINE_API void SetNormalMap(const class Texture* texture);
        ENGINE_API void SetSpecularMap(const class Texture* texture);
        ENGINE_API void SetDiffuseRoughnessMap(const class Texture* texture);
        ENGINE_API void SetAmbientOcclusionMap(const class Texture* texture);

        ENGINE_API void SetShininess(f32 value);
        ENGINE_API void SetRefractionIndex(f32 value);

        ENGINE_API const math::Vector3f& GetAmbient(void) const;
        ENGINE_API const math::Vector3f& GetDiffuse(void) const;
        ENGINE_API const math::Vector3f& GeSpecular(void) const;
        ENGINE_API const math::Vector3f& GetEmissive(void) const;

        ENGINE_API f32 GetShininess(void) const;
        ENGINE_API f32 GetRefractionIndex(void) const;

        ENGINE_API const class Texture* GetDiffuseMap(void) const;
        ENGINE_API const class Texture* GetNormalMap(void) const;
        ENGINE_API const class Texture* GetSpecularMap(void) const;
        ENGINE_API const class Texture* GetDiffuseRoughnessMap(void) const;
        ENGINE_API const class Texture* GetAmbientOcclusionMap(void) const;

        ENGINE_API void SerializeText(void) const;
        ENGINE_API void DeserializeText(std::ifstream& input);

        ENGINE_API void MoveFile(const std::filesystem::path& newPath);
        ENGINE_API const std::filesystem::path& GetFilePath(void) const;

        ENGINE_API static MeshMaterial* CreateMaterial(const char* path);

    private:


        void SerializeTexturePaths(std::ostream& output) const;
        const char* DeserializeTextureMaps(const char* text, const char* end);
        const char* DeserializePath(const char* text, const char* end, std::string& path);

        const class Texture*    m_textureMaps[5]{ nullptr };         
        std::filesystem::path   m_filePath;
        BufferData              m_data;
        Buffer                  m_materialSSBO = 0;

        friend class Mesh;
    };
}