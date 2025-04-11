#include "resource/model/DynamicMesh.h"

#include "thread/ThreadManager.h"

#include "glad/glad.h"

#include <assimp/mesh.h>
#include <assimp/scene.h>

#include <unordered_map>
#include <iostream>
#include <algorithm>

namespace engine
{
    void DynamicMesh::ProcessMesh(const void* mesh)
    {
        Mesh::ProcessMesh(mesh);    
        ProcessSkeleton(mesh);
    }

    void DynamicMesh::DeleteMesh(void)
    {
        m_boneIndexVBO.DeleteData();
        m_boneWeightVBO.DeleteData();
        Mesh::DeleteMesh();
    }

    const Bone& DynamicMesh::GetBone(int32 index) const
    {
        return m_skeleton.at(index);
    }

    int32 DynamicMesh::GetBoneIndex(const std::string& name) const
    {
        auto indexIt = m_boneMap.find(std::hash<std::string>{}(name));

        if (indexIt != m_boneMap.end())
            return indexIt->second;

        return -1;
    }

    void DynamicMesh::RenderThreadSkeletonSetup(void)
    {
        uint64 indexSize = m_weights.size() * sizeof(math::Vector4<uint32>);
        uint64 weightSize = m_weights.size() * sizeof(math::Vector4f);

        if (indexSize < sizeof(math::Vector4<uint32>) || weightSize < sizeof(math::Vector4f))
            return;

        math::Vector4f* weightBuffer = (math::Vector4f*)malloc(weightSize);
        math::Vector4<uint32>* indexBuffer = (math::Vector4<uint32>*)malloc(indexSize);

        if (weightBuffer && indexBuffer)
        {
            for (const BoneWeight& weight : m_weights)
            {
                new(weightBuffer) math::Vector4f(weight.m_weights);
                new(indexBuffer) math::Vector4<uint32>(weight.m_boneIndices);
            }
            m_weights.clear();
            m_weights.shrink_to_fit();
            SetupSkeletonVertexBuffers(indexBuffer, indexSize, weightBuffer, weightSize);
        }

        if (weightBuffer)
            free(weightBuffer);

        if (indexBuffer)
            free(indexBuffer);
    }

    void engine::DynamicMesh::SetBoneIndexAttribute()
    {
        glEnableVertexArrayAttrib(m_vao, 6);
        glVertexArrayAttribFormat(m_vao, 6, 4, GL_UNSIGNED_INT, GL_FALSE, 0);
        glVertexArrayAttribBinding(m_vao, 6, 1);
    }

    void DynamicMesh::SetBoneWeightAttribute()
    {
        glEnableVertexArrayAttrib(m_vao, 7);
        glVertexArrayAttribFormat(m_vao, 7, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(m_vao, 7, 2);

    }

    void DynamicMesh::SetupSkeletonVertexBuffers(void* indexBuffer, uint64 indexBufSize,
                                                 void* weightBuffer, uint64 weightBufSize)
    {
        m_boneIndexVBO.Init();
        m_boneWeightVBO.Init();

        m_boneIndexVBO.SetData(indexBuffer, indexBufSize);
        m_boneWeightVBO.SetData(weightBuffer, weightBufSize);

        SetBoneIndexAttribute();
        SetBoneWeightAttribute();

        glVertexArrayVertexBuffer(m_vao, 2, m_boneIndexVBO.GetBufferID(), 0,
            sizeof(math::Vector4<uint32>));

        glVertexArrayVertexBuffer(m_vao, 3, m_boneWeightVBO.GetBufferID(), 0,
            sizeof(math::Vector4f));
    }

    void DynamicMesh::ProcessWeights(const void* bone, uint32 boneIndex)
    {
        const aiBone* boneImpl = reinterpret_cast<const aiBone*>(bone);

        for (uint32 weightIndex = 0; weightIndex < boneImpl->mNumWeights; ++weightIndex)
        {
            const aiVertexWeight& weight = boneImpl->mWeights[weightIndex];
            BoneWeight& newWeight = m_weights[weight.mVertexId];

            if (newWeight.m_weightCount > 3)
                continue;

            newWeight.m_boneIndices[newWeight.m_weightCount] = boneIndex;
            newWeight.m_weights[newWeight.m_weightCount] = weight.mWeight;
            ++newWeight.m_weightCount;
        }
    }

    bool DynamicMesh::ProcessSkeleton(const void* mesh)
    {
        const aiMesh* meshImpl = reinterpret_cast<const aiMesh*>(mesh);
        std::vector<Bone> unorderedSkeleton(meshImpl->mNumBones);

        if (!meshImpl->HasBones())
            return false;
   
        std::unordered_map<void*, int64> nodes;
        nodes.reserve(meshImpl->mNumBones);

        for (uint64 boneIndex = 0; boneIndex < static_cast<uint64>(meshImpl->mNumBones); ++boneIndex)
        {
            aiBone* bone = meshImpl->mBones[boneIndex];
            nodes[bone->mNode] = boneIndex;
        }

        m_weights.resize(GetIndexCount());

        for (uint32 boneIndex = 0; boneIndex < meshImpl->mNumBones; ++boneIndex)
        {
            aiBone* bone = meshImpl->mBones[boneIndex];
            Bone& newBone = unorderedSkeleton[nodes.at(bone->mNode)];

            ProcessWeights(bone, boneIndex);
            ProcessBone(newBone, bone, nodes);
        }

        SortSkeleton(unorderedSkeleton);
        PopulateBoneNameMap();
        return true;
    }

    void DynamicMesh::ProcessBone(Bone& newBone, void* bonePtr, const BoneIndexMap& nodes)
    {
        aiBone* boneImpl = reinterpret_cast<aiBone*>(bonePtr);

        // Transpose as assimp's matrices are row major
        newBone.m_inverseBindPose = reinterpret_cast<f32*>(&boneImpl->mOffsetMatrix.Transpose());
        newBone.m_name = boneImpl->mName.C_Str();

        auto parentIterator = nodes.find(boneImpl->mNode->mParent);

        if (parentIterator != nodes.end())
            newBone.m_parent = parentIterator->second;
        else
        {
            aiNode* parentNode = boneImpl->mNode->mParent;

            while (parentNode && parentIterator == nodes.end())
            {
                parentNode = parentNode->mParent;
                parentIterator = nodes.find(parentNode);
            }
            if (parentIterator != nodes.end())
                newBone.m_parent = parentIterator->second;
        }
    }

    void DynamicMesh::PopulateBoneNameMap(void)
    {
        int32 boneIndex = 0;
        std::hash<std::string> hashFunctor;

        for (const Bone& bone : m_skeleton)
        {
            uint64 hashedName = hashFunctor(bone.m_name);
            m_boneMap[hashedName] = boneIndex++;
        }
    }

    void DynamicMesh::SortSkeleton(std::vector<Bone>& boneArray)
    {
        // This vector maps old unordered indices to
        // new positions that guarantee they will be updated
        // after their parents. Old indices are the values
        // contained and the new indices are their positions
        // within the vector
        std::vector<int32> indexArray;

        uint64 arraySize = boneArray.size();

        indexArray.reserve(arraySize);
        m_skeleton.reserve(arraySize);

        SortChildrenBones(-1, -1, boneArray, indexArray);
    
        for (int32 arrayIndex = 0; arrayIndex < static_cast<int32>(arraySize); ++arrayIndex)
           SortChildrenBones(indexArray[arrayIndex], arrayIndex, boneArray, indexArray);

        for (const Bone& bone : m_skeleton)
            std::cout << bone;


    }

    void DynamicMesh::SortChildrenBones(int32 oldParentIndex, int32 newIndex,
                                        std::vector<Bone>& boneArray,
                                        std::vector<int32>& indexArray)
    {
        for (int32 index = 0; index < static_cast<int32>(boneArray.size()); ++index)
        {
            if (boneArray[index].m_parent == oldParentIndex)
            {
                Bone& movedBone = m_skeleton.emplace_back(std::move(boneArray[index]));
                movedBone.m_parent = newIndex;

                indexArray.push_back(index);

                // There should be only one bone at the root, so we can exit this function
                if (newIndex == -1)
                    return;
            }
        }
    }

    std::ostream& operator<<(std::ostream& lhs, const Bone& rhs)
    {
        lhs << "[Bone]\nname: " << rhs.m_name << '\n';
        lhs << "parent: " << rhs.m_parent << "\n\n";

        return lhs;
    }
}