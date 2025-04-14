#pragma once

#include "ui/components/Transform.h"

#include <engine/ui/UIWindow.h>
#include <engine/core/Entity.h>
#include <engine/core/SceneGraph.h>

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

        engine::SceneGraph* m_graph;
        engine::EntityHandle m_handle;
        TransformProperty* m_transform;
        bool m_hasTransform = false;
    };
}