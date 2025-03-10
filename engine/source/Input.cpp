#include "input/Input.h"

#include <GLFW/glfw3.h>

std::mutex engine::Input::m_mutex;
engine::Input* engine::Input::m_instance = nullptr;

void engine::Input::RegisterInput(int key)
{
	// Check if key exists
	if (HasKey(key))
	{
		std::printf("Key already registered\n");
		return;
	}

	GetInstance()->m_keyMap[key].m_currentState = EInputState::UP;
	GetInstance()->m_keyMap[key].m_prevState = EInputState::UP;

}

void engine::Input::UnregisterInput(int key)
{
	// Check if key exists
	if (!HasKey(key))
	{
		std::printf("Key is not registered\n");
		return;
	}

	GetInstance()->m_keyMap.erase(key);
}

bool engine::Input::IsInputPressed(int input)
{
	// Key not registered 
	if (!GetInstance()->HasKey(input))
		return false;

	InputData& data = GetInstance()->m_keyMap[input];

	return 
		data.m_currentState == EInputState::PRESSED &&
		data.m_prevState == EInputState::UP;
}

bool engine::Input::IsInputDown(int input)
{
	// Key not registered
	if (!GetInstance()->HasKey(input))
		return false;

	return GetInstance()->m_keyMap[input].m_currentState != EInputState::UP;
}

bool engine::Input::IsInputHeld(int input)
{
	// Key not registered
	if (!GetInstance()->HasKey(input))
		return false;

	return GetInstance()->m_keyMap[input].m_currentState == EInputState::HELD;
}

bool engine::Input::IsInputReleased(int input)
{
	// Key not registered
	if (!GetInstance()->HasKey(input))
		return false;

	InputData& data = GetInstance()->m_keyMap[input];

	return data.m_currentState == EInputState::UP &&
		(data.m_prevState == EInputState::PRESSED ||
		 data.m_prevState == EInputState::HELD);
}

void engine::Input::KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	/*
	*	Keyboard Callback
	*	Gets data for the state of every key
	*
	*	Key:		ASCII numeric key code
	*	Scancode:	Platform specific key identifier
	*	Action:		State of the key, can either be press, repeat or release
	*	mods:		Checks if a modifier key is pressed, this can be the shift,
	*				alt, ctrl or windows key
	*/

	// TODO: implement support for modifier keys
	(void) window;
	(void) scanCode;
	(void) mods;

	if (GetInstance()->m_keyMap.contains(key))
	{
		InputData& input = GetInstance()->m_keyMap[key];

		input.m_prevState = input.m_currentState;
		input.m_currentState = static_cast<EInputState>(action);
		
		GetInstance()->m_resetKeys = (input.m_currentState == KEY_STATE_RELEASED);
	}
}

void engine::Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	(void) window;
	(void) mods;

	if (GetInstance()->m_keyMap.contains(button))
	{
		InputData& input = GetInstance()->m_keyMap[button];
		
		input.m_prevState = input.m_currentState;
		input.m_currentState = static_cast<EInputState>(action);

		GetInstance()->m_resetKeys = (input.m_currentState == KEY_STATE_RELEASED);
	}
}

void engine::Input::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	(void) window;

	GetInstance()->m_scrollDelta = math::Vector2<double>(xOffset, yOffset);
}

void engine::Input::CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	(void) window;

	GetInstance()->m_cursorPos = math::Vector2<double>(xPos, yPos);
}

void engine::Input::SetInputCallbacks(GLFWwindow* window)
{
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);
}

engine::Input::Input(void)
	: m_cursorPos(0.00), m_scrollDelta(0.00), m_resetKeys(false)
{
}

void engine::Input::ShutDownInputManager(void)
{
	delete m_instance;
}

void engine::Input::SetCursorMode(ECursorMode cursorMode)
{
	glfwSetInputMode(glfwGetCurrentContext(), CURSOR, cursorMode);
}

void engine::Input::ResetKeys(void)
{
	if (!GetInstance()->m_resetKeys)
		return;
	
	for (auto& key : GetInstance()->m_keyMap)
	{
		if (key.second.m_currentState == EInputState::UP &&
			key.second.m_prevState != EInputState::UP)
			key.second.m_prevState = EInputState::UP;
	}

	GetInstance()->m_resetKeys = false;
}

bool engine::Input::HasKey(int key) noexcept
{
	return GetInstance()->m_keyMap.contains(key);
}

engine::Input* engine::Input::GetInstance(void)
{
	if (!m_instance)
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		if (!m_instance)
			m_instance = new Input();
	}

	return m_instance;
}