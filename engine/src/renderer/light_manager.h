#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "scene/scene.h"
#include "engine_api.h"

struct Light
{
    glm::vec2 Position;
    glm::vec3 Color;
    float Radius;
    float Intensity;
};

class LightManager
{
public:
    ENGINE_API static bool Init();
    ENGINE_API static void Shutdown();

    ENGINE_API static void BeginFrame(Scene* scene);

    // Accessor for Renderer
    ENGINE_API static const std::vector<Light>& GetLights();

private:
    struct LightererData
    {
        std::vector<Light> Lights;
    };
    static std::unique_ptr<LightererData> s_Data;
};
