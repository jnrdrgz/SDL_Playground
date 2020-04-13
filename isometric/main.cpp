#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "../texture_manager/texturemanager.h"

#define TILE_W 64
#define TILE_H 32

static LogSystem log_system = LogSystem();
TextureManager textureManager;

enum class TileType
{
    WATER,
    GRASS,
    GROUND,
    FIRE,
    ICE,
    SNOW,
}; 

//iso tile directly from texture
struct Tile_
{
public:
    Tile_(SDL_Texture* texture){
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
                //change_texture(textureManager.get_texture("tile_selected.png", renderer));
            }
        }
    }

private:
    SDL_Texture* texture = nullptr;
    SDL_Rect rct;
};

struct TilesTexture
{
public:
    TilesTexture(SDL_Renderer* renderer){
        SDL_Surface* surface = IMG_Load("tiles_m.png");

        if(surface){
            texture = SDL_CreateTextureFromSurface(renderer, surface);   
        } else{
            printf("error loading tile map\n");
        }

        SDL_FreeSurface(surface);
    }

    static SDL_Texture* get_texture(){
        return texture;
    }
    static SDL_Texture* texture;
};

int* get_src_tile(int type){
    int wh[2]{};
    switch(type){
        case 0://TileType::WATER:
            wh[0] = 0;
            wh[1] = 0;
        break;
        case 1://TileType::GRASS:
            wh[0] = 1;
            wh[1] = 0;
        break;
        case 2://TileType::GROUND:
            wh[0] = 2;
            wh[1] = 0;
        break;
        case 3://TileType::FIRE:
            wh[0] = 3;
            wh[1] = 0;
        break;
        case 4://TileType::ICE:
            wh[0] = 4;
            wh[1] = 0;
        break;
        case 5://TileType::SNOW:
            wh[0] = 5;
            wh[1] = 0;
        break;
        default:
            throw std::logic_error("enum type not found:");
        break;
    }

    int* wh_ = wh;
    return wh_;
}

struct Tile
{
public:
    Tile(){
        dst.w = TILE_W;
        src.w = TILE_W;
        dst.h = TILE_H;
        src.h = TILE_H;
    }

    Tile(int type){
        dst.w = TILE_W;
        src.w = TILE_W;
        dst.h = TILE_H;
        src.h = TILE_H;

        src.x = get_src_tile(type)[0]*TILE_W;
        src.y = get_src_tile(type)[1]*TILE_H;
        set_position(0, 0);
    }

    void set_position(int x, int y){
        dst.x = x;
        dst.y = y;
    }
    
    void set_type(int type){
        src.x = get_src_tile(type)[0]*TILE_W;
        src.y = get_src_tile(type)[1]*TILE_H;
    }

    void draw(SDL_Renderer* renderer){
        SDL_RenderCopy(renderer, TilesTexture::texture, &src, &dst);
    }

private:
    SDL_Rect src,dst;
};
    
SDL_Texture* TilesTexture::texture = nullptr;

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 11*TILE_W, 11*TILE_H);
    log_system.init();
   
    TilesTexture tilesTexture{game.renderer};
   
    Tile tiles[10][10];
    int tiles_map[10][10] = 
    {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,2,2,0},
    {0,0,0,2,2,2,2,2,2,0},
    {0,0,0,2,1,1,1,1,2,0},
    {0,0,0,2,1,1,1,1,2,0},
    {2,2,2,2,1,1,1,1,2,0},
    {2,1,1,1,1,1,1,1,2,0},
    {2,2,2,2,2,2,2,2,2,0},
    {0,0,0,0,0,0,0,0,0,0}
};
    

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            tiles[i][j].set_type(tiles_map[i][j]);
        }
    }

    printf("1\n");

    int ix = TILE_W*5;
    int iy = TILE_H*0;

    printf("2\n");

    for(int i = 0; i < 10; i++){
        int i_ix = ix;
        int i_iy = iy;
        for(int j = 0; j < 10; j++){
            tiles[i][j].set_position(ix, iy);
            ix += TILE_W/2;
            iy += TILE_H/2;
        }
        ix = i_ix-TILE_W/2;
        iy = i_iy+TILE_H/2;
    }

    printf("3\n");

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
                    //tiles[i][j]->handle_input(game.event, game.renderer);
                }
            }
    
        }

        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                tiles[i][j].draw(game.renderer);
            }
        }
    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
