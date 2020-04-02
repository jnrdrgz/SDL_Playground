#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

static LogSystem log_system = LogSystem();

class Camera
{
public:
    static int x,y,zoom;
    Camera(){
    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_RIGHT){
                x += 10;
            }
            if(event.key.keysym.sym == SDLK_DOWN){
                y += 10;
            }
            if(event.key.keysym.sym == SDLK_LEFT){
                x -= 10;
            }
            if(event.key.keysym.sym == SDLK_UP){
                y -= 10;
            }
        }
    }

    //SDL_Rect convert_to_camera(SDL_Rect src, SDL_Rect dst){}
};

static Camera camera = Camera();
int Camera::x=0;
int Camera::y=0;
int Camera::zoom=1;

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

    void update(){
        //dst.x = Camera::x;
        //dst.y = Camera::y;


    }

    void draw(SDL_Renderer* renderer){
        SDL_Rect dst_2 = dst;
        SDL_Rect src_2 = src;
           
        if(Camera::x <= 0){
            dst_2.x -= Camera::x;
        } else if(Camera::x >= 480*4){
            dst_2.x -= Camera::x-(480*4);
            src_2.x += 480*4;
        } else {
            src_2.x += Camera::x;
        }


        if(Camera::y <= 0){
            dst_2.y -= Camera::y;
        } else {
            src_2.y += Camera::y;
        }

        SDL_RenderCopy(renderer, image, &src_2, &dst_2);
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

class GameObject
{
public:
    SDL_Rect rct = {100,100,40,40};
    bool started = false;


    GameObject(){}

    void handle_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_SPACE){
                started = !started;
            }
        }
    }

    void update(){
        if(started){
            if(rct.x>480*3){
                rct.x=480;
            }
            //} else {
            
            //}
            rct.x += 4;    
        }
    }

    void draw(SDL_Renderer* renderer){
        SDL_Rect rct_2 = rct;
        rct_2.x = rct.x-Camera::x;
        rct_2.y = rct.y-Camera::y;
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rct_2);
    }

};


int main(int argc, char* args[])
{
    Game game;
    game.init("test", 640, 480);
    log_system.init();

    Background background(game.renderer);

    GameObject cuad;

    log_system.add_text("CAMERA_X", std::to_string(Camera::x), game.renderer);
    log_system.add_text("CAMERA_Y", std::to_string(Camera::y), game.renderer);
    log_system.add_text("GO_X", std::to_string(cuad.rct.x), game.renderer);

    while(game.running){
        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_ESCAPE){
                    game.running = false;
                }
            }
            //background.handle_input(game.event);
            camera.handle_input(game.event);
            cuad.handle_input(game.event);
        }

        log_system.update_text("CAMERA_X", std::to_string(Camera::x), game.renderer);
        log_system.update_text("CAMERA_Y", std::to_string(Camera::y), game.renderer);
        log_system.update_text("GO_X", std::to_string(cuad.rct.x), game.renderer);

        cuad.update();

        background.draw(game.renderer);

        cuad.draw(game.renderer);

        log_system.draw(game.renderer);

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
