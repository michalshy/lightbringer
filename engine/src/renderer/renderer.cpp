#include "renderer.h"
#if HW_WIN
    #include <glad/glad.h>
#else
    #include <GL/glew.h>
#endif
#include <glm/ext/matrix_clip_space.hpp>
#include "light_manager.h"

std::unique_ptr<Renderer::RendererData> Renderer::s_Data = nullptr;

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
};

void Renderer::SetupOpenGLState()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);
}

bool Renderer::Init(SDL_Window* window)
{
    s_Data = std::make_unique<RendererData>();
    s_Data->window_raw = window;
    if (!window) return false;

    // Create quad geometry (4 verts)
    float vertices[] = {
        // pos.x, pos.y, pos.z,   u, v
        -0.5f, -0.5f, 0.0f,      0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,      1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,      1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,      0.0f, 1.0f
    };
    uint32_t indices[] = { 0,1,2, 2,3,0 };

    glGenVertexArrays(1, &s_Data->QuadVAO);
    glBindVertexArray(s_Data->QuadVAO);

    glGenBuffers(1, &s_Data->QuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_Data->QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &s_Data->QuadIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data->QuadIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // geometry attributes
    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1); // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Instance buffer (empty at init)
    glGenBuffers(1, &s_Data->InstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_Data->InstanceVBO);
    // allocate some initial capacity (we'll use dynamic streaming)
    size_t initialCapacity = 10000; // number of instances capacity
    glBufferData(GL_ARRAY_BUFFER, initialCapacity * (sizeof(glm::mat4) + sizeof(glm::vec4)), nullptr, GL_DYNAMIC_DRAW);

    // set up instance attributes: mat4 occupies 4 attribute slots (2..5), color -> 6
    // note: attribute locations must match shader binding
    std::size_t vec4Size = sizeof(glm::vec4);
    // attribute 2,3,4,5 = mat4 columns
    for (int i = 0; i < 4; ++i) {
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(RendererData::InstanceData), (void*)(i * vec4Size));
        glVertexAttribDivisor(2 + i, 1);
    }
    // color attribute (after the mat4)
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(RendererData::InstanceData), (void*)(4 * vec4Size));
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);

    s_Data->QuadShader = Shader("res/shaders/inst.vert", "res/shaders/inst.frag");
    s_Data->ProjectionMatrix = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
    s_Data->InstanceBuffer.reserve(1024);
    return true;
}

void Renderer::BeginFrame()
{
    int display_w, display_h;
    SDL_GL_GetDrawableSize(s_Data->window_raw, &display_w, &display_h);
    glViewport(0,0,display_w,display_h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    s_Data->QuadShader.Use(); // <-- bind shader before uniforms
    s_Data->QuadShader.SetMat4("u_ViewProjection", s_Data->ProjectionMatrix);

    auto& lights = LightManager::GetLights();
    int numLights = glm::min((int)lights.size(), 16); // MAX_LIGHTS

    s_Data->QuadShader.SetInt("u_NumLights", numLights);

    for (int i = 0; i < numLights; i++)
    {
        const Light& light = lights[i];
        s_Data->QuadShader.SetVec2("u_LightPos[" + std::to_string(i) + "]", light.Position);
        s_Data->QuadShader.SetVec3("u_LightColor[" + std::to_string(i) + "]", light.Color * light.Intensity);
        s_Data->QuadShader.SetFloat("u_LightRadius[" + std::to_string(i) + "]", light.Radius);
    }

    s_Data->InstanceBuffer.clear();
}

void Renderer::Submit(const glm::mat4& transform, const glm::vec4& color)
{
    RendererData::InstanceData inst;
    inst.Transform = transform;
    inst.Color = color;
    s_Data->InstanceBuffer.push_back(inst);
}

void Renderer::Flush()
{
    if (s_Data->InstanceBuffer.empty()) return;

    glBindVertexArray(s_Data->QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, s_Data->InstanceVBO);

    // upload instance data: either glBufferSubData or glMapBufferRange (preferred for large data)
    size_t dataSize = s_Data->InstanceBuffer.size() * sizeof(RendererData::InstanceData);
    // ensure buffer is large enough (reallocate if not)
    GLint currentSize = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &currentSize);
    if (currentSize < (GLint)dataSize) {
        glBufferData(GL_ARRAY_BUFFER, dataSize, s_Data->InstanceBuffer.data(), GL_DYNAMIC_DRAW);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, s_Data->InstanceBuffer.data());
    }

    // draw instanced
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)s_Data->InstanceBuffer.size());

    glBindVertexArray(0);
}

void Renderer::Shutdown()
{
    glDeleteProgram(s_Data->QuadShader.GetId());
    glDeleteBuffers(1, &s_Data->QuadVBO);
    glDeleteBuffers(1, &s_Data->QuadIBO);
    glDeleteBuffers(1, &s_Data->InstanceVBO);
    glDeleteVertexArrays(1, &s_Data->QuadVAO);
    s_Data.reset();
}

void Renderer::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::SetProjectionMatrix(const glm::mat4& matrix)
{
    s_Data->ProjectionMatrix = matrix;
}


void Renderer::EndFrame()
{
    Flush();
}

void Renderer::PostFrame()
{
    SDL_GL_SwapWindow(s_Data->window_raw);
}

void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
    Submit(transform, color);
}
