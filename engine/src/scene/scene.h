#pragma once

#include "entt/entt.hpp"
#include "engine_api.h"

class Entity;

class Scene
{
public:
    ENGINE_API Scene();
    ENGINE_API ~Scene();

    ENGINE_API Entity CreateEntity();
     
    template<typename... ComponentTypes>
    auto View()
    {
        return m_Registry.view<ComponentTypes...>();
    }

    template<typename... ComponentTypes>
    auto Group()
    {
        return m_Registry.group<ComponentTypes...>();
    }

private:
    entt::registry m_Registry;

    friend class Entity;
};