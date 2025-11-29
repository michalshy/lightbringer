#include "engine_pch.h"
#include "window.h"
#include "log.h"
#include <SDL.h>
#include <SDL_render.h>
#include <SDL_video.h>
#if HW_WIN
#include <glad/glad.h>
#else
#include <GL/glew.h>
#endif

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

bool Window::Init()
{
    if((SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) < 0) {
        LOG_ERROR("SDL Could not initialize! SDL Error: {}", SDL_GetError());
        return false;
    }
    else {
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            LOG_WARN("Warning: Linear texture filtering not enabled!");
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
        m_Window = SDL_CreateWindow("Game",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
        if(!m_Window) {
            LOG_ERROR("Window could not be created! SDL Error: {}", SDL_GetError());
            return false;
        }
        else {
            m_Context = SDL_GL_CreateContext(m_Window);
            if (!m_Context) {
                LOG_ERROR("Error: SDL_GL_CreateContext failed: {}", SDL_GetError());
                return false;
            }
            SDL_GL_MakeCurrent(m_Window, m_Context);

            #if HW_WIN 
                if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
                    LOG_ERROR("Failed to initialize GLAD!");
                    return false;
                }

            #elif HW_LINUX
                glewExperimental = GL_TRUE;  // Required for core profile
                if (glewInit() != GLEW_OK) {
                    LOG_ERROR("Failed to initialize GLEW!");
                    return false;
                }
            #endif

            glGetError();
        }
    }
    LOG_DEBUG("Initialized window");
    return true;
}

SDL_Window* Window::GetWindowRaw()
{
    return m_Window;
}

SDL_GLContext Window::GetContextRaw()
{
    return m_Context;
}

void Window::ToggleFullscreen()
{
    if(!m_Fullscreen)
        SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    else
        SDL_SetWindowFullscreen(m_Window, 0);

    m_Fullscreen = !m_Fullscreen;
}

void Window::Exit()
{
    SDL_DestroyWindow(m_Window);
    SDL_GL_DeleteContext(m_Context);
}