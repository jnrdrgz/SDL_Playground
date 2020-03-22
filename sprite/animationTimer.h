#ifndef ANIMATIONTIMER_H
#define ANIMATIONTIMER_H

#include <SDL2/SDL.h>

class AnimationTimer
{
private:
    int time;
    int animation_time;
    bool clicked;
    bool stopped;

public:
    AnimationTimer(){
        time = 0;
        animation_time = 1000;
        clicked = false;
        stopped = false;
    }

    AnimationTimer(int animation_time){
        time = 0;
        this->animation_time = animation_time;
        clicked = false;
        stopped = false;
    }

    void add_time(Uint32 dt){
        if(!stopped){
            time += dt;
            if(time > animation_time){
                reset();
            }
        }
    }

    int get_time(){
        return time;
    }

    int get_limit(){
        return animation_time;
    }

    void set_limit(int limit){
        animation_time = limit;
    }

    void reset(){
        time = 0;
    }

    void stop(){
        stopped = true;
    }

    void run(){
        stopped = false;
    }

    bool isRunning(){
        return !stopped;
    }

    void handle_input(SDL_Event e){
        if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == SDL_BUTTON_LEFT && !this->clicked){
                stopped = !stopped;
                printf("click down\n");
                clicked = true;    
            }
        }

        if(e.type == SDL_MOUSEBUTTONUP){
            if(e.button.button == SDL_BUTTON_LEFT && this->clicked){
                printf("click up\n");
                clicked = false;    
            }
        }
    }
};

#endif