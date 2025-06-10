#include "ui/ResourceVizualizer.h"
#include "ui/Viewport.h"

#include <engine/ui/UIComponent.h>
#include <engine/resource/animation/Animation.h>

#define WINDOW_NAME "Visualizer"


editor::ResourceVisualizerWnd::ResourceVisualizerWnd(void)
{
    m_viewport = new Viewport(WINDOW_NAME, nullptr, nullptr, { 0.35f, 0.35f, 0.35f, 1.f });
}

editor::ResourceVisualizerWnd::~ResourceVisualizerWnd(void)
{
    delete m_viewport;
}

const engine::ResourceRef<engine::IResource>&
editor::ResourceVisualizerWnd::GetResource(void) const
{
    return m_resource;
}

const engine::ResourceRef<engine::Model>& editor::ResourceVisualizerWnd::GetDisplayModel(void) const
{
    return m_displayModel;
}

void editor::ResourceVisualizerWnd::Reset(void)
{
    m_resource = engine::ResourceRef<engine::IResource>();
    m_displayModel = engine::ResourceRef<engine::Model>();
    m_displayShader = engine::ResourceRef<engine::ShaderProgram>();
    m_animator.SetModel(engine::ResourceRef<engine::Model>());
    m_animator.SetAnimation(engine::ResourceRef<engine::Animation>());
    m_animator.DeleteBuffer();
}

engine::SkeletonAnimator& editor::ResourceVisualizerWnd::GetAnimator(void)
{
    return m_animator;
}

void editor::ResourceVisualizerWnd::SelectAnimation(
    const engine::ResourceRef<engine::Animation>& anim)
{
    if (anim)
    {
        m_animator.SetAnimation(anim);
        m_animator.PlayAnimation();
        m_animator.SetLooped(true);
    }
    
    m_resourceType = AssetDetailsWnd::EAssetType::ANIMATION;
    m_resource = anim;
}

void editor::ResourceVisualizerWnd::SetDisplayModel(engine::ResourceRef<engine::Model>&& model)
{
    m_displayModel = std::forward<engine::ResourceRef<engine::Model>>(model);
    m_animator.SetModel(m_displayModel);

    if (m_resource)
        m_animator.SetAnimation(m_animator.GetAnimation());
}


void editor::ResourceVisualizerWnd::Init(void)
{
    engine::ResourceManager::LoadShader("vizualizerShader", "./shaders/ModelTextured.vert",
        "./shaders/Model.frag", true, true);

    m_displayShader = engine::ResourceManager::GetResource<engine::ShaderProgram>("vizualizerShader");
    m_animator.InitBuffer();
}

void editor::ResourceVisualizerWnd::Update(f32 deltaTime)
{
    switch (m_resourceType)
    {
    case editor::AssetDetailsWnd::EAssetType::MATERIAL:
        break;
    case editor::AssetDetailsWnd::EAssetType::ANIMATION:
        RenderAnimation(deltaTime);
        break;
    case editor::AssetDetailsWnd::EAssetType::MODEL:
        break;
    default:
        break;
    }
}

void editor::ResourceVisualizerWnd::RenderAnimation(f32 deltaTime)
{
    if (m_viewport->HasWindowResized())
        m_camera.UpdateAspectRatio(m_viewport->GetViewportSize());

    if (ui::IsWindowSelected(WINDOW_NAME))
        m_camera.Update(deltaTime);

    m_animator.Update(deltaTime);

    if (m_displayModel && m_displayModel->IsDynamic())
    {
        math::Matrix4f viewProjection = m_camera.ViewProjection();
        math::Matrix4f identity4x4{ 1.f };
        math::Matrix3f identity3x3{ 1.f };

        m_viewport->Bind();
        m_displayShader->Use();

        m_displayShader->Set("model", &identity4x4);
        m_displayShader->Set("mvp", &viewProjection);
        m_displayShader->Set("normalMat", &identity3x3);

        if (m_animator.IsPaused() || m_animator.IsPlaying())
        {
            m_displayShader->Set("isRigged", true);
            m_animator.UseSkinningBuffer();
        }
        else
            m_displayShader->Set("isRigged", false);

        m_displayModel->Draw();
        m_viewport->Unbind();
    }

    m_viewport->Render();
}
