#include <SDL2/SDL.h>
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <stdlib.h>
#include <time.h>

const int screen_w = 320;
const int screen_h = 240;

const Uint32 doomfire[] = {0x070707,0x1f0707,0x2f0f07,0x470f07,0x571707,0x671f07,0x771f07, 0x8f2707,0x9f2f07,0xaf3f07, 0xbf4707,0xc74707,0xDF4F07,0xDF5707,0xDF5707,0xD75F07,0xD7670F,0xcf6f0f,0xcf770f,0xcf7f0f,0xCF8717,0xC78717,0xC78F17, 0xC7971F, 0xBF9F1F, 0xBF9F1F, 0xBFA727, 0xBFA727, 0xBFAF2F, 0xB7AF2F, 0xB7B72F, 0xB7B737, 0xCFCF6F, 0xDFDF9F, 0xEFEFC7, 0xFFFFFF};

void spreadFire(Uint32* firePixels, int src){
    Uint32 rand = ((Uint32)random())%3;
    Uint32 dst = src - rand + 1;
    int t = firePixels[src] - (rand & 1);
    if(t < 0) t = 0;
    if(dst < screen_w) dst = screen_w;  
    firePixels[dst - screen_w] = t;
}

void doFire(Uint32* firePixels){
    for(int x = 0 ; x < screen_w; x++) {
        for (int y = 1; y < screen_h; y++) {
             spreadFire(firePixels, y * screen_w + x);
        }
    }
} 

int main(int argc, char* args[])
{
    Game game;

    game.init("DOOM FIRE", screen_w, screen_h);

    SDL_Rect rct = {100,100,50,50};
    SDL_Texture* frame_buffer = nullptr;

    frame_buffer = SDL_CreateTexture(game.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screen_w, screen_h);

    Uint32 pixels[screen_w*screen_h] = {};
    Uint32 pixels_values[screen_w*screen_h] = {};

    for(int i = 0; i < screen_w; i++){
        pixels_values[((screen_h-1)*screen_w)+i] = 35;    
    }

    auto update_scr = [](Uint32* pixels, Uint32* pixels_values){
        for(int i = 0; i < screen_w*screen_h; i++){
          pixels[i] = doomfire[pixels_values[i]];
        }
    };

    update_scr(pixels,pixels_values);

    Uint32* screen_pointer = pixels; 

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
        }
        
        doFire(pixels_values);
        update_scr(pixels,pixels_values);


        SDL_UpdateTexture(frame_buffer, nullptr, pixels, 4*screen_w);
        SDL_RenderCopy(game.renderer, frame_buffer, nullptr, nullptr);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    SDL_DestroyTexture(frame_buffer);
    game.close();
}
