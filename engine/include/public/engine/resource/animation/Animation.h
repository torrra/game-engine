#pragma once 

#include <vector>

#include "engine/resource/Resource.h"

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

namespace engine
{
    // Position / scale bone transform
    struct Vec3BoneTransform
    {
        math::Vector3f  m_vector = math::Vector3f::One();
        
        // Tick index
        int32           m_time = -1;
    };

    // Rotation bone transform
    struct QuatBoneTransform
    {
        math::Quatf    m_quaternion = math::Quatf::Identity();
        int32          m_time = -1;       
    };

    // All frame transformations for a given bone
    class BoneAnimData
    {

    public:

        BoneAnimData(void) = default;
        BoneAnimData(BoneAnimData&&) noexcept = default;
        BoneAnimData(const BoneAnimData&) = delete;
        ~BoneAnimData(void) = default;

        BoneAnimData& operator=(BoneAnimData&&) noexcept = default;

    private:
    
        std::vector<QuatBoneTransform>  m_rotations;
        std::vector<Vec3BoneTransform>  m_positions;
        std::vector<Vec3BoneTransform>  m_scales;

        std::string                     m_boneName;

        friend class Animation;
    };


    class Animation final : public IResource
    {

    public:

        ENGINE_API Animation(void) = default;
        ENGINE_API Animation(void* animData);
                   Animation(const Animation&) = delete;
        ENGINE_API Animation(Animation&&) noexcept = default;
        ENGINE_API ~Animation(void) = default;

        ENGINE_API bool LoadResource(const char* filePath) override;

        // When one or several animations are stored with a model,
        // or when several animations are in a single file, load
        // secondary animations from that data
        ENGINE_API static void LoadExtraAnimations(const void* anims, uint32 offset = 0);
     
    private:

        void ReadAnimation(const void* animation);
        void ReadBoneAnim(void* boneAnim);

        std::vector<BoneAnimData>   m_animData;
        int32                       m_tickCount = -1;
        float                       m_ticksPerSecond = 0.f;
    };
}