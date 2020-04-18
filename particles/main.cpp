#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>

#include "vector2.h"


static LogSystem log_system = LogSystem();

int random_b(int mn, int mx){
    int n = rand()%(mx-mn)+mn;
    return n;
}

//template<typename T> 
float random_fb(float mn, float mx){
    float n = (float)rand() / (float)RAND_MAX;

    return (mn + (mx - mn) * n);
}

double rand_angle(){
    int r = random()%6283;
    double ra = (double)r/100.00f; 
    return ra;
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


struct Partic
{
public:
    Partic(int ipx, int ipy){
        int wh = random_b(1, 4);
        rct.x = ipx;
        rct.y = ipy;
        rct.w = wh;
        rct.h = wh;
        angle = rand_angle();  
        gravity = Vector2(0.0f,0.13f);
        acc = Vector2(0.0f,random_fb(-2.0f,-1.0f));
        wind = Vector2(random_fb(-0.01f,0.01f),0.0f);
        vel = Vector2(0.0f,0.0f);
    }

    void update(){
        //rct.x += (cos(angle)*5.0f);
        //rct.y -= (sin(angle)*5.0f);
        if(rct.y <= floor){
            acc += gravity;
            acc += wind;
            vel += acc;
            rct.x += vel.x;
            rct.y += vel.y;
        }
        
    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
        SDL_RenderFillRect(renderer, &rct);
    }
private:
    SDL_Rect rct;
    double angle;
    Vector2 vel, acc, gravity, wind;
    float mass;
    int floor = 250;
};

struct Emitter
{
public:
    Emitter(){
        //timer.start();
        x = 250;
        y = 250;
        life_time = 4000;
    }

    void emit(){
        if(life_time >= accum){
            if(timer.getTicks() >= frequency){
                if(partics.size() < max_particles){
                    partics.push_back(Partic{x,y});
                } else {
                    partics.erase(partics.begin());
                }

                accum += timer.getTicks();
                timer.stop();
                timer.start();
            }
        }

    }

    void handle_input(SDL_Event event){
        /*const Uint8* kstate  = SDL_GetKeyboardState(NULL);
        if(kstate[SDL_SCANCODE_G]){
            if(partics.size() < max_particles){
                partics.push_back(Partic{250,250});
            } else {
                //printf("max particle hitted\n");
                partics.erase(partics.begin());
                //max_particles--;
            } 
        }*/

        if(event.type == SDL_MOUSEBUTTONDOWN){
            if(event.button.button == SDL_BUTTON_LEFT){
                printf("rileft\n");
                x = event.button.x;
                y = event.button.y;
                timer.stop();
                timer.start();
                accum = 0;
                printf("%lu\n", partics.size());
            }
        }

    }

    void draw(SDL_Renderer* renderer){
        for(auto& p : partics){
            p.update();
            p.draw(renderer);
            particles++;
        }

    }

    void update(){

    }
private:
    Timer timer;
    Uint32 frequency = 30;

    std::vector<Partic> partics;
    int x, y;
    int max_particles = 1000;
    int particles = 0;
    int life_time = 0, accum = 0;
};


int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();

    Particle p(20, 20, 1, 1, 2000);


    
    Uint32 dt = 0;

    SDL_Rect center = {220, 220, 30, 30};

    int center_center_x = center.x + center.w/2;
    int center_center_y = center.y + center.h/2;
    SDL_Rect satelite  = {center_center_x, center_center_y, 15, 15};
    satelite.x = satelite.x-satelite.w/2;
    satelite.y = satelite.y-satelite.h/2;

    Emitter emitter;

    double angle = 0.0f;

    bool start = false;

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
                if(game.event.key.keysym.sym == SDLK_a){
                   start = true;
                   angle = rand_angle();
                }
                if(game.event.key.keysym.sym == SDLK_s){
                   start = false;
                   satelite.x = center_center_x;
                   satelite.y = center_center_y;
                   satelite.w = 15; 
                   satelite.h = 15;
                   satelite.x = satelite.x-satelite.w/2;
                   satelite.y = satelite.y-satelite.h/2;
                }
                //if(game.event.key.keysym.sym == SDLK_g){
                //    partics.push_back(Partic{250,250});
                //}
            }
            
            emitter.handle_input(game.event);
        }

        p.update(dt);
        //p.draw(game.renderer);

        //satelite.x = (center.x+(center.w/2))+cos(angle)*center.w;
        //satelite.y = (center.y+(center.h/2))+sin(angle)*center.h;
        if(start){
            satelite.x += (cos(angle)*5.0f);
            satelite.y += (sin(angle)*5.0f);
        }

        emitter.emit();
        emitter.draw(game.renderer);

        //SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255); 
        //SDL_RenderFillRect(game.renderer, &center);
        //SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255); 
        //SDL_RenderFillRect(game.renderer, &satelite);
        

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
