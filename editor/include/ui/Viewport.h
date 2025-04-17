#pragma once

#include "Picking.h"

#include <engine/ui/UIWindow.h>
#include <engine/utility/FrameBuffer.h>
#include <engine/core/SceneGraph.h>
#include <math/Vector4.hpp>

namespace editor
{
    class Viewport : public ::ui::UIWindow
    {
    public:
        Viewport(const char* title, engine::SceneGraph* graph, math::Vector4f const& bgColor);
        ~Viewport(void);

        // The scene which the viewport should render
        void RenderToViewport(void);
        void RenderPickingPass(void);

        // BgColor - background color (vec4 all values between 0 - 1)
        inline void SetBgColor(math::Vector4f const& bgColor);

        Picking* GetPicking(void);
        void SetEnablePicking(bool value = true);
        void SetGraph(engine::SceneGraph* graph);

    protected:
        virtual void RenderContents(void) override;
    
    private:
        Picking* m_picking = nullptr;
        engine::SceneGraph* m_graph = nullptr;
        engine::FrameBuffer m_fbo;
        math::Vector4f m_bgColor;
        bool m_enablePicking = false;
    };
}