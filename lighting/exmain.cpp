#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <stdlib.h>
#include <time.h>

static LogSystem log_system = LogSystem();

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

int main(int argc, char* args[])
{
    Game game;

    const int testW = 500;
    const int testH = 500;
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


    /*for (int i = 0; i < testW*testH; ++i)
    {
        //b+=1;
        //if(b==0xff){
        //    b = 0;
        //}
        
        if(i%50000 == 0){
            //b = rand()%255;
            
            if(b<=255){
                b += 25;
            }else{
                b=255;
            }
        }

        //b = (i-(testW*testH/2))%255;
        if(i>(testW*testH/2)){
            b = 255;
        }
        rgb = b+(g<<8)+(r<<16);
        pixels[i] = rgb;


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

        
    }*/

    /*for (int i = 0; i < testW*testH; ++i)
    {
        pixels[i] = 0;
    }
    for (int i = 0; i < (testW*testH)-2000; i+=light_jump)
    {
        for (int j = 0; j < light_jump; j++)
        {
            b = rand()%255;
            rgb = b+(g<<8)+(r<<16);
            pixels[i] = rgb;

        }
    }*/

    //wwwwwworked
    /*int v2 = (testW*testH)/2;
    printf("%d\n", v2);
    for (int i = 0; i < testW*testH; ++i)
    {
        b = get_value(v2,i,255);
        rgb = b+(g<<8)+(r<<16);
        pixels[i] = rgb;
    }*/
    //////////

    /*

    for (int i = 0; i < testW*testH; ++i)
    {
        b = 255;
        rgb = b+(g<<8)+(r<<16);
        pixels[i] = rgb;
    }*/

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

    /*
    int v2 = testW/2;
    printf("%d\n", v2);
    int gff = 0;
    for (int i = 0; i < testH; i++)
    {
        gff++;
        b = get_value(v2,i,250);
        rgb = b+(g<<8)+(r<<16);
                
        for (int j = 0; j < testW; j++)
        {
            gff++;
            b = get_value(v2,j,250);
            if(i != 0){
                //pixels[j+i+testH] = rgb;
                pixels[(i*testH)+j] = rgb;
            }
        }
    }
    printf("n___%d\n", gff);*/

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
