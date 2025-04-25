#include "ui/EditorApplication.h"
#include <engine/game/GameScene.h>

#include <engine/thread/ThreadManager.h>

namespace editor
{
    EditorApplication::EditorApplication(const char* title, class::engine::SceneGraph* scene)
        : m_graphView("Scene Graph"), m_assetWnd("Assets"), m_menuBar(this)
    {
        Startup(title);
        m_gameSimulationView = new Viewport("Simulation view", scene, { 0.1f, 0.1f, 0.1f, 1.0f });
        m_sceneEditorView = new Viewport("Editor view", scene, { 0.1f, 0.1f, 0.1f, 1.0f });
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

        m_assetWnd.Render();
        m_gameSimulationView->RenderToViewport();
        m_menuBar.UpdateStartButton(*m_currentScene);
        m_graphView.Render();

        if (m_graphView.IsNewEntitySelected())
            m_properties.SetHandle(m_graphView.GetSelectedEntity());

        m_properties.Render();
        m_gameSimulationView->Render();

        // TODO: add scene view render
    }

    void EditorApplication::Shutdown(void)
    {
        delete m_gameSimulationView;
        delete m_sceneEditorView;
        Application::Shutdown();
    }
}