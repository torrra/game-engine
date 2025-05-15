#include "ui/gizmos/RotationGizmos.h"
#include <engine/resource/ResourceManager.h>
#include <engine/resource/model/Model.h>
#include <engine/core/components/Transform.h>
#include <engine/utility/GraphicsHelperFunctions.h>
#include <engine/input/Input.h>
#include <engine/ui/UIComponent.h>

#define ROTATION_GIZMOS_MODEL ".\\assets\\RotationGizmosAxis.obj"

editor::RotationGizmos::RotationGizmos(void)
{
    m_xAxis = GizmosAxis({-0.000f,  0.707f, 0.707f, 0.0f}, ROTATION_GIZMOS_MODEL, {1.0f, 0.0f, 0.0f}, COLOR_MAX - 3);  // X axis
    m_yAxis = GizmosAxis({0.707f,  0.000f, 0.707f, 0.0f}, ROTATION_GIZMOS_MODEL, {0.0f, 0.0f, 1.0f}, COLOR_MAX - 4);  // Y axis
    m_zAxis = GizmosAxis({-0.707f, 0.707f,  0.000f, 0.0f}, ROTATION_GIZMOS_MODEL, {0.0f, 1.0f, 0.0f}, COLOR_MAX - 5); // Z axis
    
    static bool isInit = false;
    if (!isInit)
    {
        // Init resources
        engine::ResourceManager::Load<engine::Model>(ROTATION_GIZMOS_MODEL);
        isInit = true;
    }
}

void editor::RotationGizmos::IsAxisSelected(std::string const& wndName, engine::Transform* transform)
{
    if (m_hideGizmos)
        return;

    engine::SendCmdsToGPU();
    engine::SetPixelStorageMode();

    // Inverse cursor Y axis for picking position due to picking color texture being flipped on X axis
    math::Vector2i mousePos = ui::GetWindowCursorPos(wndName.c_str());
    int32 height = static_cast<int32>(ui::GetWindowContentSize(wndName.c_str()).GetY());
    mousePos.Y() = height - mousePos.GetY();

    // Get pixel color data at mouse position
    uint8 data[4];
    engine::GetPixelData(mousePos, {1, 1}, data);

    uint32 gizmosID = uint32(data[0] + data[1] * 256 + data[2] * 256 * 256);

    if (engine::Input::IsInputPressed(MOUSE_BUTTON_LEFT) && m_selectedAxis == 0)
    {
        if (gizmosID == m_xAxis.GetPickingID())
            m_selectedAxis = X_AXIS;
        else if (gizmosID == m_yAxis.GetPickingID())
            m_selectedAxis = Y_AXIS;
        else if (gizmosID == m_zAxis.GetPickingID())
            m_selectedAxis = Z_AXIS;
    }

    math::Vector2f cursorPos = engine::Input::GetCursorPosition<f32>();
    math::Vector2f deltaCursorPos = cursorPos - m_prevCursorPos;

    math::Vector3f deltaRotation(0.0f);
    if (m_selectedAxis == X_AXIS)
        m_selectedAxis = (m_xAxis.OnSelectedRotation(deltaRotation, {1.0f, 0.0f, 0.0f}, deltaCursorPos.GetY())) ? X_AXIS : NO_AXIS;
    else if (m_selectedAxis == Y_AXIS)
        m_selectedAxis = (m_yAxis.OnSelectedRotation(deltaRotation, {0.0f, 1.0f, 0.0f}, -deltaCursorPos.GetX())) ? Y_AXIS : NO_AXIS;
    else if (m_selectedAxis == Z_AXIS)
        m_selectedAxis = (m_zAxis.OnSelectedRotation(deltaRotation, {0.0f, 0.0f, 1.0f}, -deltaCursorPos.GetY())) ? Z_AXIS : NO_AXIS;

    transform->AddRotation(deltaRotation.GetX(), deltaRotation.GetY(), deltaRotation.GetZ());
    m_prevCursorPos = cursorPos;
}