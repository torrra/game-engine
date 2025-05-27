#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "engine/resource/model/Buffer.h"
#include "engine/resource/Resource.h"
#include "engine/utility/ResourceRefDecl.h"

#include <math/Vector3.hpp>

#include <filesystem>

namespace engine
{
    class MeshMaterial : public IResource
    {
    public:

        enum EMapIndex
        {
            DIFFUSE,
            NORMAL,
            SPECULAR,
            ROUGHNESS,
            AMBIENT_OCCLUSION
        };

    private:

        enum EMapBits
        {
            DIFFUSE_BIT = 1 << DIFFUSE,
            NORMAL_BIT = 1 << NORMAL,
            SPECULAR_BIT = 1 << SPECULAR,
            ROUGHNESS_BIT = 1 << ROUGHNESS,
            AMBIENT_OCCLUSION_BIT = 1 << AMBIENT_OCCLUSION
        };

        struct BufferData
        {
            math::Vector3f          m_ambient{ 0.f };
            math::Vector3f          m_diffuse{ 0.f };
            math::Vector3f          m_specular{ 0.f };
            math::Vector3f          m_emissive{ 0.f };

            f32                     m_shininess = 1.f;
            f32                     m_refractionIndex = 1.f;
            f32                     m_opacity = 1.f;

            int32                   m_useMapsBits = 0;
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

        ENGINE_API void SetDiffuseMap(ResourceRef<class Texture>&& texture);
        ENGINE_API void SetNormalMap(ResourceRef<class Texture>&& texture);
        ENGINE_API void SetSpecularMap(ResourceRef<class Texture>&& texture);
        ENGINE_API void SetDiffuseRoughnessMap(ResourceRef<class Texture>&& texture);
        ENGINE_API void SetAmbientOcclusionMap(ResourceRef<class Texture>&& texture);

        ENGINE_API void SetOpacity(f32 value);
        ENGINE_API void SetShininess(f32 value);
        ENGINE_API void SetRefractionIndex(f32 value);

        ENGINE_API const math::Vector3f& GetAmbient(void) const;
        ENGINE_API const math::Vector3f& GetDiffuse(void) const;
        ENGINE_API const math::Vector3f& GetSpecular(void) const;
        ENGINE_API const math::Vector3f& GetEmissive(void) const;

        ENGINE_API f32 GetShininess(void) const;
        ENGINE_API f32 GetRefractionIndex(void) const;
        ENGINE_API f32 GetOpacity(void) const;

        ENGINE_API const ResourceRef<class Texture>& GetDiffuseMap(void) const;
        ENGINE_API const ResourceRef<class Texture>& GetNormalMap(void) const;
        ENGINE_API const ResourceRef<class Texture>& GetSpecularMap(void) const;
        ENGINE_API const ResourceRef<class Texture>& GetDiffuseRoughnessMap(void) const;
        ENGINE_API const ResourceRef<class Texture>& GetAmbientOcclusionMap(void) const;

        ENGINE_API const ResourceRef<class engine::Texture>* GetTextures(void);
        ENGINE_API void SetTexture(ResourceRef<Texture>&& texture, uint64 index);

        ENGINE_API void SerializeText(void) const;
        ENGINE_API void DeserializeText(std::ifstream& input);

        ENGINE_API void MoveFile(const std::filesystem::path& newPath);
        ENGINE_API const std::filesystem::path& GetFilePath(void) const;

        ENGINE_API static EditableRef<MeshMaterial> CreateMaterial(const char* path);

    private:

        void SetMapUsageBits(void);

        void SerializeTexturePaths(std::ostream& output) const;
        const char* DeserializeTextureMaps(const char* text, const char* end);
        const char* DeserializePath(const char* text, const char* end, std::string& path);

        ResourceRef<class Texture>    m_textureMaps[5]{};         
        std::filesystem::path         m_filePath;
        BufferData                    m_data;
        Buffer                        m_materialSSBO = 0;

        friend class Mesh;
    };
}