#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <stdlib.h>
#include <time.h>

static LogSystem log_system = LogSystem();
const int testW = 500;
const int testH = 500;
int firePixels[36] = {0};

int distance(int target, int guess){
    if(target <= guess){
        return guess-target;
    } else {
        return target-guess;
    }
}

int get_value(int target, int guess, int maxv){
    return distance(target, guess)/(target/maxv);  
}

int distance_from_center(int x, int y, int w, int h){
    int x_dis = distance(w/2, x);
    int y_dis = distance(h/2, y);

    int p = (x_dis+y_dis)/2;
    
    return get_value(w/2, p, 250);

}

void spreadFire(int src){
    int r = 
    firePixels[src-testW] = firePixels[src]-1;
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
    log_system.init();

    SDL_Rect rct = {100,100,50,50};
    SDL_Texture* test = nullptr;

    test = SDL_CreateTexture(game.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, testW, testH);

    // range of colors 0x4B(75) -> 0XFF(255)
    int r = 0xff;
    int g = 0xff;
    int b = 0x4B;
    int rgb = b+(g<<8)+(r<<16);
    
    int quantity_of_colors = 255-75;
    int light_jump = (testW*testH)/quantity_of_colors;
    //printf("%d\n", rgb);
    
    Uint32 pixels[testW*testH] = {};
    //Uint32 f = SDL_MapRGB();
    Uint32 f = 0xFFFF00;

    for (int i = 0; i < testH; i++)
    {
        for (int j = 0; j < testW; j++)
        {
            int index = i+j*testW;
            //r = j;
            //g = 0;
            b = distance_from_center(j,i,testW,testH);
            rgb = b+(g<<8)+(r<<16);
            pixels[index] = rgb;
        }
    }

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

    // Populate pallete.
    for(size_t i = 0; i < (sizeof(rgbs)/sizeof(int))/3; i++) {
        int r = rgbs[i * 3 + 0];
        int g = rgbs[i * 3 + 1];
        int b = rgbs[i * 3 + 2];
        int rgb = b+(g<<8)+(r<<16);
        firePixels[i] = rgb; 
    }

    /*for(int i = 0; i < 36; i++){  
        pallete[i] = rgb;
        printf("%d,%d,%d\n", r,g,b);
        if(r+25 <= 255){
            r += 25;
        } else {
            r = 25;
        }

        if(g+25 <= 255) {
            g += 25;
        } else {
            g = 25;
        }
    }*/

    int g_ = 0;
    for (int i = 0; i < testH; i++)
    {
        for (int j = 0; j < testW; j++)
        {
            //int index = i+j*testW;
            //printf("%d\n", pixels[index]);
            pixels[j] = firePixels[g_];
        }
        g_+=1;
        if(g_ == 35){
            g_ = 0;
        }
    }
        
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

        
        /*
        int r = 0xff;
        int g = 0xff;
        int b = 0x01;
        int rgb = 0;
        rgb = (((r << 8) | g) << 8) | b;
        */

        

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
