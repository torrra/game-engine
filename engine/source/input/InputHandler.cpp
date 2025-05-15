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
    for (auto& input : GetInstance()->m_inputMap)
    {
        input.second.m_prevState = input.second.m_state;
        
        if (input.second.m_state == EInputState::STATE_PRESSED)
            input.second.m_state = EInputState::STATE_HELD;
        else if (input.second.m_state == EInputState::STATE_RELEASED)
            input.second.m_state = EInputState::STATE_UP;
    }
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
