#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "bending_bar_controller.h"

#include <random>
#include <vector>
#include <ctime>
#include <cstring>

static LogSystem log_system = LogSystem();

class GameObject;

struct TimerClass
{
    int timestamp;
    char* name;
    size_t length;

    TimerClass(const char* new_name){
        timestamp = (int)time(0);

        const auto new_name_length = strlen(new_name);
        printf("%lu\n", new_name_length);
        name = new char[new_name_length+1];

        std::strcpy(name, new_name);
        name[new_name_length] = 0;
    }

    ~TimerClass(){
        int duration = (int)time(0) - timestamp;
        
        if(name){
            printf("the timer %s longed: %d seconds\n", name, duration);
            delete[] name;
        }
    }

    TimerClass(const TimerClass& other_timer_class)
        : timestamp(other_timer_class.timestamp)
    {

    }

    //move data from other_timer to this and left other zeroed
    TimerClass(TimerClass&& other_timer_class) noexcept
        : timestamp(other_timer_class.timestamp),
        name(other_timer_class.name)
    {
        other_timer_class.timestamp = 0;
        other_timer_class.name = nullptr;

    }

    TimerClass& operator=(TimerClass&& other_timer) noexcept
    {
        if(this == &other_timer) return *this;

        delete[] name;

        name = other_timer.name;
        timestamp = other_timer.timestamp;
        other_timer.name = nullptr;
        other_timer.timestamp = 0;

        return *this;
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

            if(event.key.keysym.sym == SDLK_t){
                camera_vel++;
                printf("camera vel: %d\n", camera_vel);
            }

            if(event.key.keysym.sym == SDLK_g){
                camera_vel--;
                printf("camera vel: %d\n", camera_vel);
            }
            if(event.key.keysym.sym == SDLK_PLUS){
                if(!pressed){
                    zoom += 0.25f;
                    pressed = true;
                    printf("zoom: %.2f\n", zoom);
                }
            }
            if(event.key.keysym.sym == SDLK_MINUS){
                if(!pressed){
                    if(zoom > 0.25f) zoom -= 0.25f;
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
           
    int initial_pos = 0; // this should came from somewhere 
    int back_img_length_x = (640*12) + initial_pos;
    if(Camera::x <= 0){
        dst_2.x -= Camera::x;
    } else if(Camera::x >= back_img_length_x){
        dst_2.x -= Camera::x-back_img_length_x;  
    } 
    if(Camera::y <= 0){
        dst_2.y -= Camera::y;
    } else if(Camera::y >= 480*0){
        dst_2.y -= Camera::y-(480*0);  
    }

    //dst_2.w = (int)((float)dst_2.w * Camera::zoom);
    //dst_2.h = (int)((float)dst_2.h * Camera::zoom);

    if(Camera::zoom < 1.0f){
        dst_2.h = 100;
    }


    return dst_2;
}

SDL_Rect apply_camera_back_src(SDL_Rect src){
    SDL_Rect src_2 = src;

    int initial_pos = 0; // this should came from somewhere 
    int back_img_length_x = (640*11) + initial_pos;
           
    if(Camera::x >=  back_img_length_x){
        src_2.x +=  back_img_length_x;
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

    /*if(Camera::zoom != 1.0f || Camera::zoom != 0.75f){
        src_2.w = (640*4)*(1.0/Camera::zoom);
    } else {
        src_2.w *= 1.0f; 
    }*/

    if(Camera::zoom < 1.0f){
        src_2.w = 7680;
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
    rct_2.w = (int)((float)rct_2.w * Camera::zoom);
    rct_2.h = (int)((float)rct_2.h * Camera::zoom);
      

    return rct_2;
}


class Background
{
public:
    SDL_Texture* image = nullptr;
    int limit = 640*12;
    int laps = 0;

    SDL_Rect src = {0,0,640,480};//tmp_srf->w,tmp_srf->h};
    SDL_Rect dst = {0,0,640,480};

    Background(){
    }


    Background(SDL_Renderer* renderer){
        SDL_Surface* tmp_srf = IMG_Load("race_pit.png");
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

    float v = 1.0f;
    float s = 0.3f;
    float max_v = 20.0f;

    int laps = 0;

    bool TEMP_player = false;

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

    GameObject(int x, int y, Uint8 r, Uint8 g, Uint8 b, float s, bool TEMP_player){
        this->s = s;
        
        rct.x = x;
        rct.y = y;

        rct_2 = rct;

        this->r = r;
        this->g = g;
        this->b = b;

        this->TEMP_player = TEMP_player;
    }

    void set_position(int x, int y){
        rct.x = x;
        rct.y = y;

        rct_2 = rct;
    }
    
    void set_speed(float s){
        this->s = s;
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
        //screenw_ip = 640 + 320; //screen w + initial object pos
        //if(TEMP_player) b = 0;
        if(started){
            /*if(laps < 5){
                if(rct.x>( (640*3)-(640/2) )){
                    rct.x=640+(640/2) ;
                    //v = 0;
                    s = ((float)(rand()%5))/10.0f;
                    s += 0.1f;
                    //if(TEMP_player) s -= 0.05f;
                    v = 1.0f;
                    laps++;
                }
            }*/

            
            rct.x += (int)v;    
        }

        if(rct.x > (640*12)){
            v = 0;
            s = 0;
        }

        rct_2 = apply_camera_object(rct);
    }

    void accelerate(){
        if(v < max_v){ v += s; }
    }

    void accelerate(float s){
        if(v < max_v){ v += s; }
    }

    void reduce_v(){
        
    }

    void stop(){
        v = 0;
        s = 0;
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

    //640 screenwidth
    //g.rct.x position of the object in respect of the total map
    //(it can be more than 604)
    //mod 4 to get the position in the actual camera position 
    //4 is the total number of panels of the background
    
    int screen_w = 640;
    int middle_of_screen = screen_w/2;
    int total_background_panels = 4;

    int position_in_frame = screen_w * g.rct.x % total_background_panels; 
    x = g.rct.x - position_in_frame - middle_of_screen;

}

void Camera::update(){
    x+=(640/25);
    if(x > 640*2){
        x = 640;
    }
}

class MainGame
{
public:
    Background background;

    bool following = false;

    int base_x = 320;
    int base_y = 480 - 40;

    GameObject player;
    std::vector<GameObject> rivals;
    std::vector<GameObject> tigers;   

    //controllers
    BendingBarController bender_controller;
    Uint32 timeout = SDL_GetTicks() + 500;

    MainGame(SDL_Renderer* renderer){
        init(renderer);
    }

    void init(SDL_Renderer* renderer){
        background = Background(renderer);
        bender_controller = BendingBarController(350, 20, 100, 20);
        player = GameObject(base_x, base_y, 255, 0, 0, 1.0f);

        base_x += 15;
        base_y -= 50;

        std::vector<float> velocities {1.0f,1.0f,1.0f,1.0f};

        for(int i = 0; i<4; i++){
            float s = ((float)(rand()%5))/10.0f;
            //s += 0.05f;
            printf("created %d s: %.2f\n", i,s);
            rivals.push_back(GameObject(base_x, base_y, 0xed,0xff,0xea, velocities[i]));

            s -= 0.20f;
            //s += 0.05f;
            tigers.push_back(GameObject(base_x-50, base_y, 0,0,0, velocities[i]));
            
            base_x += 15;
            base_y -= 50;
        }

        base_x = 320;
        base_y = 480 - 40;
        
        bender_controller.start();
    }

    void reset(){
        player.set_position(base_x, base_y);
        player.set_speed(0.02f);

        base_x += 15;
        base_y -= 50;

        for(int i = 0; i<4; i++){
            float s = ((float)(rand()%5))/10.0f;
            s += 0.05f;

            rivals[i].stop();
            rivals[i].set_position(base_x, base_y);
            rivals[i].set_speed(s);
            
            
            s = ((float)(rand()%5))/10.0f;
            s += 0.05f;
            tigers[i].stop();
            tigers[i].set_position(base_x-50, base_y);
            tigers[i].set_speed(s);
            

            base_x += 15;
            base_y -= 50;
        }

        base_x = 320;
        base_y = 480 - 40;
        
    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_f){
                printf("f\n");
                following = !following;
            }
            if(event.key.keysym.sym == SDLK_SPACE){
                following = true;
            }

            if(event.key.keysym.sym == SDLK_PLUS){
                //cuad.v += 0.3f;
                //cuad.accelerate();
                //player.v += 1.0f;
                
            }
            if(event.key.keysym.sym == SDLK_MINUS){
                //cuad.v -= 0.3f;
                //player.v -= 1.0f;
                
            }
            if(event.key.keysym.sym == SDLK_h){
                //for(auto &rival : rivals){
                //    rival.stop();
                //}

                for(int i = 0; i < rivals.size(); i++){
                    rivals[i].v--;//.stop();
                    //tigers[i].stop();
                }
            }

            if(event.key.keysym.sym == SDLK_b){
                printf("bender val: %.2f\n", bender_controller.get_controller_value());
                if(bender_controller.get_controller_value() < 1.0f){
                    printf("HIT!\n");
                    player.v += 1.0f;
                    bender_controller.speed_up(1);
                } else {
                    printf("MISS!\n");
                    player.v -= 1.0f;
                    bender_controller.slow_down(1);
                }
            }
        }

        player.handle_input(event);    
        //for(auto &rival : rivals){
        //    rival.handle_input(event);
        //}


        for(int i = 0; i < rivals.size(); i++){
            rivals[i].handle_input(event);
            tigers[i].handle_input(event);
        }

    }

    void update(){
        player.update();
        background.update();
        bender_controller.update();

        if(SDL_TICKS_PASSED(SDL_GetTicks(), timeout)){
            for(int i = 0; i < rivals.size(); i++){
                rivals[i].accelerate();
                tigers[i].accelerate();
                //if(rivals[i].rct.x > 3000)
                //    tigers[i].accelerate();
                //    tigers[i].accelerate();
                //    tigers[i].accelerate();
            }

            player.accelerate();

            //printf("acc\n");

            timeout = SDL_GetTicks() + 200+(rand()%200);
        }

        //for(auto &rival : rivals){
        //    rival.update();
        //}

        for(int i = 0; i < rivals.size(); i++){
            rivals[i].update();
            tigers[i].update();

            if(tigers[i].rct.x+tigers[i].rct.w > rivals[i].rct.x){
                rivals[i].stop();
                tigers[i].stop();
            }
        }

        if(following) camera.follow(640/2, player);
    }

    void draw(SDL_Renderer* renderer){
        background.draw(renderer);
        player.draw(renderer);
        
        //for(auto &rival : rivals){
        //    rival.draw(renderer);
        //}

        for(int i = 0; i < rivals.size(); i++){
            rivals[i].draw(renderer);
            tigers[i].draw(renderer);
        }


        bender_controller.draw(renderer);
    }

};

int main(int argc, char* args[])
{
    Game game;
    TimerClass timer_1("GAME DURATION TIMER");
    
    game.init("test", 640, 480);
    log_system.init();

    MainGame mainGame(game.renderer);

    srand(SDL_GetTicks());
    
    log_system.add_text("CAMERA_X", std::to_string(Camera::x), game.renderer);
    log_system.add_text("CAMERA_Y", std::to_string(Camera::y), game.renderer);

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
                if(game.event.key.keysym.sym == SDLK_r){
                    mainGame.reset();
                }
            }

            camera.handle_input(game.event);
            mainGame.handle_input(game.event);

        }

        mainGame.update();
        mainGame.draw(game.renderer);

        log_system.update_text("CAMERA_X", std::to_string(Camera::x), game.renderer);
        log_system.update_text("CAMERA_Y", std::to_string(Camera::y), game.renderer);
        log_system.draw(game.renderer);
        
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
    }

    game.close();
}
