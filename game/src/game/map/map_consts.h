#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

constexpr glm::ivec2 GRID_DIMENSIONS{151, 121};
constexpr glm::vec2 MAP_CENTER{GRID_DIMENSIONS.y/2, GRID_DIMENSIONS.x/2};
constexpr int TILE_SIZE = 15;
constexpr glm::vec2 MAP_CENTER_WORLD{MAP_CENTER.y * TILE_SIZE, MAP_CENTER.x * TILE_SIZE};
constexpr int CASTLE_SIZE = 20;
