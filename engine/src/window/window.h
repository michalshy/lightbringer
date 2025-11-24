#pragma once
#include <SDL_video.h>
#include "engine_api.h"

class Window
{
    bool m_Fullscreen{false};
    SDL_Window* m_Window;
    SDL_GLContext m_Context;
public:
    ENGINE_API bool Init();
    ENGINE_API SDL_Window* GetWindowRaw();
    ENGINE_API SDL_GLContext GetContextRaw();
    ENGINE_API void ToggleFullscreen();
    ENGINE_API void Exit();
};