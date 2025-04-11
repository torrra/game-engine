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
        m_skeletonSSBO.DeleteData();
        Mesh::DeleteMesh();
    }

    void DynamicMesh::RenderThreadSkeletonSetup(void)
    {
        if (!m_metaData.m_hasBones)
            return;

        m_skeletonSSBO.Init();
        m_skeletonSSBO.SetData(m_weights.data(), m_weights.size() * sizeof(m_weights[0]));
        m_weights.clear();
        m_weights.shrink_to_fit();
    }

    void DynamicMesh::Draw(void) const
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_skeletonSSBO.GetBufferID());
        Mesh::Draw();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
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