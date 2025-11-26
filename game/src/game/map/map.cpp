#include "map.h"
#include "game/game_components.h"
#include "game/light/light.h"
#include "game/player/player.h"
#include "map_consts.h"
#include "log.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <memory>
#include <random>
#include "renderer/light_manager.h"
#include "renderer/renderer.h"
#include "scene/components.h"
#include "scene/entity.h"
#include "scene/scene.h"

constexpr float WALL_CHANCE = 0.45f;
constexpr int ITERATIONS = 5;
constexpr int SURVIVAL_THRESHOLD = 2;
constexpr int BIRTH_THRESHOLD = 3;
constexpr glm::ivec3 STARTING_POS{0,0,0};

constexpr float ALLY_CHANCE = 0.005f;
constexpr float RESOURCE_CHANCE = 0.009f;


Map::Map()
{
    for (int y = 0; y < GRID_DIMENSIONS.y; y++)
    {
        map_grid.push_back({});
        for (int x = 0; x < GRID_DIMENSIONS.x; x++)
        {
            map_grid[y].push_back(Tile{});
        }
    }
    light_map.assign(map_grid.size(), std::vector<float>(map_grid[0].size(), 0.0f));
}

bool Map::Init(std::shared_ptr<Scene> scene)
{
    if(!scene)
        return false;

    m_Scene = scene;
    if(!InitMap())
        return false;

    LOG_DEBUG("Initialized Level");
    return true;
}

glm::vec3 Map::CheckBounds(Player& player)
{
    glm::vec3 next = player.GetNexPosition();
    glm::ivec2 size = player.GetSize();
    glm::vec3 mask{1.0f, 1.0f, 1.0f};

    // --- X AXIS ---
    int minX = static_cast<int>((next.x - size.x/2.0f) / TILE_SIZE);
    int maxX = static_cast<int>((next.x + size.x/2.0f) / TILE_SIZE);
    int minY = static_cast<int>((player.GetPosition().y - size.y/2.0f) / TILE_SIZE);
    int maxY = static_cast<int>((player.GetPosition().y + size.y/2.0f) / TILE_SIZE);

    for (int y = minY; y <= maxY; ++y)
    {
        if (map_grid[y][maxX].type == TileType::OBSTACLE) { mask.x = 0.0f; break; }
        if (map_grid[y][minX].type == TileType::OBSTACLE) { mask.x = 0.0f; break; }
    }

    // --- Y AXIS ---
    minX = static_cast<int>((player.GetPosition().x - size.x/2.0f) / TILE_SIZE);
    maxX = static_cast<int>((player.GetPosition().x + size.x/2.0f) / TILE_SIZE);
    minY = static_cast<int>((next.y - size.y/2.0) / TILE_SIZE);
    maxY = static_cast<int>((next.y + size.y/2.0) / TILE_SIZE);

    for (int x = minX; x <= maxX; ++x)
    {
        if (map_grid[maxY][x].type == TileType::OBSTACLE) { mask.y = 0.0f; break; }
        if (map_grid[minY][x].type == TileType::OBSTACLE) { mask.y = 0.0f; break; }
    }

    return mask;
}

int Map::GetSeed()
{
    return seed;
}

bool Map::InitMap()
{
    seed = std::random_device{}();
    //seed = 4164015880;
    LOG_DEBUG("Creating level for seed: {}", seed);
    rng = std::mt19937(seed);
    
    Cycle();

    InitLight();

    DefineEntities();

    return true;
}

void Map::InitLight()
{
    m_MapHeart = std::make_unique<HeartLight>(m_Scene);
    m_MapHeart->Init(MAP_CENTER_WORLD);
}

bool Map::Birth(int y, int x)
{
    int neighbours = 0;
    if(map_grid[y-1][x].type == TileType::OBSTACLE)
        ++neighbours;
    if(map_grid[y][x-1].type == TileType::OBSTACLE)
        ++neighbours;
    if(map_grid[y][x+1].type == TileType::OBSTACLE)
        ++neighbours;
    if(map_grid[y+1][x].type == TileType::OBSTACLE)
        ++neighbours;

    return neighbours >= BIRTH_THRESHOLD;
}

bool Map::Survival(int y, int x)
{
    int neighbours = 0;
    if(map_grid[y-1][x].type == TileType::OBSTACLE)
        ++neighbours;
    if(map_grid[y][x-1].type == TileType::OBSTACLE)
        ++neighbours;
    if(map_grid[y][x+1].type == TileType::OBSTACLE)
        ++neighbours;
    if(map_grid[y+1][x].type == TileType::OBSTACLE)
        ++neighbours;

    return neighbours >= SURVIVAL_THRESHOLD;
}

void Map::RunCycle()
{
    Cycle();

    auto view = m_Scene->View<CoSprite, CoMapTile>();

    for (auto [entity, sprite, tile_cords] : view.each())
    {
        glm::vec4 color = ComputeColors(tile_cords.x, tile_cords.y);
        sprite.color = color;
    }
}

void Map::Update()
{
    UpdateLightMaps();
}

void Map::Draw()
{
    for (const auto& row : map_grid)
    {
        for (const auto& col : row)
        {
            glm::vec4 final_color = col.color;
            final_color *= light_map[(size_t)col.pos.y / TILE_SIZE][(size_t)col.pos.x / TILE_SIZE];
            Renderer::DrawQuad(col.pos, col.scale, final_color);
        }
    }
}

void Map::DefineEntities()
{
    glm::vec3 start_position = STARTING_POS;
    for (int i = 0; i < (int)map_grid.size(); i++)
    {
        for (int j = 0; j < (int)map_grid[i].size(); j++)
        {
            map_grid[i][j].pos = start_position + glm::vec3(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f, 0.0f);
            map_grid[i][j].scale = { TILE_SIZE, TILE_SIZE, 1.0f };
            map_grid[i][j].color = ComputeColors(i, j);

            start_position += glm::vec3(TILE_SIZE, 0, 0);
        }
        start_position += glm::vec3(-start_position.x, TILE_SIZE, 0);
    }
}

void Map::UpdateLightMaps()
{
    // Reset light map
    for (auto& row : light_map)
        std::fill(row.begin(), row.end(), 0.3f); // ambient baseline

    for (const auto& light : LightManager::GetLights())
        UpdateLightMap(light.Position, light.Radius, light.Intensity);
}

void Map::UpdateLightMap(
    const glm::vec2& light_pos_world,
    float radius,
    float intensity)
{
    const int height = (int)map_grid.size();
    const int width = (int)map_grid[0].size();

    int lx = (int)(light_pos_world.x / TILE_SIZE);
    int ly = (int)(light_pos_world.y / TILE_SIZE);
    int r_tiles = (int)(radius / TILE_SIZE);

    for (int y = ly - r_tiles; y <= ly + r_tiles; ++y)
    {
        for (int x = lx - r_tiles; x <= lx + r_tiles; ++x)
        {
            if (x < 0 || y < 0 || y >= height || x >= width)
                continue;

            float dx = (float)(x - lx);
            float dy = (float)(y - ly);
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist > r_tiles)
                continue;

            // Raycast from light to tile center
            bool blocked = false;
            int x0 = lx, y0 = ly;
            int x1 = x, y1 = y;
            int dx_ = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
            int dy_ = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
            int err = dx_ + dy_;

            while (true)
            {
                if (y0 >= 0 && x0 >= 0 && y0 < height && x0 < width)
                {
                    if (map_grid[y0][x0].type == TileType::OBSTACLE)
                    {
                        blocked = true;
                        break;
                    }
                }

                if (x0 == x1 && y0 == y1)
                    break;

                int e2 = 2 * err;
                if (e2 >= dy_) { err += dy_; x0 += sx; }
                if (e2 <= dx_) { err += dx_; y0 += sy; }
            }

            if (!blocked)
            {
                float brightness = intensity * (1.0f - (dist / r_tiles));
                light_map[y][x] = std::max(light_map[y][x], brightness);
            }
        }
    }
}

void Map::Cycle()
{
    std::uniform_real_distribution<> dist(0.0f,1.0f);
    for(int y = 1; y < GRID_DIMENSIONS.y - 1; y++)
    {
        for(int x = 1; x < GRID_DIMENSIONS.x - 1; x++)
        {
            map_grid[y][x].type = dist(rng) > WALL_CHANCE ? TileType::NONOBSTACLE : TileType::OBSTACLE;
        }
    } 
    for(int i = 0; i < CASTLE_SIZE; i++)
    {
        for(int j = 0; j < CASTLE_SIZE; j++)
        {
            map_grid[GRID_DIMENSIONS.y/2 - CASTLE_SIZE/2 + i][GRID_DIMENSIONS.x/2 - CASTLE_SIZE/2 + j].type = TileType::SPECIAL; // means special
        }   
    }

    for(int i = 0; i < ITERATIONS; i++)
    {
        for(int y = 1; y < GRID_DIMENSIONS.y - 1; y++)
        {
            for(int x = 1; x < GRID_DIMENSIONS.x - 1; x++)
            {
                if(map_grid[y][x].type == TileType::SPECIAL) continue;
                if(!Survival(y,x))
                {
                    map_grid[y][x].type = TileType::NONOBSTACLE;
                }
                else if(Birth(y, x))
                {
                    map_grid[y][x].type = TileType::OBSTACLE;
                }
            }
        } 
    }

    for(int i = 0; i < ITERATIONS; i++)
    {
        auto next_grid = map_grid; // copy current state

        for(int y = 1; y < GRID_DIMENSIONS.y - 1; y++)
        {
            for(int x = 1; x < GRID_DIMENSIONS.x - 1; x++)
            {
                if(map_grid[y][x].type == TileType::SPECIAL) continue;

                if(!Survival(y,x))
                    next_grid[y][x].type = TileType::NONOBSTACLE;
                else if(Birth(y, x))
                    next_grid[y][x].type = TileType::OBSTACLE;
            }
        }

        map_grid = next_grid; // replace after whole step finished
    }
    ComputeColors();
    ComputeAllies();
    ComputeEnemies();
    ComputeResources();
    ComputeLight();
}

void Map::ComputeColors()
{
    for (int y = 1; y < GRID_DIMENSIONS.y - 1; y++)
    {
        for (int x = 1; x < GRID_DIMENSIONS.x - 1; x++)
        {
            map_grid[y][x].color = ComputeColors(y,x);
        }
    }
}

void Map::ComputeEnemies()
{
    // The closer the middle of the map, the chance to spawn enemy should be less possible

}

void Map::ComputeAllies()
{
    // They should be evenly distributed over map avoiding middle
    std::uniform_real_distribution<> dist(0.0f,1.0f);
    // They should be evenly distributed over map
    for(auto& row : map_grid)
    {
        for(auto& column : row)
        {
            if(column.type == TileType::NONOBSTACLE)
            {
                if(dist(rng) < ALLY_CHANCE)
                    column.type = TileType::ALLY_SPAWNER;
            }
        }
    }
}

void Map::ComputeResources()
{
    std::uniform_real_distribution<> dist(0.0f,1.0f);
    // They should be evenly distributed over map
    for(auto& row : map_grid)
    {
        for(auto& column : row)
        {
            if(column.type == TileType::NONOBSTACLE || column.type == TileType::SPECIAL)
            {
                if(dist(rng) < RESOURCE_CHANCE)
                    column.type = TileType::RESOURCE_SPAWNER;
            }
        }
    }
}

void Map::ComputeLight()
{
    map_grid[(size_t)MAP_CENTER.x][(size_t)MAP_CENTER.y].type = TileType::LIGHT;
}

glm::vec4 Map::ComputeColors(int i, int j)
{
    glm::vec4 color = glm::vec4{1.0f};

    switch (map_grid[i][j].type) {
        case TileType::OBSTACLE:
            color = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
            break;

        case TileType::NONOBSTACLE:
            color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
            break;

        case TileType::SPECIAL:
            color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
            break;

        case TileType::ALLY_SPAWNER:
            color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f};
            break;

        case TileType::ENEMY_SPAWNER:
            color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
            break;

        case TileType::RESOURCE_SPAWNER:
            color = glm::vec4{1.0f, 0.0f, 1.0f, 1.0f};
            break;

        case TileType::LIGHT:
            color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
            break;

        default:
            LOG_ERROR("We do not handle this TileType!");
            break;
    } 
    return color;
}
