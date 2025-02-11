#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include <SDL2/SDL.h>
#include "../sprite/sprite.h"
#include <unordered_map>
#include "../controllers/bending_bar_controller.h"

static LogSystem log_system = LogSystem();

class InputHandler;

class GameObject
{
private:
    SDL_Rect rct;
    std::unordered_map<std::string, Sprite> sprites;
    Sprite current_sprite;
    InputHandler* inputHandler;
    std::string current_sprite_tag;
    int animation_speed = 0;////->>>>>

public:
    int x_velocity = 0;
    int y_velocity = 0;


    void draw(SDL_Renderer* renderer){      
        current_sprite.draw(renderer);
    }

    bool current_sprite_finished_animation(){
        return current_sprite.animiation_finished();
    }

    //get
    Sprite get_sprite(std::string name){return sprites[name];}

    int get_x(){return rct.x;}

    int get_y(){return rct.y;}

    SDL_Rect get_rct(){return current_sprite.get_rct();}
    
    int get_vel_x(){return x_velocity;}
    int get_vel_y(){return y_velocity;}

    //set
    void set_current_sprite(Sprite sprite){
        if(!(sprite == current_sprite)){
            current_sprite = sprite;
        }
    }
    void set_position(int x, int y){
        current_sprite.TESTING_set_position(x, y);
        rct.x = x;
        rct.y = y;
    }

    void set_anim_vel(int v){
        current_sprite.set_timer_limit(v);
    }

    //sprite
    void add_sprite(std::string name, Sprite sprite){
        sprites[name] = sprite;
    }

    void stop(){
        current_sprite.stop();
    }

    void run(){
        current_sprite.run();
    }

    void update_sprite_animation(Uint32 dt){
        current_sprite.update(dt);
    }


    virtual void update(SDL_Renderer* renderer, Uint32 dt){

    }
};

class Horse : public GameObject
{
public:
    int anim_vel = 1000;

    Horse(SDL_Renderer* renderer){
        
        /*********************

        !!!!!!!!!!!!
        !ADD TEXTURE MANAGER TO NOT LOAD THE SAME TEXTURE FOUR TIMES !!!!!!!!!!!!!!!! 
        !!!!!!!!!!!!

        **********************/

        Sprite horse_running_right(renderer, "horse_filt.png", 0, 25, 20, 100, 100, 1*1000);
        horse_running_right.set_static_sprite(9);

        add_sprite("running_right", horse_running_right);


        set_current_sprite(get_sprite("running_right"));
        set_position(0, 180);   
        horse_running_right.set_size(140,140);
        

        stop();   


        //horse running right 0,25
    }

    void update(SDL_Renderer* renderer, Uint32 dt) override{
        
        int new_x = get_x()+x_velocity;
        int new_y = get_y()+y_velocity;
        
        if(!(new_x > 640-150))
            set_position(new_x, new_y);
        
        if(x_velocity <= 0){
            stop();
            x_velocity = 0;
        }

        update_sprite_animation(dt);
        draw(renderer);

    }

    SDL_Rect get_rct(){
        return GameObject::get_rct();
    }


    void set_velocity(int x, int y){
        x_velocity = x;
        y_velocity = y;
    }

    void sum_velocity(int x, int y){
        x_velocity += x;
        y_velocity += y;
    }

    void run(){
        GameObject::run();
    }
    void stop(){
        GameObject::stop();
    }

    int get_anim_vel(){
        return anim_vel;
    }

    void handle_input(SDL_Event event){
        if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.sym == SDLK_RIGHT){
                //set_velocity(10, 0);
                //run();
            }
            if(event.key.keysym.sym == SDLK_LEFT){
                //set_velocity(-1, 0);
                //stop();
            }
            if(event.key.keysym.sym == SDLK_SPACE){
                //set_velocity(0, 0);
                run();
                //horse.stop();
            }
            if (event.key.keysym.sym == SDLK_UP){
                run();
                
                sum_velocity(1,0);
                anim_vel -= 50;
                GameObject::set_anim_vel(anim_vel);
            }
            if (event.key.keysym.sym == SDLK_DOWN){
                sum_velocity(-1,0);
                anim_vel += 50;
                GameObject::set_anim_vel(anim_vel);
            }
        }
    }

};

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



    void scroll(int v){
        if(src.x < 640*3){
            if(laps != 10){
                if(src.x < limit-(640*2)){
                    src.x += v;
                } else {
                    src.x = 640;
                    src.x += v;
                    laps++;
                }
            } else {
                src.x += 10;
            }
        }
    }

    //scroll following a reference object or rct
    void reference_scroll(GameObject object){
        if(object.get_rct().x > 640-200){
            scroll(object.get_vel_x());
        }
    }

    void draw(SDL_Renderer* renderer){
        SDL_RenderCopy(renderer, image, &src, &dst);
    }



};


int main(int argc, char* args[])
{
    Game game;

    int screenW = 640;
    int screenH = 480;
    game.init("test", screenW, screenH);
    log_system.init();

    
    /*SDL_Surface* tmp_srf = IMG_Load("back_test.png");
    SDL_Texture* background = SDL_CreateTextureFromSurface(game.renderer, tmp_srf);
    SDL_Rect src = {0,0,640,480};//tmp_srf->w,tmp_srf->h};

    int limit = 640*4;
    
    SDL_FreeSurface(tmp_srf);

    SDL_Rect dst = {0,0,screenW, screenH};

    int laps = 0;*/

    //background
    Background background(game.renderer);

    //game objects
    Horse horse(game.renderer);

    //controllers
    BendingBarController bender_controller(350, 20, 100, 20);

    log_system.add_text("Laps", std::to_string(background.laps), game.renderer);
    log_system.add_text("Horse_VEL", std::to_string(horse.get_vel_x()), game.renderer);
    log_system.add_text("Horse_ANIMVEL", std::to_string(horse.get_anim_vel()), game.renderer);

    Uint32 start_time_frame = 0;
    Uint32 dt = 0;

    while(game.running){
        start_time_frame = SDL_GetTicks();
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            horse.handle_input(game.event);
            bender_controller.handle_input(game.event);
        }

        background.draw(game.renderer);
        
        horse.update(game.renderer, dt);
        bender_controller.update();
        background.reference_scroll(horse);

        bender_controller.draw(game.renderer);

        log_system.update_text("Laps", std::to_string(background.laps), game.renderer);
        log_system.update_text("Horse_VEL", std::to_string(horse.get_vel_x()), game.renderer);
        log_system.update_text("Horse_ANIMVEL", std::to_string(horse.get_anim_vel()), game.renderer);

        log_system.draw(game.renderer);


        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);
        dt = (SDL_GetTicks() - start_time_frame);
        
    }
    
    game.close();
}