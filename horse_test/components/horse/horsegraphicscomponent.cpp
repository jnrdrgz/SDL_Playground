#include "horsegraphicscomponent.h"

void HorseGraphicsComponent::load(GameObject& g, SDL_Renderer *renderer){
	Sprite horse_running_right(renderer, "horse_filt.png", 0, 25, 20, 100, 100, 1*1000);
    horse_running_right.set_static_sprite(9);
    horse_running_right.set_size(140,140);

    g.add_sprite("running_right", horse_running_right);
    g.set_current_sprite(g.get_sprite("running_right"));
    g.set_position(0, 180);   

    g.stop();
}

void HorseGraphicsComponent::draw(GameObject& g, SDL_Renderer *renderer, Uint32 dt){
	g.update_sprite_animation(dt);
	g.current_sprite.draw(renderer);
}