#pragma once

#include <engine/ui/Application.h>

#include "EditorCamera.h"

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

        AssetsWnd                   m_assetWnd;
        PropertyWnd                 m_properties;
        EditorCamera                m_editorViewCamera;
        SceneGraphUI                m_graphView;
        Viewport*                   m_sceneEditorView;
        Viewport*                   m_gameSimulationView;
        class ::engine::GameScene*  m_currentScene = nullptr;
        MenuBar                     m_menuBar;

        friend class MenuBar;
    };
}