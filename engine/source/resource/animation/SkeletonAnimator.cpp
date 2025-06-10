#include "resource/animation/SkeletonAnimator.h"
#include "resource/animation/Animation.h"
#include "resource/model/Model.h"
#include "resource/model/DynamicMesh.h"
#include "thread/ThreadManager.h"
#include "utility/ResourceRef.h"

#include "InternalOpenGLError.hpp"
#include "ConsoleLog.hpp"

#include <glad/glad.h>

namespace engine
{
    void SkeletonAnimator::SetAnimation(ResourceRef<Animation>&& anim)
    {
        m_currentAnim = std::forward<ResourceRef<Animation>>(anim);
        InitAnimation(true);
    }

    void SkeletonAnimator::SetAnimation(const ResourceRef<Animation>& anim)
    {
        m_currentAnim = anim;
        InitAnimation(true);
    }

    void SkeletonAnimator::SetModel(const ResourceRef<Model>& model)
    {
        StopAnimation();
        m_model = model;
        ResizeIndexArrays();
    }

    void SkeletonAnimator::PauseAnimation(void)
    {
        m_animState = EAnimationState::PAUSED;
    }

    void SkeletonAnimator::PlayAnimation(void)
    {
        if (!m_currentAnim)
            return;

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

        if (!m_currentAnim)
            return;

        if (m_animState == EAnimationState::PLAYING || m_forceNextUpdate)
        {
            m_elapsed += deltaTime * m_speedMultiplier;

            if (m_elapsed >= m_targetInterval)
            {
                if (UpdateNextKeyFrame() == EAnimationState::STOPPED)
                    return;
            }
            
           int32 boneCount = static_cast<int32>(m_currentAnimIndices.size());
           f32 lerpTime = math::Clamp(m_elapsed / m_targetInterval, 0.f, 1.f);

           m_transforms.clear();
           m_transforms.resize(m_currentAnimIndices.size());

           for (int32 boneNum = 0; boneNum < boneCount; ++boneNum)
               UpdateSingleBone(boneNum, lerpTime);

            CalculateSkinningMatrices();
            ThreadManager::AddTask<ThreadManager::ETaskType::GRAPHICS>([this]()
                {
                    InitBuffer();
                    m_skinningSSBO.SetData(m_skinningMatrices.data(), m_skinningMatrices.size() * sizeof(math::Matrix4f));
                    OpenGLError(); });

            m_forceNextUpdate = false;
        }
    }

    void SkeletonAnimator::SetKeyFrame(int32 keyIndex)
    {
        PauseAnimation();

        if (keyIndex >= 0 && keyIndex < m_currentAnim->GetTickCount())
        {
            m_currentKeyFrame = keyIndex;
            m_forceNextUpdate = true;
        }
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

    const ResourceRef<Animation>& SkeletonAnimator::GetAnimation(void) const
    {
        return m_currentAnim;
    }

    f32 SkeletonAnimator::GetAnimationSpeed(void) const
    {
        return m_speedMultiplier;
    }

    int32 SkeletonAnimator::GetCurrentKeyFrame(void) const
    {
        return m_currentKeyFrame;
    }

    bool SkeletonAnimator::IsPlaying(void) const
    {
        return m_animState == EAnimationState::PLAYING;
    }

    bool SkeletonAnimator::IsPaused(void) const
    {
        return m_animState == EAnimationState::PAUSED;
    }

    bool SkeletonAnimator::IsLockedInPlace(void) const
    {
        return m_lockedInPlace;
    }

    void SkeletonAnimator::SetLockedInPlace(bool locked)
    {
        m_lockedInPlace = locked;
    }

    void SkeletonAnimator::InitBuffer(void)
    {
        m_skinningSSBO.Init();
    }

    void SkeletonAnimator::DeleteBuffer(void)
    {
        m_skinningSSBO.DeleteData();
    }

    void SkeletonAnimator::UseSkinningBuffer(void)
    {
        m_skinningSSBO.Init();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_skinningSSBO.GetBufferID());
    }

    void SkeletonAnimator::InitAnimation(bool current)
    {
        if (!m_model || !m_model->IsLoaded())
        {
            PrintLog(ErrorPreset(), "Unable to init animation, model is invalid \
or has not finished loading!");
            return;
        }

        if (current)
        {
            constexpr float defaultInterval = 1.f / 30.f;

            if (!math::AlmostEqual(m_currentAnim->GetTicksPerSecond(), 0.0f))
                m_targetInterval = 1.f / m_currentAnim->GetTicksPerSecond();
            else
                m_targetInterval = defaultInterval;

            MapBonesByName(m_currentAnimIndices, m_currentAnim);
        }       
    }

    void SkeletonAnimator::MapBonesByName(std::vector<BoneIndex>& indices,
                                         const ResourceRef<class Animation>& anim)
    {

       ResizeIndexArrays();

        if (!anim)
            return;

       uint64 meshBoneOffset = 0;

        for (uint64 meshNum = 0; meshNum < m_model->GetDynamicMeshes().size(); ++meshNum)
        {
            const DynamicMesh& mesh = m_model->GetDynamicMeshes()[meshNum];

            for (int32 boneNum = 0; boneNum < static_cast<int32>(mesh.GetBoneCount()); ++boneNum)
            {
                const Bone& bone = mesh.GetBone(boneNum);              // get bone by index
                indices[boneNum + meshBoneOffset].m_bone = &bone;

                if (memcmp(bone.m_name.c_str(), "ik_", 3) != 0)
                {
                    // get keyframes matching with bone name
                    indices[boneNum + meshBoneOffset].m_animDataIndex = anim->GetBoneAnimData(bone.m_name);
                    indices[boneNum + meshBoneOffset].m_parentIndex = bone.m_parent;
                }
            }

            meshBoneOffset += mesh.GetBoneCount();
        }
    }

    void SkeletonAnimator::UpdateSingleBone(int32 index, f32 lerpTime)
    {
        const BoneIndex& indices = m_currentAnimIndices[index];

        // return if this bone is not included in this animation
        if (!indices.m_animDataIndex)
            return;

        BoneTransform& keyFrame = m_transforms[index];
        int32 nextKeyFrameIndex = CalculateNextKeyFrameIndex();

        const AnimBone& boneKeyFrames = *indices.m_animDataIndex;

        if (nextKeyFrameIndex != -1)
        {
            keyFrame = Interpolate(boneKeyFrames[m_currentKeyFrame],
                boneKeyFrames[nextKeyFrameIndex], lerpTime);
        }
        else
            keyFrame = boneKeyFrames[m_currentKeyFrame];

        if (m_lockedInPlace && indices.m_parentIndex == -1)
            keyFrame.m_position = math::Vector3f::Zero();

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
            m_currentAnimIndices.clear();
            m_transforms.clear();

            m_currentAnimIndices.resize(m_model->GetBoneCount());
            m_transforms.resize(m_model->GetBoneCount());
        }
    }

    void SkeletonAnimator::RetrieveDataFromCache(const SkeletonAnimator& cached)
    {
        m_animState = cached.m_animState;
        m_currentKeyFrame = cached.m_currentKeyFrame;
        m_elapsed = cached.m_elapsed;
    }

    int32 SkeletonAnimator::CalculateNextKeyFrameIndex(void)
    {
        if ((m_currentKeyFrame + 1) < (m_currentAnim->GetTickCount() - 1))
            return m_currentKeyFrame + 1;

        else if (m_looped)
            return 0;

        else
            return -1;
    }

    void SkeletonAnimator::CalculateSkinningMatrices(void)
    {
        std::vector<math::Matrix4f> matrices;
        
        matrices.resize(m_transforms.size(), {1.f});
        m_skinningMatrices.resize(m_transforms.size(), {1.f});

        int64 currentBone = 0;
        
        for (BoneIndex& indices : m_currentAnimIndices)
        {
            math::Matrix4f& animMatrix = matrices[currentBone];
        
            if (!indices.m_animDataIndex)
            {
                if (!indices.m_bone)
                    continue;

                if (indices.m_parentIndex != -1)
                    animMatrix = matrices[indices.m_parentIndex] * indices.m_bone->m_localTransform.Transpose();      
        
                m_skinningMatrices[currentBone] = indices.m_bone->m_inverseBindPose * animMatrix.Transpose();
                ++currentBone;
                continue;
            }
        
            const BoneTransform& transform = m_transforms[currentBone];
        
            if (indices.m_parentIndex != -1)
            {
                animMatrix = matrices[indices.m_parentIndex] *  // parent anim matrix
                    math::TransformMatrix(transform.m_rotation, // keyframe transform matrix
                        transform.m_position,
                        transform.m_scaling);    
            }
        
            else
            {
                animMatrix = math::TransformMatrix(transform.m_rotation,
                    transform.m_position,
                    transform.m_scaling);
            }
        
            m_skinningMatrices[currentBone] = indices.m_bone->m_inverseBindPose * animMatrix.Transpose();
            ++currentBone;
        }
    }
}