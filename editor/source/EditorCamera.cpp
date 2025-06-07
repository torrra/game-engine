#include "EditorCamera.h"

#include <math/Arithmetic.hpp>

#include <engine/input/InputHandler.h>

editor::EditorCamera::EditorCamera(void)
{
    CalculateProjectionMatrix();
}

void editor::EditorCamera::Update(f32 deltaTime)
{
    if (!engine::InputHandler::IsInputDown(MOUSE_BUTTON_RIGHT))
        return;

    UpdateSpeed();
    UpdatePosition(deltaTime);
    UpdateRotation();
}

void editor::EditorCamera::UpdateAspectRatio(const math::Vector2f& size)
{
    math::Vector2f newSize = size;

    /*
        When resizing the viewport it can sometimes cause the X or Y to become zero
        therefore added these checks to avoid dividing by a negative number or zero
    */
    newSize.X() = (size.GetX() > 0.0f) ? size.GetX() : 1.0f;
    newSize.Y() = (size.GetY() > 0.0f) ? size.GetY() : 1.0f;

    m_aspectRatio = newSize.GetX() / newSize.GetY();
    CalculateProjectionMatrix();
}

math::Matrix4f editor::EditorCamera::GetViewMatrix(void) const
{
    math::Matrix4f view{ 1.f };

    view[3][0] = -m_position.GetX();
    view[3][1] = -m_position.GetY();
    view[3][2] = -m_position.GetZ();

    view = m_rotationQuat.Inverse().RotationMatrix() * view;
    return view;
}

math::Matrix4f editor::EditorCamera::ViewProjection(void)
{
    return m_projectionMatrix * GetViewMatrix();
}

math::Vector3f editor::EditorCamera::GetPosition(void) const
{
    return m_position;
}

const math::Matrix4f& editor::EditorCamera::GetProjectionMatrix(void) const
{
    return m_projectionMatrix;
}

void editor::EditorCamera::UpdatePosition(f32 deltaTime)
{
    const f32 movementScale = m_speed * deltaTime;

    math::Vector3f movement{ 0.f };

    movement.X() += (engine::InputHandler::IsInputDown(KEY_D)) ? movementScale : 0.f;
    movement.X() -= (engine::InputHandler::IsInputDown(KEY_A)) ? movementScale : 0.f;

    movement.Z() -= (engine::InputHandler::IsInputDown(KEY_W)) ? movementScale : 0.f;
    movement.Z() += (engine::InputHandler::IsInputDown(KEY_S)) ? movementScale : 0.f;

    m_position += m_rotationQuat.Rotate(movement);

    if (engine::InputHandler::IsInputDown(KEY_Q))
        m_position.Y() += movementScale;

    if (engine::InputHandler::IsInputDown(KEY_E))
        m_position.Y() -= movementScale;
}

void editor::EditorCamera::UpdateRotation(void)
{
    const math::Vector2f deltaPos = engine::InputHandler::GetCursorDeltaPos<f32>();

    m_rotationEuler.X() -= deltaPos.GetY() * 0.1f;
    m_rotationEuler.Y() -= deltaPos.GetX() * 0.1f;

    m_rotationQuat =
    math::Quatf(
        math::Degree(m_rotationEuler.GetX()),
        math::Degree(m_rotationEuler.GetY()),
        math::Degree(0.f)
    );
}

void editor::EditorCamera::UpdateSpeed(void)
{
    m_speed += engine::InputHandler::GetScrollDelta<f32>().GetY();
    m_speed = math::Clamp(m_speed, 1.f, 100.f);
}

void editor::EditorCamera::CalculateProjectionMatrix(void)
{
    constexpr f32 near = 0.005f;
    constexpr f32 far = 250.f;
    constexpr f32 fovRad = 1.0471976f;

    const f32 tanAngle = tanf(fovRad * 0.5f);
    const f32 farMinusNearDenom = 1.0f / (far - near);

    m_projectionMatrix[0][0] = 1.0f / (m_aspectRatio * tanAngle);
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
