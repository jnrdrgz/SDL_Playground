#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "../particles/vector2.h"

#define SCREENW 640
#define SCREENH 480

static LogSystem log_system = LogSystem();


enum class GunType
{
    RIFLE,
    SHUTGUN,
    REVOLVER,
};

struct Player
{
public:
    Player(int x, int y, int w, int h){
        rct.x = x;
        rct.y = y;
        rct.w = w;
        rct.h = h;


    }

    void draw(SDL_Renderer* renderer){
        SDL_RenderFillRect(renderer, &rct);
    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_RIGHT){
            
            }
        }
    }

public:
    SDL_Rect rct;
    int chamber;
};



int main(int argc, char* args[])
{
    Game game;
    game.init("test", SCREENW, SCREENH);
    log_system.init();

    int chamber_width = 50;
    int chamber_height = 50;
    SDL_Rect chamber2 = {SCREENW/2,SCREENH/2-chamber_height/2,chamber_width,chamber_height};
    SDL_Rect chamber1 = {chamber2.x-chamber_width,chamber2.y,chamber_width,chamber_height};

    Player player(chamber1.x,chamber1.y,20,20);

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
                
                }
            }
        }    
        SDL_SetRenderDrawColor( game.renderer, 0,0,0, 255);
        SDL_RenderDrawRect(game.renderer, &chamber1);
        SDL_RenderDrawRect(game.renderer, &chamber2);
        player.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
