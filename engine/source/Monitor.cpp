#include "utility/Monitor.h"

#include <glfw/glfw3.h>

math::Vector2i engine::Monitor::GetPrimaryMonitorSize(void)
{
	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	return {vidMode->width, vidMode->height};
}
