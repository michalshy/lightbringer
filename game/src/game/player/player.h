#pragma once

#include "camera/camera.h"
#include "scene/components.h"
#include "scene/scene.h"
#include "scene/entity.h"
#include <glm/fwd.hpp>
#include <memory>

constexpr float NEXT_MOVE_MULT = 10.0f;

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
    const glm::vec3 GetPosition() { return m_PlayerEntity.GetComponent<CoTransform>().transform[3]; }
    const glm::vec3 GetNexPosition() { return glm::vec3{m_PlayerEntity.GetComponent<CoTransform>().transform[3]} + NEXT_MOVE_MULT * m_State.queued_move; }
    const glm::ivec2 GetSize();
    void UpdateInternal(float delta_time);
    void UpdateMove(glm::vec3 mask);
private:
    void MultiplyMove(glm::vec3 mult);
    void ClearMove();
    struct PlayerState
    {
        float movement_speed = 5.0f;
        bool ignore_movement = false;
        glm::vec3 queued_move;
    } m_State;
    
    void ProcessKeyboard(PlayerMovement dir, float delta_time);
    void CheckCollision(glm::vec3 playerPos, glm::ivec2 playerSize, glm::vec3 otherPos, glm::ivec2 otherSize);
public:
    PlayerState GetState() { return m_State; }
};