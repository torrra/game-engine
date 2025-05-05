#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/anim.h>

#include "resource/ResourceManager.h"
#include "resource/animation/Animation.h"

namespace engine::importer
{
    // Reuse same importer in the same thread as creating
    // one is expensive and requires heap allocations
    thread_local Assimp::Importer g_animImporter;
}


namespace engine
{
    Animation::Animation(const void* animData)
    {
        ReadAnimation(animData);
    }

    bool Animation::LoadResource(const char* filePath)
    {
        const aiScene* scene = importer::g_animImporter.ReadFile(filePath, 0);

        if (scene && scene->HasAnimations())
        {
            ReadAnimation(scene->mAnimations[0]);
            LoadExtraAnimations(scene, 1);
            importer::g_animImporter.FreeScene();
            return true;
        }

        return false;
    }

    void Animation::ReadAnimation(const void* animation)
    {
        const aiAnimation* animImpl = reinterpret_cast<const aiAnimation*>(animation);

        m_tickCount = static_cast<int32>(animImpl->mDuration);
        m_ticksPerSecond = static_cast<f32>(animImpl->mTicksPerSecond);
        m_animData.reserve(animImpl->mNumChannels);

        for (uint32 boneNum = 0; boneNum < animImpl->mNumChannels; ++boneNum)
            ReadBoneAnim(animImpl->mChannels[boneNum]);
    }

    void Animation::ReadBoneAnim(void* boneAnim)
    {
        const aiNodeAnim* animNode = reinterpret_cast<const aiNodeAnim*>(boneAnim);

        BoneAnimData& boneData = m_animData.emplace_back();

        boneData.m_boneName = animNode->mNodeName.C_Str();
        boneData.m_positions.resize(animNode->mNumPositionKeys);
        boneData.m_rotations.resize(animNode->mNumRotationKeys);
        boneData.m_scales.resize(animNode->mNumScalingKeys);

        // Copy all bone transformations for each frame

        for (uint32 posNum = 0; posNum < animNode->mNumPositionKeys; ++posNum)
        {
            Vec3BoneTransform& pos = boneData.m_positions[posNum];
            aiVectorKey& posKeyData = animNode->mPositionKeys[posNum];

            pos.m_vector = *reinterpret_cast<math::Vector3f*>(&posKeyData.mValue);
            pos.m_time = static_cast<int32>(posKeyData.mTime);
        }

        for (uint32 rotationNum = 0; rotationNum < animNode->mNumRotationKeys; ++rotationNum)
        {
            QuatBoneTransform& rotation = boneData.m_rotations[rotationNum];
            aiQuatKey& rotationKeyData = animNode->mRotationKeys[rotationNum];

            rotation.m_quaternion = *reinterpret_cast<math::Quatf*>(&rotationKeyData.mValue);
            rotation.m_time = static_cast<int32>(rotationKeyData.mTime);
        }

        for (uint32 scaleNum = 0; scaleNum < animNode->mNumScalingKeys; ++scaleNum)
        {
            Vec3BoneTransform& scale = boneData.m_scales[scaleNum];
            aiVectorKey& scaleKeyData = animNode->mScalingKeys[scaleNum];

            scale.m_vector = *reinterpret_cast<math::Vector3f*>(&scaleKeyData.mValue);
            scale.m_time = static_cast<int32>(scaleKeyData.mTime);
        }
    }

    void Animation::LoadExtraAnimations(const void* anims, uint32 offset)
    {
        const aiScene* scene = reinterpret_cast<const aiScene*>(anims);

        for (uint32 animNum = offset; animNum < scene->mNumAnimations; ++animNum)
        {
            const aiAnimation* anim = scene->mAnimations[animNum];
            ResourceManager::CreateFromData<Animation>(anim->mName.C_Str(), (const void*)anim);
        }
    }
}