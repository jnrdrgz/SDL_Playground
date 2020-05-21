#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

const int tile_size = 16;
const int screen_width = 640;
const int screen_height = 480;
const int horizontal_tiles = screen_width/tile_size;
const int vertical_tiles = screen_height/tile_size;

static LogSystem log_system = LogSystem();

SDL_Texture* tilesheet = nullptr;
int tiles[horizontal_tiles][vertical_tiles];


struct Tile
{
public:
    int index;
};

struct Grid
{
public:
    Grid(){
        int t = 0;
        for(int i = 0; i < horizontal_tiles; i++){
            for(int j = 0; j < vertical_tiles; j++){
                rcts[t].x = i*tile_size;
                rcts[t].y = j*tile_size;
                rcts[t].w = tile_size;
                rcts[t].h = tile_size;
                t++;
            }
        }
    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        //for(int i = 0; i < horizontal_tiles*vertical_tiles; i++){}
        SDL_RenderDrawRects(renderer, rcts, horizontal_tiles*vertical_tiles);
    }

    SDL_Rect rcts[horizontal_tiles*vertical_tiles];

};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 640, 480);
    log_system.init();

    Grid grid;

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

        grid.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
