#include "Window.h"
#include "input/Input.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utility/Timer.h"
#include "utility/MemoryCheck.h"

#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 5

engine::Window::Window(const char* title, uint32 width, uint32 height)
	: m_title(title), m_size({width, height}), m_windowPtr(nullptr)
{
	static bool isInitialized = false;

	if (!isInitialized)
	{
		Init();
		isInitialized = true;
	}
	else
		CreateWindow();
}

f32 engine::Window::GetAspectRatio(void) const noexcept
{
	return m_aspectRatio;
}

GLFWwindow* engine::Window::GetWindowPtr(void) const noexcept
{
	return m_windowPtr;
}

bool engine::Window::IsIconified(void) const noexcept
{
	return glfwGetWindowAttrib(m_windowPtr, GLFW_ICONIFIED);
}

bool engine::Window::IsFullScreen(void) const noexcept
{
	return glfwGetWindowAttrib(m_windowPtr, GLFW_MAXIMIZED);
}

bool engine::Window::ShouldWindowClose(void)
{
	return glfwWindowShouldClose(m_windowPtr);
}

void engine::Window::MakeCurrentContext(void)
{
	glfwMakeContextCurrent(m_windowPtr);
}

void engine::Window::ClearWindow(f32 red, f32 green, f32 blue, f32 alpha)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(red, green, blue, alpha);
}

void engine::Window::UpdateBuffers(void)
{
	glfwSwapBuffers(m_windowPtr);
	glfwPollEvents();

	g_engineTime.Update();
}

void engine::Window::Shutdown(void)
{
	m_title.clear();
	glfwTerminate();
}

GLFWwindow* engine::Window::GetCurrentContext(void)
{
	return glfwGetCurrentContext();
}

int32 engine::Window::Init(void)
{
	int32 result;

	result = InitGLFW();
	result = CreateWindow();
	result = InitGlad();

	return result;
}

int32 engine::Window::InitGLFW(void)
{
	bool result = glfwInit();

	if (!result)
	{
		printf("Failed to initialized GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return 0;
}

int32 engine::Window::InitGlad(void)
{
	int32 result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	if (!result)
	{
		printf("Failed to initialize glad\n");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return 0;
}

int32 engine::Window::CreateWindow()
{
	m_windowPtr = glfwCreateWindow(
		m_size.GetX(), m_size.GetY(),
		m_title.c_str(),
		NULL, NULL
	);

	if (!m_windowPtr)
	{
		printf("Window pointer is null\n");
		return -1;
	}

	glfwMakeContextCurrent(m_windowPtr);
	glfwSetWindowUserPointer(m_windowPtr, this);
	glfwSetWindowSizeCallback(m_windowPtr, SizeCallback);

	Input::SetInputCallbacks(m_windowPtr);

	// Set screen aspect ratio
	SetAspectRatio();

	return 0;
}

void engine::Window::SetSize(uint32 width, uint32 height)
{
	m_size.X() = width;
	m_size.Y() = height;

	// Update screen aspect ratio
	SetAspectRatio();
}

void engine::Window::SetAspectRatio(void)
{
	/*
	*	Only update aspect ratio when screen size changes to avoid
	*	unnecessary divisions
	*/
	m_aspectRatio = GetWidth<f32>() / GetHeight<f32>();
}

void engine::Window::SizeCallback(GLFWwindow* window, int32 width, int32 height)
{
	Window* windowPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

	windowPtr->SetSize(width, height);

	glViewport(0, 0, width, height);
}
