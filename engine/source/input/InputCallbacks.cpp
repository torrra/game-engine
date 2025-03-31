#include "input/InputCallbacks.h"

#include "Window.h"
#include "input/Input.h"

void engine::input::KeyboardCallback(GLFWwindow* window, int32 key, int32 scanCode, int32 action, int32 mods)
{
    (void) window;

    Input::KeyboardCallback(key, scanCode, action, mods);
}

void engine::input::MouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods)
{
    (void) window;

    Input::MouseButtonCallback(button, action, mods);
}

void engine::input::MouseScrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset)
{
    (void) window;

    Input::MouseScrollCallback(xOffset, yOffset);
}

void engine::input::CursorPosCallback(GLFWwindow* window, f64 xPos, f64 yPos)
{
    (void) window;

    Input::CursorPosCallback(xPos, yPos);
}

void engine::input::SetCursorMode(int32 mode)
{
    glfwSetInputMode(glfwGetCurrentContext(), CURSOR, mode);
}

int32 engine::input::InitCallbacks(void)
{
    GLFWwindow* window = glfwGetCurrentContext();

    if (window)
    {
        glfwSetKeyCallback(window, KeyboardCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetScrollCallback(window, MouseScrollCallback);
        glfwSetCursorPosCallback(window, CursorPosCallback);
    }
    else
    {
        printf("Failed to set input callbacks\n");

        return -1;
    }

    return 0;
}
