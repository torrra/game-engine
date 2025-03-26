#pragma once

#include "engine/CoreTypes.h"

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

namespace engine::input
{
    void KeyboardCallback(GLFWwindow* window, int32 key, int32 scanCode, int32 action, int32 mods);
    void MouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods);
    void MouseScrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset);
    void CursorPosCallback(GLFWwindow* window, f64 xPos, f64 yPos);

    void SetCursorMode(int32 mode);

    int32 InitCallbacks(void);
}