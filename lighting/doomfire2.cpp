#include <SDL2/SDL.h>
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <stdlib.h>
#include <time.h>

const int testW = 500;
const int testH = 500;
int firePixels[36] = {0};

void spreadFire(int src){
    int r = firePixels[src-testW] = firePixels[src]-1;
}

void doFire(){
    for(int x = 0; x < testW; x++){
        for(int y = 0; y < testH; y++){
            spreadFire(y*testW+x);
        }    
    }
}

int main(int argc, char* args[])
{
    Game game;

    game.init("test", testW, testH);
    
    SDL_Rect rct = {100,100,50,50};
    SDL_Texture* test = nullptr;

    test = SDL_CreateTexture(game.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, testW, testH);
    
    Uint32 pixels[testW*testH] = {};

    int rgbs[] = {
        0x07,0x07,0x07,
        0x1F,0x07,0x07,
        0x2F,0x0F,0x07,
        0x47,0x0F,0x07,
        0x57,0x17,0x07,
        0x67,0x1F,0x07,
        0x77,0x1F,0x07,
        0x8F,0x27,0x07,
        0x9F,0x2F,0x07,
        0xAF,0x3F,0x07,
        0xBF,0x47,0x07,
        0xC7,0x47,0x07,
        0xDF,0x4F,0x07,
        0xDF,0x57,0x07,
        0xDF,0x57,0x07,
        0xD7,0x5F,0x07,
        0xD7,0x5F,0x07,
        0xD7,0x67,0x0F,
        0xCF,0x6F,0x0F,
        0xCF,0x77,0x0F,
        0xCF,0x7F,0x0F,
        0xCF,0x87,0x17,
        0xC7,0x87,0x17,
        0xC7,0x8F,0x17,
        0xC7,0x97,0x1F,
        0xBF,0x9F,0x1F,
        0xBF,0x9F,0x1F,
        0xBF,0xA7,0x27,
        0xBF,0xA7,0x27,
        0xBF,0xAF,0x2F,
        0xB7,0xAF,0x2F,
        0xB7,0xB7,0x2F,
        0xB7,0xB7,0x37,
        0xCF,0xCF,0x6F,
        0xDF,0xDF,0x9F,
        0xEF,0xEF,0xC7,
        0xFF,0xFF,0xFF
    };
        
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
        
        SDL_UpdateTexture(test, nullptr, pixels, 4*testW);
        SDL_RenderCopy(game.renderer, test, nullptr, nullptr);
    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    SDL_DestroyTexture(test);
    game.close();
}
