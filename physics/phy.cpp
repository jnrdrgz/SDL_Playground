#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

#include <vector>
#include "vector2.h"

static LogSystem log_system = LogSystem();

struct Body
{
    
};

struct World
{
public:

private:
    Timer timer;
    std::vector<Body> bodies;
};

int main(int argc, char* args[])
{
    int screen_w = 500, screen_h = 500;

    Game game;
    game.init("test", screen_w, screen_h);
    log_system.init();

    World world{};
    
    
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
            }
        }

     
        log_system.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
