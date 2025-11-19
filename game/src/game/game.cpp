#include "game.h"
#include "camera/camera.h"
#include "game/map/map_consts.h"
#include "game/player/player.h"
#include "renderer/light_manager.h"
#include "scene/components.h"
#include "renderer/renderer.h"
#include <SDL_events.h>
#include <glm/ext/matrix_transform.hpp>
#include <memory>

Game::Game()
{
    m_Scene = std::make_shared<Scene>();
    m_Map = std::make_unique<Map>();
    m_Camera = std::make_shared<Camera>(1280.0f, 720.0f);
    m_Player = std::make_unique<Player>(m_Scene->CreateEntity());
}

bool Game::Init()
{
    if(!m_Scene)
        return false;

    if(!m_Map || !m_Map->Init(m_Scene))
        return false;

    if(!m_Player->Init(m_Camera))
        return false;

    return true;
}

void Game::Update(float delta_time)
{
    LightManager::BeginFrame(m_Scene.get());
    m_Map->Update(); // Updates light map atm

    m_DeltaTime = delta_time;
    m_GlobalTime += m_DeltaTime;

    m_Player->UpdateInternal(delta_time);
    m_Player->UpdateMove(m_Map->CheckBounds(*m_Player));
}

void Game::Draw()
{
    Renderer::SetProjectionMatrix(m_Camera->GetViewProjectionMatrix());

    for(auto [ent, sprite, transform] : m_Scene->View<CoSprite, CoTransform>().each())
    {
        glm::vec4 final_color = sprite.color;
        if(m_Debug.respect_lightmap)
            final_color *= m_Map->GetLightMap()[transform.transform[3].y/TILE_SIZE][transform.transform[3].x/TILE_SIZE];
        
        Renderer::DrawQuad(transform.transform, final_color );
    }
}

void Game::PollEvents(SDL_Event& /*e*/)
{
    //tbd
}


