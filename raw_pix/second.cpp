#include <SDL2/SDL.h>
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <stdlib.h>
#include <time.h>

const int screen_w = 640;
const int screen_h = 360;

const Uint32 doomfire[] = {0x070707,0x1f0707,0x2f0f07,0x470f07,0x571707,0x671f07,0x771f07, 0x8f2707,0x9f2f07,0xaf3f07, 0xbf4707,0xc74707,0xDF4F07,0xDF5707,0xDF5707,0xD75F07,0xD7670F,0xcf6f0f,0xcf770f,0xcf7f0f,0xCF8717,0xC78717,0xC78F17, 0xC7971F, 0xBF9F1F, 0xBF9F1F, 0xBFA727, 0xBFA727, 0xBFAF2F, 0xB7AF2F, 0xB7B72F, 0xB7B737, 0xCFCF6F, 0xDFDF9F, 0xEFEFC7, 0xFFFFFF};

Uint32 random_between(Uint32 mn, Uint32 mx){
    Uint32 n = rand()%(mx-mn)+mn;
    return n;
}

int distance(int target, int guess){
    if(target <= guess){
        return guess-target;
    } else {
        return target-guess;
    }
}

int get_value(int target, int guess, int maxv){
    int d = distance(target, guess)/(target/maxv);
    //if(d>maxv) d = maxv;
    return d;  
}

int distance_from_center(int x, int y, int w, int h){
    int x_dis = distance(w/2, x);
    int y_dis = distance(h/2, y);

    int p = (x_dis+y_dis)/2;
    
    return get_value(w/2, p, 250);

}

void pixels_to(Uint32* pix){
    Timer timer;
    timer.start();
    for (int i = 0; i < screen_w*screen_h; ++i)
    {
        Uint32 col = 0x00FF00;
        *pix = col;
        pix++;
    }
    printf("%d\n", timer.getTicks());
}

void pixels_to2(Uint32* pix){
    printf("%d\n", screen_w*screen_h); 
    for (int i = 0; i < screen_w*screen_h; ++i)
    {
        Uint32 col = 0x00FF00;
        *pix = col;
        pix++;
    
    }
}

int get_color(int r, int g, int b){
    return b+(g<<8)+(r<<16);
    //return a+(b<<8)+(g<<16)+(r<<24);
}

void paint_row(Uint32* pix, int row, int r, int g, int b){
    pix += row*screen_w;
    for (int i = 0; i < screen_w; ++i)
    {
        //printf("%d\n", i);
        Uint32 col = get_color(r,g,b);
        *pix = col;
        pix++;
    }
}

void paint_row_uint32(Uint32* pix, int row, uint32_t col){
    pix += row*screen_w;
    for (int i = 0; i < screen_w; ++i)
    {
        *pix = col;
        pix++;
    }
}

void paint_row_doomfire_random(Uint32* pix, int row, int doomfire_index){
    
    pix += row*screen_w;
    for (int i = 0; i < screen_w; ++i)
    {
        int r_index = random_between(doomfire_index-5, doomfire_index);
        if(r_index < 0) r_index = 0;
        Uint32 col = doomfire[r_index];

        *pix = col;
        pix++;
    }
}

void paint_frow(Uint32* pix){
    pix += 0*screen_w;
    for (int i = 0; i < screen_w; ++i)
    {
        printf("%d\n", i);
        Uint32 col = get_color(0,255,124);
        *pix = col;
        pix++;
    }
}

void spreadFire(Uint32* firePixels, int src){
    Uint32 rand = ((Uint32)random())%3;
    Uint32 dst = src - rand + 1;
    //printf("dst: %d\n", dst);
    //printf("rand: %d\n", rand);

    int t = firePixels[src] - (rand & 1);
    if(t < 0) t = 0;
    //printf("t: %d\n", t);
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

    game.init("test", screen_w, screen_h);

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
          //printf("%d\n", pixels_values[i]);
          //printf("llegamos:\n");
          pixels[i] = doomfire[pixels_values[i]];
        }
    };

    update_scr(pixels,pixels_values);

    

    Uint32* screen_pointer = pixels; 

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_M]){
            pixels_to2(pixels);
        }

        if(kbstate[SDL_SCANCODE_T]){
            doFire(pixels_values);
            update_scr(pixels,pixels_values);
        }

        if(kbstate[SDL_SCANCODE_U]){}

        if(kbstate[SDL_SCANCODE_J]){}

        if(kbstate[SDL_SCANCODE_I]){}

        if(kbstate[SDL_SCANCODE_O]){}


        if(kbstate[SDL_SCANCODE_H]){}
        
        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                
            }
        }
        
        doFire(pixels_values);
        update_scr(pixels,pixels_values);


        SDL_UpdateTexture(frame_buffer, nullptr, pixels, 4*screen_w);
        SDL_RenderCopy(game.renderer, frame_buffer, nullptr, nullptr);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

        //SDL_Delay(1000);
    }
    
    SDL_DestroyTexture(frame_buffer);
    game.close();
}
