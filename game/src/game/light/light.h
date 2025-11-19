#pragma once

#include "scene/scene.h"
#include <glm/fwd.hpp>
#include <memory>
#include "scene/entity.h"
class HeartLight
{
    std::shared_ptr<Scene> m_Scene;
    Entity m_HeartEntity;
public:
    HeartLight(std::shared_ptr<Scene> scene) : m_Scene(scene) {}
    void Init(glm::ivec2 pos);
};