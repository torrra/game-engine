#pragma once

#include "engine/viewport/FrameBuffer.h"

#include <math/Vector4.hpp>

namespace engine
{
    class Viewport
    {
    public:
        // Title - window title (this needs to be unique)
        Viewport(const char* title);
        
        // Title - window title (this needs to be unique)
        // BgColor - background color (vec4 all values between 0 - 1)
        Viewport(const char* title, math::Vector4f bgColor);
        ~Viewport(void) = default;

        // Draw the viewport window
        void DrawViewport(void);

        // The scene which the viewport should render
        void RenderToViewport(class SceneGraph* sceneGraph);

        // BgColor - background color (vec4 all values between 0 - 1)
        inline void SetBgColor(math::Vector4f const& bgColor);
    
    private:
        std::string m_title;
        math::Vector4f m_bgColor;
        FrameBuffer m_fbo;
    };
}