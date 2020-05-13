#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

static LogSystem log_system = LogSystem();

#define SCREENW 500
#define SCREENH 500

struct Camera
{
    Camera(){
        y = x = 0;
        zoom = 1;
    }
    int x, y, zoom;
};

struct GameObj
{
public:
    GameObj(){}
    void draw(SDL_Renderer* renderer, Camera camera){
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_Rect rct_ = {rct.x,rct.y,rct.w*camera.zoom,rct.h*camera.zoom};
        SDL_RenderFillRect(renderer, &rct_);
    }
    SDL_Rect rct = {225,225,25,25};
};



int main(int argc, char* args[])
{
    Game game;
    game.init("test", SCREENW, SCREENH);
    log_system.init();

    Camera camera;
    GameObj go;

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_RIGHT){
                
                }
                if(game.event.key.keysym.sym == SDLK_PLUS){
                    camera.zoom = 2;
                }

                if(game.event.key.keysym.sym == SDLK_MINUS){
                    camera.zoom = 1;
                }
            }
        }

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);

        go.draw(game.renderer, camera);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }
    
    
    game.close();
}
