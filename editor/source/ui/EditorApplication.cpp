#include "ui/EditorApplication.h"
#include "Input.h"

#include <engine/game/GameScene.h>
#include <engine/thread/ThreadManager.h>
#include <engine/ui/UIComponent.h>
#include <engine/input/InputHandler.h>
#include <engine/resource/model/Model.h>
#include <engine/resource/ResourceManager.h>
#include <engine/Engine.h>

#include "ui/components/RigidBodyDynamicComponent.h"

#define SIMULATION_VIEW_WINDOW "Simulation view"
#define EDITOR_VIEW_WINDOW "Editor view"

namespace editor
{
    EditorApplication::EditorApplication(const char* title, class::engine::SceneGraph* scene)
        : m_graphView("Scene Graph"), m_assetWnd("Assets", this), m_menuBar(this)
    {
        Startup(title);
        m_gameSimulationView = new Viewport(SIMULATION_VIEW_WINDOW, scene, this, { 0.1f, 0.1f, 0.1f, 1.0f });
        m_sceneEditorView = new Viewport(EDITOR_VIEW_WINDOW, scene, this, { 0.1f, 0.1f, 0.1f, 1.0f });
        m_gizmosUI = new GizmosUI();
        m_visualizer.Init();
        m_assetDetails.SetVisualizer(&m_visualizer);

        engine::ResourceManager::Load<engine::Model>("./assets/lightBall.obj", true);
        engine::ResourceManager::Load<engine::Model>("./assets/lightArrow.obj", true);
        engine::ResourceManager::LoadShader("lightProgram", "./shaders/Model.vs",
                                            "./shaders/Model.frag", true, true);

        m_lightBall = engine::ResourceManager::GetResource<engine::Model>("./assets/lightBall.obj");
        m_lightArrow = engine::ResourceManager::GetResource<engine::Model>("./assets/lightArrow.obj");
        m_lightShader = engine::ResourceManager::GetResource<engine::ShaderProgram>("lightProgram");
    }

    void EditorApplication::SetCurrentScene(::engine::GameScene* scene)
    {
        m_currentScene = scene;
        m_properties = PropertyWnd(scene->GetGraph());
        m_graphView.SetGraph(scene->GetGraph());
    }


    void EditorApplication::Render(::engine::SceneGraph* scene)
    {
        if (!m_currentScene)
            return;

        engine::ThreadManager::ExecuteRenderThreadTasks();
        static bool lockMouse = false;
        LockMousePosition(SIMULATION_VIEW_WINDOW, lockMouse);

        // Render editor windows
        m_assetDetails.Render();
        m_visualizer.Update(m_currentScene->GetTime().GetDeltaTime());
        m_assetWnd.Render();
        m_menuBar.Render(*m_currentScene);
        m_graphView.Render();

        // Get selected entity to display its components
        if (m_graphView.IsNewEntitySelected())
            m_properties.SetHandle(m_graphView.GetSelectedEntity());

        m_properties.Render();

        //m_gameSimulationView->RenderToViewport();
        //m_gameSimulationView->Render();
        //m_gameSimulationView->RenderInGameUI();

        
        // Editor camera
        if (m_sceneEditorView->HasWindowResized())
            m_editorViewCamera.UpdateAspectRatio(m_sceneEditorView->GetViewportSize());


        if (ui::IsWindowSelected(EDITOR_VIEW_WINDOW))
            m_editorViewCamera.Update(m_currentScene->GetTime().GetDeltaTime());

        math::Matrix4f viewProjection = m_editorViewCamera.ViewProjection();

        // Editor viewport
        m_sceneEditorView->RenderPickingPass(viewProjection);
        m_sceneEditorView->RenderToDebugViewport(viewProjection);
        
        m_gizmosUI->Render();
        m_sceneEditorView->Render();


        if (engine::InputHandler::IsInputPressed(MOUSE_BUTTON_LEFT) &&
            ui::IsWindowSelected(EDITOR_VIEW_WINDOW))
            PickEntity();

        m_gizmosUI->UpdateGizmos(EDITOR_VIEW_WINDOW);
    }

    void EditorApplication::ResetScene(::engine::GameScene& activeScene)
    {
        engine::ThreadManager::SynchronizeGameThread(nullptr);
        engine::ThreadManager::SynchronizeAnimationThread();
        engine::ThreadManager::ExecuteRenderThreadTasks();
        activeScene.Stop();

        m_graphView.ClearGraph();
        activeScene.Reset();
        engine::Engine::GetEngine()->GetUIManager().DeleteAllCanvases();
        engine::ThreadManager::SynchronizeGameThread(activeScene.GetGraph());
        m_graphView.SetGraph(activeScene.GetGraph());
    }

    void EditorApplication::LoadNewScene(::engine::GameScene& activeScene, const std::filesystem::path& filePath)
    {
        engine::ThreadManager::SynchronizeGameThread(nullptr);
        activeScene.Stop();

        m_graphView.ClearGraph();
        m_properties.SetHandle(engine::Entity::INVALID_HANDLE);
        m_gizmosUI->DeselectEntity();
        activeScene.GetGraph()->CleanRigidBodies();

        activeScene.LoadNewScene(false, filePath);

        engine::ThreadManager::SynchronizeGameThread(activeScene.GetGraph());
        m_graphView.SetGraph(activeScene.GetGraph());
    }

    void EditorApplication::ResetApplication(void)
    {
        m_graphView.ClearGraph();
        m_properties.SetHandle(engine::Entity::INVALID_HANDLE);
        m_graphView.SetGraph(m_currentScene->GetGraph());
    }

    void EditorApplication::Shutdown(void)
    {
        //RigidBodyDynamicComponent::ReleaseStaticData();
        m_assetDetails.SelectAsset("", AssetDetailsWnd::EAssetType::INVALID);
        engine::Engine::GetEngine()->GetUIManager().ClearAllCanvases();
        m_visualizer.Reset();
        m_assetDetails.SelectAsset("", AssetDetailsWnd::EAssetType::INVALID);

        m_lightBall = ModelRef();
        m_lightArrow = ModelRef();
        m_lightShader = ShaderRef();

        delete m_gameSimulationView;
        delete m_sceneEditorView;
        delete m_gizmosUI;
        Application::Shutdown();
    }

    AssetDetailsWnd& EditorApplication::GetAssetDetailsWindow(void)
    {
        return m_assetDetails;
    }

    const EditorApplication::ModelRef& EditorApplication::GetLightArrow(void) const
    {
        return m_lightArrow;
    }

    const EditorApplication::ModelRef& EditorApplication::GetLightBall(void) const
    {
        return m_lightBall;
    }

    const EditorApplication::ShaderRef& EditorApplication::GetLightShader(void) const
    {
        return m_lightShader;
    }

    void EditorApplication::PickEntity(void)
    {
        engine::EntityHandle handle = m_sceneEditorView->GetPicking()->FindSelectedEntity(EDITOR_VIEW_WINDOW);

        if (handle != engine::Entity::EHandleUtils::INVALID_HANDLE)
        {
            m_graphView.SelectEntity(handle);
            m_properties.SetHandle(handle);

            if (engine::Transform* transform = m_currentScene->GetGraph()->GetComponent<engine::Transform>(handle))
                m_gizmosUI->SetSelectedTransform(transform);
        }
    }
}