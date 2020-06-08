#include "texture_cache.h"

std::unordered_map<std::string, SDL_Texture*> TextureCache::TextureCache::textures;

TextureCache::TextureCache(){}

SDL_Texture* TextureCache::get_texture(SDL_Renderer* renderer, std::string name){
    auto t = textures.find(name);

    if(t == textures.end()){        
        SDL_Surface* tmp_srf = IMG_Load(name.c_str());
        if(!tmp_srf){
            printf("Error loading surface: \n, %s\n", SDL_GetError());
            throw "error loading surface";
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmp_srf);
        if(!texture){
            printf("Error loading texture: \n, %s\n", SDL_GetError());
            throw "Error loading texture";
        }

        SDL_FreeSurface(tmp_srf);

        textures[name] = texture;
        return texture;
        }

    return textures[name];
}


