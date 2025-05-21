#include "ui/gizmos/Gizmos.h"
#include <engine/input/InputHandler.h>
#include <engine/utility/GraphicsHelperFunctions.h>
#include <engine/resource/ResourceManager.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/shader/Shader.h>

#include <math/Arithmetic.hpp>

#define TRANSLATION_GIZMOS_MODEL ".\\assets\\TranslationGizmosAxis.obj"
#define ROTATION_GIZMOS_MODEL ".\\assets\\RotationGizmosAxis.obj"
#define SCALE_GIZMOS_MODEL ".\\assets\\ScaleGizmosAxis.obj"

#define COLOR_MAX 16'000'000
#define MAX_INCREMENT 0.0125f

editor::Gizmos::Gizmos(void)
{
    m_position = math::Vector3f(0.0f);
    m_selectedAxis = 0;
    m_hideGizmos = true;

    static bool isInit = false;
    if (!isInit)
    {
        engine::ResourceManager::LoadShader("GizmosShader", ".\\shaders\\Model.vs", ".\\shaders\\Model.frag", true, true);
        isInit = true;
    }
}

void editor::Gizmos::RenderPass(const math::Matrix4f& projViewMatrix)
{
    if (m_hideGizmos)
        return;

    engine::DisableDepthTest();
    m_xAxis.RenderPass(projViewMatrix);
    m_zAxis.RenderPass(projViewMatrix);
    m_yAxis.RenderPass(projViewMatrix);
    engine::EnableDepthTest();
}

void editor::Gizmos::RenderPickingPass(const math::Matrix4f & projViewMatrix)
{
    if (m_hideGizmos)
        return;

    engine::DisableDepthTest();
    m_xAxis.RenderPickingPass(projViewMatrix);
    m_zAxis.RenderPickingPass(projViewMatrix);
    m_yAxis.RenderPickingPass(projViewMatrix);
    engine::EnableDepthTest();
}

void editor::Gizmos::HideGizmos(bool value)
{
    m_hideGizmos = value;

    if (m_hideGizmos)
        m_selectedAxis = NO_AXIS;
}

bool editor::Gizmos::IsGizmosSelected(void) const
{
    return m_selectedAxis != NO_AXIS;
}

void editor::Gizmos::SetPosition(math::Vector3f const& position)
{
    m_xAxis.SetPosition(position);
    m_yAxis.SetPosition(position);
    m_zAxis.SetPosition(position);
}

editor::GizmosAxis::GizmosAxis(void)
    : m_modelMatrix(1.0f), m_modelPath("./"), m_direction(1.0f, 0.0f, 0.0f, 0.0f), 
      m_color(1.0f), m_pickingID(0)
{}

editor::GizmosAxis::GizmosAxis(math::Quatf const& direction, std::string const& modelPath, math::Vector3f const& color, uint32 pickingID)
    : m_direction(direction), m_modelPath(modelPath), m_color(color), m_pickingID(pickingID)
{
    m_modelMatrix = m_direction.Inverse().RotationMatrix();
}

void editor::GizmosAxis::RenderPass(const math::Matrix4f& projViewMatrix)
{
    const engine::Model* model = engine::ResourceManager::GetResource<engine::Model>(m_modelPath);
    const engine::ShaderProgram* shader = engine::ResourceManager::GetResource<engine::ShaderProgram>("GizmosShader");

    math::Matrix3f normalMatrix(1.0f);
    const math::Matrix4f& mvpMatrix = projViewMatrix * m_modelMatrix;
    mvpMatrix;
    shader->Use();
    shader->Set("mvp", &mvpMatrix);
    shader->Set("normalMat", &normalMatrix);
    shader->Set("objectColor", m_color);

    engine::Draw(
        model->GetStaticMeshes()[0].GetVertexArrayID(), 
        model->GetStaticMeshes()[0].GetIndexCount()
    );
}

void editor::GizmosAxis::RenderPickingPass(const math::Matrix4f& projViewMatrix)
{
    const engine::Model* model = engine::ResourceManager::GetResource<engine::Model>(m_modelPath);
    const engine::ShaderProgram* pickingShader = engine::ResourceManager::GetResource<engine::ShaderProgram>("Picking Shader");

    // Calculate the color via the entities picking identifier
    uint8 r = static_cast<uint8>((m_pickingID & 0x000000FF) >> 0);
    uint8 g = static_cast<uint8>((m_pickingID & 0x0000FF00) >> 8);
    uint8 b = static_cast<uint8>((m_pickingID & 0x00FF0000) >> 16);

    // Convert color components (red, green, blue) to float within 0 - 1 range
    constexpr f32 denominator = 1.0f / 255.0f;
    math::Vector3f color =
    {
        static_cast<f32>(r) * denominator,
        static_cast<f32>(g) * denominator,
        static_cast<f32>(b) * denominator
    };

    const math::Matrix4f& mvp = projViewMatrix * m_modelMatrix;
    pickingShader->Use();
    pickingShader->Set("mvp", &mvp);
    pickingShader->Set("pickingColor", color);

    engine::Draw(
        model->GetStaticMeshes()[0].GetVertexArrayID(),
        model->GetStaticMeshes()[0].GetIndexCount()
    );
}

bool editor::GizmosAxis::OnSelectedPosition(math::Vector3f& position, math::Vector3f const& axis, f32 moveIncrement)
{
    if (engine::InputHandler::IsInputReleased(MOUSE_BUTTON_LEFT))
        return false;

    position += axis * math::Clamp(moveIncrement, -MAX_INCREMENT, MAX_INCREMENT);

    return true;
}

bool editor::GizmosAxis::OnSelectedRotation(math::Vector3f& rotation, math::Vector3f const& axis, f32 rotateIncrement)
{
    if (engine::InputHandler::IsInputReleased(MOUSE_BUTTON_LEFT))
        return false;

    rotation += axis * rotateIncrement;

    return true;
}

bool editor::GizmosAxis::OnSelectedScale(math::Vector3f& scale, math::Vector3f const& axis, f32 scaleIncrement)
{
    if (engine::InputHandler::IsInputReleased(MOUSE_BUTTON_LEFT))
        return false;

    scale += axis * math::Clamp(scaleIncrement, -MAX_INCREMENT, MAX_INCREMENT);
    scale = {
        math::Clamp(scale.GetX(), 0.05f, FLT_MAX),
        math::Clamp(scale.GetY(), 0.05f, FLT_MAX),
        math::Clamp(scale.GetZ(), 0.05f, FLT_MAX)
    };

    return true;
}

uint32 editor::GizmosAxis::GetPickingID(void) const
{
    return m_pickingID;
}

void editor::GizmosAxis::SetPosition(math::Vector3f const& position)
{
    m_modelMatrix[3][0] = position.GetX();
    m_modelMatrix[3][1] = position.GetY();
    m_modelMatrix[3][2] = position.GetZ();
}

void editor::GizmosAxis::SetScale(math::Vector3f const& size)
{
    m_modelMatrix[0][0] = size.GetX();
    m_modelMatrix[1][1] = size.GetY();
    m_modelMatrix[2][2] = size.GetZ();
}

void editor::GizmosAxis::SetColor(math::Vector3f const& color)
{
    m_color = color;
}