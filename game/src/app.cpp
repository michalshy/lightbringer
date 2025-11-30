#include <SDL_events.h>
#include <SDL_keycode.h>
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <spdlog/spdlog.h>
#include "log.h"
#include "app.h"
#include "renderer/light_manager.h"
#include "renderer/renderer.h"
#include "resources/resource_manager.h"
#include "timer/timer.h"
#include "config_params.h"

App::App()
{
    window = std::make_unique<Window>();
    game = std::make_unique<Game>();
    timer = std::make_unique<Timer>();
#ifdef BUILD_WITH_EDITOR
    editor = std::make_unique<Editor>();
#endif
}

bool App::Init()
{
    spdlog::set_level(LOG_LEVEL_DEBUG);
    
    if (!window || !window->Init())
        return false;

    if (!ResourceManager::Init())
        return false;

    if (!Renderer::Init(window->GetWindowRaw(), TILESET))
        return false;
    
    if (!game || !game->Init())
        return false;

    if (!LightManager::Init())
        return false;

    if (!timer || !timer->Init())
        return false;

#ifdef BUILD_WITH_EDITOR

    if(!editor || !editor->Init(game.get(), window->GetWindowRaw(), window->GetContextRaw()))
    LOG_DEBUG("Initialized Game with editor");
#else
    LOG_DEBUG("Initialized Game");
#endif
    initialized = true;
    return true;
}

void App::Run()
{
    if(!initialized)
        return;

    timer->Start();

    while(!quit)
    {
        float delta_time = timer->Delta();
        PollEvents();
        
        Renderer::SetClearColor({0.1f, 0.12f, 0.15f, 1.0f});
        Renderer::BeginFrame(TILESET); 
        OnFrame(delta_time); 
        
        EndFrame();  
        PostFrame(); 
    }

    Exit();
}

void App::Exit()
{
    Renderer::Shutdown();

    ResourceManager::Shutdown();

#ifdef BUILD_WITH_EDITOR
    editor->Exit();
#endif
    window->Exit();


    SDL_Quit();
}

void App::PollEvents()
{
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0)
    {
        game->PollEvents(e);
#ifdef BUILD_WITH_EDITOR
        editor->PollEvents(e);
#endif
        if(e.type == SDL_QUIT)
            quit = true;
        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(window->GetWindowRaw()))
            quit = true;
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym) {
                case SDLK_BACKQUOTE:
                {
                    window->ToggleFullscreen();
                    break;
                }
                //todo define this for ifdef debug
                case SDLK_ESCAPE:
                {
                    quit = true;
                    break;
                }
            }
        }
    }
}

void App::OnFrame(float delta_time)
{
    game->Update(delta_time);
    game->Draw();
#ifdef BUILD_WITH_EDITOR
    editor->OnFrame(delta_time);
#endif
}

void App::EndFrame()
{
    Renderer::EndFrame();
#ifdef BUILD_WITH_EDITOR
    editor->EndFrame();
#endif
}

void App::PostFrame()
{
    Renderer::PostFrame();
#ifdef BUILD_WITH_EDITOR
    editor->PostFrame();
#endif
}