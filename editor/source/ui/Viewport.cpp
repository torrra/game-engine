#include "ui/Viewport.h"

#include <engine/ui/UIComponent.h>
#include <engine/thread/ThreadManager.h>
#include <engine/core/SceneGraph.h>
#include <engine/utility/MemoryCheck.h>

#include <math/Vector2.hpp>

editor::Viewport::Viewport(const char* title)
    : Viewport(title, {0.0f, 0.0f, 0.0f, 1.0f})
{
}

editor::Viewport::Viewport(const char* title, math::Vector4f const& bgColor)
    : m_bgColor(bgColor)
{
    SetName(title);
    SetFlags(
    ::ui::EWndFlags::NO_COLLAPSE |
    ::ui::EWndFlags::NO_SCROLL_BAR |
    ::ui::EWndFlags::NO_MOUSE_SCROLL
    );
}

void editor::Viewport::RenderToViewport(engine::SceneGraph* sceneGraph)
{
    m_fbo.Bind();
    SetViewportBg(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);
    engine::ThreadManager::RenderScene(sceneGraph);
    m_fbo.Unbind();
}

void editor::Viewport::SetBgColor(math::Vector4f const& bgColor)
{
    m_bgColor = bgColor;
}

void editor::Viewport::RenderContents(void)
{
    // Transform
    //math::Vector2f position = ui::GetPos();
    math::Vector2f regionAvail = ::ui::GetAvailSpace();
    math::Vector2i sizePx
    (
        static_cast<int32>(regionAvail.GetX()),
        static_cast<int32>(regionAvail.GetY())
    );

    SetViewportTransform({0, 0}, sizePx);
    m_fbo.RescaleFBO(sizePx.GetX(), sizePx.GetY());

    ui::Image(m_fbo.GetFrameTexture(), regionAvail);
}
