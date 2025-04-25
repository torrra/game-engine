#pragma once

#include <engine/ui/Application.h>

#include "ui/MenuBar.h"
#include "ui/Properties.h"
#include "ui/SceneGraph.h"
#include "ui/Viewport.h"
#include "ui/Assets.h"

namespace editor
{
    class EditorApplication final : public ::engine::Application
    {
    public:

        EditorApplication(const char* title, class ::engine::SceneGraph* scene);
        EditorApplication(const EditorApplication&) = delete;
        EditorApplication(EditorApplication&&) = delete;

        void SetCurrentScene(class ::engine::GameScene* scene) override;
        void Render(class ::engine::SceneGraph* scene) override;

        void Shutdown(void) override;

    private:

        Viewport*                   m_sceneEditorView;
        Viewport*                   m_gameSimulationView;
        PropertyWnd                 m_properties;
        SceneGraphUI                m_graphView;
        AssetsWnd                   m_assetWnd;
        class ::engine::GameScene*  m_currentScene = nullptr;
        MenuBar                     m_menuBar;

        friend class MenuBar;
    };
}