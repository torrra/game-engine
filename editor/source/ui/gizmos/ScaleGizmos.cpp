#include "ui/gizmos/ScaleGizmos.h"
#include <engine/input/InputHandler.h>
#include <engine/ui/UIComponent.h>
#include <engine/utility/GraphicsHelperFunctions.h>
#include <engine/resource/ResourceManager.h>
#include <engine/resource/model/Model.h>
#include <engine/core/components/Transform.h>

#define SCALE_GIZMOS_MODEL ".\\assets\\ScaleGizmosAxis.obj"

editor::ScaleGizmos::ScaleGizmos(void)
{
    m_xAxis = GizmosAxis({0.707f,   0.0f,   0.0f, 0.707f}, SCALE_GIZMOS_MODEL, {1.0f, 0.0f, 0.0f}, COLOR_MAX - 6);      // X axis
    m_yAxis = GizmosAxis({0.707f,   0.0f, 0.707f,   0.0f}, SCALE_GIZMOS_MODEL, {0.0f, 0.0f, 1.0f}, COLOR_MAX - 7);  // Y axis
    m_zAxis = GizmosAxis({-0.707f, 0.707f,   0.0f,   0.0f}, SCALE_GIZMOS_MODEL, {0.0f, 1.0f, 0.0f}, COLOR_MAX - 8);  // Z axis

    static bool isInit = false;
    if (!isInit)
    {
        // Init resource
        engine::ResourceManager::Load<engine::Model>(SCALE_GIZMOS_MODEL, true);
        isInit = true;
    }
}

void editor::ScaleGizmos::IsAxisSelected(std::string const& wndName, engine::Transform* transform)
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

    if (engine::InputHandler::IsInputPressed(MOUSE_BUTTON_LEFT) && m_selectedAxis == 0)
    {
        if (gizmosID == m_xAxis.GetPickingID())
            m_selectedAxis = X_AXIS;
        else if (gizmosID == m_yAxis.GetPickingID())
            m_selectedAxis = Y_AXIS;
        else if (gizmosID == m_zAxis.GetPickingID())
            m_selectedAxis = Z_AXIS;
    }

    math::Vector2f cursorPos = engine::InputHandler::GetCursorPosition<f32>();
    math::Vector2f deltaCursorPos = cursorPos - m_prevCursorPos;

    if (m_selectedAxis == X_AXIS)
        m_selectedAxis = (m_xAxis.OnSelectedScale(transform->SetScale(), {1.0f, 0.0f, 0.0f}, deltaCursorPos.GetX())) ? X_AXIS : NO_AXIS;
    else if (m_selectedAxis == Y_AXIS)
        m_selectedAxis = (m_yAxis.OnSelectedScale(transform->SetScale(), {0.0f, 1.0f, 0.0f}, -deltaCursorPos.GetY())) ? Y_AXIS : NO_AXIS;
    else if (m_selectedAxis == Z_AXIS)
        m_selectedAxis = (m_zAxis.OnSelectedScale(transform->SetScale(), {0.0f, 0.0f, 1.0f}, -deltaCursorPos.GetX())) ? Z_AXIS : NO_AXIS;

    m_prevCursorPos = cursorPos;
}