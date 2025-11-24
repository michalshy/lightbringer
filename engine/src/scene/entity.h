#pragma once

#include "scene.h"
#include "entt/entt.hpp"
#include <cstdint>
#include <utility>
#include "engine_api.h"

class Entity
{
public:
    ENGINE_API Entity() = default;
    ENGINE_API Entity(entt::entity handle, Scene* scene);

    template<class T, class... Args>
    T& AddComponent(Args&&... args)
    {
        return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template<class T>
    T& GetComponent()
    {
        return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template<class T>
    T& TryGetComponent()
    {
        return m_Scene->m_Registry.try_get<T>(m_EntityHandle);
    }

    template<class T>
    void RemoveComponent()
    {
        m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    ENGINE_API uint32_t GetHandle()
    {
        return static_cast<uint32_t>(m_EntityHandle);
    }

private:
    entt::entity m_EntityHandle;
    Scene* m_Scene;
};