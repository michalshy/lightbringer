#pragma once

#include <chrono>
#include "engine_api.h"

class Timer
{
    float m_DeltaTime;
    std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
public:
    ENGINE_API Timer() = default;
    ENGINE_API bool Init();
    ENGINE_API void Start();
    ENGINE_API float Delta();
    ENGINE_API float GetDelta();
};