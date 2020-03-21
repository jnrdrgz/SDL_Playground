#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

static LogSystem log_system = LogSystem();

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();
    SDL_RenderSetScale(game.renderer,1,1);

    SDL_Rect r = {0,0,100,100};

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
        }

        SDL_SetRenderDrawColor( game.renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(game.renderer, &r);
    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
