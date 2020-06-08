#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>
#include "animations/animation.h"

static LogSystem log_system = LogSystem();

const int screen_w = 640;
const int screen_h = 480;

int main(int argc, char* args[])
{
    Game game;
    game.init("test", screen_w, screen_h);
    log_system.init();

    SDL_Texture* test_texture = nullptr;
    SDL_Surface* tmp_srf = IMG_Load("sheet.png");
    test_texture = SDL_CreateTextureFromSurface(game.renderer, tmp_srf);
    SDL_FreeSurface(tmp_srf);

    Animation anim(game.renderer, test_texture, 0, 1, 7, 50, 36, 750);
    SDL_Rect rct = {100,100, 50,36};

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_M]){
            printf("Pressing M\n");
        }

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

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        anim.update();
        anim.draw(game.renderer, &rct);
        

        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
