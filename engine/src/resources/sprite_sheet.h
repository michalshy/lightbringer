#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <glm/glm.hpp>

class SpriteSheet
{
    std::string name;
    std::vector<glm::vec4> m_SpritesRects;
public:
    SpriteSheet(std::string_view path);
    glm::vec4 GetSprite(int pos);
};