#include "input/Input.h"
#include "input/InputCallbacks.h"

std::mutex engine::Input::m_mutex;
engine::Input* engine::Input::m_instance = nullptr;

int32 engine::Input::StartUp(void)
{
    return input::InitCallbacks();
}

void engine::Input::ShutDown(void)
{
    if (m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

void engine::Input::RegisterInput(int32 key)
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

void engine::Input::UnregisterInput(int32 key)
{
    // Check if key exists
    if (!HasKey(key))
    {
        std::printf("Key is not registered\n");
        return;
    }

    GetInstance()->m_keyMap.erase(key);
}

bool engine::Input::IsInputPressed(int32 input)
{
    // Key not registered 
    if (!GetInstance()->HasKey(input))
        return false;

    InputData& data = GetInstance()->m_keyMap[input];

    return 
        data.m_currentState == EInputState::PRESSED &&
        data.m_prevState == EInputState::UP;
}

bool engine::Input::IsInputDown(int32 input)
{
    // Key not registered
    if (!GetInstance()->HasKey(input))
        return false;

    return GetInstance()->m_keyMap[input].m_currentState != EInputState::UP;
}

bool engine::Input::IsInputHeld(int32 input)
{
    // Key not registered
    if (!GetInstance()->HasKey(input))
        return false;

    return GetInstance()->m_keyMap[input].m_currentState == EInputState::HELD;
}

bool engine::Input::IsInputReleased(int32 input)
{
    // Key not registered
    if (!GetInstance()->HasKey(input))
        return false;

    InputData& data = GetInstance()->m_keyMap[input];

    return data.m_currentState == EInputState::UP &&
        (data.m_prevState == EInputState::PRESSED ||
         data.m_prevState == EInputState::HELD);
}

void engine::Input::KeyboardCallback(int32 key, int32 scanCode, int32 action, int32 mods)
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

void engine::Input::MouseButtonCallback(int32 button, int32 action, int32 mods)
{
    (void) mods;

    if (GetInstance()->m_keyMap.contains(button))
    {
        InputData& input = GetInstance()->m_keyMap[button];
        
        input.m_prevState = input.m_currentState;
        input.m_currentState = static_cast<EInputState>(action);

        GetInstance()->m_resetKeys = (input.m_currentState == KEY_STATE_RELEASED);
    }
}

void engine::Input::MouseScrollCallback(f64 xOffset, f64 yOffset)
{
    GetInstance()->m_scrollDelta = math::Vector2d(xOffset, yOffset);
}

void engine::Input::CursorPosCallback(f64 xPos, f64 yPos)
{
    GetInstance()->m_cursorPos = math::Vector2d(xPos, yPos);
}

engine::Input::Input(void)
    : m_cursorPos(0.00), m_scrollDelta(0.00), m_resetKeys(false)
{
}

void engine::Input::SetCursorMode(ECursorMode cursorMode)
{
    input::SetCursorMode(cursorMode);
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

bool engine::Input::HasKey(int32 key) noexcept
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