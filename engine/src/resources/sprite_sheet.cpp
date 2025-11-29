#include "sprite_sheet.h"

SpriteSheet::SpriteSheet(std::string_view path)
{

}

glm::vec4 SpriteSheet::GetSprite(int pos)
{
    if(m_SpritesRects.size() >= pos)
        return m_SpritesRects[pos];
    
    return glm::vec4{1.0f};
}