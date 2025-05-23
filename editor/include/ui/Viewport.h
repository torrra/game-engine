#pragma once

#include "Picking.h"

#include <engine/ui/UIWindow.h>
#include <engine/utility/FrameBuffer.h>
#include <engine/core/SceneGraph.h>

#include <math/Vector2.hpp>
#include <math/Vector4.hpp>
#include <math/Matrix4.hpp>

namespace editor
{
    class Viewport : public ::ui::UIWindow
    {
    public:
        Viewport(const char* title, engine::SceneGraph* graph, math::Vector4f const& bgColor);
        ~Viewport(void);

        // The scene which the viewport should render
        void RenderToViewport(void);
        void RenderToDebugViewport(const math::Matrix4f& viewProjection);
        void RenderPickingPass(const math::Matrix4f& viewProjection);

        // BgColor - background color (vec4 all values between 0 - 1)
        inline void SetBgColor(math::Vector4f const& bgColor);

        Picking* GetPicking(void);
        void SetEnablePicking(bool value = true);
        void SetGraph(engine::SceneGraph* graph);

        bool HasWindowResized(void) const;
        const math::Vector2f& GetViewportSize(void) const;

    protected:
        virtual void RenderContents(void) override;
    
    private:

        void RenderDebugLights(const class ::engine::Model* lightBall,
                               const class ::engine::Model* lightArrow,
                               const class ::engine::ShaderProgram* basicShader,
                               const math::Matrix4f& viewProjection);

        Picking* m_picking = nullptr;
        engine::SceneGraph* m_graph = nullptr;
        engine::FrameBuffer m_fbo;
        math::Vector4f m_bgColor;
        math::Vector2f m_size;
        math::Vector2f m_prevSize;
        bool m_enablePicking = false;
        bool m_sizeUpdated = false;
    };
}