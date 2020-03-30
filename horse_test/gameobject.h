#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <unordered_map>
#include <string>
#include "sprite.h"
#include "components/base/inputcomponent.h"
#include "components/base/graphicscomponent.h"
#include "components/base/updatecomponent.h"

class GameObject
{
private:
    SDL_Rect rct;
    std::unordered_map<std::string, Sprite> sprites;
    std::string current_sprite_tag;

    InputComponent* inputcomponent = nullptr;
    GraphicsComponent* graphicscomponent = nullptr;
    UpdateComponent* updatecomponent = nullptr;
public:
    Sprite current_sprite;
    int x_velocity = 0;
    int y_velocity = 0;
    int anim_vel = 1000;
    
    GameObject(){}
    GameObject( SDL_Renderer* renderer, 
                GraphicsComponent* graphicscomponent, 
                InputComponent* inputcomponent,
                UpdateComponent* updatecomponent);

    void destroy();

    void draw(SDL_Renderer* renderer, int dt);
    void handle_input(SDL_Event event);
    
    bool current_sprite_finished_animation();

    //get
    Sprite get_sprite(std::string name);
    SDL_Rect get_rct();
    
    //position and velocity
    int get_x();
    int get_y();
    int get_vel_x();
    int get_vel_y();

    //set
    void set_current_sprite(Sprite sprite);
    void set_position(int x, int y);
    void set_anim_vel(int v);
    void set_size(int w, int h);

    
    //sprite
    void add_sprite(std::string name, Sprite sprite);

    //animation
    void stop();
    void run();
    void update_sprite_animation(Uint32 dt);

    void sum_velocity(int x, int y);

    virtual void update(SDL_Renderer* renderer);
    void update_r(Uint32 dt);
};

#endif