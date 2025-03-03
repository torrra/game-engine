#pragma once

#include "InputData.h"

#include <math/MathGeneric.hpp>
#include <math/Vector2.hpp>

#include <mutex>
#include <string>
#include <unordered_map>

#include "utility/MemoryCheck.h"
#include "EngineExport.h"

/*
*	--------- Keyboard Input ---------
*	- Keyboard Key pressed		DONE
*	- Keyboard Key held			DONE
*	- Keyboard Key release		DONE
*	- Mouse button press		DONE
*	- Mouse button held			DONE
*	- Mouse button release		DONE
*	- Mouse position			DONE
*	- Mouse delta position		DONE
*	- Mouse delta scroll		DONE
*	- Set mouse mode			DONE
*	- Handle modifier keys		
*	- Controller support?		
*	- Input register			DONE
*	- Remove singleton
*/

struct GLFWwindow;

namespace engine
{
	enum EInputState
	{
		PRESSED = KEY_STATE_PRESSED,
		HELD = KEY_STATE_HELD,
		RELEASED = KEY_STATE_RELEASED,
		NONE = KEY_STATE_NONE
	};

	enum ECursorMode
	{
		NORMAL = CURSOR_MODE_NORMAL,
		HIDDEN = CURSOR_MODE_HIDDEN,
		DISABLED = CURSOR_MODE_DISABLED,
		CAPTURED = CURSOR_MODE_CAPTURED
	};

	class Input
	{
	public:
								~Input(void) = default;

		// Input manager functions
		static void ENGINE_API RegisterInput(int key);
		static void ENGINE_API UnregisterInput(int key);
		static void ENGINE_API ShutDownInputManager(void);
		static void ENGINE_API SetCursorMode(ECursorMode cursorMode);

		// Mouse getter functions
		template <math::CScalarType TValueType>
		static math::Vector2<TValueType> ENGINE_API GetCursorPosition(void);

		template <math::CScalarType TValueType>
		static math::Vector2<TValueType> ENGINE_API GetCursorDeltaPos(void);

		template <math::CScalarType TValueType>
		static math::Vector2<TValueType> ENGINE_API GetScrollDelta(void);

		// Input state functions
		static bool ENGINE_API IsInputPressed(int input);
		static bool ENGINE_API IsInputHeld(int input);
		static bool ENGINE_API IsInputReleased(int input);

		// Callback functions
		static void ENGINE_API StaticKeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
		static void ENGINE_API MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void ENGINE_API MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		static void ENGINE_API CursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	private:
						Input(void) = default;
						Input(Input const& inputManager) = delete;
		Input&			operator=(Input const& inputManager) = delete;
		static bool		HasKey(int keyCode) noexcept;
		static Input*	GetInstance(void);

		std::unordered_map<int, EInputState> m_keyMap;

		math::Vector2<double>					m_cursorPos;
		math::Vector2<double>					m_scrollDelta;

		static std::mutex						m_mutex;
		static Input*							m_instance;
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