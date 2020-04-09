#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <cmath>

#define NPI 3.14159265359

static LogSystem log_system = LogSystem();

double v2_len(SDL_Point a,SDL_Point b){    
    return std::sqrt(std::pow(a.x-b.x,2) + std::pow(a.y-b.y,2));
}

int in_what_cuadrant(SDL_Point b){
    int cuadrant = 0;

    if(b.x > 250 && b.y < 250){
        cuadrant = 1;
    }    
    if(b.x <= 250 && b.y <= 250){
        cuadrant = 2;
    }
    if(b.x <= 250 && b.y > 250){
        cuadrant = 3;
    }
    if(b.x > 250 && b.y >= 250){
        cuadrant = 4;
    }

    //printf("%d\n", cuadrant);
    return cuadrant;
}

double angle_rad(SDL_Point a,SDL_Point b,SDL_Point c){
    double hipot = v2_len(a, c);
    double height = v2_len(b, c);
    double width = v2_len(a, b);//b

    return atan(height/hipot);
}

double angle_deg(SDL_Point a,SDL_Point b,SDL_Point c){
    double hipot = v2_len(a, c);
    double height = v2_len(b, c);
    double width = v2_len(a, b);//b


    return (atan(height/hipot)*(180.0/NPI))*2*in_what_cuadrant(c);
}

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();

    SDL_Surface* tmp_srf = IMG_Load("img.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game.renderer, tmp_srf);
    SDL_FreeSurface(tmp_srf);
    int angle = 0;
    SDL_Rect dst = {220,220,30,30};


    SDL_Point p1 = {dst.x+(dst.w/2), dst.y+(dst.h/2)};
    SDL_Point p2 = {dst.x+100, dst.y+(dst.h/2)};
    SDL_Point p3 = {dst.x+(dst.w/2), dst.y-100};

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                p3.x = game.event.button.x;
                p3.y = game.event.button.y;
                p2.x = game.event.button.x;
                
                angle = (int)(angle_deg(p1,p2,p3));
                printf("%d\n", angle);
        
                
            }
            if(game.event.type == SDL_KEYDOWN){
                
            }
        }


    
        SDL_SetRenderDrawColor( game.renderer, 0,0,0, 255);
        SDL_RenderDrawLine(game.renderer, p1.x, p1.y, p2.x, p2.y);
        SDL_RenderDrawLine(game.renderer, p1.x, p1.y, p3.x, p3.y);
        SDL_RenderDrawLine(game.renderer, p2.x, p2.y, p3.x, p3.y);

        SDL_RenderCopyEx(game.renderer, texture, NULL, &dst, angle*-1, 0, SDL_FLIP_NONE);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
