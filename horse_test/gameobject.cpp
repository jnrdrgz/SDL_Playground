#include "gameobject.h"

GameObject::GameObject(SDL_Renderer* renderer, 
                GraphicsComponent* graphicscomponent, 
                InputComponent* inputcomponent,
                UpdateComponent* updatecomponent)
{
    this->inputcomponent = inputcomponent;
    this->graphicscomponent = graphicscomponent;
    this->updatecomponent = updatecomponent;

    graphicscomponent->load(*this, renderer);
}

void GameObject::draw(SDL_Renderer* renderer, int dt){
    if(graphicscomponent != nullptr){
        graphicscomponent->draw(*this, renderer, dt);
    } else{
        current_sprite.draw(renderer);
    }
    
}

void GameObject::destroy(){
    for(auto it = sprites.begin(); it != sprites.end(); ++it){
        it->second.destroy();
    }
    
    inputcomponent = nullptr;
    graphicscomponent = nullptr;
    updatecomponent = nullptr;
}

void GameObject::handle_input(SDL_Event event){
    inputcomponent->handle_input(*this, event);
}

bool GameObject::current_sprite_finished_animation(){
    return current_sprite.animiation_finished();
}

//get
Sprite GameObject::get_sprite(std::string name){return sprites[name];}

int GameObject::get_x(){return rct.x;}

int GameObject::get_y(){return rct.y;}

SDL_Rect GameObject::get_rct(){return current_sprite.get_rct();}

int GameObject::get_vel_x(){return x_velocity;}
int GameObject::get_vel_y(){return y_velocity;}

//set
void GameObject::set_current_sprite(Sprite sprite){
    if(!(sprite == current_sprite)){
        current_sprite = sprite;
    }
}
void GameObject::set_position(int x, int y){
    current_sprite.TESTING_set_position(x, y);
    rct.x = x;
    rct.y = y;
}

void GameObject::set_anim_vel(int v){
    current_sprite.set_timer_limit(v);
}

void GameObject::set_size(int w, int h){
    current_sprite.set_size(w, h);
}

void GameObject::set_color_mod(Uint8 r, Uint8 g, Uint8 b){
    current_sprite.set_color_mod(r,g,b);
    for(auto it = sprites.begin(); it != sprites.end(); ++it){
        it->second.set_color_mod(r,g,b);
    }
}
//sprite
void GameObject::add_sprite(std::string name, Sprite sprite){
    sprites[name] = sprite;
}

void GameObject::stop(){
    current_sprite.stop();
}

void GameObject::run(){
    current_sprite.run();
}


void GameObject::sum_velocity(int x, int y){
    x_velocity += x;
    y_velocity += y;
}

void GameObject::update_sprite_animation(Uint32 dt){
    current_sprite.update(dt);
}

void GameObject::update(SDL_Renderer* renderer){

}

void GameObject::update_r(Uint32 dt){
    updatecomponent->update(*this, dt);
}