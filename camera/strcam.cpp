#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>

static LogSystem log_system = LogSystem();
const int screen_w = 640;
const int screen_h = 480;

//int camerax = 0;
//int cameray = 0;
//int camerazoom = 1;
struct GameObject;
struct Camera
{
public:
    Camera(){
        x = 0;
        y = 0;
        zoom = 1;
    }
    int x, y;
    int zoom;
    int limitx;
    int limity;
    void follow(GameObject g);
    void follow_rct(SDL_Rect rct, int vx, int vy);
};

struct GameObject
{
public:
    GameObject(){
        rct.x = screen_w/2;
        rct.y = screen_h/2;
        rct.w = 30;
        rct.h = 30;
        app_camera_rct.w = 30;
        app_camera_rct.h = 30;
    }

    void handle_input(){
        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_RIGHT]){
            rct.x += vx;
        }
        if(kbstate[SDL_SCANCODE_LEFT]){
            rct.x -= vx;
        }
        if(kbstate[SDL_SCANCODE_UP]){
            rct.y -= vy;
        }
        if(kbstate[SDL_SCANCODE_DOWN]){
            rct.y += vy;
        }
    }

    void draw(SDL_Renderer* renderer, Camera camera){
        app_camera_rct.x = (rct.x-camera.x)*camera.zoom;
        app_camera_rct.y = (rct.y-camera.y)*camera.zoom;
        app_camera_rct.w = rct.w*camera.zoom;
        app_camera_rct.h = rct.h*camera.zoom;
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &app_camera_rct);
    }

    SDL_Rect rct, app_camera_rct;
    int vx = 6, vy = 6;
};

struct Background
{
public:
    Background(){
        src.x = 0;
        src.y = 0;
        src.w = screen_w;
        src.h = screen_h;
        app_camera_src.w = screen_w;
        app_camera_src.h = screen_h;

        dst.x = 0;
        dst.y = 0;
        dst.w = screen_w;
        dst.h = screen_h;
    }

    Background(SDL_Renderer* renderer) : Background(){
        load_img(renderer);
    }

    void load_img(SDL_Renderer* renderer){
        SDL_Surface* temp_surface = IMG_Load("back.png");
        texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
        SDL_FreeSurface(temp_surface);

    }

    void draw(SDL_Renderer* renderer, Camera camera){
        app_camera_src.x = src.x+camera.x;
        app_camera_src.y = src.y+camera.y;
        app_camera_src.w = screen_w/camera.zoom;
        app_camera_src.h = screen_h/camera.zoom;
        
        SDL_RenderCopy(renderer, texture, &app_camera_src, &dst);
    }

    void test_scroll(){
        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_D]){
            src.x += vx;
        }
        if(kbstate[SDL_SCANCODE_A]){
            src.x -= vx;
        }
        if(kbstate[SDL_SCANCODE_W]){
            src.y -= vy;
        }
        if(kbstate[SDL_SCANCODE_S]){
            src.y += vy;
        }
    }

    SDL_Texture* texture = NULL;
    SDL_Rect src,dst,app_camera_src;
    int vx = 5, vy = 5;
    
};

void Camera::follow(GameObject g){
    if(g.rct.x-x > screen_w-g.rct.w){
        x += g.vx;
    }
    if(g.rct.x-x < g.rct.w){
        x -= g.vx;
        if(x < 0) x = 0;
    }
    if(g.rct.y-y > screen_h-g.rct.h){
        y += g.vy;
    }
    if(g.rct.y-y < g.rct.h){
        y -= g.vy;
        if(y < 0) y = 0;
    }
}

void Camera::follow_rct(SDL_Rect rct, int vx, int vy){
    if(rct.x-x > screen_w-rct.w){
        x += vx;
    }
    if(rct.x-x < rct.w){
        x -= vx;
        if(x < 0) x = 0;
    }
    if(rct.y-y > screen_h-rct.h){
        y += vy;
    }
    if(rct.y-y < rct.h){
        y -= vy;
        if(y < 0) y = 0;
    }
}


struct MainGame
{
public:
    MainGame(SDL_Renderer* renderer){
        printf("Game Created\n");
        background = Background(renderer);
    }

    void handle_input(){
        player.handle_input();
        background.test_scroll();
        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_M]){
            camera.zoom = 2;
        }
        if(kbstate[SDL_SCANCODE_N]){
            camera.zoom = 1;
        }
        if(kbstate[SDL_SCANCODE_L]){
            camera.x += 5;
        }
        if(kbstate[SDL_SCANCODE_J]){
            camera.x -= 5;
        }
        if(kbstate[SDL_SCANCODE_I]){
            camera.y -= 5;
        }
        if(kbstate[SDL_SCANCODE_K]){
            camera.y += 5;
        }
    }

    void update(){
        camera.follow(player);
    }

    void draw(SDL_Renderer* renderer){
        background.draw(renderer, camera);
        player.draw(renderer, camera);    
    }
    

    GameObject player;
    Background background;
    Camera camera;
};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", screen_w, screen_h);
    log_system.init();

    //GameObject player;
    //Background back(game.renderer);

    MainGame mainGame(game.renderer);
    //printf("%d,%d,\n", game_.camera.x,game_.camera.y);
    while(game.running){
        
        SDL_RenderClear(game.renderer);

        mainGame.handle_input();
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

        mainGame.update();
        mainGame.draw(game.renderer);
    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
