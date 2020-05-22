#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

const int tile_size = 16;
const int screen_width = 800;
const int screen_height = 640;
const int horizontal_tiles = screen_width/tile_size;
const int vertical_tiles = screen_height/tile_size;
const int texture_width = 768;
const int texture_height = 352;


static LogSystem log_system = LogSystem();

int tiles[horizontal_tiles][vertical_tiles];

struct Tile
{
public:
    Tile(){
        set_index(1);
    }

    Tile(int index){
        set_index(index);
    }

    void set_index(int index){
        this->index = index;
        update_tile_img();
    }

    void set_pos(int x, int y){
        dst.x = tile_size*x;
        dst.y = tile_size*y;
    }

    void update_tile_img(){
        //int srcx = (index%tile_size)-1;
        //int srcy = (tile_size/index)-1;

        // texutre dim  768 x 352:
        int texture_cols = (texture_width/tile_size);
        int texture_rows = (texture_height/tile_size);

        int srcx = index%texture_cols;
        int srcy = index/texture_cols;
        
        src.x = srcx*tile_size;        
        src.y = srcy*tile_size;
        //printf("%d,%d\n", srcx,srcy);
        //printf("%d,%d\n", src.x,src.y);
        
        src.w = tile_size;
        src.h = tile_size;

        dst.w = tile_size;
        dst.h = tile_size;
    }

    int index;
    SDL_Rect src,dst;

};

struct TileMap
{
public:
    TileMap(SDL_Renderer* renderer){
        load(renderer);
    }

    void load(SDL_Renderer* renderer){
        SDL_Surface* tmp_surf = IMG_Load("tilesheet.png");
        if(!tmp_surf) throw std::logic_error("surf not loaded");
        tilesheet_texture = SDL_CreateTextureFromSurface(renderer, tmp_surf);
        if(!tilesheet_texture) throw std::logic_error("texture not loaded");

        SDL_FreeSurface(tmp_surf);
        for(int i = 0; i < horizontal_tiles; i++){
            for(int j = 0; j < vertical_tiles; j++){
                tiles[i][j].set_index(0);
                tiles[i][j].set_pos(i,j);
            }
        }


    }

     void handle_input(){
        const Uint8 *state = SDL_GetKeyboardState(NULL);
    
        if(edit_mode){
            int x, y;
            if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                int tile_x = x/tile_size;
                int tile_y = y/tile_size;
                int texture_cols = (texture_width/tile_size);
                int index = (tile_y*texture_cols)+tile_x;
                
                printf("x: %d, y: %d\n", x,y);
                printf("tilex: %d, tiley: %d\n", tile_x,tile_y);
                printf("index: %d\n", (tile_y*texture_cols)+tile_x);
                
                if(selection){
                    select_tile_index = index;

                    selection = false;
                    editing = true;
                }
                else if(editing){    
                    tiles[tile_x][tile_y].set_index(select_tile_index);
                }               
            }


        } else {

        }

        if(state[SDL_SCANCODE_M]){
            printf("Mode chaned to edit\n");
            edit_mode = true;
            selection = true;
        }
        if(state[SDL_SCANCODE_E]){
            printf("Edit mode escaped\n");
            edit_mode = false;
        }

    }
    void draw(SDL_Renderer* renderer){
        if(edit_mode && selection){
            SDL_RenderCopy(renderer, tilesheet_texture, NULL, &edit_rct);
        } else {
            for(int i = 0; i < horizontal_tiles; i++){
                for(int j = 0; j < vertical_tiles; j++){
                    SDL_RenderCopy(renderer, tilesheet_texture, &tiles[i][j].src, &tiles[i][j].dst);
                }
            }
        }
    }

    Tile tiles[horizontal_tiles][vertical_tiles];
    SDL_Texture* tilesheet_texture = nullptr;
    bool edit_mode = false, selection = false, editing = false;
    int select_tile_index = 0;
    SDL_Rect edit_rct = {0,0,texture_width,texture_height};

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
    game.init("test", screen_width, screen_height);
    log_system.init();

    Grid grid;
    TileMap tilemap(game.renderer);

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

        //grid.draw(game.renderer);
        tilemap.handle_input();
        tilemap.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
