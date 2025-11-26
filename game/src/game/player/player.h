#pragma once

#include "camera/camera.h"
#include "scene/components.h"
#include "scene/scene.h"
#include "scene/entity.h"
#include <glm/fwd.hpp>
#include <memory>

enum class PlayerMovement {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Player
{
    Entity m_PlayerEntity;
    std::shared_ptr<Camera> m_Camera;
public:
    Player(Entity&& entity);
    bool Init(std::shared_ptr<Camera> camera);
    void HandleInput(float delta_time);
    void HandleCamera(float delta_time);
    void ToggleInput(bool state);
    const glm::vec3 GetPosition() { return m_PlayerEntity.GetComponent<CoTransform>().position; }
    const glm::vec3 GetNexPosition() { return glm::vec3{m_PlayerEntity.GetComponent<CoTransform>().position} + m_State.queued_move; }
    const glm::ivec2 GetSize();
    void UpdateInternal(float delta_time);
    void UpdateMove(glm::vec3 mask);
private:
    void MultiplyMove(glm::vec3 mult);
    void ClearMove();
    struct PlayerState
    {
        float movement_speed{ 50.0f };
        bool ignore_movement{ false };
        glm::vec3 queued_move{ 0.0f, 0.0f, 0.0f };
    } m_State;
    
    void ProcessKeyboard(PlayerMovement dir, float delta_time);
public:
    PlayerState GetState() { return m_State; }
};