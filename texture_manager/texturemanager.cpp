#include "texturemanager.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    for(auto &t : textures){
        SDL_DestroyTexture(t.second);
    }
    textures.clear();
}

void TextureManager::set_texture(std::string name, SDL_Texture* text)
{
    textures[name] = text;
}


SDL_Texture* TextureManager::get_texture(std::string name, SDL_Renderer* r)
{
    auto t = textures.find(name);

    if(t == textures.end()){
        SDL_Texture* txt = NULL;
        SDL_Surface* surface = IMG_Load(name.c_str());

        if(surface){
            txt = SDL_CreateTextureFromSurface(r, surface);
            set_texture(name, txt);
        } else {
            return NULL;
        }

        SDL_FreeSurface(surface);

        return txt;
    }

    return t->second;
}
