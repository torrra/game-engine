#include "ui/MenuBar.h"
#include "ui/EditorApplication.h"
#include <engine/Engine.h>
#include <engine/ui/UIComponent.h>
#include <engine/game/GameScene.h>

#include "ui/SceneGraph.h"

editor::MenuBar::MenuBar(EditorApplication* application)
    : m_application(application)
{
}

void editor::MenuBar::Render(::engine::Engine& engine)
{
    (void)engine;
}

void editor::MenuBar::UpdateStartButton(engine::GameScene& activeScene)
{
    ::ui::StartMainMenuBar();

    if (m_gameRunning && ::ui::Button("Stop"))
    {
        engine::ThreadManager::SynchronizeGameThread(nullptr);
        m_gameRunning = false;
        activeScene.Stop();
        printf("Game running: %s\n", "false");

        m_application->m_graphView.ClearGraph();
        activeScene.Reset();
        engine::ThreadManager::SynchronizeGameThread(activeScene.GetGraph());
        m_application->m_graphView.SetGraph(activeScene.GetGraph());
    }
    else if (!m_gameRunning && ::ui::Button("Start"))
    {
        activeScene.Start();
        m_gameRunning = true;
        printf("Game running: %s\n", "true");
    }

    ::ui::EndMainMenuBar();
}
