#pragma once

#include <chrono>

class Timer
{
    float m_DeltaTime;
    std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
public:
    Timer() = default;
    bool Init();
    void Start();
    float Delta();
    float GetDelta();
};