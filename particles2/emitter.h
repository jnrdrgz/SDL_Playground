#ifndef EMITTER_H
#define EMITTER_H
#include <SDL2/SDL.h>
#include "particle.h"
#include "my_random.h"

struct Emitter
{
public:
    Emitter(){}
    Emitter(Vector2 position){
        this->position.x = position.x;
        this->position.y = position.y;
    }

    void push_particle(int x, int y){
        int d = random_between(5,15);
        Particle p(x,y,d,d,random_betweenf(-0.5f,-0.1f),random_betweenf(-0.1f, 0.04f));
        p.set_color(255,255,0);
        particles.push_back(p);
    }

    void push_particle(){
        int d = random_between(5,15);
        Particle p(position.x,position.y,d,d,random_betweenf(-0.5f,-0.1f),random_betweenf(-0.1f, 0.04f));
        p.set_color(255,255,0);
        particles.push_back(p);
    }

    void push_particle(int d, SDL_Color color, bool color_mode = false){
        Particle p(position.x,position.y,d,d,particles_g,particles_w);
        p.set_color(color.r,color.g,color.b);
        
        particles.push_back(p);
    }

    void update(){
        for(auto& particle : particles){
            particle.update();
        }
    }

    void draw(SDL_Renderer* renderer){
        for(auto particle : particles){
            particle.draw(renderer);
        }
    }
    
    Vector2 position;
    Vector2 velocity, speed, gravity;
    float mass;
    Uint32 lifetime;
    SDL_Color color;
    std::vector<Particle> particles;

    //
    int particles_dimension; //only squares for now
    float particles_g; //gravity
    float particles_w; //wind
    SDL_Color particles_color; //color
};

#endif //emitter_h