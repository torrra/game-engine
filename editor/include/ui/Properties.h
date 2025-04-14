#pragma once

#include "ui/components/CameraComponent.h"
#include "ui/components/RendererComponent.h"
#include "ui/components/ScriptComponent.h"
#include "ui/components/TransformComponent.h"

#include <engine/ui/UIWindow.h>
#include <engine/core/SceneGraph.h>

#include <vector>

namespace editor
{
    class PropertyWnd : public ::ui::UIWindow
    {
    public:
        PropertyWnd(void);
        PropertyWnd(engine::SceneGraph* graph);
        ~PropertyWnd(void);

        void SetHandle(engine::EntityHandle handle);

    protected:
        virtual void RenderContents(void) override;
        
    private:
        void InitComponents(void);

        std::vector<BaseComponent*> m_components;
        engine::SceneGraph* m_graph;
        engine::EntityHandle m_handle;
        CameraComponent* m_camera;
        TransformComponent* m_transform;
        RendererComponent* m_renderer;
        ScriptComponent* m_script;
    };
}