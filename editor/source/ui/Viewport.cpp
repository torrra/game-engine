#include "ui/Viewport.h"
#include "Picking.h"
#include "ui/EditorApplication.h"
#include <engine/ui/UIComponent.h>
#include <engine/ui/UIWindow.h>
#include <engine/thread/ThreadManager.h>
#include <engine/core/SceneGraph.h>
#include <engine/utility/MemoryCheck.h>
#include <engine/physics/PhysicsEngine.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/ResourceManager.h>
#include <engine/utility/Colors.h>
#include <engine/Engine.h>

#include <math/Vector2.hpp>


editor::Viewport::Viewport(const char* title, engine::SceneGraph* graph, EditorApplication* editorApp, math::Vector4f const& bgColor)
    : m_bgColor(bgColor), m_editorApp(editorApp)
{
    SetName(title);
    SetFlags(
        ui::EWndFlags::NO_COLLAPSE |
        ui::EWndFlags::NO_SCROLL_BAR |
        ui::EWndFlags::NO_MOUSE_SCROLL
    );
    SetGraph(graph);

    if (m_graph)
    {
        m_picking = new Picking(m_graph);
        m_enablePicking = true;
    }
}

editor::Viewport::~Viewport(void)
{
    if (m_picking)
        delete m_picking;

    m_picking = nullptr;
    m_graph = nullptr;
    m_editorApp = nullptr;
}

void editor::Viewport::RenderToViewport(void)
{
    m_fbo.Bind();

    // Use the size of this viewport
    SetViewportTransform({0, 0},
    {
        static_cast<int32>(m_size.GetX()),
        static_cast<int32>(m_size.GetY())
    });
    SetViewportBg(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);
   
    engine::ThreadManager::RenderScene(m_graph);

    m_fbo.Unbind();
}

void editor::Viewport::RenderInGameUI(void)
{
    engine::Engine::GetEngine()->GetUIManager().RenderCanvases({0.0f, 0.0f}, m_size);
}

void editor::Viewport::RenderToDebugViewport(const math::Matrix4f& view, const math::Matrix4f& projection)
{
    
    m_fbo.Bind();
    // Use the size of this viewport
    SetViewportTransform({0, 0}, 
    {
        static_cast<int32>(m_size.GetX()),
        static_cast<int32>(m_size.GetY())
    });
    SetViewportBg(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);

    engine::ThreadManager::ExecuteRenderThreadTasks(); 
    math::Matrix4f viewProjection = projection * view;

    if (m_graph)
    {
        m_graph->RenderFromCacheSingleCamera(view, projection);

        engine::ResourceRef<engine::Model> lightBall =
        engine::ResourceManager::GetResource<engine::Model>("./assets/lightBall.obj");

        engine::ResourceRef<engine::Model> lightArrow =
        engine::ResourceManager::GetResource<engine::Model>("./assets/lightArrow.obj");

        engine::ResourceRef<engine::ShaderProgram> basicShader =
        engine::ResourceManager::GetResource<engine::ShaderProgram>("lightProgram");

        if (lightBall && lightArrow && basicShader)
            RenderDebugLights(lightBall, lightArrow, basicShader, viewProjection);
    }

    engine::PhysicsEngine::Get().UpdateDebugDraw(&viewProjection);

   for (auto& navPoint : m_graph->GetComponentArray<engine::NavigationPoint>())
       navPoint.RenderNavPoint(viewProjection);

    m_editorApp->m_gizmosUI->RenderGizmos(viewProjection, m_editorApp->m_editorViewCamera.GetPosition());
    m_fbo.Unbind();
}

void editor::Viewport::RenderPickingPass(const math::Matrix4f& viewProjection)
{
    if (!m_enablePicking)
        return;

    math::Vector2i sizePx(
        math::Max(static_cast<int32>(m_size.GetX()), 1),
        math::Max(static_cast<int32>(m_size.GetY()), 1)
    );

    SetViewportTransform({0, 0}, sizePx);
    SetViewportBg(0.0f, 0.0f, 0.0f, 1.0f);
    m_picking->RenderSceneColored(m_graph, viewProjection);
    m_editorApp->m_gizmosUI->RenderGizmosPicking(viewProjection);
}

void editor::Viewport::SetBgColor(math::Vector4f const& bgColor)
{
    m_bgColor = bgColor;
}

editor::Picking* editor::Viewport::GetPicking(void)
{
    return m_picking;
}

void editor::Viewport::SetEnablePicking(bool value)
{
    m_enablePicking = value;

    if (m_enablePicking && m_graph && !m_picking)
        m_picking = new Picking(m_graph);
}

void editor::Viewport::SetGraph(engine::SceneGraph* graph)
{
    m_graph = graph;

    SetEnablePicking(m_enablePicking);
}

bool editor::Viewport::HasWindowResized(void) const
{
    return m_sizeUpdated;
}

const math::Vector2f& editor::Viewport::GetViewportSize(void) const
{
    return m_size;
}

void editor::Viewport::RenderContents(void)
{
    // Transform
    m_size = ::ui::GetAvailSpace();
    m_sizeUpdated = (m_size != m_prevSize);
    math::Vector2i sizePx(
        math::Max(static_cast<int32>(m_size.GetX()), 1),
        math::Max(static_cast<int32>(m_size.GetY()), 1)
    ); 

    SetViewportTransform({0, 0}, sizePx);
    m_fbo.RescaleFBO(sizePx.GetX(), sizePx.GetY());
    ui::Image(m_fbo.GetFrameTexture(), m_size);

    m_prevSize = m_size;
}

void editor::Viewport::RenderDebugLights(const engine::ResourceRef<engine::Model>& lightBall,
                                         const engine::ResourceRef<engine::Model>& lightArrow,
                                         const engine::ResourceRef<engine::ShaderProgram>& basicShader,
                                         const math::Matrix4f& viewProjection)
{
    const engine::ComponentArray<engine::LightSource>& cachedLights = m_graph->GetCachedLights();

    for (const engine::LightSource& light : cachedLights)
    {
        if (!light.IsValid())
            continue;

        engine::Transform* transform = m_graph->GetCachedTransform(light.GetOwner());

        if (!transform)
            return;


       math::Matrix4f modelMat = engine::Transform::ToWorldMatrix(*transform);
       math::Matrix4f mvp = viewProjection * modelMat;

       basicShader->Use();
       basicShader->Set("mvp", &mvp);
       basicShader->Set("model", &modelMat);

       if (light.IsActive())
           basicShader->Set("objectColor", math::Vector3f(LIGHT_YELLOW));
       else
           basicShader->Set("objectColor", math::Vector3f(LIGHT_GRAY));

       lightBall->Draw();

       switch (light.GetType())
       {
       case engine::LightSource::ELightType::DIRECTIONAL:
       case engine::LightSource::ELightType::SPOTLIGHT:
           lightArrow->Draw();
           break;

       default:
           break;
       }
    }
}
