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

	GetInstance()->m_keyMap[key] = EInputState::NONE;
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
	// Key not registed
	if (!GetInstance()->HasKey(input))
		return false;

	bool result = GetInstance()->m_keyMap[input] == EInputState::PRESSED;
	if (result)
		GetInstance()->m_keyMap[input] = EInputState::HELD;

	return result;
}

bool engine::Input::IsInputHeld(int input)
{
	// Key not registed
	if (!GetInstance()->HasKey(input))
		return false;

	EInputState keyState = GetInstance()->m_keyMap[input];

	return
		keyState != EInputState::NONE &&
		keyState != EInputState::RELEASED;
}

bool engine::Input::IsInputReleased(int input)
{
	// Key not registed
	if (!GetInstance()->HasKey(input))
		return false;

	bool result = GetInstance()->m_keyMap[input] == EInputState::RELEASED;
	if (result)
		GetInstance()->m_keyMap[input] = EInputState::NONE;

	return result;
}

void engine::Input::StaticKeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
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
		GetInstance()->m_keyMap[key] = static_cast<EInputState>(action);
}

void engine::Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	(void) window;
	(void) mods;

	if (GetInstance()->m_keyMap.contains(button))
		GetInstance()->m_keyMap[button] = static_cast<EInputState>(action);
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

void engine::Input::ShutDownInputManager(void)
{
	delete m_instance;
}

void engine::Input::SetCursorMode(ECursorMode cursorMode)
{
	glfwSetInputMode(glfwGetCurrentContext(), CURSOR, cursorMode);
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