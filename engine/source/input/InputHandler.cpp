#include "input/InputCallbacks.h"
#include "input/InputHandler.h"

std::mutex engine::InputHandler::m_mutex;
engine::InputHandler* engine::InputHandler::m_instance = nullptr;

bool engine::InputHandler::StartUp(void)
{
    return input::InitCallbacks() != 0;
}

void engine::InputHandler::ShutDown(void)
{
    if (m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

void engine::InputHandler::UpdateKeyState(void)
{
    if (GetInstance()->m_scrollUpdated == EScrollState::FIRST_FRAME)
        GetInstance()->m_scrollUpdated = EScrollState::SECOND_FRAME;

    else if (GetInstance()->m_scrollUpdated == EScrollState::SECOND_FRAME)
    {
        GetInstance()->m_scrollDelta = math::Vector2d::Zero();
        GetInstance()->m_scrollUpdated = EScrollState::NO_INPUT;
    }


    for (auto& input : GetInstance()->m_inputMap)
    {
        input.second.m_prevState = input.second.m_state;

        if (input.second.m_state == EInputState::STATE_PRESSED)
            input.second.m_state = EInputState::STATE_HELD;
        else if (input.second.m_state == EInputState::STATE_RELEASED)
            input.second.m_state = EInputState::STATE_UP;
    }

    GetInstance()->m_mousePosDelta = GetInstance()->m_mousePos - GetInstance()->m_prevMousePos;
    GetInstance()->m_prevMousePos = GetInstance()->m_mousePos;
}

void engine::InputHandler::SetCursorMode(ECursorMode mode)
{
    input::SetCursorMode(mode);
}

bool engine::InputHandler::IsInputPressed(int32 keyCode)
{
    return GetInstance()->m_inputMap[keyCode].m_prevState == EInputState::STATE_PRESSED;
}

bool engine::InputHandler::IsInputHeld(int32 keyCode)
{
    return GetInstance()->m_inputMap[keyCode].m_state == EInputState::STATE_HELD;
}

bool engine::InputHandler::IsInputDown(int32 keyCode)
{
    return GetInstance()->m_inputMap[keyCode].m_state == EInputState::STATE_PRESSED ||
        GetInstance()->m_inputMap[keyCode].m_state == EInputState::STATE_HELD;
}

bool engine::InputHandler::IsInputReleased(int32 keyCode)
{
    return GetInstance()->m_inputMap[keyCode].m_prevState == EInputState::STATE_RELEASED;
}

void engine::InputHandler::KeyboardCallback(int32 key, int32 scanCode, int32 action, int32 mods)
{
    (void) scanCode; 
    (void) mods;

    InputState input;
    input.m_state = static_cast<EInputState>(action);

    if (input.m_state == EInputState::STATE_PRESSED)
        input.m_prevState = EInputState::STATE_UP;
    else if (input.m_state == EInputState::STATE_HELD || input.m_state == EInputState::STATE_RELEASED)
        input.m_prevState = EInputState::STATE_HELD;
    else if (input.m_state == EInputState::STATE_UP)
        input.m_prevState = EInputState::STATE_UP;

    GetInstance()->m_inputMap[key] = input;
}

void engine::InputHandler::MouseButtonCallback(int32 button, int32 action, int32 mods)
{
    (void) mods;

    InputState input;
    input.m_state = static_cast<EInputState>(action);

    if (input.m_state == EInputState::STATE_PRESSED)
        input.m_prevState = EInputState::STATE_UP;
    else if (input.m_state == EInputState::STATE_HELD || input.m_state == EInputState::STATE_RELEASED)
        input.m_prevState = EInputState::STATE_HELD;
    else if (input.m_state == EInputState::STATE_UP)
        input.m_prevState = EInputState::STATE_UP;

    GetInstance()->m_inputMap[button] = input;
}

void engine::InputHandler::MouseScrollCallback(f64 xOffset, f64 yOffset)
{
    GetInstance()->m_scrollDelta = math::Vector2d(xOffset, yOffset);
    GetInstance()->m_scrollUpdated = EScrollState::FIRST_FRAME;
}

void engine::InputHandler::CursorPosCallback(f64 xPos, f64 yPos)
{
    GetInstance()->m_mousePos = {xPos, yPos};
}

engine::InputHandler::InputHandler(void)
    : m_mousePos(0.00), m_prevMousePos(0.00), m_scrollDelta(0.00),
    m_mousePosDelta(0.0), m_scrollUpdated(EScrollState::NO_INPUT)
{}

engine::InputHandler* engine::InputHandler::GetInstance(void)
{
    if (!m_instance)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (!m_instance)
            m_instance = new InputHandler();
    }

    return m_instance;
}
