#pragma once

#include <math/Matrix4.hpp>

#include "engine/CoreTypes.h"
#include "engine/resource/model/Mesh.h"

namespace engine
{
    struct BoneWeight
    {
        math::Vector4<uint32>   m_boneIndices{ 0 };
        math::Vector4f          m_weights{ 0.f };
        uint32                  m_weightCount = 0;
    };

    struct Bone
    {
        math::Matrix4f  m_inverseBindPose{ 1.f };
        std::string     m_name;
        uint64		    m_parent = static_cast<uint64>(-1);
    };


    class DynamicMesh final : public Mesh
    {
    public:

        void ProcessMesh(const void* mesh) override;

    private:

        uint64 GetVertexCount(void);

        std::vector<Bone>           m_skeleton;
        std::vector<BoneWeight>     m_weights;
        Buffer                      m_skeletonVBO = 0;
    };
}