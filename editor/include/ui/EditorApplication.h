#pragma once

#include <engine/ui/Application.h>
#include "EditorCamera.h"
#include "ui/MenuBar.h"
#include "ui/Properties.h"
#include "ui/SceneGraph.h"
#include "ui/Viewport.h"
#include "ui/Assets.h"
#include "ui/AssetDetails.h"
#include "ui/gizmos/GizmosUI.h"

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
        void ResetScene(class ::engine::GameScene& activeScene) override;
        void LoadNewScene(class ::engine::GameScene& activeScene,
                          const std::filesystem::path& filePath);

        void Shutdown(void) override;

        AssetDetailsWnd& GetAssetDetailsWindow(void);

    private:
        void PickEntity(void);

        AssetsWnd                   m_assetWnd;
        AssetDetailsWnd             m_assetDetails;
        PropertyWnd                 m_properties;
        EditorCamera                m_editorViewCamera;
        SceneGraphUI                m_graphView;
        GizmosUI*                   m_gizmosUI;
        Viewport*                   m_sceneEditorView;
        Viewport*                   m_gameSimulationView;
        class ::engine::GameScene*  m_currentScene = nullptr;
        MenuBar                     m_menuBar;

        friend class MenuBar;
        friend class Viewport;
    };
}