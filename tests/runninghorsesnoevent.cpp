#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../sprite/sprite.h"
#include <unordered_map>

class InputHandler;

class GameObject
{
private:
    SDL_Rect rct;
    std::unordered_map<std::string, Sprite> sprites;
    Sprite current_sprite;
    InputHandler* inputHandler;

public:
    void add_sprite(std::string name, Sprite sprite){
        sprites[name] = sprite;
    }

    void draw(SDL_Renderer* renderer){      
        current_sprite.draw(renderer);
    }

    Sprite get_sprite(std::string name){
        return sprites[name];
    }

    void set_current_sprite(Sprite sprite){
        current_sprite = sprite;
    }

    void set_position(int x, int y){
        current_sprite.TESTING_set_position(x, y);
        rct.x = x;
        rct.y = y;
    }

    int get_x(){
        return rct.x;
    }
    int get_y(){
        return rct.y;
    }


    void update_sprite_animation(Uint32 dt){
        current_sprite.update(dt);
    }

    virtual void update(SDL_Renderer* renderer, Uint32 dt){

    }
};

class Horse : GameObject
{
public:
    bool running_right = true;
    bool running_left = false;
    bool turning_right = false;
    bool turning_left = false;
    int velocity = 2;

    Horse(SDL_Renderer* renderer){
        
        /*********************

        !!!!!!!!!!!!
        !ADD TEXTURE MANAGER TO NOT LOAD THE SAME TEXTURE FOUR TIMES !!!!!!!!!!!!!!!! 
        !!!!!!!!!!!!

        **********************/

        Sprite horse_running_right(renderer, "horse.png", 0, 25, 20, 100, 100, 1*1000);
        Sprite horse_turning_left(renderer, "horse.png", 50, 115-50, 20, 100, 100, 2*1000);
        Sprite horse_running_left(renderer, "horse.png", 116, 140-116, 20, 100, 100, 1*1000);
        Sprite horse_turning_right(renderer, "horse.png", 177, 240-177, 20, 100, 100, 2*1000);

        add_sprite("running_right", horse_running_right);
        add_sprite("turning_right", horse_turning_left);
        add_sprite("running_left", horse_running_left);
        add_sprite("turning_right", horse_turning_right);

        set_current_sprite(get_sprite("running_right"));
        set_position(0, 150);        


        //horse running right 0,25
        //horse turning to left 50,115
        //horse running left 116,140
        //horse turning to right 177, 240
    }

    void update(SDL_Renderer* renderer, Uint32 dt) override{
        update_sprite_animation(dt);
        draw(renderer);

        int new_x = get_x()+velocity;

        

        if(new_x > 640-100){
            running_left = true;
            velocity = -2;
        }
        else if(new_x < 0){
            running_right = true;
            velocity = 2;
        }

        if(running_right){
            set_current_sprite(get_sprite("running_right"));
            running_right = false;
        }
        if(running_left){
            set_current_sprite(get_sprite("running_left"));
            running_left = false;
        }


        set_position(new_x, get_y());
        
    }
};

class InputHandler
{
public:
    bool clicked = false;
    bool pressed = false;
    virtual void handle_input(SDL_Event e, GameObject* gameObject);
};

class HorseInputHandler : InputHandler
{
public:
    void handle_input(SDL_Event e, GameObject* gameObject) override{
        if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == SDL_BUTTON_LEFT && !this->clicked){
                printf("Horse click down\n");
                clicked = true;    
            }
        }

        if(e.type == SDL_MOUSEBUTTONUP){
            if(e.button.button == SDL_BUTTON_LEFT && !this->clicked){
                printf("Horse click up\n");
                clicked = false;    
            }
        }
    }
};



int main(int argc, char* args[])
{
    Game game;
    game.init("test", 640, 480);
    
    Horse horse(game.renderer);

    //horse running right 0,25
    //horse turning to left 50,115
    //horse running left 116,140
    //horse turning to right 177, 240

    Uint32 start_time_frame = 0;
    Uint32 dt = 0;

    while(game.running){
        start_time_frame = SDL_GetTicks();
        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
    
            }

        }

        horse.update(game.renderer, dt);
        

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

        dt = (SDL_GetTicks() - start_time_frame);
    }
    
    game.close();
}