// lighterer.cpp
#include "engine_pch.h"
#include "light_manager.h"
#include "scene/components.h"
#include "scene/scene.h"

std::unique_ptr<LightManager::LightererData> LightManager::s_Data = nullptr;

bool LightManager::Init()
{
    s_Data = std::make_unique<LightererData>();
    return true;
}

void LightManager::Shutdown()
{
    s_Data.reset();
}

void LightManager::BeginFrame(Scene* scene)
{
    s_Data->Lights.clear();

    for(auto [ent, transform, light] : scene->View<CoTransform, CoLight>().each())
    {
        s_Data->Lights.push_back({
                { transform.position },
                light.color,
                light.radius,
                light.intensity
            });
    }
}

const std::vector<Light>& LightManager::GetLights()
{
    return s_Data->Lights;
}
