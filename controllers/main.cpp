#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

#include <iostream>

static LogSystem log_system = LogSystem();

class BendingBarController
{
private:
    SDL_Rect bar;
    SDL_Rect bender;
    int bend_vel = 0;

    int distance(int target, int guess){
        if(target <= guess){
            return guess-target;
        } else {
            return target-guess;
        }
    }

    int get_value(int target, int guess, int maxv){
        printf("%d,%d,%d\n", target, guess, maxv);
       return distance(target, guess)/(target/maxv);
    }

    int bar_stop_n(){
        return (bar.x+bar.w) - bar.x;
    }

    int bender_x_within_bar(){
        return (bender.x+(bender.w/2)) - bar.x;
    }

    template <typename T> 
    T t_distance(T target, T guess){ 
        return (target <= guess) ? guess-target : target-guess;
    }

    template <typename T> 
    T t_get_value(T target, T guess, T maxv){
        return t_distance(target, guess)/(target/maxv);
    } 

public:

    BendingBarController(int x, int y, int w, int h){
        bar.x = x;
        bar.y = y;
        bar.w = w;
        bar.h = h;

        bender.x = x;
        bender.y = y;
        bender.w = w/10;
        bender.h = h;
        
    }

    void set_vel(int v){
        bend_vel = v;
    }

    void draw(SDL_Renderer* renderer){

        SDL_SetRenderDrawColor( renderer, 255,0,0, 255);
        SDL_RenderFillRect(renderer, &bar);


        SDL_SetRenderDrawColor( renderer, 0,0,0, 255);
        SDL_RenderFillRect(renderer, &bender);

    }

    void update(){
        /*if(bender.x + bender.w > bar.x + bar.w){
            bend_vel = -2;
        } else if(bender.x < bar.x){
            bend_vel = 2;
        }*/

        if(bender.x + bender.w > bar.x + bar.w){
            bend_vel = -1;
        } else if(bender.x < bar.x){
            bend_vel = 1;
        }

        bender.x += bend_vel;
    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_i){
                printf("change\n");
                bend_vel = 2;
            }
            if(event.key.keysym.sym == SDLK_SPACE){
                printf("%d\n", get_value(50, bender_x_within_bar(), 3));
                printf("%.2f\n", t_get_value<float>(50.0, (float)bender_x_within_bar(), 3));
            }
        }  
    }

};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();

    BendingBarController bender_controller(350, 20, 100, 20);

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                
            }
            bender_controller.handle_input(game.event);
        }

        bender_controller.update();
        bender_controller.draw(game.renderer);

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
