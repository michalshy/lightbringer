#pragma once

#include "entt/entt.hpp"

class Entity;

class Scene
{
public:
    Scene();
    ~Scene();

    Entity CreateEntity();
     
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