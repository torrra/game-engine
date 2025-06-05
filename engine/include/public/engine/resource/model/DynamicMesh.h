#pragma once

#include <math/Matrix4.hpp>

#include "engine/CoreTypes.h"
#include "engine/resource/model/Mesh.h"

#include <unordered_map>

namespace engine
{
    struct BoneWeight
    {
        math::Vector4i          m_boneIndices{ -1 };
        math::Vector4f          m_weights{ 0.f };
    };

    struct Bone
    {

        math::Matrix4f  m_localTransform{ 1.f };
        math::Matrix4f  m_inverseBindPose{ 1.f };
        std::string     m_name;
        int32		    m_parent = -1;
    };


    class DynamicMesh final : public Mesh
    {

    private:

        using BoneIndexMap = std::unordered_map<void*, int32>;
        using BoneNameMap = std::unordered_map<uint64, int32>;

    public:

        
        ENGINE_API DynamicMesh(void) = default;
        ENGINE_API DynamicMesh(DynamicMesh&&) noexcept = default;
        ENGINE_API DynamicMesh(const DynamicMesh&) = delete;
        ENGINE_API ~DynamicMesh(void) = default;

        ENGINE_API void ProcessMesh(const void* mesh) override;
        ENGINE_API void DeleteMesh(void) override;
        ENGINE_API void RenderThreadSkeletonSetup(void);

        ENGINE_API const Bone& GetBone(int32 index) const;
        ENGINE_API int32 GetBoneIndex(const std::string& name) const;
        ENGINE_API uint64 GetBoneCount(void) const;

        ENGINE_API bool ProcessSkeleton(const void* mesh);

    private:

        void ProcessWeights(const void* bone, uint32 boneIndex);
        void ProcessBone(Bone& newBone, void* bonePtr, const BoneIndexMap& nodes);
        void PopulateBoneNameMap(void);
        std::vector<int32> SortSkeleton(std::vector<Bone>& boneArray);

        void SortChildrenBones(int32 oldParentIndex, int32 newIndex,
                               std::vector<Bone>& boneArray,   
                               std::vector<int32>& indexArray);

        void SetBoneIndexAttribute(void);
        void SetBoneWeightAttribute(void);
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