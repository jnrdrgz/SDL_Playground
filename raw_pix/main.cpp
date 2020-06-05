#include <SDL2/SDL.h>
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <stdlib.h>
#include <time.h>

const int screen_w = 640;
const int screen_h = 360;

const Uint32 doomfire[] = {0x070707,0x1f0707,0x2f0f07,0x470f07,0x571707,0x671f07, 0x771f07, 0x8f2707, 0x9f2f07, 0xaf3f07, 0xbf4707,0xc74707,0xDF4F07,0xDF5707,0xDF5707,0xD75F07,0xD7670F,0xcf6f0f,0xcf770f,0xcf7f0f,0xCF8717,0xC78717,0xC78F17, 0xC7971F, 0xBF9F1F, 0xBF9F1F, 0xBFA727, 0xBFA727, 0xBFAF2F, 0xB7AF2F, 0xB7B72F, 0xB7B737, 0xCFCF6F, 0xDFDF9F, 0xEFEFC7, 0xFFFFFF};

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




int main(int argc, char* args[])
{
    Game game;

    game.init("test", screen_w, screen_h);

    SDL_Rect rct = {100,100,50,50};
    SDL_Texture* frame_buffer = nullptr;

    frame_buffer = SDL_CreateTexture(game.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screen_w, screen_h);

    // range of colors 0x4B(75) -> 0XFF(255)
    int r = 0xff;
    int g = 0xff;
    int b = 0x4B;
    int rgb = b+(g<<8)+(r<<16);
    
    int quantity_of_colors = 255-75;
    
    Uint32 pixels[screen_w*screen_h] = {};
    //Uint32 f = SDL_MapRGB();
    Uint32 f = 0xFFFF00;

    Uint32* screen_fpx = pixels; 

    for (int i = 0; i < screen_w*screen_h; ++i)
    {
        Uint32 r = 0xFF0000;
        pixels[i] = r;        
    }

    int top_row = 0;
    int bottom_row = screen_h-1;
    while(game.running){
        
        SDL_RenderClear(game.renderer);

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_M]){
            pixels_to2(pixels);
        }

        if(kbstate[SDL_SCANCODE_T]){
            Uint32 col = 0x00FF00;
            *screen_fpx = col;
            screen_fpx++;
        }

        if(kbstate[SDL_SCANCODE_U]){
            paint_row(pixels, top_row, 255,get_value(255, top_row, 255),0);

            printf("%d\n", get_value(255, top_row, 255));

            top_row++;
        }

        if(kbstate[SDL_SCANCODE_J]){
            paint_row(pixels, bottom_row, 255,255,0);
            bottom_row--;
        }

        if(kbstate[SDL_SCANCODE_I]){
            Uint32 val = get_value(screen_h/2, bottom_row, 35);
            Uint32 col = doomfire[val];
            if(val > 3){
                col = doomfire[random_between(val-3, val)];
            }
            paint_row_uint32(pixels, bottom_row, col);
            printf("%d\n", get_value(screen_h, bottom_row, 35));

            bottom_row--;
        }

        if(kbstate[SDL_SCANCODE_O]){
            Uint32 val = get_value(screen_h/2, bottom_row, 35);
            
            paint_row_doomfire_random(pixels, bottom_row, val);
            printf("%d\n", get_value(screen_h, bottom_row, 35));

            bottom_row--;
        }


        if(kbstate[SDL_SCANCODE_H]){
            paint_frow(pixels);
        }
        
        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                
            }
        }
        

        SDL_UpdateTexture(frame_buffer, nullptr, pixels, 4*screen_w);
        SDL_RenderCopy(game.renderer, frame_buffer, nullptr, nullptr);

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    SDL_DestroyTexture(frame_buffer);
    game.close();
}
