#pragma once

enum class TileType
{
    NONOBSTACLE,
    OBSTACLE,
    SPECIAL,
    ALLY_SPAWNER,
    ENEMY_SPAWNER,
    RESOURCE_SPAWNER,
    LIGHT,
};

struct CoMapTile
{
    int x;
    int y;
};