#include "ui/UIWindow.h"
#include "ui/InternalUIWindow.h"

engine::UIWindow::UIWindow(void)
    : m_flags(0)
{
}

void engine::UIWindow::Render(void)
{
    ui::StartWindowRenderer(m_title.c_str(), m_flags);
    RenderContents();
    ui::EndWindowRenderer();
}

void engine::UIWindow::RenderContents(void)
{}

engine::ui::UIWindowRect engine::UIWindow::GetWindowRect(void)
{
    return ui::GetCurrentWindowRect();
}

void engine::UIWindow::SetName(const char* title)
{
    m_title = title;
}

void engine::UIWindow::SetFlags(int32 flags)
{
    m_flags = flags;
}

void engine::UIWindow::SetWindowPos(math::Vector2f const& position, EActive activeFlag)
{
    ui::SetWindowPos(position, activeFlag);
}

void engine::UIWindow::SetWindowSize(math::Vector2f const& size, EActive activeFlag)
{
    ui::SetWindowSize(size, activeFlag);
}