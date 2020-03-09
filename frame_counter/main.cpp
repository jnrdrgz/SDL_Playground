#include <SDL2/SDL.h>
#include "game.h"
//#include "text.h"
#include "../log_system/log_system.h"

static LogSystem log_system = LogSystem();




int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();

    Uint32 t = SDL_GetTicks();
    printf("%d\n", t);

    
    
    log_system.add_text("fps", std::to_string(t), game.renderer);

    while(game.running){
        t = SDL_GetTicks();
        //printf("%d\n", t);

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

        
        log_system.update_text("fps", std::to_string(t), game.renderer);
        log_system.draw(game.renderer);

        
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    game.close();
}
