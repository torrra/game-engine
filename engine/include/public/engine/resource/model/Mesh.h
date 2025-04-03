#pragma once

#include "engine/CoreTypes.h"
#include "engine/resource/model/Buffer.h"

#include <math/Vector3.hpp>
#include <math/Vector2.hpp>

#include <vector>

struct aiMesh;


namespace engine
{
    struct MeshMaterial
    {
        math::Vector3f m_ambient;
        math::Vector3f m_diffuse;
        math::Vector3f m_specular;
        math::Vector3f m_emissive;

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
            bool m_hasVertexColors = false;

        };

        enum class EMapIndex
        {
            DIFFUSE           = 0,
            NORMAL            = 1,
            SPECULAR          = 2,
            ROUGHNESS         = 3,
            AMBIENT_OCCLUSION = 4
        };

    public:

                    Mesh(void) = default;
        virtual     ~Mesh(void);

        uint32      GetVertexArrayID(void) const;
        uint32      GetIndexCount(void) const;

    protected:

        virtual void		ProcessMesh(const void* mesh);

        void        SetupBuffers(void);
        uint32		SetAttributes(void);
        void		SetAttribute(uint32 index, int32 size, uint32 stride) const;
        void		CreateVBO(void);
        void		CreateEBO(void);
        void		PostLoad(void);

        void		ProcessVertices(const aiMesh* mesh);
        //void        ProcessMaterial(const void* material);

        MeshMaterial            m_material;
        class Texture*          m_maps[5]{ nullptr };
        std::vector<f32>		m_vertices;
        std::vector<int32>		m_indices;

        uint32					m_indexCount;
        uint32					m_vao;
        Buffer                  m_vbo = 0;
        Buffer                  m_ebo = 0;

        MeshMetaData			m_metaData;

        friend class Model;
    };
}
