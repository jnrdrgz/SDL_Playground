#include <SDL2/SDL.h>
#include "game.h"
#include "text.h"
#include "log_system.h"

static LogSystem log_system = LogSystem();

class Bomb
{
public:
    SDL_Texture* texture = nullptr;
    SDL_Rect rct = {200,200,50,50};
    
    Bomb(SDL_Renderer *r){
        SDL_Surface* tmp_srf = IMG_Load("arrow.png");
        texture = SDL_CreateTextureFromSurface(r, tmp_srf);
        SDL_FreeSurface(tmp_srf);
    }

    void draw(SDL_Renderer* r){
        SDL_RenderCopyEx(r, texture, NULL, &rct, 0, 0, SDL_FLIP_NONE);
    }

    void handle_input(SDL_Event e){
        if(e.type == SDL_MOUSEMOTION){
            //printf("%d, %d\n", e.button.x,e.button.y);
        }

    }

    void update(){

    }

};


int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();

    Uint32 t = SDL_GetTicks();
    printf("%d\n", t);
    
    log_system.add_text("fps", std::to_string(t), game.renderer);
    log_system.add_text("otra_cosa", "cosardadadr", game.renderer);

    while(game.running){
        t = SDL_GetTicks();
        //printf("%d\n", t);

        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_MOUSEBUTTONDOWN){
                log_system.delete_text("otra_cosa");
            }
        }

        
        log_system.update_text("fps", std::to_string(t), game.renderer);
        log_system.draw(game.renderer);

        
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    game.close();
}
