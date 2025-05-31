#pragma once
#include "engine/CoreTypes.h"
#include "engine/utility/ResourceRefDecl.h"
#include "engine/resource/model/Buffer.h"
#include "engine/resource/animation/Animation.h"
#include "engine/resource/model/DynamicMesh.h"

#include <math/Matrix4.hpp>

#include <vector>
#include <mutex>

namespace engine
{
    class SkeletonAnimator
    {
    private:

        enum class EAnimationState
        {
            STOPPED,
            PLAYING,
            PAUSED
        };

        struct BoneIndex
        {
            const AnimBone* m_animDataIndex = nullptr;
            const Bone* m_bone = nullptr;
            int32 m_parentIndex = -1;
        };
        
    public:

        ENGINE_API SkeletonAnimator(void) = default;

        SkeletonAnimator(SkeletonAnimator&&) = default;
        SkeletonAnimator(const SkeletonAnimator&) = default;

        ENGINE_API void SetAnimation(ResourceRef<class Animation>&& anim, bool crossfade);
        ENGINE_API void SetModel(const ResourceRef<class Model>& model);

        ENGINE_API void PauseAnimation(void);
        ENGINE_API void PlayAnimation(void);
        ENGINE_API void StopAnimation(void);

        ENGINE_API void Update(f32 deltaTime);
        ENGINE_API void SetKeyFrame(int32 keyIndex);
        ENGINE_API void SetAnimationSpeed(f32 speedMultiplier);

        ENGINE_API bool IsLooped(void) const;
        ENGINE_API void SetLooped(bool looped);

        ENGINE_API bool IsPlaying(void) const;
        ENGINE_API bool IsPaused(void) const;

        ENGINE_API void InitBuffer(void);
        ENGINE_API void UseSkinningBuffer(void);
        ENGINE_API void ResizeIndexArrays(void);

        ENGINE_API SkeletonAnimator& operator=(SkeletonAnimator&&) noexcept = default;

    private:

        void InitAnimation(bool current);
       
        void MapBonesByName(std::vector<BoneIndex>& indices, const ResourceRef<class Animation>& anim);

        void UpdateSingleBone(int32 index, f32 lerpTime);
        EAnimationState UpdateNextKeyFrame(void);
        int32 CalculateNextKeyFrameIndex(void);
        void SetSkinningMatrices(void);

        std::vector<BoneIndex>          m_currentAnimIndices;
        std::vector<BoneIndex>          m_nextAnimIndices;
        std::vector<BoneTransform>      m_transforms;

        ResourceRef<Animation>          m_currentAnim;
        ResourceRef<Animation>          m_nextAnim;
                                        
        ResourceRef<class Model>        m_model;
                      

        int32                           m_currentKeyFrame = -1;
        f32                             m_targetInterval = -1.f;
        f32                             m_speedMultiplier = 1.f;
        f32                             m_elapsed = 0.f;

        Buffer                          m_skinningSSBO = 0;
        EAnimationState                 m_animState = EAnimationState::STOPPED;
        bool                            m_looped = false;
    };
}