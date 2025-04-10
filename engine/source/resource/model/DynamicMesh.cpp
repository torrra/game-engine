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
       // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_skeletonSSBO.GetBufferID());
        Mesh::Draw();
    }

    const Bone& DynamicMesh::GetBone(int32 index) const
    {
        return m_skeleton.at(index);
    }

    const Bone& DynamicMesh::GetBone(const std::string& name) const
    {
        return m_skeleton.at(GetBoneIndex(name));
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

        if (!meshImpl->HasBones())
            return false;
   
        std::unordered_map<void*, int64> nodes;
        nodes.reserve(meshImpl->mNumBones);

        for (uint64 boneIndex = 0; boneIndex < static_cast<uint64>(meshImpl->mNumBones); ++boneIndex)
        {
            aiBone* bone = meshImpl->mBones[boneIndex];
            nodes[bone->mNode] = boneIndex;
        }

        m_weights.resize(m_indices.size());
        m_skeleton.resize(meshImpl->mNumBones);

        for (uint32 boneIndex = 0; boneIndex < meshImpl->mNumBones; ++boneIndex)
        {
            aiBone* bone = meshImpl->mBones[boneIndex];
            Bone& newBone = m_skeleton[nodes.at(bone->mNode)];

            ProcessWeights(bone, boneIndex);
            ProcessBone(newBone, bone, nodes);
        }

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

    std::ostream& operator<<(std::ostream& lhs, const Bone& rhs)
    {
        lhs << "[Bone]\nname: " << rhs.m_name << '\n';
        lhs << "parent: " << rhs.m_parent << "\n\n";

        return lhs;
    }
}