#include "engine_pch.h"
#include "log.h"
#include "resources/sprite_sheet.h"

#include "resource_manager.h"
#include <cstdint>
#include <filesystem>
#include <memory>
#include <SDL_image.h>
#include <string_view>
#include "renderer/renderer.h"

static const std::string RESOURCES_DIRECTORY = "res";
static const std::string SPRITESHEETS_DIRECTORY = "sprites/sheets";
static const std::string SHADER_DIRECTORY = "shaders";

std::unique_ptr<ResourceManager::ResourceData> ResourceManager::s_Data = nullptr;

bool ResourceManager::Init()
{
    s_Data = std::make_unique<ResourceData>();
    LOG_DEBUG("INITIALIZED RESOURCE MANAGER");
    return s_Data != nullptr;
}

void ResourceManager::Shutdown()
{
    s_Data.reset();
}

uint32_t ResourceManager::RegisterSpriteSheet(const std::string& sheet_name)
{
    // load using SDL_image
    SDL_Surface* surface = IMG_Load(sheet_name.c_str());
    if (!surface)
    {
        return 0;
    }

    GLenum format;
    if (surface->format->BytesPerPixel == 4)
    {
        format = (surface->format->Rmask == 0x000000ff) 
                 ? GL_RGBA 
                 : GL_BGRA;
    }
    else if (surface->format->BytesPerPixel == 3)
    {
        format = (surface->format->Rmask == 0x000000ff) 
                 ? GL_RGB 
                 : GL_BGR;
    }
    else
    {
        SDL_FreeSurface(surface);
        return 0;
    }

    s_Data->sprites.emplace(sheet_name, SpriteSheet(sheet_name, surface));
    LOG_DEBUG("Emplaced sprite sheet {}", sheet_name.data());

    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 surface->w, surface->h, 0,
                 format, GL_UNSIGNED_BYTE, surface->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(surface);

    s_Data->textures.emplace(sheet_name, tex_id);
    LOG_DEBUG("Emplaced tex {}", tex_id);

    return tex_id;
}

glm::vec4 ResourceManager::GetSprite(std::string sheet_name, int position)
{
    return s_Data->sprites.at(sheet_name).GetSprite(position);
}

uint32_t ResourceManager::GetTexture(std::string tex_name)
{
    return s_Data->textures[tex_name];
}

