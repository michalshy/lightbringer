#pragma once

#include <SDL.h>
#include <SDL_video.h>
#include <glm/glm.hpp>
#include "shader.h"
#include <memory>
#include <string_view>
#include <vector>

class Window; 

class Renderer
{
public:
    ENGINE_API static bool Init(SDL_Window* window, std::string_view path);
    ENGINE_API static void Shutdown();
    
    ENGINE_API static void BeginFrame(std::string_view tileset);
    ENGINE_API static void EndFrame();
    ENGINE_API static void PostFrame();
    
    ENGINE_API static void SetClearColor(const glm::vec4& color);
    ENGINE_API static void SetProjectionMatrix(const glm::mat4& matrix);

    ENGINE_API static void Submit(const glm::mat4& transform, const glm::vec4& color, const glm::vec4& rect);
    ENGINE_API static void Submit(const glm::vec2& pos, const glm::vec2& scale, const glm::vec4& color, const glm::vec4& rect);
    ENGINE_API static void Submit(const glm::vec2& pos, const glm::vec2& scale, float rotation, const glm::vec4& color, const glm::vec4& rect);
    ENGINE_API static void Flush();

    ENGINE_API static void DrawQuad(const glm::mat4& transform, const glm::vec4& rect);
    ENGINE_API static void DrawQuad(const glm::vec2& pos, const glm::vec2& scale, const glm::vec4& rect);
    ENGINE_API static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const glm::vec4& rect);
    ENGINE_API static void DrawQuad(const glm::vec3& pos, const glm::vec3& scale, const glm::vec4& color, const glm::vec4& rect);

private:
    struct RendererData {
        SDL_Window* window_raw = nullptr;
        uint32_t QuadVAO = 0, QuadVBO = 0, QuadIBO = 0, InstanceVBO = 0;
        Shader QuadShader;
        glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

        struct InstanceData {
            glm::mat4 Transform;
            glm::vec4 Color;
            glm::vec4 TexRect;
        };
        std::vector<InstanceData> InstanceBuffer;
    };
    static std::unique_ptr<RendererData> s_Data;

    static void SetupOpenGLState();
};