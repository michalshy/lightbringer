#include "timer.h"
#include <chrono>

constexpr float MAX_DELTA = 1.0f;
constexpr float MIN_DELTA = 0.0f;

bool Timer::Init()
{
    // init static variables
    Timer::m_DeltaTime = 0.0f;
    Timer::m_LastTime = std::chrono::steady_clock::now();
    return true;
}

void Timer::Start()
{
    m_LastTime = std::chrono::steady_clock::now();
}

float Timer::Delta()
{
    auto current_time = std::chrono::steady_clock::now();
    float duration = std::chrono::duration<float>(current_time - m_LastTime).count();
    m_LastTime = current_time;
    if(duration > MIN_DELTA)
    {
        if (duration < MIN_DELTA)
            duration = MIN_DELTA;
        else if (duration > MAX_DELTA)
            duration = MAX_DELTA;

        m_DeltaTime = duration;
    }
    return m_DeltaTime;
}

float Timer::GetDelta()
{
    return m_DeltaTime;
}