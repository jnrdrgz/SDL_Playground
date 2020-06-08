#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>

struct TextureCache
{
    TextureCache();

    static SDL_Texture* get_texture(SDL_Renderer* renderer, std::string name);

private:
    static std::unordered_map<std::string, SDL_Texture*> textures;
};

#endif //TEXTURE_CACHE_H