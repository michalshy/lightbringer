#pragma once

#include "engine_api.h"
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "sprite_sheet.h"

// Resource manager loads all supported resources from set directory
// and availables them to be used by engine and game

// Abstracts loading of resources
class ResourceManager
{
public:
    ENGINE_API static bool RegisterSpriteSheet(std::string sheet_name);
    ENGINE_API static glm::vec2 GetSprite(std::string sheet_name, int position);
private:
    struct ResourceData
    {
        std::vector<SpriteSheet> sprites;
    };
    std::unique_ptr<ResourceData> m_Data;
};