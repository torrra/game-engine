#pragma once

#include <engine/CoreTypes.h>
#include <engine/core/Entity.h>

#include <math/Vector3.hpp>
#include <math/Quaternion.hpp>
#include <math/Matrix4.hpp>

#define COLOR_MAX 16'000'000
#define NO_AXIS 0
#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3

namespace engine
{
    class Transform;
}

namespace editor
{
    class GizmosAxis
    {
    public:
        GizmosAxis(void);
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

    class Gizmos
    {
    public:
        Gizmos(void);
        ~Gizmos(void) = default;

        void RenderPass(const math::Matrix4f& projViewMatrix);
        void RenderPickingPass(const math::Matrix4f& projViewMatrix);
        virtual void IsAxisSelected(std::string const& wndName, engine::Transform* transform) = 0;
        void HideGizmos(bool value);

        bool IsGizmosSelected(void) const;
        void SetPosition(math::Vector3f const& position);

    protected:
        class GizmosAxis m_xAxis;
        class GizmosAxis m_yAxis;
        class GizmosAxis m_zAxis;
        math::Vector3f m_position;
        math::Vector2f m_prevCursorPos;
        uint8 m_selectedAxis;
        bool m_hideGizmos;
    };
}