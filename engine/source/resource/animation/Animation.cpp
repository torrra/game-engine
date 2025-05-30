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
    const BoneTransform& engine::AnimBone::operator[](int32 index) const
    {
        return m_keyframes[index];
    }

    BoneTransform Interpolate(const BoneTransform& transformA, 
                              const BoneTransform& transformB, f32 ratio)
    {
        BoneTransform result;

        result.m_position = math::Vector3f::Lerp(transformA.m_position,
                                                transformB.m_position, ratio);

        result.m_scaling = math::Vector3f::Lerp(transformA.m_scaling,
                                                transformB.m_scaling, ratio);

        result.m_rotation = math::Quatf::Slerp(transformA.m_rotation,
                                               transformB.m_rotation, ratio);

        return result;
    }

    BoneTransform operator*(const BoneTransform& transformA, const BoneTransform& transformB)
    {
        BoneTransform result;

        result.m_position = transformA.m_position + transformA.m_rotation * transformB.m_position;
        result.m_rotation = transformA.m_rotation * transformB.m_rotation;
        result.m_scaling = transformA.m_scaling * transformB.m_scaling;

        return result;
    }

    BoneTransform operator-(const BoneTransform& transform)
    {

        BoneTransform result;

        result.m_position = -transform.m_position;
        result.m_rotation = -transform.m_rotation;
        result.m_scaling = { 1.f / transform.m_scaling.GetX(),
                             1.f / transform.m_scaling.GetY(),
                             1.f / transform.m_scaling.GetZ() };

        return result;
    }


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

    const std::vector<AnimBone>& Animation::GetBoneData(void) const
    {
        return m_boneData;
    }

    void Animation::ReadAnimation(const void* animation)
    {
        const aiAnimation* animImpl = reinterpret_cast<const aiAnimation*>(animation);

        m_tickCount = static_cast<int32>(animImpl->mDuration);
        m_ticksPerSecond = static_cast<f32>(animImpl->mTicksPerSecond);

        m_boneData.reserve(animImpl->mNumChannels);

        for (uint32 boneNum = 0; boneNum < animImpl->mNumChannels; ++boneNum)
        {
            aiNodeAnim* boneAnim = animImpl->mChannels[boneNum];
            AnimBone& newBone = m_boneData.emplace_back();

            newBone.m_boneName = boneAnim->mNodeName.C_Str();
            newBone.m_keyframes.resize(static_cast<uint32>(animImpl->mDuration));

            ReadBonePositions(newBone, boneAnim);
            ReadBoneRotations(newBone, boneAnim);
            ReadBoneScalings(newBone, boneAnim);
        }
    }

    void Animation::ReadBonePositions(AnimBone& newBone, void* boneAnim)
    {
        const aiNodeAnim* animNode = reinterpret_cast<const aiNodeAnim*>(boneAnim);
        uint32 index = 0, nextFrameTick = 0, prevFrameIndex = 0;

        for (BoneTransform& transform : newBone.m_keyframes)
        {
            if (index >= animNode->mNumPositionKeys)
                transform.m_position = newBone.m_keyframes[animNode->mNumPositionKeys - 1].m_position;

            else if (nextFrameTick > index)
            {
                aiVectorKey& nextKey = animNode->mPositionKeys[prevFrameIndex + 1];
                aiVectorKey& prevKey = animNode->mPositionKeys[prevFrameIndex];
                math::Vector3f prev{ prevKey.mValue.x, prevKey.mValue.y, prevKey.mValue.z };
                math::Vector3f next{ nextKey.mValue.x, nextKey.mValue.y, nextKey.mValue.z };

                transform.m_position = math::Vector3f::Lerp(prev, next,
                                       static_cast<f32>(index) / static_cast<f32>(nextFrameTick));
            }
            else
            {
                aiVectorKey& key = animNode->mPositionKeys[index];
                transform.m_position = { key.mValue.x, key.mValue.y, key.mValue.z };

                prevFrameIndex = index;

                if (index + 1 < animNode->mNumPositionKeys)
                    nextFrameTick = static_cast<uint32>(animNode->mPositionKeys[index + 1].mTime);
            }

            ++index;
        }
    }

    void Animation::ReadBoneScalings(AnimBone& newBone, void* boneAnim)
    {
        const aiNodeAnim* animNode = reinterpret_cast<const aiNodeAnim*>(boneAnim);
        uint32 index = 0, nextFrameTick = 0, prevFrameIndex = 0;

        for (BoneTransform& transform : newBone.m_keyframes)
        {
            if (index >= animNode->mNumScalingKeys)
                transform.m_scaling = newBone.m_keyframes[animNode->mNumScalingKeys - 1].m_scaling;

            else if (nextFrameTick > index)
            {
                aiVectorKey& nextKey = animNode->mScalingKeys[prevFrameIndex + 1];
                aiVectorKey& prevKey = animNode->mScalingKeys[prevFrameIndex];
                math::Vector3f prev{ prevKey.mValue.x, prevKey.mValue.y, prevKey.mValue.z };
                math::Vector3f next{ nextKey.mValue.x, nextKey.mValue.y, nextKey.mValue.z };

                transform.m_scaling = math::Vector3f::Lerp(prev, next,
                    static_cast<f32>(index) / static_cast<f32>(nextFrameTick));
            }
            else
            {
                aiVectorKey& key = animNode->mScalingKeys[index];
                transform.m_scaling = { key.mValue.x, key.mValue.y, key.mValue.z };

                prevFrameIndex = index;

                if (index + 1 < animNode->mNumScalingKeys)
                    nextFrameTick = static_cast<uint32>(animNode->mScalingKeys[index + 1].mTime);
            }

            ++index;
        }
    }

    void Animation::ReadBoneRotations(AnimBone& newBone, void* boneAnim)
    {
        const aiNodeAnim* animNode = reinterpret_cast<const aiNodeAnim*>(boneAnim);
        uint32 index = 0, nextFrameTick = 0, prevFrameIndex = 0;

        for (BoneTransform& transform : newBone.m_keyframes)
        {
            if (index >= animNode->mNumRotationKeys)
                transform.m_rotation = newBone.m_keyframes[animNode->mNumRotationKeys - 1].m_rotation;

            else if (nextFrameTick > index)
            {
                aiQuatKey& nextKey = animNode->mRotationKeys[prevFrameIndex + 1];
                aiQuatKey& prevKey = animNode->mRotationKeys[prevFrameIndex];

                math::Quatf prev{ prevKey.mValue.w, prevKey.mValue.x, 
                                  prevKey.mValue.y, prevKey.mValue.z };

                math::Quatf next{ nextKey.mValue.w, nextKey.mValue.x, 
                                  nextKey.mValue.y, nextKey.mValue.z };

                transform.m_rotation = math::Quatf::Slerp(prev, next,
                    static_cast<f32>(index) / static_cast<f32>(nextFrameTick));
            }
            else
            {
                aiQuatKey& key = animNode->mRotationKeys[index];
                transform.m_rotation = {key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z };

                prevFrameIndex = index;

                if (index + 1 < animNode->mNumRotationKeys)
                    nextFrameTick = static_cast<uint32>(animNode->mRotationKeys[index + 1].mTime);
            }
            ++index;
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

    uint64 Animation::GetBoneCount(void) const
    {
        return m_boneData.size();
    }

    int32 Animation::GetTickCount(void) const
    {
        if (!m_boneData.empty())
            return static_cast<int32>(m_boneData[0].m_keyframes.size());

        else
            return 0;
    }

    f32 Animation::GetTicksPerSecond(void) const
    {
        return m_ticksPerSecond;
    }

}