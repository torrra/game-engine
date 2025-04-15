#include "engine/ui/UIWindow.h"
#include "ui/InternalUIWindow.h"
#include "window/WindowLib.h"

ui::UIWindow::UIWindow(void)
    : m_flags(0)
{
}

void ui::UIWindow::Render(void)
{
    engine::ui::StartWindowRenderer(m_title.c_str(), m_flags);
    RenderContents();
    engine::ui::EndWindowRenderer();
}

void ui::UIWindow::RenderContents(void)
{}

engine::ui::UIWindowRect ui::UIWindow::GetWindowRect(void)
{
    return engine::ui::GetCurrentWindowRect();
}

void ui::UIWindow::SetViewportBg(f32 red, f32 green, f32 blue, f32 alpha)
{
    engine::wnd::ClearWindow(red, green, blue, alpha);
}

void ui::UIWindow::SetViewportTransform(math::Vector2i position, math::Vector2i size)
{
    engine::wnd::Viewport(position, size);
}

void ui::UIWindow::SetName(const char* title)
{
    m_title = title;
}

void ui::UIWindow::SetFlags(int32 flags)
{
    m_flags = flags;
}

void ui::UIWindow::SetWindowPos(math::Vector2f const& position, EActive activeFlag)
{
    engine::ui::SetWindowPos(position, activeFlag);
}

void ui::UIWindow::SetWindowSize(math::Vector2f const& size, EActive activeFlag)
{
    engine::ui::SetWindowSize(size, activeFlag);
}