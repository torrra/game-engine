#pragma once

#include <math/Vector3.hpp>
#include <math/Matrix4.hpp>
#include <math/Quaternion.hpp>

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "engine/core/Component.h"
#include "engine/core/components/Transform.h"

namespace engine
{
    struct Frustum
    {
        f32 m_near = 0.005f;
        f32 m_far = 250.0f;
        f32 m_fovRad = 1.0471976f;
        f32 m_ratio = 1.77777777778f;

    };
    class Camera : public Component

    {
    public:
       

        ENGINE_API Camera(EntityHandle, class SceneGraph*);
        ENGINE_API Camera(Camera&&) noexcept = default;
        ENGINE_API Camera(const Camera&) = default;

        ENGINE_API ~Camera(void) = default;

        ENGINE_API void Move(const math::Vector3f& translation, f32 speed, f32 deltaTime);
        ENGINE_API void Rotate(f32 deltaPitch, f32 deltaYaw, f32 deltaRoll, f32 rotationSpeed);
        ENGINE_API math::Matrix4f ViewProjection(void);

        ENGINE_API void Register(void) override;
        ENGINE_API void Unregister(void) override;

        ENGINE_API math::Vector3f GetPosition(void) const noexcept;
        ENGINE_API math::Vector3f GetRotation(void) const noexcept;
        ENGINE_API math::Quatf GetRotationQuat(void) const noexcept;

        ENGINE_API f32 GetFOV(void) const noexcept;
        ENGINE_API f32 GetNearPlane(void) const noexcept;
        ENGINE_API f32 GetFarPlane(void) const noexcept;

        ENGINE_API math::Vector3f& Position(void);
        ENGINE_API math::Vector3f& Rotation(void);

        ENGINE_API void SetFOV(f32 fov);
        ENGINE_API void SetNearPlane(f32 nearPlane);
        ENGINE_API void SetFarPlane(f32 farPlane);

        ENGINE_API void SerializeText(std::ostream& output,
                                      EntityHandle owner,
                                      uint64 index) const override;
        ENGINE_API
        const char* DeserializeText(const char* text, const char* end) override;

        ENGINE_API
        Camera& operator=(const Camera&) = default;

        ENGINE_API
        Camera& operator=(Camera&&) noexcept = default;

        ENGINE_API
        math::Matrix4f GetViewMatrix(Transform* inTransform);

        ENGINE_API
        const math::Matrix4f& GetProjectionMatrix(void) const;

    private:
        void CalculateProjectionMatrix(void);

        f32 RotateAxis(f32 existingAngle, f32 deltaAngle, f32 rotationSpeed);

        Transform*     m_transform;
        Frustum        m_frustum;
        math::Quatf    m_rotQuat = math::Quatf(1.f, 0.f, 0.f, 0.f);
        math::Matrix4f m_projectionMatrix{1.f};
        math::Vector3f m_rotation{ 0.f };
    };

    template<>
    inline constexpr Entity::EComponentFlags Entity::GetComponentFlag<Camera>()
    {
        return CAMERA;
    }

}
