#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <glm/glm.hpp>
#include <SDL.h>

class SpriteSheet
{
    std::vector<glm::vec4> m_SpritesRects;
public:
    SpriteSheet(std::string_view path, SDL_Surface* surface);
    glm::vec4 GetSprite(int pos);
};