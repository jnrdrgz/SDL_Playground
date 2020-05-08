#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "../particles/vector2.h"

#include <vector>

#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255

static LogSystem log_system = LogSystem();

int random_between(int mn, int mx){
    int n = rand()%(mx-mn)+mn;
    return n;
}

//template<typename T> 
float random_betweenf(float mn, float mx){
    float n = (float)rand() / (float)RAND_MAX;

    return (mn + (mx - mn) * n);
}

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
    }

    Particle(int x, int y, int w, int h, float gy, float wx) : Particle(){
        rct.x = x;
        rct.y = y;
        rct.w = w;
        rct.h = h;

        gravity.y = gy;
        wind.x = wx;
    }
    
    
    void update(){
        time_left = lifetime - life.getTicks();
        if(time_left <= 0){
            active = false;
            life.stop();
        }
        
        if(active){
            color.a = (255*time_left)/lifetime;

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

    SDL_Rect rct;
    Vector2 velocity, speed, gravity, wind;
    float mass;
    Uint32 lifetime;
    int time_left;
    Timer life;
    bool active;

    SDL_Color color;
};

struct Emitter
{
public:
    Emitter(){}

    void push_particle(){
        int d = random_between(5,15);
        particles.push_back(Particle(320,240,d,d,random_betweenf(-0.5f,-0.1f),random_betweenf(-0.1f, 0.1f)));
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
};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 640, 480);
    log_system.init();

    //Particle particle;
    //particle.rct.x = 320;
    //particle.rct.y = 240;
    //particle.rct.w = 20;
    //particle.rct.h = 20;

    //particle.gravity.y = -0.05f;
    //particle.wind.x = -0.03f;

    bool start = false;

    Emitter emitter;

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_RIGHT){
                    start = true;
                }
                emitter.push_particle();
            }
        }

        SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);
        
        //particle.update();
        emitter.update();
        
        //particle.draw(game.renderer);

        emitter.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    
    game.close();
}
