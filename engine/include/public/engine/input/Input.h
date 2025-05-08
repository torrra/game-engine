#pragma once

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include "InputData.h"

#include <math/MathGeneric.hpp>
#include <math/Vector2.hpp>

#include <mutex>
#include <unordered_map>

/*
*	--------- Keyboard Input ---------
*	- Handle modifier keys		
*	- Controller support		
*	----------------------------------
*/

namespace engine
{
    enum EInputState
    {
        UP = KEY_STATE_RELEASED,
        PRESSED = KEY_STATE_PRESSED,
        HELD = KEY_STATE_HELD,
    };

    enum ECursorMode
    {
        MODE_NORMAL = CURSOR_MODE_NORMAL,
        MODE_HIDDEN = CURSOR_MODE_HIDDEN,
        MODE_DISABLED = CURSOR_MODE_DISABLED,
        MODE_CAPTURED = CURSOR_MODE_CAPTURED
    };

    struct InputData
    {
        EInputState m_currentState = EInputState::UP;
        EInputState m_prevState = EInputState::UP;
    };

    class Input
    {
    private:

        // Mouse scroll delta callback is called to late (right before reset),
        // so we're keeping scroll through the next frame
        enum class EScrollState
        {
            // No input detected yet
            NO_INPUT,

            // Frame at which scroll delta is available
            FIRST_FRAME,

            // Reset scroll delta
            SECOND_FRAME
        };


    public:
        ~Input(void) = default;

        // Input manager functions
        ENGINE_API static int32 StartUp(void);
        ENGINE_API static void ShutDown(void);
        ENGINE_API static void RegisterInput(int32 key);
        ENGINE_API static void UnregisterInput(int32 key);
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
        static void KeyboardCallback(int32 key, int32 scanCode, int32 action, int32 mods);
        static void MouseButtonCallback(int32 button, int32 action, int32 mods);
        static void MouseScrollCallback(f64 xOffset, f64 yOffset);
        static void CursorPosCallback(f64 xPos, f64 yPos);

    private:
        Input(void);
        Input(Input const& inputManager) = delete;
        Input& operator=(Input const& inputManager) = delete;
        static bool HasKey(int32 keyCode) noexcept;
        ENGINE_API static Input*	GetInstance(void);

        std::unordered_map<int32, InputData> m_keyMap;
        
        math::Vector2d m_cursorPos;
        math::Vector2d m_lastCursorPos;
        math::Vector2d m_scrollDelta;

        static std::mutex			    m_mutex;
        static Input*					m_instance;
        bool							m_dirty;
        bool                            m_cursorUpdated;
        EScrollState                    m_scrollUpdated;
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
        math::Vector2d delta = GetInstance()->m_cursorPos - GetInstance()->m_lastCursorPos;

        return math::Vector2<TValueType>(
            static_cast<TValueType>(delta.GetX()),
            static_cast<TValueType>(delta.GetY())
        );
    }

    template<math::CScalarType TValueType>
    inline math::Vector2<TValueType> Input::GetScrollDelta(void)
    {
        return math::Vector2<TValueType>
        (
            static_cast<TValueType>(GetInstance()->m_scrollDelta.GetX()),
            static_cast<TValueType>(GetInstance()->m_scrollDelta.GetY())
        );
    }
}