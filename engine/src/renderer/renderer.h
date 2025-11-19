#pragma once

#include <SDL.h>
#include <SDL_video.h>
#include <glm/glm.hpp>
#include "shader.h"
#include <memory>
#include <vector>

class Window; 

class Renderer
{
public:
    static bool Init(SDL_Window* window);
    static void Shutdown();
    
    static void BeginFrame();
    static void EndFrame();  
    static void PostFrame();  
    
    static void SetClearColor(const glm::vec4& color);
    static void SetProjectionMatrix(const glm::mat4& matrix);

    static void Submit(const glm::mat4& transform, const glm::vec4& color);
    static void Flush();
    
    static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

private:
    struct RendererData {
        SDL_Window* window_raw = nullptr;
        uint32_t QuadVAO = 0, QuadVBO = 0, QuadIBO = 0, InstanceVBO = 0;
        Shader QuadShader;
        glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

        // instance data (vector of small struct)
        struct InstanceData {
            glm::mat4 Transform;
            glm::vec4 Color;
        };
        std::vector<InstanceData> InstanceBuffer;
    };
    static std::unique_ptr<RendererData> s_Data;

    static void SetupOpenGLState();
};