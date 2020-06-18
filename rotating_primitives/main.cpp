#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>
#include <string>
#include <unordered_map>

static LogSystem log_system = LogSystem();

const int screen_w = 640;
const int screen_h = 480;

std::unordered_map<std::string, SDL_Texture*> textures;

struct GameObject
{
    GameObject(SDL_Renderer* renderer, std::string img_file, std::string tag){
        load_img(renderer, img_file.c_str());
        this->tag = tag;
        img_file_name = img_file;
        printf("GameObject %s created!\n", tag.c_str());
    
    }

    ~GameObject(){
        if(texture){
            //SDL_DestroyTexture(texture);
        }

        //printf("GameObject %s destroyed!\n", tag.c_str());    
    }

    //GameObject(const GameObject& other){}// copy constructor
 
    //GameObject& operator=(const GameObject& other){} // copy assignment

    void load_img(SDL_Renderer* renderer, const char* img){
        SDL_Surface* temp_surface = IMG_Load(img);
        texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
        SDL_FreeSurface(temp_surface);
    }

    void set_posdim(int x, int y, int w, int h){
        rct.x = x;
        rct.y = y;
        rct.w = w;
        rct.h = h;
    }

    void draw(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderDrawRect(renderer, &rct);
        SDL_RenderCopyEx(renderer, texture, NULL, &rct, rotation, NULL, SDL_FLIP_NONE);
    }

    SDL_Texture* texture = NULL;
    std::string tag,img_file_name;
    double rotation = 0.0;
    SDL_Rect rct;
};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", screen_w, screen_h);
    log_system.init();

    //SDL_Rect outlineRect = { 0,0,100,100 };    
    //SDL_Texture* texture = nullptr;

    Timer enemy_spawn_timer;
    enemy_spawn_timer.start();

    GameObject player(game.renderer, "arrow.png", "player");
    player.set_posdim(screen_w/2,screen_h/2,25,25);
    std::vector<GameObject> enemies;

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_RIGHT]){
            //printf("Pressing RIGHT\n");
            player.rotation += 3.0;
        }
        if(kbstate[SDL_SCANCODE_LEFT]){
            //printf("Pressing LEFT\n");
            player.rotation -= 3.0;
        }

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

        //SDL_SetRenderTarget(game.renderer, texture);

        //SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );        
        //SDL_RenderDrawRect( gRenderer, &outlineRect );

        //SDL_SetRenderTarget( game.renderer, NULL );
        
        //SDL_SetRenderDrawColor( game.renderer, 0, 0, 0, 255);
        
        //update
        if(enemy_spawn_timer.getTicks() > 3000)
        {
            if(enemies.size() < 10) {
                GameObject new_enemy(game.renderer, "enemy.png", "enemy");
                new_enemy.set_posdim(random()%screen_w,random()%screen_h,25,25);
                enemies.push_back(new_enemy);   
            }
            enemy_spawn_timer.stop();
            enemy_spawn_timer.start();
            printf("enemy created!\n");
        }
        //draw
        player.draw(game.renderer);
        for(auto e : enemies){
            e.draw(game.renderer);
        }
    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    for(auto& e : enemies){
        SDL_DestroyTexture(e.texture);
    }
    game.close();
}
