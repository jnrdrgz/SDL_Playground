#include "horseupdatecomponent.h"

void HorseUpdateComponent::update(GameObject& g, Uint32 dt){
	int new_x = g.get_x()+g.x_velocity;
    int new_y = g.get_y()+g.y_velocity;
    
    if(!(new_x > 640-150))
        g.set_position(new_x, new_y);
    
    if(g.x_velocity <= 0){
        g.stop();
        g.x_velocity = 0;
    }
}