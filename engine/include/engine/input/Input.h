#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "InputData.h"

#include <math/MathGeneric.hpp>
#include <math/Vector2.hpp>

#include <mutex>
#include <string>
#include <unordered_map>

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
		ENGINE_API static void RegisterInput(int32 key);
		ENGINE_API static void UnregisterInput(int32 key);
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
		ENGINE_API static bool IsInputPressed(int32 input);
		ENGINE_API static bool IsInputDown(int32 input);
		ENGINE_API static bool IsInputHeld(int32 input);
		ENGINE_API static bool IsInputReleased(int32 input);

		// Callback functions
		ENGINE_API static void KeyboardCallback(GLFWwindow* window, int32 key, int32 scanCode, int32 action, int32 mods);
		ENGINE_API static void MouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods);
		ENGINE_API static void MouseScrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset);
		ENGINE_API static void CursorPosCallback(GLFWwindow* window, f64 xPos, f64 yPos);
		ENGINE_API static void SetInputCallbacks(GLFWwindow* window);
	
	private:
						Input(void);
						Input(Input const& inputManager) = delete;
		Input&			operator=(Input const& inputManager) = delete;
		static bool		HasKey(int32 keyCode) noexcept;
		ENGINE_API static Input*	GetInstance(void);

		std::unordered_map<int32, InputData> m_keyMap;
		
		math::Vector2d					m_cursorPos;
		math::Vector2d					m_scrollDelta;

		static std::mutex						m_mutex;
		static Input*							m_instance;
		bool									m_resetKeys;
	};

	// Template function definitions
	template<math::CScalarType TValueType>
	inline math::Vector2<TValueType> Input::GetCursorPosition(void)
	{
		math::Vector2d cursorPos = Input::GetInstance()->m_cursorPos;

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