#include "scene.h"
#include "entity.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

Entity Scene::CreateEntity()
{
    return {m_Registry.create(), this};
}