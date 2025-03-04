#include "utility/Timer.h"

#include <glfw/glfw3.h>

float engine::DeltaTime(void)
{
	static float prevTime = 0.0f;
	float currentTime = static_cast<float>(glfwGetTime());

	float deltaTime = currentTime - prevTime;
	prevTime = currentTime;

	return deltaTime;
}