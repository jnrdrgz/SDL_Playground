#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "../particles/vector2.h"

static LogSystem log_system = LogSystem();

float SmoothStart2(float t){return t*t;}
float SmoothStart3(float t){return t*t*t;}
float SmoothStart4(float t){return t*t*t*t;}

Vector2 p2p(Vector2 a, Vector2 b, float t){
    Vector2 v = a + ((b-a)*t);
    printf("v.x: %.2f, v.y: %.2f\n", v.x, v.y);

    return v;
}

struct GameObject
{
    
};

struct World
{
    void step(Uint32 time){
        //rct.x += vx;
        //rct.y += vy;

        float sm_t1 = SmoothStart2(this->time);
        Vector2 v = p2p(Vector2(0.0f,700.0f), 
            Vector2(700.0f,0.0f), sm_t1);

        rct.x = (int)(v.x*-1.0f);
        rct.y = 1400-(int)v.y;

        this->time += (float)time/1000.0f;
        if(this->time > 1.0f){
            this->time = 0.0f;
        }
    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderFillRect(renderer, &rct);
    }

    SDL_Rect rct = {0,700-25,25,25};
    int vx = 3;
    int vy = -vx;
    float time = 0.0f;
};


int main(int argc, char* args[])
{
    Game game;
    game.init("test", 700, 700);
    log_system.init();


    World world;
    Timer timer;
    timer.start();
    
    uint32_t interval = 100;
    
    log_system.add_text("interval", std::to_string(interval), game.renderer);
    log_system.add_text("world time", std::to_string(world.time), game.renderer);
    

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_UP){
                    interval += 25;
                }
                if(game.event.key.keysym.sym == SDLK_DOWN){
                    interval -= 25;
                }
            }
        }

        if(timer.getTicks() > interval){
            world.step(50);
            timer.stop();
            timer.start();
        }
        
        log_system.update_text("interval", std::to_string(interval), game.renderer);
        log_system.update_text("world time", std::to_string(world.time), game.renderer);
    
        
        log_system.draw(game.renderer);
        world.draw(game.renderer);
        
        if(world.rct.x > 700){
            world.rct.x = 0;
            world.rct.y = 700-25;
        }

        if(world.rct.y < 0){
            world.rct.x = 0;
            world.rct.y = 700-25;
        }


        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    
    game.close();
}
