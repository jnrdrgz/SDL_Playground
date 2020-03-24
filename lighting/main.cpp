#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

static LogSystem log_system = LogSystem();

int main(int argc, char* args[])
{
    Game game;

    int testW = 500;
    int testH = 500;
    game.init("test", testW, testH);
    log_system.init();

    SDL_Rect rct = {100,100,50,50};
    SDL_Texture* test = nullptr;

    test = SDL_CreateTexture(game.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, testW, testH);

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

        Uint32 pixels[testW*testH] = {};
        //Uint32 f = SDL_MapRGB();
        Uint32 f = 0xFF0000;
        for (int i = 0; i < testW*testH; ++i)
        {

            pixels[i] = f;

            //f -= 1;
            //if(f==0x00){
            //    f = 0xFF;
            //}
            
            //if(f == 0xFFFFFF){
            //    f = 0x000000;
            //} else if (f == 0x000000){
            //    f = 0xFFFFFF;
            //}
            
            
            //if(f>255){
            //    f=75;
            //}

            
        }

        SDL_UpdateTexture(test, nullptr, pixels, 4*testW);
        SDL_RenderCopy(game.renderer, test, nullptr, nullptr);

        //SDL_SetRenderDrawColor( game.renderer, 0, 0, 0, 255);        
        //SDL_RenderDrawRect(game.renderer, &rct);

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    SDL_DestroyTexture(test);
    game.close();
}
