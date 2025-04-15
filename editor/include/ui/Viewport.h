#pragma once

#include <engine/ui/UIWindow.h>
#include <engine/utility/FrameBuffer.h>
#include <engine/core/SceneGraph.h>
#include <math/Vector4.hpp>

namespace editor
{
    class Viewport : public ::ui::UIWindow
    {
    public:
        // Title - window title (this needs to be unique)
        Viewport(const char* title);
        
        // Title - window title (this needs to be unique)
        // BgColor - background color (vec4 all values between 0 - 1)
        Viewport(const char* title, math::Vector4f const& bgColor);
        ~Viewport(void) = default;

        // The scene which the viewport should render
        void RenderToViewport(engine::SceneGraph* sceneGraph);

        // BgColor - background color (vec4 all values between 0 - 1)
        inline void SetBgColor(math::Vector4f const& bgColor);

    protected:
        virtual void RenderContents(void) override;
    
    private:
        engine::FrameBuffer m_fbo;
        math::Vector4f m_bgColor;
    };
}