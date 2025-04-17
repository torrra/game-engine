#include "window/WindowLib.h"
#include "Window.h"

#include <glad/glad.h>
#include <cstdio>


engine::wnd::Wnd::operator GLFWwindow*(void)
{
    return reinterpret_cast<GLFWwindow*>(m_ptr);
}

engine::wnd::Wnd::operator GLFWwindow* (void) const
{
    return reinterpret_cast<GLFWwindow*>(m_ptr);
}

engine::wnd::Wnd& engine::wnd::Wnd::operator=(GLFWwindow* wndPtr)
{
    m_ptr = wndPtr;

    return *this;
}

::int32 engine::wnd::InitWndLib(void)
{
    ::int32 result = glfwInit();

    // Check if window library initialized successfully
    if (result == GLFW_FALSE)
    {
        printf("Failed to initialize GLFW.\n");
        return -1;
    }

    // Set default GLFW hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return 0;
}

::int32 engine::wnd::InitGraphicsAPI(void)
{
    int32 result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    // Check graphics API initialized successfully
    if (!result)
    {
        printf("Failed to initialize glad\n");
        return -1;
    }

    // Enable depth as the game engine is made for 3D projects
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, POLYGON_MODE);

    return 0;
}

void engine::wnd::ShutDown(void)
{
    glfwTerminate();
}

engine::wnd::Wnd engine::wnd::CreateWindow(const char* title, int32 width, int32 height)
{
    Wnd window;
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    glfwSetWindowSizeCallback(window, SizeCallback);

    return window;
}

engine::wnd::Wnd engine::wnd::CreateWindow(const char* title)
{
    Wnd window;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(mode->width, mode->height, title, monitor, nullptr);
    
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

    glfwSetWindowSizeCallback(window, SizeCallback);

    return window;
}

void engine::wnd::ClearWindow(f32 red, f32 green, f32 blue, f32 alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void engine::wnd::SetViewportTransform(math::Vector2i const& position, math::Vector2i const& size)
{
    glViewport(position.GetX(), position.GetY(), size.GetX(), size.GetY());
}

void engine::wnd::Update(Wnd* window)
{
    glfwSwapBuffers(*window);
    glfwPollEvents();
}

void engine::wnd::SizeCallback(GLFWwindow* window, int32 width, int32 height)
{
    Window* wndClass = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

    // Check user window pointer is set
    if (wndClass)
    {
        wndClass->SetSize({width, height});

        // Prevent divisions by 0
        f32 aspectRatio = (height != 0) ? (f32) (width / height) : width;
        wndClass->SetAspectRatio(aspectRatio);
    }

    // Update window content size
    glViewport(0, 0, width, height);
}

void engine::wnd::SetCurrentContext(Wnd* window, Window* wndClass)
{
    glfwSetWindowUserPointer(*window, wndClass);
    glfwMakeContextCurrent(*window);
}

bool engine::wnd::ShouldWindowClose(Wnd* window)
{
    if (!window)
        return false;

    return glfwWindowShouldClose(*window);
}

bool engine::wnd::IsMinimized(Wnd* window)
{
    if (!window)
        return false;

    // Check if window is minimized
    return glfwGetWindowAttrib(*window, GLFW_ICONIFIED);
}

bool engine::wnd::IsFullScreen(Wnd* window)
{
    if (!window)
        return false;

    // Check if window is in full screen mode
    return glfwGetWindowAttrib(*window, GLFW_MAXIMIZED);
}

