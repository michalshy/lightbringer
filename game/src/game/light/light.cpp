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
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.1f));
    model = glm::scale(model, glm::vec3(TILE_SIZE, TILE_SIZE, 1.0f));
    m_HeartEntity.AddComponent<CoTransform>(model);
    m_HeartEntity.AddComponent<CoLight>(CoLight{glm::vec3{1.0f, 1.0f, 0.6f}, 500.0f, 0.7f, true});
}