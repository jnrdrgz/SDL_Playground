#include <SDL2/SDL.h>
//#include "text.h"
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

static LogSystem log_system = LogSystem();

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();
    Timer timer;
    timer.start();
    

    Uint32 t = SDL_GetTicks();
    printf("%d\n", t);

    int countedFrames = 0;
    log_system.add_text("ms", std::to_string(t), game.renderer);
    log_system.add_text("seconds", std::to_string(t), game.renderer);
    log_system.add_text("frames", std::to_string(countedFrames), game.renderer);
    log_system.add_text("FPS", "", game.renderer);

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
        
        float avgFPS = (float)countedFrames / (float)timer.getTicks_in_seconds();
        if( avgFPS > 2000000 )
        {
            avgFPS = 0;
        }

        log_system.update_text("ms", std::to_string(t), game.renderer);
        log_system.update_text("seconds", std::to_string(timer.getTicks_in_seconds()), game.renderer);
        log_system.update_text("frames", std::to_string(countedFrames), game.renderer);
        log_system.update_text("FPS", std::to_string(avgFPS), game.renderer);
        log_system.draw(game.renderer);

        
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
        countedFrames++;
    }
    
    game.close();
}
