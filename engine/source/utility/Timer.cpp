#include "utility/Timer.h"

#include <glfw/glfw3.h>

engine::Time engine::g_engineTime;

engine::Time::Time(void)
    : m_deltaTime(0.0f), m_totalTime(0.0f), m_lastTime(0.0f)
{
}

void engine::Time::Update()
{
    constexpr f32 updateTime = 1.0f / 60.0f;

    m_totalTime = static_cast<f32>(glfwGetTime());
    
    m_deltaTime = m_totalTime - m_lastTime;
    
    if (m_deltaTime > updateTime)
        m_deltaTime = updateTime;

    m_lastTime = m_totalTime;
}

void engine::Time::Reset(void)
{
    m_totalTime = m_lastTime = m_deltaTime = 0.f;
}

f32 engine::Time::GetDeltaTime(void) const
{
    return m_deltaTime;
}

f32 engine::Time::GetTotalTime(void) const
{
    return m_totalTime;
}