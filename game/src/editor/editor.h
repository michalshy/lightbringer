#pragma once
#include "backends/imgui_impl_sdl2.h"
#include <SDL_video.h>
#include "game/game.h"

class Editor
{
    SDL_GLContext m_Context;
    SDL_Window* m_Window;
    Game* m_Game;

    struct CameraOptions
    {
        bool free_cam = false; // todo: set this as some project parameters
    } m_CamOpt;


public:
    bool Init(Game* game, SDL_Window* window, SDL_GLContext context);
    void OnFrame(float delta_time);
    void EndFrame();
    void PostFrame();
    void Exit();
    void PollEvents(SDL_Event& e);
private:
    void UpdateUI();
    void HandleMouseWheel(float w, float mouse_x, float mouse_y); 
    void HandleKeyDown(float delta_time);
    void ShowMatrix(const glm::mat4& mat);
};