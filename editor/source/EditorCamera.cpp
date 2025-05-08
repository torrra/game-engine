#include "EditorCamera.h"

#include <math/Arithmetic.hpp>

#include <engine/input/Input.h>

editor::EditorCamera::EditorCamera(void)
{
    CalculateProjectionMatrix();
}

void editor::EditorCamera::Update(f32 deltaTime)
{
    if (engine::Input::IsInputDown(KEY_LEFT_ALT) ||
        engine::Input::IsInputDown(KEY_RIGHT_ALT))
    {
        UpdateSpeed();
    }

    if (!engine::Input::IsInputDown(MOUSE_BUTTON_RIGHT))
        return;

    UpdatePosition(deltaTime);
    UpdateRotation();
}

math::Matrix4f editor::EditorCamera::ViewProjection(void)
{
    math::Matrix4f view{ 1.f };

    view[3][0] = -m_position.X();
    view[3][1] = -m_position.Y();
    view[3][2] = -m_position.Z();

    view = m_rotationQuat.Inverse().RotationMatrix() * view;

    return m_projectionMatrix * view;
}

void editor::EditorCamera::UpdatePosition(f32 deltaTime)
{
    const f32 movementScale = m_speed * deltaTime;

    math::Vector3f movement{ 0.f };

    movement.X() += (engine::Input::IsInputDown(KEY_D)) ? movementScale : 0.f;
    movement.X() -= (engine::Input::IsInputDown(KEY_A)) ? movementScale : 0.f;

    movement.Z() -= (engine::Input::IsInputDown(KEY_W)) ? movementScale : 0.f;
    movement.Z() += (engine::Input::IsInputDown(KEY_S)) ? movementScale : 0.f;

    m_position += m_rotationQuat.Rotate(movement);

    if (engine::Input::IsInputDown(KEY_Q))
        m_position.Y() += movementScale;

    if (engine::Input::IsInputDown(KEY_E))
        m_position.Y() -= movementScale;
}

void editor::EditorCamera::UpdateRotation(void)
{
    const math::Vector2f deltaPos = engine::Input::GetCursorDeltaPos<f32>();
    m_rotationEuler.X() -= deltaPos.GetY() * 0.1;
    m_rotationEuler.Y() -= deltaPos.GetX() * 0.1;

    m_rotationQuat =
    math::Quatf(
        math::Degree(m_rotationEuler.GetX()),
        math::Degree(m_rotationEuler.GetY()),
        math::Degree(0.f)
    );
}

void editor::EditorCamera::UpdateSpeed(void)
{
    m_speed += engine::Input::GetScrollDelta<f32>().GetY();
    m_speed = math::Clamp(m_speed, 0.f, 100.f);
}

void editor::EditorCamera::CalculateProjectionMatrix(void)
{
    constexpr f32 near = 0.005f;
    constexpr f32 far = 250.f;
    constexpr f32 fovRad = 1.0471976f;
    constexpr f32 ratio = 1.77777777778f;

    const f32 tanAngle = tanf(fovRad * 0.5f);
    const f32 farMinusNearDenom = 1.0f / (far - near);

    m_projectionMatrix[0][0] = 1.0f / (ratio * tanAngle);
    m_projectionMatrix[1][0] = 0.0f;
    m_projectionMatrix[2][0] = 0.0f;
    m_projectionMatrix[3][0] = 0.0f;

    m_projectionMatrix[0][1] = 0.0f;
    m_projectionMatrix[1][1] = 1.0f / tanAngle;
    m_projectionMatrix[2][1] = 0.0f;
    m_projectionMatrix[3][1] = 0.0f;

    m_projectionMatrix[0][2] = 0.0f;
    m_projectionMatrix[1][2] = 0.0f;
    m_projectionMatrix[2][2] = -(far + near) * farMinusNearDenom;
    m_projectionMatrix[3][2] = -((2.0f * far * near) * farMinusNearDenom);

    m_projectionMatrix[0][3] = 0.0f;
    m_projectionMatrix[1][3] = 0.0f;
    m_projectionMatrix[2][3] = -1.0f;
    m_projectionMatrix[3][3] = 0.0f;
}
