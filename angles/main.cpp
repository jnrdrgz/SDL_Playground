#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <cmath>

#define NPI 3.14159265359

static LogSystem log_system = LogSystem();

float rad_to_deg(float n){return n*180.0/NPI;}
float deg_to_rad(float n){return n*NPI/180.0;}

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
        
                
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_DOWN){
                    angle += 3;
                }
                if(game.event.key.keysym.sym == SDLK_UP){
                    angle -= 3;
                }


                if(game.event.key.keysym.sym == SDLK_w){
                    dst.x += cos(deg_to_rad((float)(angle*-1))) * 5;
                    dst.y -= sin(deg_to_rad((float)(angle*-1))) * 5;
                }

                if(game.event.key.keysym.sym == SDLK_s){
                    
                }
            }
        }


        SDL_SetRenderDrawColor( game.renderer, 0,0,0, 255);
        
        SDL_RenderCopyEx(game.renderer, texture, NULL, &dst, angle, 0, SDL_FLIP_NONE);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
