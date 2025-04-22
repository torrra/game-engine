#pragma once

#include "engine/EngineExport.h"
#include "engine/CoreTypes.h"
#include "engine/resource/model/Buffer.h"
#include "engine/resource/material/MeshMaterial.h"

#include <math/Vector3.hpp>
#include <math/Vector2.hpp>

#include <vector>
#include <string>
#include <iostream>

namespace engine
{
   

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


    public:


       ENGINE_API   Mesh(void);
       ENGINE_API   Mesh(Mesh&&) noexcept = default;
       ENGINE_API   Mesh(const Mesh&) = delete;

       ENGINE_API
       virtual     ~Mesh(void) = default;

       ENGINE_API MeshMaterial&        Material(void);
       ENGINE_API const MeshMaterial&  GetMaterial(void) const;
               
       ENGINE_API uint32               GetVertexArrayID(void) const;
       ENGINE_API uint32               GetIndexCount(void) const;
       ENGINE_API const f32*           GetVertices(void) const;
       ENGINE_API const uint32*        GetIndices(void) const;
       ENGINE_API uint64               GetVertexStride(void);     

       ENGINE_API virtual void          Draw(bool useMaterial) const;


    protected:

        virtual void		ProcessMesh(const void* mesh);
        virtual void        DeleteMesh(void);
        virtual void        SetupGraphics(void);

    private:

        uint32		SetAttributes(void);
        void		SetAttribute(uint32 index, int32 size, uint32 relativeOffset) const;
        void		CreateVBO(void);
        void		CreateEBO(void);
        void		PostLoad(void);

        void		ProcessVertices(const void* mesh);
        void        ProcessMaterial(const void* material, const std::string& dir);

        void        ImportTexturesFromMaterial(void);

        void        StoreTexturePath(const void* mesh, MeshMaterial::EMapIndex index,
                                     const std::string& dir);

       
        std::string*            m_texturePaths = nullptr;
        std::vector<f32>		m_vertices;
        std::vector<f32>        m_vertexAttributes;
        std::vector<uint32>		m_indices;

        MeshMaterial            m_material;

        uint32					m_indexCount = 0;

        Buffer                  m_positionVBO = 0;
        Buffer                  m_attributesVBO = 0;
        Buffer                  m_ebo = 0;       

    protected:

        uint32					m_vao = 0;
        MeshMetaData			m_metaData;

        friend class Model;
    };
}
