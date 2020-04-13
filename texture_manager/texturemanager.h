#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>

class TextureManager
{
    public:
        TextureManager();
        ~TextureManager();
        void set_texture(std::string name, SDL_Texture* text);
        SDL_Texture* get_texture(std::string name, SDL_Renderer* r);

    private:
        std::unordered_map<std::string, SDL_Texture*> textures;
};

#endif
