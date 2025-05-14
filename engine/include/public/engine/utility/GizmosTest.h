#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "engine/core/Entity.h"

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>
#include <math/Matrix4.hpp>

namespace engine
{
    class GizmosAxis
    {
    public:
        GizmosAxis(void) = delete;
        GizmosAxis(math::Quatf const& direction, std::string const& modelPath, math::Vector3f const& color, uint32 pickingID);
        ~GizmosAxis(void) = default;

        void RenderPass(const math::Matrix4f& projViewMatrix);
        void RenderPickingPass(const math::Matrix4f& projViewMatrix);
        bool OnSelectedPosition(math::Vector3f& position, math::Vector3f const& axis, f32 moveIncrement);
        bool OnSelectedRotation(math::Vector3f& rotation, math::Vector3f const& axis, f32 rotateIncrement);
        bool OnSelectedScale(math::Vector3f& scale, math::Vector3f const& axis, f32 scaleIncrement);

        uint32 GetPickingID(void) const;
        void SetPosition(math::Vector3f const& position);
        void SetScale(math::Vector3f const& size);
        void SetColor(math::Vector3f const& color);

    private:
        math::Matrix4f m_modelMatrix;
        std::string m_modelPath;
        math::Quatf m_direction;
        math::Vector3f m_color;
        uint32 m_pickingID;
    };

    class TranslationGizmos
    {
    public:
        ENGINE_API TranslationGizmos(void);
        ENGINE_API ~TranslationGizmos(void) = default;

        ENGINE_API void RenderPass(const math::Matrix4f& projViewMatrix);
        ENGINE_API void RenderPickingPass(const math::Matrix4f& projViewMatrix);
        ENGINE_API void IsAxisSelected(std::string const& wndName, class Transform* transform);
        ENGINE_API void HideGizmos(bool value);

        ENGINE_API bool IsGizmosSelected(void) const;
        ENGINE_API void SetPosition(math::Vector3f const& position);

    private:
        GizmosAxis m_xAxis;
        GizmosAxis m_yAxis;
        GizmosAxis m_zAxis;
        math::Vector3f m_position;
        math::Vector2f m_prevCursorPos;
        uint8 m_selectedAxis;
        bool m_hideGizmos;
    };

    class RotationGizmos
    {
    public:
        ENGINE_API RotationGizmos(void);
        ENGINE_API ~RotationGizmos(void) = default;

        ENGINE_API void RenderPass(const math::Matrix4f& projViewMatrix);
        ENGINE_API void RenderPickingPass(const math::Matrix4f& projViewMatrix);
        ENGINE_API void IsAxisSelected(std::string const& wndName, class Transform* transform);
        ENGINE_API void HideGizmos(bool value);

        ENGINE_API bool IsGizmosSelected(void) const;
        ENGINE_API void SetPosition(math::Vector3f const& position);

    private:
        GizmosAxis m_xAxis;
        GizmosAxis m_yAxis;
        GizmosAxis m_zAxis;
        math::Quatf m_rotation;
        math::Vector3f m_position;
        math::Vector2f m_prevCursorPos;
        uint8 m_selectedAxis;
        bool m_hideGizmos;
    };

    class ScaleGizmos
    {
    public:
        ENGINE_API ScaleGizmos(void);
        ENGINE_API ~ScaleGizmos(void) = default;

        ENGINE_API void RenderPass(const math::Matrix4f& projViewMatrix);
        ENGINE_API void RenderPickingPass(const math::Matrix4f& projViewMatrix);
        ENGINE_API void IsAxisSelected(std::string const& wndName, class Transform* transform);
        ENGINE_API void HideGizmos(bool value);

        ENGINE_API bool IsGizmosSelected(void) const;
        ENGINE_API void SetPosition(math::Vector3f const& position);

    private:
        GizmosAxis m_xAxis;
        GizmosAxis m_yAxis;
        GizmosAxis m_zAxis;
        math::Vector3f m_position;
        math::Vector2f m_prevCursorPos;
        uint8 m_selectedAxis;
        bool m_hideGizmos;
    };

}