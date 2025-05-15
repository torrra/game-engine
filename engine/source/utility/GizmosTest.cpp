//#include "utility/GizmosTest.h"
//#include "resource/ResourceManager.h"
//#include "resource/model/Model.h"
//#include "resource/shader/Shader.h"
//#include "core/SceneGraph.h"
//#include "core/Entity.h"
//#include "core/components/Transform.h"
//#include "utility/GraphicsHelperFunctions.h"
//#include "ui/UIComponent.h"
//#include "input/Input.h"
//#include <glad/glad.h>
//#include <imgui/imgui.h>
//
//#include "utility/MemoryCheck.h"
//
//#define TRANSLATION_GIZMOS_MODEL ".\\assets\\TranslationGizmosAxis.obj"
//#define ROTATION_GIZMOS_MODEL ".\\assets\\RotationGizmosAxis.obj"
//#define SCALE_GIZMOS_MODEL ".\\assets\\ScaleGizmosAxis.obj"
//
//#define COLOR_MAX 16'000'000
//#define MAX_INCREMENT 0.0125f
//#define NO_AXIS 0
//#define X_AXIS 1
//#define Y_AXIS 2
//#define Z_AXIS 3
//
//engine::GizmosAxis::GizmosAxis(math::Quatf const& direction, std::string const& modelPath, math::Vector3f const& color, uint32 pickingID)
//    : m_direction(direction), m_modelPath(modelPath), m_color(color), m_pickingID(pickingID)
//{
//    m_modelMatrix = m_direction.Inverse().RotationMatrix();
//}
//
//void engine::GizmosAxis::RenderPass(const math::Matrix4f& projViewMatrix)
//{
//    const Model* model = ResourceManager::GetResource<Model>(m_modelPath);
//    const ShaderProgram* shader = ResourceManager::GetResource<ShaderProgram>("GizmosShader");
//
//    math::Matrix3f normalMatrix(1.0f);
//    const math::Matrix4f& mvpMatrix = projViewMatrix * m_modelMatrix;
//    mvpMatrix;
//    shader->Use();
//    shader->Set("mvp", &mvpMatrix);
//    shader->Set("normalMat", &normalMatrix);
//    shader->Set("objectColor", m_color );
//
//    glBindVertexArray(model->GetStaticMeshes()[0].GetVertexArrayID());
//    glDrawElements(GL_TRIANGLES, model->GetStaticMeshes()[0].GetIndexCount(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//}
//
//void engine::GizmosAxis::RenderPickingPass(const math::Matrix4f& projViewMatrix)
//{
//    const Model* model = ResourceManager::GetResource<Model>(m_modelPath);
//    const ShaderProgram* pickingShader = ResourceManager::GetResource<ShaderProgram>("Picking Shader");
//
//    // Calculate the color via the entities picking identifier
//    uint8 r = static_cast<uint8>((m_pickingID & 0x000000FF) >> 0);
//    uint8 g = static_cast<uint8>((m_pickingID & 0x0000FF00) >> 8);
//    uint8 b = static_cast<uint8>((m_pickingID & 0x00FF0000) >> 16);
//
//    // Convert color components (red, green, blue) to float within 0 - 1 range
//    constexpr f32 denominator = 1.0f / 255.0f;
//    math::Vector3f color =
//    {
//        static_cast<f32>(r) * denominator,
//        static_cast<f32>(g) * denominator,
//        static_cast<f32>(b) * denominator
//    };
//
//    const math::Matrix4f& mvp = projViewMatrix * m_modelMatrix;
//    pickingShader->Use();
//    pickingShader->Set("mvp", &mvp);
//    pickingShader->Set("pickingColor", color);
//
//    glBindVertexArray(model->GetStaticMeshes()[0].GetVertexArrayID());
//    glDrawElements(GL_TRIANGLES, model->GetStaticMeshes()[0].GetIndexCount(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//}
//
//bool engine::GizmosAxis::OnSelectedPosition(math::Vector3f& position, math::Vector3f const& axis, f32 moveIncrement)
//{
//    if (ImGui::IsKeyReleased(ImGuiKey_MouseLeft))
//        return false;
//
//    position += axis * math::Clamp(moveIncrement, -MAX_INCREMENT, MAX_INCREMENT);
//
//    return true;
//}
//
//bool engine::GizmosAxis::OnSelectedRotation(math::Vector3f& rotation, math::Vector3f const& axis, f32 rotateIncrement)
//{
//    if (ImGui::IsKeyReleased(ImGuiKey_MouseLeft))
//        return false;
//
//    rotation += axis * rotateIncrement;
//
//    return true;
//}
////
////bool engine::GizmosAxis::OnSelectedScale(math::Vector3f& scale, math::Vector3f const& axis, f32 scaleIncrement)
//{
//    if (ImGui::IsKeyReleased(ImGuiKey_MouseLeft))
//    {
//
//        return false;
//    }
//
//    scale += axis * math::Clamp(scaleIncrement, -MAX_INCREMENT, MAX_INCREMENT);
//    scale = {
//        math::Clamp(scale.GetX(), 0.05f, FLT_MAX),
//        math::Clamp(scale.GetY(), 0.05f, FLT_MAX),
//        math::Clamp(scale.GetZ(), 0.05f, FLT_MAX)
//    };
//
//    return true;
//}
////
////uint32 engine::GizmosAxis::GetPickingID(void) const
//{
//    return m_pickingID;
//}
////
////void engine::GizmosAxis::SetPosition(math::Vector3f const& position)
//{
//    m_modelMatrix[3][0] = position.GetX();
//    m_modelMatrix[3][1] = position.GetY();
//    m_modelMatrix[3][2] = position.GetZ();
//}
////
////void engine::GizmosAxis::SetScale(math::Vector3f const& size)
//{
//    m_modelMatrix[0][0] = size.GetX();
//    m_modelMatrix[1][1] = size.GetY();
//    m_modelMatrix[2][2] = size.GetZ();
//}
//
//void engine::GizmosAxis::SetColor(math::Vector3f const& color)
//{
//    m_color = color;
//}
//
//// Translation Gizmos
//#pragma region TranslationGizmos
//engine::TranslationGizmos::TranslationGizmos(void)
//    : m_xAxis({ 0.707f,   0.0f,   0.0f, 0.707f}, TRANSLATION_GIZMOS_MODEL, {1.0f, 0.0f, 0.0f}, COLOR_MAX),      // X axis
//      m_yAxis({ 0.707f,   0.0f, 0.707f,   0.0f}, TRANSLATION_GIZMOS_MODEL, {0.0f, 0.0f, 1.0f}, COLOR_MAX - 1),  // Y axis
//      m_zAxis({-0.707f, 0.707f,   0.0f,   0.0f}, TRANSLATION_GIZMOS_MODEL, {0.0f, 1.0f, 0.0f}, COLOR_MAX - 2),  // Z axis
//      m_selectedAxis(0),
//      m_position(0.0f),
//      m_hideGizmos(true)
//{
//    // Init resources
//    ResourceManager::Load<Model>(TRANSLATION_GIZMOS_MODEL);
//    ResourceManager::LoadShader("GizmosShader", ".\\shaders\\Model.vs", ".\\shaders\\Model.frag");
//}
//
//void engine::TranslationGizmos::RenderPass(const math::Matrix4f& projViewMatrix)
//{
//    if (m_hideGizmos)
//        return;
//
//    glDisable(GL_DEPTH_TEST);
//    m_xAxis.RenderPass(projViewMatrix);
//    m_zAxis.RenderPass(projViewMatrix);
//    m_yAxis.RenderPass(projViewMatrix);
//    glEnable(GL_DEPTH_TEST);
//}
//
//void engine::TranslationGizmos::RenderPickingPass(const math::Matrix4f& projViewMatrix)
//{
//    if (m_hideGizmos)
//        return;
//
//    glDisable(GL_DEPTH_TEST);
//    m_xAxis.RenderPickingPass(projViewMatrix);
//    m_zAxis.RenderPickingPass(projViewMatrix);
//    m_yAxis.RenderPickingPass(projViewMatrix);
//    glEnable(GL_DEPTH_TEST);
//}
//
//void engine::TranslationGizmos::IsAxisSelected(std::string const& wndName, class engine::Transform* transform)
//{
//    if (m_hideGizmos)
//        return;
//
//    engine::SendCmdsToGPU();
//    engine::SetPixelStorageMode();
//
//    // Inverse cursor Y axis for picking position due to picking color texture being flipped on X axis
//    math::Vector2i mousePos = ui::GetWindowCursorPos(wndName.c_str());
//    int32 height = static_cast<int32>(ui::GetWindowContentSize(wndName.c_str()).GetY());
//    mousePos.Y() = height - mousePos.GetY();
//
//    // Get pixel color data at mouse position
//    uint8 data[4];
//    engine::GetPixelData(mousePos, {1, 1}, data);
//
//    uint32 gizmosID = uint32(data[0] + data[1] * 256 + data[2] * 256 * 256);
//    
//    if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft) && m_selectedAxis == 0)
//    {
//        if (gizmosID == m_xAxis.GetPickingID())
//            m_selectedAxis = X_AXIS;
//        else if (gizmosID == m_yAxis.GetPickingID())
//            m_selectedAxis = Y_AXIS;
//        else if (gizmosID == m_zAxis.GetPickingID())
//            m_selectedAxis = Z_AXIS;
//    }
//
//    math::Vector2f cursorPos = ImGui::GetMousePos();
//    math::Vector2f deltaCursorPos = cursorPos - m_prevCursorPos;
//
//    if (m_selectedAxis == X_AXIS)
//        m_selectedAxis = (m_xAxis.OnSelectedPosition(m_position, {1.0f, 0.0f, 0.0f}, deltaCursorPos.GetX())) ? X_AXIS : NO_AXIS;
//    else if (m_selectedAxis == Y_AXIS)
//        m_selectedAxis = (m_yAxis.OnSelectedPosition(m_position, {0.0f, 1.0f, 0.0f}, -deltaCursorPos.GetY())) ? Y_AXIS : NO_AXIS;
//    else if (m_selectedAxis == Z_AXIS)
//        m_selectedAxis = (m_zAxis.OnSelectedPosition(m_position, {0.0f, 0.0f, 1.0f}, -deltaCursorPos.GetX())) ? Z_AXIS : NO_AXIS;
//
//    m_prevCursorPos = cursorPos;
//    transform->SetPosition(m_position);
//    SetPosition(m_position);
//}
//
//void engine::TranslationGizmos::HideGizmos(bool value)
//{
//    m_hideGizmos = value;
//
//    if (m_hideGizmos)
//        m_selectedAxis = NO_AXIS;
//}
//
//bool engine::TranslationGizmos::IsGizmosSelected(void) const
//{
//    return m_selectedAxis != NO_AXIS;
//}
//
//void engine::TranslationGizmos::SetPosition(math::Vector3f const& position)
//{
//    m_xAxis.SetPosition(position);
//    m_yAxis.SetPosition(position);
//    m_zAxis.SetPosition(position);
//}
//#pragma endregion
//
//// Rotation Gizmos
//#pragma region RotationGizmos
//engine::RotationGizmos::RotationGizmos(void)
//    : m_xAxis({-0.000f,  0.707f, 0.707f, 0.0f}, ROTATION_GIZMOS_MODEL, {1.0f, 0.0f, 0.0f}, COLOR_MAX - 3),  // X axis
//      m_yAxis({ 0.707f,  0.000f, 0.707f, 0.0f}, ROTATION_GIZMOS_MODEL, {0.0f, 0.0f, 1.0f}, COLOR_MAX - 4),  // Y axis
//      m_zAxis({-0.707f, 0.707f,  0.000f, 0.0f}, ROTATION_GIZMOS_MODEL, {0.0f, 1.0f, 0.0f}, COLOR_MAX - 5), // Z axis
//      m_position(0.0f),
//      m_selectedAxis(0),
//      m_hideGizmos(true)
//{
//    // Init resources
//    ResourceManager::Load<Model>(ROTATION_GIZMOS_MODEL);
//    ResourceManager::LoadShader("GizmosShader", ".\\shaders\\Model.vs", ".\\shaders\\Model.frag");
//}
//
//void engine::RotationGizmos::RenderPass(const math::Matrix4f& projViewMatrix)
//{
//    if (m_hideGizmos)
//        return;
//
//    glDisable(GL_DEPTH_TEST);
//    m_xAxis.RenderPass(projViewMatrix);
//    m_zAxis.RenderPass(projViewMatrix);
//    m_yAxis.RenderPass(projViewMatrix);
//    glEnable(GL_DEPTH_TEST);
//}
//
//void engine::RotationGizmos::RenderPickingPass(const math::Matrix4f& projViewMatrix)
//{
//    if (m_hideGizmos)
//        return;
//
//    glDisable(GL_DEPTH_TEST);
//    m_xAxis.RenderPickingPass(projViewMatrix);
//    m_zAxis.RenderPickingPass(projViewMatrix);
//    m_yAxis.RenderPickingPass(projViewMatrix);
//    glEnable(GL_DEPTH_TEST);
//}
//
//void engine::RotationGizmos::IsAxisSelected(std::string const& wndName, engine::Transform* transform)
//{
//    if (m_hideGizmos)
//        return;
//
//    engine::SendCmdsToGPU();
//    engine::SetPixelStorageMode();
//
//    // Inverse cursor Y axis for picking position due to picking color texture being flipped on X axis
//    math::Vector2i mousePos = ui::GetWindowCursorPos(wndName.c_str());
//    int32 height = static_cast<int32>(ui::GetWindowContentSize(wndName.c_str()).GetY());
//    mousePos.Y() = height - mousePos.GetY();
//
//    // Get pixel color data at mouse position
//    uint8 data[4];
//    engine::GetPixelData(mousePos, {1, 1}, data);
//
//    uint32 gizmosID = uint32(data[0] + data[1] * 256 + data[2] * 256 * 256);
//
//    if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft) && m_selectedAxis == 0)
//    {
//        if (gizmosID == m_xAxis.GetPickingID())
//            m_selectedAxis = X_AXIS;
//        else if (gizmosID == m_yAxis.GetPickingID())
//            m_selectedAxis = Y_AXIS;
//        else if (gizmosID == m_zAxis.GetPickingID())
//            m_selectedAxis = Z_AXIS;
//    }
//
//    math::Vector2f cursorPos = ImGui::GetMousePos();
//    math::Vector2f deltaCursorPos = cursorPos - m_prevCursorPos;
//    
//    math::Vector3f deltaRotation(0.0f);
//    if (m_selectedAxis == X_AXIS)
//        m_selectedAxis = (m_xAxis.OnSelectedRotation(deltaRotation, {1.0f, 0.0f, 0.0f}, deltaCursorPos.GetY())) ? X_AXIS : NO_AXIS;
//    else if (m_selectedAxis == Y_AXIS)
//        m_selectedAxis = (m_yAxis.OnSelectedRotation(deltaRotation, {0.0f, 1.0f, 0.0f}, -deltaCursorPos.GetX())) ? Y_AXIS : NO_AXIS;
//    else if (m_selectedAxis == Z_AXIS)
//        m_selectedAxis = (m_zAxis.OnSelectedRotation(deltaRotation, {0.0f, 0.0f, 1.0f}, -deltaCursorPos.GetY())) ? Z_AXIS : NO_AXIS;
//
//    transform->AddRotation(deltaRotation.GetX(), deltaRotation.GetY(), deltaRotation.GetZ());
//    m_prevCursorPos = cursorPos;
//}
//
//void engine::RotationGizmos::HideGizmos(bool value)
//{
//    m_hideGizmos = value;
//
//    if (m_hideGizmos)
//        m_selectedAxis = NO_AXIS;
//}
//
//bool engine::RotationGizmos::IsGizmosSelected(void) const
//{
//    return m_selectedAxis != NO_AXIS;
//}
//
//void engine::RotationGizmos::SetPosition(math::Vector3f const& position)
//{
//    m_xAxis.SetPosition(position);
//    m_yAxis.SetPosition(position);
//    m_zAxis.SetPosition(position);
//}
//
//#pragma endregion
//
//// Scale Gizmos
//#pragma region ScaleGizmos
//engine::ScaleGizmos::ScaleGizmos(void)
//    : m_xAxis({ 0.707f,   0.0f,   0.0f, 0.707f}, SCALE_GIZMOS_MODEL, {1.0f, 0.0f, 0.0f}, COLOR_MAX - 6),      // X axis
//      m_yAxis({ 0.707f,   0.0f, 0.707f,   0.0f}, SCALE_GIZMOS_MODEL, {0.0f, 0.0f, 1.0f}, COLOR_MAX - 7),  // Y axis
//      m_zAxis({-0.707f, 0.707f,   0.0f,   0.0f}, SCALE_GIZMOS_MODEL, {0.0f, 1.0f, 0.0f}, COLOR_MAX - 8),  // Z axis
//      m_selectedAxis(0),
//      m_position(0.0f),
//      m_hideGizmos(true)
//{
//    // Init resources
//    ResourceManager::Load<Model>(SCALE_GIZMOS_MODEL);
//    ResourceManager::LoadShader("GizmosShader", ".\\shaders\\Model.vs", ".\\shaders\\Model.frag");
//}
//
//void engine::ScaleGizmos::RenderPass(const math::Matrix4f& projViewMatrix)
//{
//    if (m_hideGizmos)
//        return;
//
//    glDisable(GL_DEPTH_TEST);
//    m_xAxis.RenderPass(projViewMatrix);
//    m_zAxis.RenderPass(projViewMatrix);
//    m_yAxis.RenderPass(projViewMatrix);
//    glEnable(GL_DEPTH_TEST);
//}
//
//void engine::ScaleGizmos::RenderPickingPass(const math::Matrix4f& projViewMatrix)
//{
//    if (m_hideGizmos)
//        return;
//
//    glDisable(GL_DEPTH_TEST);
//    m_xAxis.RenderPickingPass(projViewMatrix);
//    m_zAxis.RenderPickingPass(projViewMatrix);
//    m_yAxis.RenderPickingPass(projViewMatrix);
//    glEnable(GL_DEPTH_TEST);
//}
//
//void engine::ScaleGizmos::IsAxisSelected(std::string const& wndName, engine::Transform* transform)
//{
//    if (m_hideGizmos)
//        return;
//
//    engine::SendCmdsToGPU();
//    engine::SetPixelStorageMode();
//
//    // Inverse cursor Y axis for picking position due to picking color texture being flipped on X axis
//    math::Vector2i mousePos = ui::GetWindowCursorPos(wndName.c_str());
//    int32 height = static_cast<int32>(ui::GetWindowContentSize(wndName.c_str()).GetY());
//    mousePos.Y() = height - mousePos.GetY();
//
//    // Get pixel color data at mouse position
//    uint8 data[4];
//    engine::GetPixelData(mousePos, {1, 1}, data);
//
//    uint32 gizmosID = uint32(data[0] + data[1] * 256 + data[2] * 256 * 256);
//
//    if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft) && m_selectedAxis == 0)
//    {
//        if (gizmosID == m_xAxis.GetPickingID())
//            m_selectedAxis = X_AXIS;
//        else if (gizmosID == m_yAxis.GetPickingID())
//            m_selectedAxis = Y_AXIS;
//        else if (gizmosID == m_zAxis.GetPickingID())
//            m_selectedAxis = Z_AXIS;
//    }
//
//    math::Vector2f cursorPos = ImGui::GetMousePos();
//    math::Vector2f deltaCursorPos = cursorPos - m_prevCursorPos;
//
//    if (m_selectedAxis == X_AXIS)
//        m_selectedAxis = (m_xAxis.OnSelectedScale(transform->SetScale(), {1.0f, 0.0f, 0.0f}, deltaCursorPos.GetX())) ? X_AXIS : NO_AXIS;
//    else if (m_selectedAxis == Y_AXIS)
//        m_selectedAxis = (m_yAxis.OnSelectedScale(transform->SetScale(), {0.0f, 1.0f, 0.0f}, -deltaCursorPos.GetY())) ? Y_AXIS : NO_AXIS;
//    else if (m_selectedAxis == Z_AXIS)
//        m_selectedAxis = (m_zAxis.OnSelectedScale(transform->SetScale(), {0.0f, 0.0f, 1.0f}, -deltaCursorPos.GetX())) ? Z_AXIS : NO_AXIS;
//
//    m_prevCursorPos = cursorPos;
//}
//
//void engine::ScaleGizmos::HideGizmos(bool value)
//{
//    m_hideGizmos = value;
//
//    if (m_hideGizmos)
//        m_selectedAxis = NO_AXIS;
//}
//
//bool engine::ScaleGizmos::IsGizmosSelected(void) const
//{
//    return m_selectedAxis != NO_AXIS;
//}
//
//void engine::ScaleGizmos::SetPosition(math::Vector3f const& position)
//{
//    m_xAxis.SetPosition(position);
//    m_yAxis.SetPosition(position);
//    m_zAxis.SetPosition(position);
//}
//#pragma endregion
//