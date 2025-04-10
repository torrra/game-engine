#pragma once

#include "engine/CoreTypes.h"
#include "engine/resource/model/Buffer.h"

#include <math/Vector3.hpp>
#include <math/Vector2.hpp>

#include <vector>
#include <string>

struct aiMesh;


namespace engine
{
    struct MeshMaterial
    {
        math::Vector3f m_ambient{0.f};
        math::Vector3f m_diffuse{0.f};
        math::Vector3f m_specular{0.f};
        math::Vector3f m_emissive{0.f};

        f32             m_refractionIndex = 1.f;
        f32             m_opacity = 1.f;
    };

    class Mesh
    {
    protected:

        struct MeshMetaData
        {
            bool m_hasDiffuseMap : 1 = false;
            bool m_hasNormalMap : 1 = false;
            bool m_hasSpecularMap : 1 = false;
            bool m_hasRoughnessMap : 1 = false;

            // Ambient occlusion map
            bool m_hasAOMap : 1 = false;

            // Non-texture material
            bool m_hasMaterial : 1 = false;

            // Skeleton
            bool m_hasBones : 1 = false;

            // Vertex attributes

            // texture coordinates
            bool m_hasUV : 1 = false;

            bool m_hasNormals = false;

            // tangent and bi tangent
            bool m_hasTangents = false;
            bool m_hasVertexColors : 1 = false;
        };

        enum EMapIndex
        {
            DIFFUSE           = 0,
            NORMAL            = 1,
            SPECULAR          = 2,
            ROUGHNESS         = 3,
            AMBIENT_OCCLUSION = 4
        };

    public:

                    Mesh(void);
                    Mesh(Mesh&&) noexcept = default;
                    Mesh(const Mesh&) = delete;
        virtual     ~Mesh(void) = default;
                

        uint32      GetVertexArrayID(void) const;
        uint32      GetIndexCount(void) const;

        const f32*      GetVertices(void) const;
        const uint32*   GetIndices(void) const;
        uint64          GetVertexStride(void);

        void        UseTextureMaps(void) const;

       virtual void        Draw(void) const;

    protected:

        virtual void		ProcessMesh(const void* mesh);
        virtual void        DeleteMesh(void);
        virtual void        SetupGraphics(void);

        uint32		SetAttributes(void);
        void		SetAttribute(uint32 index, int32 size, uint32 relativeOffset) const;
        void		CreateVBO(void);
        void		CreateEBO(void);
        void		PostLoad(void);

        void		ProcessVertices(const aiMesh* mesh);
        void        ProcessMaterial(const void* material, const std::string& name);

        void        ImportTexturesFromMaterial();

        MeshMaterial            m_material;
        const class Texture*    m_maps[5]{ nullptr };
        std::string*            m_texturePaths = nullptr;
        std::vector<f32>		m_vertices;
        std::vector<f32>        m_vertexAttributes;
        std::vector<uint32>		m_indices;

        uint32					m_indexCount;
        uint32					m_vao;

        Buffer                  m_positionVBO = 0;
        Buffer                  m_attributesVBO = 0;
        Buffer                  m_ebo = 0;

        MeshMetaData			m_metaData;

        friend class Model;
    };
}
