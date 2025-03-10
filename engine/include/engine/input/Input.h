#pragma once

#include "InputData.h"

#include <math/MathGeneric.hpp>
#include <math/Vector2.hpp>

#include <mutex>
#include <string>
#include <unordered_map>

#include "engine/utility/MemoryCheck.h"
#include "engine/EngineExport.h"

/*
*	--------- Keyboard Input ---------
*	- Handle modifier keys		
*	- Controller support		
*	- Remove singleton			
*	----------------------------------
*/

struct GLFWwindow;

namespace engine
{
	enum EInputState
	{
		PRESSED = KEY_STATE_PRESSED,
		HELD = KEY_STATE_HELD,
		UP = KEY_STATE_RELEASED,
		NONE = KEY_STATE_NONE
	};

	enum ECursorMode
	{
		NORMAL = CURSOR_MODE_NORMAL,
		HIDDEN = CURSOR_MODE_HIDDEN,
		DISABLED = CURSOR_MODE_DISABLED,
		CAPTURED = CURSOR_MODE_CAPTURED
	};

	struct InputData
	{
		EInputState m_currentState = EInputState::UP;
		EInputState m_prevState = EInputState::UP;
	};

	class Input
	{
	public:
								~Input(void) = default;

		// Input manager functions
		ENGINE_API static void RegisterInput(int key);
		ENGINE_API static void UnregisterInput(int key);
		ENGINE_API static void ShutDownInputManager(void);
		ENGINE_API static void SetCursorMode(ECursorMode cursorMode);
		ENGINE_API static void ResetKeys(void);

		// Mouse getter functions
		template <math::CScalarType TValueType>
		static math::Vector2<TValueType> GetCursorPosition(void);

		template <math::CScalarType TValueType>
		static math::Vector2<TValueType> GetCursorDeltaPos(void);

		template <math::CScalarType TValueType>
		static math::Vector2<TValueType> GetScrollDelta(void);

		// Input state functions
		ENGINE_API static bool IsInputPressed(int input);
		ENGINE_API static bool IsInputDown(int input);
		ENGINE_API static bool IsInputHeld(int input);
		ENGINE_API static bool IsInputReleased(int input);

		// Callback functions
		ENGINE_API static void KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
		ENGINE_API static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		ENGINE_API static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		ENGINE_API static void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
		ENGINE_API static void SetInputCallbacks(GLFWwindow* window);
	
	private:
						Input(void);
						Input(Input const& inputManager) = delete;
		Input&			operator=(Input const& inputManager) = delete;
		static bool		HasKey(int keyCode) noexcept;
		ENGINE_API static Input*	GetInstance(void);

		std::unordered_map<int, InputData> m_keyMap;

		math::Vector2<double>					m_cursorPos;
		math::Vector2<double>					m_scrollDelta;

		static std::mutex						m_mutex;
		static Input*							m_instance;
		bool									m_resetKeys;
	};

	// Template function definitions
	template<math::CScalarType TValueType>
	inline math::Vector2<TValueType> Input::GetCursorPosition(void)
	{
		math::Vector2<double> cursorPos = Input::GetInstance()->m_cursorPos;

		return math::Vector2<TValueType>(
			static_cast<TValueType>(cursorPos.GetX()),
			static_cast<TValueType>(cursorPos.GetY())
		);
	}

	template<math::CScalarType TValueType>
	inline math::Vector2<TValueType> Input::GetCursorDeltaPos(void)
	{
		static math::Vector2<TValueType> lastPos = GetCursorPosition<TValueType>();
		math::Vector2<TValueType> cursorPos = GetCursorPosition<TValueType>();

		math::Vector2<TValueType> deltaPos = cursorPos - lastPos;

		lastPos = cursorPos;

		return deltaPos;
	}

	template<math::CScalarType TValueType>
	inline math::Vector2<TValueType> Input::GetScrollDelta(void)
	{
		return reinterpret_cast<TValueType>(m_scrollDelta);
	}
}