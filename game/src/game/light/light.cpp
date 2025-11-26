#include "light.h"
#include "game/map/map_consts.h"
#include "renderer/light_manager.h"
#include "scene/components.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

constexpr glm::vec3 HEART_COLOR{1.0, 1.0, 1.0};
constexpr float BASIC_RADIUS = 200.0f; 

void HeartLight::Init(glm::ivec2 pos)
{
    m_HeartEntity = m_Scene->CreateEntity();
    m_HeartEntity.AddComponent<CoTransform>();
    m_HeartEntity.GetComponent<CoTransform>().position = glm::vec3(GRID_DIMENSIONS.x * TILE_SIZE / 2, GRID_DIMENSIONS.y * TILE_SIZE / 2, 0.1f);
    m_HeartEntity.AddComponent<CoLight>(CoLight{glm::vec3{1.0f, 1.0f, 0.6f}, 500.0f, 0.7f, true});
}