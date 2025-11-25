#pragma once

// Level class of Lightbringer Game
// Simple grid defined by cellular automata algorithm
#include "game/light/light.h"
#include "scene/scene.h"
#include <memory>
#include <random>
#include <vector>
#include "game/game_components.h"
#include "game/player/player.h"
#include "glm/glm.hpp"

struct Tile
{
    glm::vec3 pos{ 0.0f };
    glm::vec3 scale{ 0.0f };
    glm::vec3 rot{ 0.0f };
    glm::vec4 color{ 0.0f }; // later replaced by texture
    TileType type{ TileType::OBSTACLE };
};

class Map
{
    // Leaving int here since 
    // it might be used as different types of obstacles
    int seed{ 0 };
    std::vector<std::vector<Tile>> map_grid;
    std::vector<std::vector<float>> light_map;
    std::shared_ptr<Scene> m_Scene;
    std::mt19937 rng;

    std::unique_ptr<HeartLight> m_MapHeart; // This is key element of the game, the light which shall be protected :)
public:
    Map();
    bool Init(std::shared_ptr<Scene> scene);
    glm::vec3 CheckBounds(Player& player);
    int GetSeed();
    void RunCycle();
    void Update();
    void Draw();
    const std::vector<std::vector<float>>& GetLightMap() { return light_map; }
private:
    void DefineEntities();
    void UpdateLightMaps();
    void UpdateLightMap(const glm::vec2& light_pos_world, float radius, float intensity = 1.0f); 
    void Cycle();
    bool InitMap();
    void InitLight();
    bool Birth(int y, int x);
    bool Survival(int y, int x);
    void ComputeEnemies();
    void ComputeAllies();
    void ComputeResources();
    void ComputeLight();
    glm::vec4 ComputeColors(int i, int j);
};