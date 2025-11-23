#pragma once
#include <SDL_video.h>

class Window
{
    bool m_Fullscreen{false};
    SDL_Window* m_Window;
    SDL_GLContext m_Context;
public:
    bool Init();
    SDL_Window* GetWindowRaw();
    SDL_GLContext GetContextRaw();
    void ToggleFullscreen();
    void Exit();
};