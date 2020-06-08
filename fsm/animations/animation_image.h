#ifndef ANIMATION_IMAGE_H
#define ANIMATION_IMAGE_H

#include <SDL2/SDL.h>
#include <string>

struct AnimationImage
{
public:
    //void load_texture(SDL_Renderer* renderer, std::string file_name){
    //    texture = TextureCache::get_texture(renderer,file_name);
    //}
public:
    AnimationImage(){

    }
    AnimationImage(SDL_Renderer* r, std::string file_name, int init_f, int w, int h){
        row = 0;

        src.x = 0;
        src.y = 0;
        src.w = w;
        src.h = h;

        //textureFrames_columns = tfc;
        //totalTextureFrames = total_frames;
        initial_frame = init_f;

        //load_texture(r, file_name);

    }

    AnimationImage(SDL_Renderer* r, std::string file_name, int init_f, int row, int w, int h) : AnimationImage(r, file_name, init_f, w, h){
        this->row = row;
    }

    //test
    AnimationImage(SDL_Renderer* r, SDL_Texture* texture, int init_f, int row, int w, int h){
        src.x = 0;
        src.y = 0;
        src.w = w;
        src.h = h;

        this->row = row;
        initial_frame = init_f;

        this->texture = texture;

    }

    void update(int frame){
        //printf("%d,%d\n", src.w*frame, row);
        src.x = src.w*frame;
        src.y = src.h*(row-1);
    }

    void draw(SDL_Renderer* renderer, SDL_Rect* dst, SDL_RendererFlip flip = SDL_FLIP_NONE){
        SDL_RenderCopyEx(renderer, texture, &src, dst, 0, 0, flip);
    }

public:
    SDL_Rect src;
    SDL_Texture* texture = nullptr;
    int textureFrames_columns;
    int totalTextureFrames;
    int initial_frame, row;
};

#endif //ANIMATION_IMAGE_H