#pragma once
#include "engine/CoreTypes.h"
#include "engine/utility/ResourceRef.h"

#include <vector>
#include <utility>
#include <atomic>

namespace engine
{
    class SkeletonAnimator
    {
    private:

        struct BoneFrameIndex
        {
            int32 m_animDataIndex = -1;
            int32 m_nextFrameIndex = 0;

            // When to update each component
            int32 m_nextPosTick = -1;
            int32 m_nextScaleTick = -1;
            int32 m_nextRotationTick = -1;
        };
        
    public:

        SkeletonAnimator(void) = default;
        SkeletonAnimator(const SkeletonAnimator&) = delete;
        SkeletonAnimator(SkeletonAnimator&&) noexcept = default;

        void SetAnimation(ResourceRef<class Animation>&& anim);

        void PauseAnimation(void);
        void PlayAnimation(void);

        void SetKeyFrame(int32 keyIndex);
        void SetAnimSpeed(uint32 ticksPerSecond);

    private:

        std::vector<BoneFrameIndex>     m_animIndices;
        ResourceRef<class Animation>    m_currentAnim;
        ResourceRef<class Animation>    m_nextAnim;
        int32                           m_currentKeyFrame = -1;
        std::atomic<bool>               m_isUpdating = false;
    };
}