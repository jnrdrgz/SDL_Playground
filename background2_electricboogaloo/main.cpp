#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"

#include <random>
#include <vector>
#include <ctime>

static LogSystem log_system = LogSystem();

class GameObject;

struct TimerClass
{
    int timestamp;
    TimerClass(){
        timestamp = (int)time(0);
    }

    ~TimerClass(){
        int duration = (int)time(0) - timestamp;
        printf("the timer longed: %d\n", duration);
    }

    TimerClass(const TimerClass& other_timer_class)
        : timestamp(other_timer_class.timestamp)
    {

    }

};

class Camera
{
private:
    bool pressed = false;
public:
    static int x,y;
    static float zoom;
    int camera_vel = 14;
    Camera(){
    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_RIGHT){
                x += camera_vel;
            }
            if(event.key.keysym.sym == SDLK_DOWN){
                y += camera_vel;
            }
            if(event.key.keysym.sym == SDLK_LEFT){
                x -= camera_vel;
            }
            if(event.key.keysym.sym == SDLK_UP){
                y -= camera_vel;
            }
            if(event.key.keysym.sym == SDLK_PLUS){
                if(!pressed){
                    zoom += 0.5f;
                    pressed = true;
                    printf("zoom: %.2f\n", zoom);
                }
            }
            if(event.key.keysym.sym == SDLK_MINUS){
                if(!pressed){
                    zoom -= 0.5f;
                    pressed = true;
                    printf("zoom: %.2f\n", zoom);
                }
            }
        }

        if(event.type == SDL_KEYUP){
            if(event.key.keysym.sym == SDLK_PLUS){
                if(pressed){
                    pressed = false;
                }
            }

            if(event.key.keysym.sym == SDLK_MINUS){
                if(pressed){
                    pressed = false;
                }
            }
        }
    }

    void follow(int when_obj_get_here, GameObject g);
    void update();

    //SDL_Rect convert_to_camera(SDL_Rect src, SDL_Rect dst){}
};

static Camera camera = Camera();
int Camera::x=0;
int Camera::y=0;
float Camera::zoom=1.0f;

SDL_Rect apply_camera_back_dst(SDL_Rect dst){
    SDL_Rect dst_2 = dst;
           
    if(Camera::x <= 0){
        dst_2.x -= Camera::x;
    } else if(Camera::x >= 480*4){
        dst_2.x -= Camera::x-(480*4);  
    } 
    if(Camera::y <= 0){
        dst_2.y -= Camera::y;
    } else if(Camera::y >= 480*0){
        dst_2.y -= Camera::y-(480*0);  
    }

    //dst_2.w = (int)((float)dst_2.w * Camera::zoom);
    //dst_2.h = (int)((float)dst_2.h * Camera::zoom);

    return dst_2;
}

SDL_Rect apply_camera_back_src(SDL_Rect src){
    SDL_Rect src_2 = src;
           
    if(Camera::x >= 480*4){
        src_2.x += 480*4;
    } else if (Camera::x > 0){
        src_2.x += Camera::x;
    }

//    if(!(Camera::y <= 0)){
//        src_2.y += Camera::y;
//    } 

    if(Camera::y >= 480*0){
        src_2.y += 480*0;
    } else if (Camera::y > 0){
        src_2.y += Camera::y;
    }


    //float val = (1.0f/Camera::zoom);

    //src_2.w = (int)((float)src_2.w * val);
    //src_2.h = (int)((float)src_2.h * val);


    //src_2.x = (int)((float)src_2.x * val);

    return src_2;
}


SDL_Rect apply_camera_object(SDL_Rect rct){
    SDL_Rect rct_2 = rct;
    rct_2.x = rct.x-Camera::x;
    rct_2.y = rct.y-Camera::y;
      
    //rct_2.w = (int)((float)rct_2.w * Camera::zoom);
    //rct_2.h = (int)((float)rct_2.h * Camera::zoom);
      

    return rct_2;
}


class Background
{
public:
    SDL_Texture* image = nullptr;
    int limit = 640*4;
    int laps = 0;

    SDL_Rect src = {0,0,640,480};//tmp_srf->w,tmp_srf->h};
    SDL_Rect dst = {0,0,640,480};

    Background(SDL_Renderer* renderer){
        SDL_Surface* tmp_srf = IMG_Load("bt2_2.png");
        image = SDL_CreateTextureFromSurface(renderer, tmp_srf);
        SDL_FreeSurface(tmp_srf);
    }

    SDL_Rect dst_2 = dst;
    SDL_Rect src_2 = src;
           
    void update(){
        //dst.x = Camera::x;
        //dst.y = Camera::y;

        dst_2 = apply_camera_back_dst(dst);
        src_2 = apply_camera_back_src(src);

    }

    void draw(SDL_Renderer* renderer){
        /*if(Camera::x <= 0){
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
        }*/

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
    SDL_Rect rct_2 = rct;
    bool started = false;

    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;

    float v = 0.0f;
    float s = 0.3f;
    float max_v = 10.0f;

    int laps = 0;

    GameObject(){}
    GameObject(float s){
        this->s = s;
    }

    GameObject(int x, int y, float s){
        this->s = s;
        rct.x = x;
        rct.y = y;

        rct_2 = rct;
    }


    GameObject(int x, int y, Uint8 r, Uint8 g, Uint8 b, float s){
        this->s = s;
        
        rct.x = x;
        rct.y = y;

        rct_2 = rct;

        this->r = r;
        this->g = g;
        this->b = b;
    }

    void set_position(int x, int y){
        rct.x = x;
        rct.y = y;

        rct_2 = rct;
    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_SPACE){
                started = !started;
                printf("input handled\n");
            }
        }
    }

    void update(){
        screenw_ip = 640 + 320; //screen w + initial object pos
        if(started){
            if(laps < 10){
                if(rct.x>(640*3)-(640/2)){
                    rct.x=640+(640/2);
                    //v = 0;
                    s = ((float)(rand()%5))/10.0f;
                    s += 0.1f;
                    v = 1.0f;
                    laps++;
                }
            }
            if(v < max_v){
                v += s;
            }
            rct.x += (int)v;    
        }

        rct_2 = apply_camera_object(rct);
    }

    void draw(SDL_Renderer* renderer){
        //rct_2.x = rct.x-Camera::x;
        //rct_2.y = rct.y-Camera::y;
        SDL_SetRenderDrawColor( renderer, r, g, b, 255);
        SDL_RenderFillRect(renderer, &rct_2);
    }

};

void Camera::follow(int when_obj_get_here, GameObject g){
    /*if(g.rct.x<640){
        if(x < g.rct_2.x-(640/2)){
            x += (int)g.v + 10;
        } 
    } else {
        x = g.rct_2.x-(640/2);
    }*/

    x = ((g.rct.x - (640*g.rct.x%4)))-(640/2);

}

void Camera::update(){
    x+=(640/25);
    if(x > 640*2){
        x = 640;
    }
}


int main(int argc, char* args[])
{
    Game game;
    TimerClass timer_1;
    
    game.init("test", 640, 480);
    log_system.init();

    Background background(game.renderer);

    srand(SDL_GetTicks());

    bool following =  true;

    //Camera::x = 640/2;
    following = false;

    int base_x = 320;
    int base_y = 480 - 40;
    GameObject cuad(base_x, base_y, 255, 0, 0, 0.2f);

    base_x += 15;
    base_y -= 50;

    std::vector<GameObject> gos;    
    for(int i = 0; i<4; i++){
        float s = ((float)(rand()%5))/10.0f;
        s += 0.05f;
        printf("created %d s: %.2f\n", i,s);
        gos.push_back(GameObject(base_x, base_y, rand()%255,rand()%255,rand()%255, s));
        
        base_x += 15;
        base_y -= 50;
    }


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
                if(game.event.key.keysym.sym == SDLK_f){
                    printf("f\n");
                    following = !following;
                }
                if(game.event.key.keysym.sym == SDLK_SPACE){
                    following = true;
                }
            }
            //background.handle_input(game.event);
            camera.handle_input(game.event);
            cuad.handle_input(game.event);

            for(auto &go : gos){
                go.handle_input(game.event);
            }

        }

        log_system.update_text("CAMERA_X", std::to_string(Camera::x), game.renderer);
        log_system.update_text("CAMERA_Y", std::to_string(Camera::y), game.renderer);
        log_system.update_text("GO_X", std::to_string(cuad.rct.x), game.renderer);

        cuad.update();
        background.update();
        for(auto &go : gos){
            go.update();
        }

        if(following) camera.follow(640/2, cuad);

        background.draw(game.renderer);
        cuad.draw(game.renderer);
        
        for(auto &go : gos){
            if(go.laps == cuad.laps) go.draw(game.renderer);
        }

        //if(following) camera.update();

        log_system.draw(game.renderer);
    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }

    //for(auto go : gos){
    //    delete go;
    //}
    //
    
    game.close();
}
