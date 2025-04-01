#include "utility/Timer.h"

#include <glfw/glfw3.h>

engine::Time engine::g_engineTime;

engine::Time::Time(void)
    : m_deltaTime(0.0f), m_totalTime(0.0f), m_lastTime(0.0f)
{
}

void engine::Time::Update()
{
    m_totalTime = static_cast<f32>(glfwGetTime());
    
    m_deltaTime = m_totalTime - m_lastTime;
    
    m_lastTime = m_totalTime;
}

f32 engine::Time::GetDeltaTime(void)
{
    return m_deltaTime;
}

f32 engine::Time::GetTotalTime(void)
{
    return m_totalTime;
}