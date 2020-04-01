#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

static LogSystem log_system = LogSystem();

class Background
{
public:
    SDL_Texture* image = nullptr;
    int limit = 640*4;
    int laps = 0;

    SDL_Rect src = {0,0,640,480};//tmp_srf->w,tmp_srf->h};
    SDL_Rect dst = {0,0,640,480};
    

    Background(SDL_Renderer* renderer){
        SDL_Surface* tmp_srf = IMG_Load("bt2.png");
        image = SDL_CreateTextureFromSurface(renderer, tmp_srf);
        SDL_FreeSurface(tmp_srf);
    }

    void draw(SDL_Renderer* renderer){
        SDL_RenderCopy(renderer, image, &src, &dst);
    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_RIGHT){
                src.x += 10;
            }
            if(event.key.keysym.sym == SDLK_LEFT){
                src.x -= 10;
            }

            if(event.key.keysym.sym == SDLK_PLUS){
                dst.h += 20;
                src.w -= 20;
            }
            if(event.key.keysym.sym == SDLK_MINUS){
                dst.h -= 20;
                src.w += 20;
            }
        }
    }

};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 640, 480);
    log_system.init();

    Background background(game.renderer);

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
            background.handle_input(game.event);
        }

        background.draw(game.renderer);

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
