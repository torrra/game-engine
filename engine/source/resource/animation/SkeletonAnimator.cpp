#include "resource/animation/SkeletonAnimator.h"
#include "resource/animation/Animation.h"
#include "resource/model/Model.h"
#include "resource/model/DynamicMesh.h"
#include "thread/ThreadManager.h"
#include "utility/ResourceRef.h"

#include "physics/InternalPhysxDebugDraw.h"

#include "ConsoleLog.hpp"

#include <glad/glad.h>

namespace engine
{
    void SkeletonAnimator::SetAnimation(ResourceRef<class Animation>&& anim,
                                        bool crossfade)
    {
        if (crossfade)
        {
            // do stuff
        }
        else
        {
            m_nextAnim = ResourceRef<Animation>();
            m_nextAnimIndices.clear();
            m_currentAnim = std::forward<ResourceRef<Animation>>(anim);

            InitAnimation(true);
        }
    }

    void SkeletonAnimator::SetModel(const ResourceRef<Model>& model)
    {
        StopAnimation();
        m_model = model;
    }

    void SkeletonAnimator::PauseAnimation(void)
    {
        m_animState = EAnimationState::PAUSED;
    }

    void SkeletonAnimator::PlayAnimation(void)
    {
        if (m_animState == EAnimationState::PAUSED)
            m_animState = EAnimationState::PLAYING;

        else
        {
            m_animState = EAnimationState::PLAYING;
            m_elapsed = 0.f;
            m_currentKeyFrame = 0;
        }
    }

    void SkeletonAnimator::StopAnimation(void)
    {
        m_currentKeyFrame = -1;
        m_elapsed = 0.f;
        m_animState = EAnimationState::STOPPED;
    }

    void SkeletonAnimator::Update(f32 deltaTime)
    {
        if (!m_model || !m_model->IsDynamic())
            return;

        if (m_animState == EAnimationState::PLAYING)
        {

            m_elapsed += deltaTime;
            
           int32 boneCount = static_cast<int32>(m_currentAnimIndices.size());
           m_transforms.clear();
           m_transforms.resize(m_currentAnimIndices.size());

                for (int32 boneNum = 0; boneNum < boneCount; ++boneNum)
                    UpdateSingleBone(boneNum, 0);
        }

        if (m_animState != EAnimationState::STOPPED)
        {
            ThreadManager::AddTask<ThreadManager::ETaskType::GRAPHICS>(
                &SkeletonAnimator::SetSkinningMatrices, this);
        }
    }

    void SkeletonAnimator::SetKeyFrame(int32 keyIndex)
    {
        PauseAnimation();

        if (keyIndex >= 0 && keyIndex < m_currentAnim->GetTickCount())
            m_currentKeyFrame = keyIndex;
        else
            PrintLog(ErrorPreset(), "Invalid key frame");
    }

    void SkeletonAnimator::SetAnimationSpeed(f32 speedMultiplier)
    {
        m_speedMultiplier = math::Max(0.01f, speedMultiplier);
    }

    bool SkeletonAnimator::IsLooped(void) const
    {
        return m_looped;
    }

    void SkeletonAnimator::SetLooped(bool looped)
    {
        m_looped = looped;
    }

    bool SkeletonAnimator::IsPlaying(void) const
    {
        return m_animState == EAnimationState::PLAYING;
    }

    bool SkeletonAnimator::IsPaused(void) const
    {
        return m_animState == EAnimationState::PAUSED;
    }

    void SkeletonAnimator::InitBuffer(void)
    {
        m_skinningSSBO.Init();
    }

    void SkeletonAnimator::UseSkinningBuffer(void)
    {
        m_skinningSSBO.Init();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_skinningSSBO.GetBufferID());
    }

    void SkeletonAnimator::InitAnimation(bool current)
    {
        if (!m_model)
            return;

        if (current)
        {
            m_targetInterval = m_currentAnim->GetTicksPerSecond();
            MapBonesByName(m_currentAnimIndices, m_currentAnim);
        }
        
        else
            MapBonesByName(m_nextAnimIndices, m_nextAnim);

        
    }

    void SkeletonAnimator::MapBonesByName(std::vector<BoneIndex>& indices,
                                         const ResourceRef<class Animation>& anim)
    {
        if (indices.empty())
            ResizeIndexArrays();

        if (!anim)
            return;

        for (int32 boneNum = 0; boneNum < static_cast<int32>(m_model->GetBoneCount()); ++boneNum)
        {
            const Bone& bone = m_model->GetDynamicMeshes()[0].GetBone(boneNum);              // get bone by index
            indices[boneNum].m_bone = &bone;
            indices[boneNum].m_animDataIndex = anim->GetBoneAnimData(bone.m_name);         // get keyframes matching with bone name
            indices[boneNum].m_parentIndex = bone.m_parent;
        }
    }

    void SkeletonAnimator::UpdateSingleBone(int32 index, f32 lerpTime)
    {
        const BoneIndex& indices = m_currentAnimIndices[index];

        // return if this bone is not included in this animation
        if (!indices.m_animDataIndex)
            return;

        BoneTransform& keyFrame = m_transforms[index];
        //int32 nextKeyFrameIndex = CalculateNextKeyFrameIndex();
        int32 nextKeyFrameIndex = -1;

        const AnimBone& boneKeyFrames = *indices.m_animDataIndex;

        if (nextKeyFrameIndex != -1)
        {
            keyFrame = Interpolate(boneKeyFrames[m_currentKeyFrame],
                boneKeyFrames[nextKeyFrameIndex], lerpTime);
        }
        else
            keyFrame = boneKeyFrames[m_currentKeyFrame];

    }
 
    SkeletonAnimator::EAnimationState SkeletonAnimator::UpdateNextKeyFrame(void)
    {
        m_currentKeyFrame = CalculateNextKeyFrameIndex();

        if (m_currentKeyFrame == -1)
            return EAnimationState::STOPPED;

        else
        {
            m_elapsed = 0.f;
            return EAnimationState::PLAYING;
        }
    }

    void SkeletonAnimator::ResizeIndexArrays(void)
    {
        if (m_model && m_model->IsDynamic())
        {
            m_currentAnimIndices.resize(m_model->GetBoneCount());
            m_nextAnimIndices.resize(m_model->GetBoneCount());
            m_transforms.resize(m_model->GetBoneCount());
        }
    }

    int32 SkeletonAnimator::CalculateNextKeyFrameIndex(void)
    {
        if (m_currentKeyFrame + 1 < m_currentAnim->GetTickCount() - 1)
            return m_currentKeyFrame + 1;

        else if (m_looped)
            return 0;

        else
            return -1;
    }

    void SkeletonAnimator::SetSkinningMatrices(void)
    {
        std::vector<math::Matrix4f> matrices;
        std::vector<math::Matrix4f> skinning;

        matrices.resize(m_transforms.size(), {1.f});
        skinning.reserve(m_transforms.size());

        int64 currentBone = 0;

        for (BoneIndex& indices : m_currentAnimIndices)
        {
            math::Matrix4f& animMatrix = matrices[currentBone];

            if (!indices.m_animDataIndex)
            {
                if (indices.m_parentIndex != -1)
                    animMatrix = matrices[indices.m_parentIndex] * indices.m_bone->m_localTransform;


                ++currentBone;
                skinning.emplace_back(indices.m_bone->m_inverseBindPose * animMatrix);
                continue;
            }

            const BoneTransform& transform = m_transforms[currentBone];

            if (indices.m_parentIndex != -1)
            {
                animMatrix = matrices[indices.m_parentIndex] *           // parent anim matrix
                    indices.m_bone->m_localTransform *                     // current bone local matrix
                    math::TransformMatrix(transform.m_rotation, // keyframe transform matrix
                        transform.m_position,
                        transform.m_scaling);


                math::Vector3f start =
                {
                    animMatrix[3][0],
                    animMatrix[3][1],
                    animMatrix[3][2],
                };

                math::Matrix4f& parentPose = matrices[indices.m_parentIndex];

                math::Vector3f end =
                {
                    parentPose[3][0],
                    parentPose[3][1],
                    parentPose[3][2],
                };

                PhysicsEngine::Get().AddDebugLine(start, end, (uint32)-1);

            }

            else
            {
                animMatrix = math::TransformMatrix(transform.m_rotation,
                    transform.m_position,
                    transform.m_scaling);
            }

            skinning.emplace_back(indices.m_bone->m_inverseBindPose * animMatrix);

            ++currentBone;
        }
        
        m_skinningSSBO.SetData(skinning.data(), skinning.size() * sizeof(f32));
    }
}