#pragma once

#include "camera/camera.h"
#include "game/map/map.h"
#include "game/player/player.h"
#include "scene/scene.h"
#include <SDL_events.h>
#include <memory>

// LIGHTBRINGER!
class Game
{
    friend class Editor;

    std::shared_ptr<Scene> m_Scene = nullptr;
    std::unique_ptr<Map> m_Map = nullptr; // math representation of level
    std::shared_ptr<Camera> m_Camera = nullptr;
    std::unique_ptr<Player> m_Player = nullptr;

    float m_DeltaTime;
    float m_GlobalTime;

    struct DebugOptions
    {
        bool respect_lightmap = true;
    } m_Debug;
public:
    Game();
    bool Init();
    void Update(float delta_time);
    void Draw();
    void PollEvents(SDL_Event& e);
private:
};