#pragma once

#include "engine/CoreTypes.h"

#include <math/Vector2.hpp>

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

#ifndef OPENGL_VERSION_MAJOR
    #define OPENGL_VERSION_MAJOR 4
#endif

#ifndef OPENGL_VERSION_MINOR
    #define OPENGL_VERSION_MINOR 5
#endif

#ifndef POLYGON_MODE
    #define POLYGON_MODE GL_FILL
#endif

namespace engine
{
    class Window;
}

namespace engine::wnd
{
    class Wnd
    {
    public:
        Wnd(void) = default;
        ~Wnd(void) = default;

        operator GLFWwindow* (void);
        operator GLFWwindow* (void) const;
        Wnd& operator=(GLFWwindow* wndPtr);

    private:
        GLFWwindow* m_ptr = nullptr;
    };

    // Init window library
    ::int32 InitWndLib(void);

    // Init graphics API
    ::int32 InitGraphicsAPI(void);

    // Terminate window library
    void ShutDown(void);

    // Create window with a given size (windowed mode)
    Wnd CreateWindow(const char* title, int32 width, int32 height);

    // Create window in full screen mode
    Wnd CreateWindow(const char* title);
    void ClearWindow(f32 red, f32 green, f32 blue, f32 alpha = 1.0f);
    void Update(Wnd* window);

    // Callback functions
    void SizeCallback(GLFWwindow* window, int32 width, int32 height);

    // Getter & Setter functions
    void SetCurrentContext(Wnd* window, class engine::Window* wndClass);

    bool ShouldWindowClose(Wnd* window);
    bool IsMinimized(Wnd* window);
    bool IsFullScreen(Wnd* window);
}