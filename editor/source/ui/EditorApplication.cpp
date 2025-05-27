#include "ui/EditorApplication.h"
#include "Input.h"

#include <engine/game/GameScene.h>
#include <engine/thread/ThreadManager.h>
#include <engine/ui/UIComponent.h>
#include <engine/input/InputHandler.h>

#include "ui/components/RigidBodyDynamicComponent.h"

#define SIMULATION_VIEW_WINDOW "Simulation view"
#define EDITOR_VIEW_WINDOW "Editor view"

namespace editor
{
    EditorApplication::EditorApplication(const char* title, class::engine::SceneGraph* scene)
        : m_graphView("Scene Graph"), m_assetWnd("Assets", this), m_menuBar(this)
    {
        Startup(title);
        m_gameSimulationView = new Viewport(SIMULATION_VIEW_WINDOW, scene, { 0.1f, 0.1f, 0.1f, 1.0f });
        m_sceneEditorView = new Viewport(EDITOR_VIEW_WINDOW, scene, { 0.1f, 0.1f, 0.1f, 1.0f });
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

        static bool lockMouse = false;
        LockMousePosition(SIMULATION_VIEW_WINDOW, lockMouse);

        // Render editor windows
        m_assetWnd.Render();
        m_menuBar.Render(*m_currentScene);
        m_graphView.Render();

        // Get selected entity to dislay its components
        if (m_graphView.IsNewEntitySelected())
            m_properties.SetHandle(m_graphView.GetSelectedEntity());

        m_properties.Render();

        m_gameSimulationView->RenderToViewport();
        m_gameSimulationView->Render();

        // Editor camera
        if (m_sceneEditorView->HasWindowResized())
            m_editorViewCamera.UpdateAspectRatio(m_sceneEditorView->GetViewportSize());

        if (ui::IsWindowSelected(EDITOR_VIEW_WINDOW))
            m_editorViewCamera.Update(m_currentScene->GetTime().GetDeltaTime());

        // Editor viewport
        m_sceneEditorView->RenderPickingPass(m_editorViewCamera.ViewProjection());
        m_sceneEditorView->RenderToDebugViewport(m_editorViewCamera.ViewProjection());
        m_sceneEditorView->Render();

        if (engine::InputHandler::IsInputPressed(MOUSE_BUTTON_LEFT) &&
            ui::IsWindowSelected(EDITOR_VIEW_WINDOW))
            PickEntity();
    }

    void EditorApplication::ResetScene(::engine::GameScene& activeScene)
    {
        engine::ThreadManager::SynchronizeGameThread(nullptr);
        activeScene.Stop();

        m_graphView.ClearGraph();
        activeScene.Reset();
        engine::ThreadManager::SynchronizeGameThread(activeScene.GetGraph());
        m_graphView.SetGraph(activeScene.GetGraph());
    }

    void EditorApplication::LoadNewScene(::engine::GameScene& activeScene, const std::filesystem::path& filePath)
    {
        engine::ThreadManager::SynchronizeGameThread(nullptr);
        activeScene.Stop();

        m_graphView.ClearGraph();
        m_properties.SetHandle(engine::Entity::INVALID_HANDLE);
        activeScene.GetGraph()->CleanRigidBodies();

        activeScene.LoadNewScene(false, filePath);

        engine::ThreadManager::SynchronizeGameThread(activeScene.GetGraph());
        m_graphView.SetGraph(activeScene.GetGraph());
    }

    void EditorApplication::Shutdown(void)
    {
        //RigidBodyDynamicComponent::ReleaseStaticData();
        delete m_gameSimulationView;
        delete m_sceneEditorView;
        Application::Shutdown();
    }

    void EditorApplication::PickEntity(void)
    {
        engine::EntityHandle handle = m_sceneEditorView->GetPicking()->FindSelectedEntity(EDITOR_VIEW_WINDOW);

        if (handle != engine::Entity::EHandleUtils::INVALID_HANDLE)
        {
            m_graphView.SelectEntity(handle);
            m_properties.SetHandle(handle);
        }
    }
}