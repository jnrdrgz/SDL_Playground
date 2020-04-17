#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

static LogSystem log_system = LogSystem();

float random_fb(float i, float l){
    //6.28319
    //random between i, l
    return 0.0f;
}

struct Particle
{

public:
    Particle(){
        
    }
    
    Particle(int w, int h, int vx, int m, int lf) :  acceleration_x{vx}, mass{m}, life_span{lf}{
        rct.w = w; 
        rct.h = h;
    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); 
        SDL_RenderFillRect(renderer, &rct);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    void update(Uint32 dt){
        if(active){
            life += dt;
            step += dt;

            if(life > 220){
                if(color.a != 0) color.a -= 1;
            }

            if(step == acum){
                acceleration_x += velocity_x;
                acceleration_y += velocity_y; 
                step = 0;
            }

            rct.x += velocity_x;
            rct.y += velocity_y;
            

            if(life>life_span) active = false;
            
        }



    }

    SDL_Rect rct = {200, 200, 25, 25};
    SDL_Color color = {255,0,0,255};
    bool active = true;
    int velocity_x = 0, velocity_y = 0; 
    int acceleration_x = 0, acceleration_y = 0; 
    int life_span = 2000;
    int life = 0;
    int mass = 1;
    int acum = 0;
    int step = 16*6;
};

class Emitter
{
public:
    Emitter(){
        timer.start();
    }


private:
    Timer timer;
};


int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();

    Particle p(20, 20, 1, 1, 2000);

    Uint32 dt = 0;

    SDL_Rect center = {220, 220, 30, 30};

    int center_center_x = center.w/2;
    int center_center_y = center.h/2;
    SDL_Rect satelite  = {220, 190, 15, 15};
    SDL_Rect satelite1 = {220, 220, 15, 15};
    SDL_Rect satelite2 = {190, 190, 15, 15};
    SDL_Rect satelite3 = {190, 220, 15, 15};
    
    float angle = 0.0f;
    float angle1 = 1.0f;
    float angle2 = 3.0f;
    float angle3 = 5.0f;

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        const Uint8* kstate  = SDL_GetKeyboardState(NULL);
        if(kstate[SDL_SCANCODE_RIGHT])
            center.x += 3;
        
        if(kstate[SDL_SCANCODE_LEFT])
            center.x -= 3;

        if(kstate[SDL_SCANCODE_UP])
            center.y -= 3;

        if(kstate[SDL_SCANCODE_DOWN])
            center.y += 3;


        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_RIGHT){
                   // dt = 16;
                }
            }
        }

        p.update(dt);
        p.draw(game.renderer);

        satelite.x = (center.x+(center.w/2))+cos(angle)*center.w;
        satelite.y = (center.y+(center.h/2))+sin(angle)*center.h;
        satelite1.x = (center.x+(center.w/2))+cos(angle1)*center.w;
        satelite1.y = (center.y+(center.h/2))+sin(angle1)*center.h;
        satelite2.x = (center.x+(center.w/2))+cos(angle2)*center.w;
        satelite2.y = (center.y+(center.h/2))+sin(angle2)*center.h;
        satelite3.x = (center.x+(center.w/2))+cos(angle3)*center.w;
        satelite3.y = (center.y+(center.h/2))+sin(angle3)*center.h;

        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255); 
        SDL_RenderFillRect(game.renderer, &center);
        SDL_RenderFillRect(game.renderer, &satelite);
        SDL_RenderFillRect(game.renderer, &satelite1);
        SDL_RenderFillRect(game.renderer, &satelite2);
        SDL_RenderFillRect(game.renderer, &satelite3);
        
        angle += 0.05f;
        angle1 += 0.05f;
        angle2 += 0.05f;
        angle3 += 0.05f;
        if(angle == 6.28319f) angle = 0.0f;
        if(angle1 == 6.28319f) angle1 = 0.0f;
        if(angle2 == 6.28319f) angle2 = 0.0f;
        if(angle3 == 6.28319f) angle3 = 0.0f;

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
