#pragma once

#include "engine/input/InputData.h"
#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"
#include <math/MathGeneric.hpp>
#include <math/Vector2.hpp>
#include <unordered_map>
#include <mutex>

namespace engine
{
    enum EInputState
    {
        STATE_UP = KEY_STATE_NONE,
        STATE_RELEASED = KEY_STATE_RELEASED,
        STATE_PRESSED = KEY_STATE_PRESSED,
        STATE_HELD = KEY_STATE_HELD,
    };

    enum ECursorMode
    {
        MODE_NORMAL = CURSOR_MODE_NORMAL,
        MODE_HIDDEN = CURSOR_MODE_HIDDEN,
        MODE_DISABLED = CURSOR_MODE_DISABLED,
        MODE_CAPTURED = CURSOR_MODE_CAPTURED
    };

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

    // Structure to represent a keyboard or mouse button
    struct InputState
    {
        EInputState m_state = EInputState::STATE_UP;
        EInputState m_prevState = EInputState::STATE_UP;
    };

    // Class to handle all inputs
    class InputHandler
    {
    public:
        ENGINE_API static bool StartUp(void);
        ENGINE_API static void ShutDown(void);
        ENGINE_API static void UpdateKeyState(void);
        ENGINE_API static void SetCursorMode(ECursorMode mode);

        ENGINE_API static bool IsInputPressed(int32 keyCode);
        ENGINE_API static bool IsInputHeld(int32 keyCode);
        ENGINE_API static bool IsInputDown(int32 keyCode);
        ENGINE_API static bool IsInputReleased(int32 keyCode);

        static void KeyboardCallback(int32 key, int32 scanCode, int32 action, int32 mods);
        static void MouseButtonCallback(int32 button, int32 action, int32 mods);
        static void MouseScrollCallback(f64 xOffset, f64 yOffset);
        static void CursorPosCallback(f64 xPos, f64 yPos);

        // Mouse getter functions
        template <math::CScalarType TValueType>
        static math::Vector2<TValueType> GetCursorPosition(void);

        template <math::CScalarType TValueType>
        static math::Vector2<TValueType> GetCursorDeltaPos(void);

        template <math::CScalarType TValueType>
        static math::Vector2<TValueType> GetScrollDelta(void);

    private:
        InputHandler(void);
        InputHandler(InputHandler const&) = delete;
        
        ENGINE_API static InputHandler* GetInstance(void);

        static std::mutex m_mutex;
        static InputHandler* m_instance;
        std::unordered_map<int32, InputState> m_inputMap;
        math::Vector2d m_mousePos;
        math::Vector2d m_prevMousePos;
        math::Vector2d m_mousePosDelta;
        math::Vector2d m_scrollDelta;
        EScrollState m_scrollUpdated;
    };

    template<math::CScalarType TValueType>
    inline math::Vector2<TValueType> engine::InputHandler::GetCursorPosition(void)
    {
        return math::Vector2<TValueType>
            (
                static_cast<TValueType>(GetInstance()->m_mousePos.GetX()),
                static_cast<TValueType>(GetInstance()->m_mousePos.GetY())
            );
    }

    template<math::CScalarType TValueType>
    inline math::Vector2<TValueType> engine::InputHandler::GetCursorDeltaPos(void)
    {
        return math::Vector2<TValueType>(
            static_cast<TValueType>(GetInstance()->m_mousePosDelta.GetX()),
            static_cast<TValueType>(GetInstance()->m_mousePosDelta.GetY())
        );
    }

    template<math::CScalarType TValueType>
    inline math::Vector2<TValueType> engine::InputHandler::GetScrollDelta(void)
    {
        return math::Vector2<TValueType>
            (
                static_cast<TValueType>(GetInstance()->m_scrollDelta.GetX()),
                static_cast<TValueType>(GetInstance()->m_scrollDelta.GetY())
            );
    }
}