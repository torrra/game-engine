#pragma once 

#include <vector>

#include "engine/resource/Resource.h"

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

namespace engine
{
 
    struct BoneTransform
    {   
        math::Quatf     m_rotation = math::Quatf::Identity();
        math::Vector3f  m_position = math::Vector3f::Zero();
        math::Vector3f  m_scaling = math::Vector3f::One();

    };

    struct AnimBone
    {
        std::string                 m_boneName;
        std::vector<BoneTransform>  m_keyframes;

        const BoneTransform& operator[](int32 index) const;
    };

    BoneTransform Interpolate(const BoneTransform& transformA,
                              const BoneTransform& transformB, f32 ratio);

    BoneTransform operator*(const BoneTransform& transformA, const BoneTransform& transformB);
    BoneTransform operator-(const BoneTransform& transform);


    class Animation final : public IResource
    {
    public:

        ENGINE_API Animation(void) = default;
        ENGINE_API Animation(const void* animData);
                   Animation(const Animation&) = delete;
        ENGINE_API Animation(Animation&&) noexcept = default;
        ENGINE_API ~Animation(void) = default;

        ENGINE_API bool LoadResource(const char* filePath) override;
        ENGINE_API const std::vector<AnimBone>& GetBoneData(void) const;

        // When one or several animations are stored with a model,
        // or when several animations are in a single file, load
        // secondary animations from that data
        ENGINE_API static void LoadExtraAnimations(const void* anims, uint32 offset = 0);

        ENGINE_API uint64 GetBoneCount(void) const;
        ENGINE_API int32 GetTickCount(void) const;
        ENGINE_API f32 GetTicksPerSecond(void) const;

    private:

        // Read entire animation
        void ReadAnimation(const void* animation);

        // Read a single bone's transformations

        void ReadBonePositions(AnimBone& newBone, void* boneAnim);
        void ReadBoneScalings(AnimBone& newBone, void* boneAnim);
        void ReadBoneRotations(AnimBone& newBone, void* boneAnim);

        std::vector<AnimBone>       m_boneData;
        int32                       m_tickCount = -1;
        f32                         m_ticksPerSecond = 0.f;
    };
}
