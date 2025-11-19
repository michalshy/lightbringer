#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "scene/scene.h"

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
    static bool Init();
    static void Shutdown();

    static void BeginFrame(Scene* scene);

    // Accessor for Renderer
    static const std::vector<Light>& GetLights();

private:
    struct LightererData
    {
        std::vector<Light> Lights;
    };
    static std::unique_ptr<LightererData> s_Data;
};
