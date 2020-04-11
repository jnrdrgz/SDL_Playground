#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "../texture_manager/texturemanager.h"

#define TILE_W 40
#define TILE_H 24

static LogSystem log_system = LogSystem();
TextureManager textureManager; 

//iso tile directly from texture
struct Tile
{
public:
    Tile(SDL_Texture* texture){
        this->texture = texture;
        rct.x = 0;
        rct.y = 0;
        rct.w = TILE_W;
        rct.h = TILE_H;
    }

    void draw(SDL_Renderer* renderer){
        SDL_RenderCopy(renderer, texture, nullptr, &rct);
    }


    void set_position(int x, int y){
        rct.x = x;
        rct.y = y;
    }

    void change_texture(SDL_Texture* new_texture){
        //if(texture){
        //    SDL_DestroyTexture(texture);
        //}

        texture = new_texture;
    }

    void handle_input(SDL_Event event, SDL_Renderer *renderer){
        if(event.type == SDL_MOUSEMOTION){
            if(event.button.x > rct.x && event.button.y > rct.y
                && event.button.x < rct.x+rct.w && event.button.y < rct.y+rct.h)
            {
                change_texture(textureManager.get_texture("tile_selected.png", renderer));
            }
        }
    }

private:
    SDL_Texture* texture = nullptr;
    SDL_Rect rct;
};

//iso tile directly from texture
struct GenIsoTile
{
public:
    GenIsoTile(SDL_Texture* texture){
        this->texture = texture;
        rct.x = 0;
        rct.y = 0;
        rct.w = TILE_W;
        rct.h = TILE_H;
    }

    void draw(SDL_Renderer* renderer){
        SDL_RenderCopyEx(renderer, texture, nullptr, &rct, 45, 0, SDL_FLIP_NONE);
    }



private:
    SDL_Texture* texture = nullptr;
    SDL_Rect rct;
};

    
int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    log_system.init();


    Tile tile(textureManager.get_texture("tile_p.png", game.renderer));

    Tile* tiles[10][10];

    int ix = TILE_W*5;
    int iy = TILE_H*5;

    for(int i = 0; i < 10; i++){
        int i_ix = ix;
        int i_iy = iy;
        for(int j = 0; j < 10; j++){
            tiles[i][j] = new Tile(textureManager.get_texture("tile_p.png", game.renderer));
            tiles[i][j]->set_position(ix, iy);
            ix += TILE_W/2;
            iy += TILE_H/2;
        }
        ix = i_ix-TILE_W/2;
        iy = i_iy+TILE_H/2;
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
                if(game.event.key.keysym.sym == SDLK_RIGHT){
                
                }
            }
            for(int i = 0; i < 10; i++){
                for(int j = 0; j < 10; j++){
                    tiles[i][j]->handle_input(game.event, game.renderer);
                }
            }
    
        }

        //tile.draw(game.renderer, TILE_W*4.5,TILE_H*5.5f);
        //tile.draw(game.renderer, TILE_W*5,TILE_H*5);
        //tile.draw(game.renderer, TILE_W*5.5,TILE_H*5.5);
        //tile.draw(game.renderer, TILE_W*5,TILE_H*6);
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                tiles[i][j]->draw(game.renderer);
            }
        }
    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
