#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "animationTimer.h"


class Sprite
{
private:
    AnimationTimer anim_timer;
    SDL_Texture* texture = nullptr;
    
    int textureFrames_columns = 4; //
    int textureFrames_rows = 4; //
    int totalTextureFrames = textureFrames_columns*textureFrames_rows;
    SDL_Rect src_rct;
    SDL_Rect dst_rct = {100,100,200,200};
    int frame_time_jump;
    int initial_frame = 0;

    bool paused = false;
    bool clicked = false;

    std::string file_name;

    int static_frame;

    void load_texture(SDL_Renderer* r, std::string file_name){
        SDL_Surface* tmp_srf = IMG_Load(file_name.c_str());
        if(!tmp_srf){
            printf("Error loading surface: \n, %s\n", SDL_GetError());
        }

        texture = SDL_CreateTextureFromSurface(r, tmp_srf);
        if(!texture){
            printf("Error loading texture: \n, %s\n", SDL_GetError());
        }

        SDL_FreeSurface(tmp_srf);
    }

public:
    int get_total_frames() const { return totalTextureFrames; }
    int get_initial_frame() const { return initial_frame; }
    std::string get_file_name() const { return file_name; }

    Sprite(SDL_Renderer* r, std::string file_name, int init_f, int total_frames, int tfc, int w, int h, int animation_time){
        textureFrames_columns = tfc;
        totalTextureFrames = total_frames;

        dst_rct.w = w;
        dst_rct.h = h;

        src_rct.x = 0;
        src_rct.y = 0;
        src_rct.w = w;
        src_rct.h = h;

        initial_frame = init_f;
        static_frame = 0;

        this->file_name = file_name; 

        anim_timer.set_limit(animation_time);
        frame_time_jump = anim_timer.get_limit()/totalTextureFrames;

        load_texture(r, file_name);

    }

    Sprite(){
        dst_rct.x = 100;
        dst_rct.y = 100;
        
        anim_timer.set_limit(2000);   
        frame_time_jump = anim_timer.get_limit()/totalTextureFrames;

    }

    bool operator==(const Sprite& sprite) const {
        return sprite.get_total_frames()==totalTextureFrames && 
               sprite.get_file_name()==file_name && 
               sprite.get_initial_frame()==initial_frame; 
    }

    ~Sprite(){
        //SDL_DestroyTexture(texture);
        //texture = nullptr;
    }

    void reset_timer(){
        anim_timer.reset();
    }

    
    void update(int dt){
        anim_timer.add_time(dt);
        
        int frame = static_frame;
        if(anim_timer.isRunning()){
            frame = anim_timer.get_time()/frame_time_jump;
            frame += initial_frame;
        }

        int horizontal_ftj = frame % textureFrames_columns;
        int vertical_ftj = frame / textureFrames_columns;
        
        src_rct.x = horizontal_ftj * src_rct.w;
        src_rct.y = vertical_ftj * src_rct.h;
        //printf("%d, %d, %d, %d\n", src_rct.x, src_rct.y, src_rct.w, src_rct.h );
        //printf("%d, %d, %d, %d\n", dst_rct.x, dst_rct.y, dst_rct.w, dst_rct.h );
        //printf("%d\n", anim_timer.get_time());
    }

    int TESTING_get_src_x(){
        int frame = anim_timer.get_time()/frame_time_jump;
        frame += initial_frame;
        int horizontal_ftj = frame % textureFrames_columns;
        
        return horizontal_ftj;
    }

    int TESTING_get_src_y(){
        int frame = anim_timer.get_time()/frame_time_jump;
        frame += initial_frame;
        int vertical_ftj = frame / textureFrames_columns;

        return vertical_ftj;
    }

    void TESTING_set_position(int x, int y){
        dst_rct.x = x;
        dst_rct.y = y;
    }


    SDL_Rect get_rct(){
        return dst_rct;
    }


    void TESTING_handle_input(SDL_Event e){
        if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == SDL_BUTTON_LEFT && !this->clicked){
                if(anim_timer.isRunning()){
                    anim_timer.stop();
                    //anim_timer.reset();
                } else {
                    anim_timer.run();
                }

                printf("horse anim timer stopped\n");
                this->clicked = true;
            }

        }

        if(e.type == SDL_MOUSEBUTTONUP){
            if(e.button.button == SDL_BUTTON_LEFT && this->clicked){
                printf("horse anim click up\n");
                this->clicked = false;

            }
        }
    }

    bool animiation_finished(){
        return anim_timer.finished();
    }

    void draw(SDL_Renderer* r){
        //if(!SDL_RenderCopyEx(r, texture, &src_rct, &dst_rct, 0, 0, SDL_FLIP_NONE)){
        //    printf("%s\n", SDL_GetError());
        //}
        SDL_RenderCopyEx(r, texture, &src_rct, &dst_rct, 0, 0, SDL_FLIP_NONE);
    }

    void stop(){
        anim_timer.stop();
        anim_timer.reset();
    }

    void freeze(){
        anim_timer.stop();
    }

    void run(){
        anim_timer.run();
    }

    void set_size(int w, int h){
        dst_rct.w = w;
        dst_rct.h = h;
    }

    void set_static_sprite(int static_frame){
        this->static_frame = static_frame; 
    }

    void set_timer_limit(int animation_time){
        anim_timer.set_limit(animation_time);
        frame_time_jump = anim_timer.get_limit()/totalTextureFrames;
    }

};


#endif