#include "sprite_sheet.h"
#include "SDL_surface.h"
#include "log.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

struct SpriteConfig
{
    int sprite_width;
    int sprite_height;
    int padding;
    int margin;
};

static SpriteConfig load_config(const std::string& filename) 
{
    LOG_DEBUG("Loading config for sprite_sheet: {}", filename);
    std::ifstream file(filename);
    SpriteConfig cfg{};

    std::string line;
    while (std::getline(file, line)) 
    {
        LOG_DEBUG("read line in sprite sheet {}", line);

        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos)
            continue;

        std::string key = line.substr(0, colon_pos);
        std::string value_str = line.substr(colon_pos + 1);

        auto trim = [](std::string& s) {
            size_t start = s.find_first_not_of(" \t");
            size_t end   = s.find_last_not_of(" \t");
            if (start == std::string::npos) { s.clear(); return; }
            s = s.substr(start, end - start + 1);
        };

        trim(key);
        trim(value_str);

        int value = std::stoi(value_str);

        if (key == "sprite_width")
            cfg.sprite_width = value;
        else if (key == "sprite_height")
            cfg.sprite_height = value;
        else if (key == "padding")
            cfg.padding = value;
        else if (key == "margin")
            cfg.margin = value;
    }

    LOG_DEBUG("Established sheet config: margin={}, padding={}, height={}, width={}", 
               cfg.margin, cfg.padding, cfg.sprite_height, cfg.sprite_width);

    return cfg;
}

SpriteSheet::SpriteSheet(std::string_view path, SDL_Surface* surface)
{
    std::filesystem::path p{path};
    std::filesystem::path without_ext = p.parent_path() / p.stem();

    SpriteConfig sc = load_config(without_ext.string() + ".prefab");

    glm::vec4 pos;

    LOG_DEBUG("Established sheet config: {},{},{},{}", sc.margin, sc.padding, sc.sprite_height, sc.sprite_width);

    for(int i = sc.margin; i < surface->h; i+=(sc.sprite_width + sc.padding))
    {
        for(int j = sc.margin; j < surface->w; j+=(sc.sprite_height + sc.padding))
        {
            float x0 = float(j) / surface->w;
            float y0 = float(i) / surface->h;
            float x1 = float(j + sc.sprite_height) / surface->w;
            float y1 = float(i + sc.sprite_width)  / surface->h;

            pos.x = x0;
            pos.y = y0;
            pos.z = x1;
            pos.w = y1;

            m_SpritesRects.push_back(pos);
        }
    }
}

glm::vec4 SpriteSheet::GetSprite(int pos)
{
    if(m_SpritesRects.size() >= pos)
        return m_SpritesRects[pos];
    
    return glm::vec4{1.0f};
}