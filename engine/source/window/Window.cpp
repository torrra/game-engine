#include "Window.h"

#include "window/WindowLib.h"

engine::Window::Window(const char* title)
    : m_title(title), m_windowPtr(nullptr), m_size(0, 0), m_aspectRatio(1.0f)
{
}

engine::Window::~Window(void)
{
    if (m_windowPtr)
    {
        delete m_windowPtr;
    }
}

int32 engine::Window::StartUp(void)
{
    static bool isInitialized = false;

    // Init window library
    if (!isInitialized)
    {
        if (wnd::InitWndLib())
            return -1;

        isInitialized = true;
    }

    return 0;
}

void engine::Window::ShutDown(void)
{
    wnd::ShutDown();
}

int32 engine::Window::CreateWindow(void)
{
    m_windowPtr = new wnd::Wnd;
    *m_windowPtr = wnd::CreateWindow(m_title.c_str());

    wnd::SetCurrentContext(m_windowPtr, this);

    static bool isInitialized = false;

    // Init graphics library
    if (!isInitialized)
    {
        if (wnd::InitGraphicsAPI())
            return -1;

        isInitialized = true;
    }

    return  0;
}

int32 engine::Window::CreateWindow(int32 width, int32 height)
{
    m_windowPtr = new wnd::Wnd;
    *m_windowPtr = wnd::CreateWindow(m_title.c_str(), width, height);

    wnd::SetCurrentContext(m_windowPtr, this);

    static bool isInitialized = false;

    // Init graphics library
    if (!isInitialized)
    {
        if (wnd::InitGraphicsAPI())
            return -1;

        isInitialized = true;
    }

    SetSize({width, height});
    SetAspectRatio((height != 0) ? (f32) (width / height) : width);

    return  0;
}

void engine::Window::ClearWindow(f32 red, f32 green, f32 blue, f32 alpha)
{
    wnd::ClearWindow(red, green, blue, alpha);
}

void engine::Window::Update(void)
{
    wnd::Update(m_windowPtr);
}

bool engine::Window::ShouldWindowClose(void) const noexcept
{
    return wnd::ShouldWindowClose(m_windowPtr);
}

f32 engine::Window::GetAspectRatio(void) const noexcept
{
    return m_aspectRatio;
}

engine::wnd::Wnd* engine::Window::GetPtr(void) const noexcept
{
    return m_windowPtr;
}

std::string engine::Window::GetTitle(void) const noexcept
{
    return m_title;
}

bool engine::Window::IsMinimized(void) const noexcept
{
    return wnd::IsMinimized(m_windowPtr);
}

bool engine::Window::IsFullScreen(void) const noexcept
{
    return wnd::IsFullScreen(m_windowPtr);
}

void engine::Window::SetSize(math::Vector2i const& size)
{
    m_size = size;
}

void engine::Window::SetAspectRatio(f32 aspectRatio)
{
    m_aspectRatio = aspectRatio;
}
