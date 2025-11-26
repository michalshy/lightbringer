#include "player.h"
#include "camera/camera.h"
#include "log.h"
#include "scene/entity.h"
#include "scene/components.h"
#include "glm/glm.hpp"
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include "game/map/map_consts.h"
#include <SDL.h>
#include <memory>

constexpr glm::ivec2 PLAYER_SIZE{6,12};

Player::Player(Entity&& entity) : m_PlayerEntity(entity) 
{
    const glm::vec3 start_position {(GRID_DIMENSIONS.x/2)*TILE_SIZE, (GRID_DIMENSIONS.y/2 - 1)*TILE_SIZE, 0.1f};
    // lets create components
    m_PlayerEntity.AddComponent<CoTransform>();
    auto& t = m_PlayerEntity.GetComponent<CoTransform>();
    t.position = start_position;
    t.scale = glm::vec3(PLAYER_SIZE.x, PLAYER_SIZE.y, 1.0f);
    m_PlayerEntity.AddComponent<CoSprite>(glm::vec4{0.0f, 0.0f, 1.0f, 1.0f});
    m_PlayerEntity.AddComponent<CoCollider>(glm::ivec2(PLAYER_SIZE.x, PLAYER_SIZE.y), false);
    m_PlayerEntity.AddComponent<CoLight>(CoLight{glm::vec3{1.0f, 1.0f, 0.6f}, 350.0f, 0.7f, true});
}

bool Player::Init(std::shared_ptr<Camera> camera)
{
    if(!camera)
        return false;

    m_Camera = camera;
    return true;
}

void Player::HandleInput(float delta_time)
{
    if(m_State.ignore_movement)
        return;
    
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_W])
        ProcessKeyboard(PlayerMovement::UP, delta_time);
    if (state[SDL_SCANCODE_S])
        ProcessKeyboard(PlayerMovement::DOWN, delta_time);
    if (state[SDL_SCANCODE_A])
        ProcessKeyboard(PlayerMovement::LEFT, delta_time);
    if (state[SDL_SCANCODE_D])
        ProcessKeyboard(PlayerMovement::RIGHT, delta_time);

    HandleCamera(delta_time);
}

void Player::HandleCamera(float /*delta_time*/) // TODO: USE DELTA TIME TO SMOOTH looking IN INPUT DIRECTIOn
{
    glm::vec3 pos = glm::vec3(m_PlayerEntity.GetComponent<CoTransform>().position); // x,y,z
    m_Camera->SetPosition(pos);
}

void Player::ToggleInput(bool state)
{
    m_State.ignore_movement = !state;
}

const glm::ivec2 Player::GetSize() 
{
    return PLAYER_SIZE;
}

void Player::UpdateInternal(float delta_time)
{
    HandleInput(delta_time);
}

void Player::UpdateMove(glm::vec3 mask)
{
    MultiplyMove(mask);
    m_PlayerEntity.GetComponent<CoTransform>().Translate(m_State.queued_move);
    ClearMove();
}

void Player::MultiplyMove(glm::vec3 mask)
{
    m_State.queued_move *= mask;
}

void Player::ClearMove()
{
    m_State.queued_move = glm::vec3(0.f, 0.f, 0.f);
}

void Player::ProcessKeyboard(PlayerMovement dir, float delta_time)
{
    float velocity = m_State.movement_speed * delta_time;

    if (dir == PlayerMovement::UP)    m_State.queued_move += glm::vec3{0.0f, velocity, 0.0f};
    if (dir == PlayerMovement::DOWN)  m_State.queued_move += glm::vec3{0.0f, -velocity, 0.0f};
    if (dir == PlayerMovement::LEFT)  m_State.queued_move += glm::vec3{-velocity, 0.0f, 0.0f};
    if (dir == PlayerMovement::RIGHT) m_State.queued_move += glm::vec3{velocity, 0.0f, 0.0f};
}
