#include "resource/model/DynamicMesh.h"

#include <assimp/mesh.h>

#include <unordered_map>

namespace engine
{
    void DynamicMesh::ProcessMesh(const void* mesh)
    {
        Mesh::ProcessMesh(mesh);

        const aiMesh* meshImpl = reinterpret_cast<const aiMesh*>(mesh);

        if (meshImpl->HasBones())
            m_weights.resize(m_indices.size());

        for (uint32 boneIndex = 0; boneIndex < meshImpl->mNumBones; ++boneIndex)
        {
            aiBone* bone = meshImpl->mBones[boneIndex];
            Bone&   newBone = m_skeleton.emplace_back();

            for (uint32 weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex)
            {
                const aiVertexWeight& weight = bone->mWeights[weightIndex];

                BoneWeight& newWeight = m_weights[weight.mVertexId];

                if (newWeight.m_weightCount > 3)
                    continue;

                newWeight.m_boneIndices[newWeight.m_weightCount] = boneIndex;
                newWeight.m_weights[newWeight.m_weightCount] = weight.mWeight;
                ++newWeight.m_weightCount;
            }
            newBone.m_inverseBindPose = reinterpret_cast<f32*>(&bone->mOffsetMatrix.Transpose());
            newBone.m_name = bone->mName.C_Str();
            //newBone.m_parent = bone->
        }
    }

    uint64 DynamicMesh::GetVertexCount(void)
    {
        uint64 vertexSize = sizeof(math::Vector3f);

        if (m_metaData.m_hasNormals)
            vertexSize += sizeof(math::Vector3f);

        if (m_metaData.m_hasUV)
            vertexSize += sizeof(math::Vector2f);

        if (m_metaData.m_hasTangents)
            vertexSize += sizeof(math::Vector3f) * 2;

        if (m_metaData.m_hasVertexColors)
            vertexSize += sizeof(math::Vector4f);

        return m_vertices.size() / vertexSize;
    }
}