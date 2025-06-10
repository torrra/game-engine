#include "ui/ResourceVizualizer.h"

#include "ui/Viewport.h"

editor::ResourceVisualizerWnd::ResourceVisualizerWnd(void)
{
    SetName("Resource Visualizer");
    m_viewport = new Viewport("Visualizer", nullptr, nullptr, { 1.f, 0.713725f, 0.45098f, 1.f });
}

editor::ResourceVisualizerWnd::~ResourceVisualizerWnd(void)
{
    delete m_viewport;
}

void editor::ResourceVisualizerWnd::RenderContents(void)
{
}
