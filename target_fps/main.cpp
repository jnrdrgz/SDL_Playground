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

    Uint32 target_fps = 60;
    Uint32 i_time = SDL_GetTicks();
    Uint32 frames = 0;

    log_system.add_text("frames", std::to_string(frames), game.renderer);
    log_system.add_text("i_time", std::to_string(i_time), game.renderer);   
    log_system.add_text("f_time", std::to_string(0), game.renderer);   

    SDL_Rect rct = {100,100,30,30};

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
                    rct.x += 5;
                }
                if(game.event.key.keysym.sym == SDLK_LEFT){
                    rct.x -= 5;
                }
                if(game.event.key.keysym.sym == SDLK_UP){
                    rct.y -= 5;
                }
                if(game.event.key.keysym.sym == SDLK_DOWN){
                    rct.y += 5;
                }
            }
        }

        SDL_SetRenderDrawColor(game.renderer, 244, 0, 0, 255);
        SDL_RenderDrawRect(game.renderer, &rct);

        log_system.update_text("frames", std::to_string(frames), game.renderer);
        log_system.update_text("i_time", std::to_string(i_time), game.renderer);
        
        log_system.draw(game.renderer);

       
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

        frames++;
        if(frames == target_fps){
            log_system.update_text("f_time", std::to_string(1000-(SDL_GetTicks()-i_time)), game.renderer);
            SDL_Delay(1000-(SDL_GetTicks()-i_time));
            i_time = SDL_GetTicks();
            frames = 0;
        }

       
    }
    
    
    game.close();
}
