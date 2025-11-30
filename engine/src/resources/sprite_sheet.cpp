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

// TODO: REFACTOR
SpriteSheet::SpriteSheet(std::string_view path, SDL_Surface* surface)
{
    std::filesystem::path p{path};
    std::filesystem::path without_ext = p.parent_path() / p.stem();

    SpriteConfig sc = load_config(without_ext.string() + ".prefab");
    LOG_DEBUG("Sheet config: margin={}, padding={}, height={}, width={}",
              sc.margin, sc.padding, sc.sprite_height, sc.sprite_width);

    const int texW = surface->w;
    const int texH = surface->h;

    // Clear any existing
    m_SpritesRects.clear();
    m_SpritesRects.reserve( (texW / sc.sprite_width) * (texH / sc.sprite_height) );

    int count = 0;

    int px1 = sc.sprite_width;
    int py1 = sc.sprite_height;

    for (int y = sc.margin; y + sc.sprite_height <= texH; y += sc.sprite_height + sc.padding)
    {
        for (int x = sc.margin; x + sc.sprite_width <= texW; x += sc.sprite_width + sc.padding)
        {
            int px0 = x;
            int py0 = y;
            
            float u0 = float(px0) / float(texW);
            float v0 = float(py0) / float(texH);
            float u1 = float(px1) / float(texW);
            float v1 = float(py1) / float(texH);

            m_SpritesRects.emplace_back(u0, v0, u1, v1);

            if (++count <= 6)
            {
                LOG_DEBUG("rect[{}] px:({},{})-({},{}) norm:({:.6f},{:.6f})-({:.6f},{:.6f})",
                          count-1, px0, py0, px1, py1, u0, v0, u1, v1);
            }
        }
    }

    LOG_DEBUG("Total sprites rects: {}", (int)m_SpritesRects.size());
}

glm::vec4 SpriteSheet::GetSprite(int pos)
{
    if(m_SpritesRects.size() >= pos)
        return m_SpritesRects[pos];
    
    return glm::vec4{1.0f};
}