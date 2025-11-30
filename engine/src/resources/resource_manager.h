#pragma once

#include "engine_api.h"
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include "sprite_sheet.h"

// Resource manager loads all supported resources from set directory
// and availables them to be used by engine and game

// Abstracts loading of resources
class ResourceManager
{
public:
    ENGINE_API static bool Init();
    ENGINE_API static void Shutdown();
    // Registers sprite sheet to be used by other objects, returns texture id sprite sheet was assigned to
    ENGINE_API static uint32_t RegisterSpriteSheet(const std::string& sheet_name);
    ENGINE_API static glm::vec4 GetSprite(std::string sheet_name, int position);
    ENGINE_API static uint32_t GetTexture(std::string tex_name);
private:
    struct ResourceData
    {
        std::unordered_map<std::string, SpriteSheet> sprites;
        std::unordered_map<std::string, uint32_t> textures;
    };
    static std::unique_ptr<ResourceData> s_Data;
};