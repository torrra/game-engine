#pragma once

#include <math/Matrix4.hpp>

#include "engine/CoreTypes.h"
#include "engine/resource/model/Mesh.h"

#include <unordered_map>

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
        int64		    m_parent = -1;
    };


    class DynamicMesh final : public Mesh
    {

    private:

        using BoneIndexMap = std::unordered_map<void*, int64>;
        using BoneNameMap = std::unordered_map<uint64, int32>;

    public:

        
        DynamicMesh(void) = default;
        DynamicMesh(DynamicMesh&&) noexcept = default;
        DynamicMesh(const DynamicMesh&) = delete;
        ~DynamicMesh(void) = default;

        void ProcessMesh(const void* mesh) override;
        void DeleteMesh(void) override;
        void RenderThreadSkeletonSetup(void);

        const Bone& GetBone(int32 index) const;
        int32 GetBoneIndex(const std::string& name) const;

    private:

        void ProcessWeights(const void* bone, uint32 boneIndex);
        bool ProcessSkeleton(const void* mesh);
        void ProcessBone(Bone& newBone, void* bonePtr, const BoneIndexMap& nodes);
        void PopulateBoneNameMap(void);
        void SortSkeleton(std::vector<Bone>& boneArray);

        void SortChildrenBones(int32 oldParentIndex, int32 newIndex,
                               std::vector<Bone>& boneArray,   
                               std::vector<int32>& indexArray);
        void SetBoneIndexAttribute();
        void SetBoneWeightAttribute();
        void SetupSkeletonVertexBuffers(void* indexBuffer, uint64 indexBufSize,
                                        void* weightBuffer, uint64 weightBufSize);

        BoneNameMap                 m_boneMap;
        std::vector<Bone>           m_skeleton;
        std::vector<BoneWeight>     m_weights;
        Buffer                      m_boneIndexVBO = 0;
        Buffer                      m_boneWeightVBO = 0;
    };

    std::ostream& operator<<(std::ostream& lhs, const Bone& rhs);
}