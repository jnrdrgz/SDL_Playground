#ifndef PARTICLE_H
#define PARTICLE_H
#include <SDL2/SDL.h>
#include "../timer.h"
#include "../particles/vector2.h"


struct Particle
{
public:
    Particle(){
        life.start();
        active = true;
        gravity = Vector2(0.0f,0.0f);
        speed = Vector2(0.0f,0.0f);
        velocity = Vector2(0.0f,0.0f);
        color = {255,0,0,255};
        lifetime = 1000; //ms
        fade = true;
    }

    Particle(int x, int y, int w, int h, float gy, float wx) : Particle(){
        rct.x = x;
        rct.y = y;
        rct.w = w;
        rct.h = h;

        gravity.y = gy;
        wind.x = wx;
    }
    
    Particle(int x, int y, int w, int h, float gy, float wx, SDL_Color color) : Particle(){
        rct.x = x;
        rct.y = y;
        rct.w = w;
        rct.h = h;

        gravity.y = gy;
        wind.x = wx;

        this->color = color;
    }
    
    
    void update(){
        time_left = lifetime - life.getTicks();
        if(time_left <= 0){
            active = false;
            life.stop();
        }
        
        if(active){
            if(fade) color.a = (255*time_left)/lifetime;
            //color.g = (((255-95)*time_left)/lifetime)+95;

            speed += gravity;
            speed += wind;
            speed += velocity;

            if(rct.x >= 500 || rct.x <= 0){speed.x = 0;}
            if(rct.y >= 400 || rct.y <= 0){speed.y = 0;}

            rct.x += speed.x;
            rct.y += speed.y;
        }
    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rct);
    }

    void set_color(Uint8 r,Uint8 g,Uint8 b){
        color.r = r;
        color.g = g;
        color.b = b;
    }

    SDL_Rect rct;
    Vector2 velocity, speed, gravity, wind;
    float mass;
    Uint32 lifetime;
    int time_left;
    Timer life;
    bool active, fade;

    SDL_Color color;
};

#endif //particle_h