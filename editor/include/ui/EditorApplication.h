#pragma once

#include <engine/ui/Application.h>
#include <engine/utility/ResourceRef.h>
#include "EditorCamera.h"
#include "ui/MenuBar.h"
#include "ui/Properties.h"
#include "ui/SceneGraph.h"
#include "ui/Viewport.h"
#include "ui/Assets.h"
#include "ui/AssetDetails.h"
#include "ui/gizmos/GizmosUI.h"
#include "ui/ResourceVizualizer.h"

namespace editor
{
    class EditorApplication final : public ::engine::Application
    {
    private:

        using ModelRef = engine::ResourceRef<engine::Model>;
        using ShaderRef = engine::ResourceRef<engine::ShaderProgram>;

    public:

        EditorApplication(const char* title, class ::engine::SceneGraph* scene);
        EditorApplication(const EditorApplication&) = delete;
        EditorApplication(EditorApplication&&) = delete;

        void SetCurrentScene(class ::engine::GameScene* scene) override;
        void Render(class ::engine::SceneGraph* scene) override;
        void ResetScene(class ::engine::GameScene& activeScene) override;
        void LoadNewScene(class ::engine::GameScene& activeScene,
                          const std::filesystem::path& filePath);

        void ResetApplication(void) override;

        void Shutdown(void) override;

        AssetDetailsWnd& GetAssetDetailsWindow(void);

        const ModelRef& GetLightArrow(void) const;
        const ModelRef& GetLightBall(void) const;
        const ShaderRef& GetLightShader(void) const;

    private:
        void PickEntity(void);

        ResourceVisualizerWnd       m_visualizer;
        AssetsWnd                   m_assetWnd;
        AssetDetailsWnd             m_assetDetails;
        PropertyWnd                 m_properties;
        EditorCamera                m_editorViewCamera;
        SceneGraphUI                m_graphView;

        ModelRef                    m_lightArrow;
        ModelRef                    m_lightBall;
        ShaderRef                   m_lightShader;

        GizmosUI*                   m_gizmosUI;
        Viewport*                   m_sceneEditorView;
        Viewport*                   m_gameSimulationView;
        class ::engine::GameScene*  m_currentScene = nullptr;
        MenuBar                     m_menuBar;

        friend class MenuBar;
        friend class Viewport;
    };
}