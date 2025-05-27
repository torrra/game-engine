#pragma once

#include <engine/CoreTypes.h>
#include <math/Matrix4.hpp>
#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>

namespace editor
{
   class EditorCamera
   {
   public:

       EditorCamera(void);
       EditorCamera(const EditorCamera&) = delete;
       EditorCamera(EditorCamera&&) noexcept = default;
       ~EditorCamera(void) = default;

       void             Update(f32 deltaTime);
       void             UpdateAspectRatio(const math::Vector2f& size);
       math::Matrix4f   ViewProjection(void);
       math::Vector3f   GetPosition(void) const;

       EditorCamera& operator=(EditorCamera&&) noexcept = default;

   private:

       void UpdatePosition(f32 deltaTime);
       void UpdateRotation(void);
       void UpdateSpeed(void);
       void CalculateProjectionMatrix(void);

       math::Matrix4f   m_projectionMatrix{ 1.f };
       math::Quatf      m_rotationQuat = math::Quatf::Identity();
       math::Vector3f   m_position = math::Vector3f::Zero();
       math::Vector2f   m_rotationEuler = math::Vector2f::Zero();
       f32              m_speed = 0.5f;
       f32              m_aspectRatio = 1.77777777778f;
       
   };
}