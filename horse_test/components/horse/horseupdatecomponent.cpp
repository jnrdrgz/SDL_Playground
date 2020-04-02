#include "horseupdatecomponent.h"

void HorseUpdateComponent::update(GameObject& g, Uint32 dt){
	int new_x = g.get_x()+g.x_velocity;
    int new_y = g.get_y()+g.y_velocity;
    
    data->meters_to_finnish -= g.x_velocity;
    data->meters_to_travelled += g.x_velocity;
    data->laps_to_finnish = data->meters_to_finnish/(640*2);
    data->position = data->meters_to_travelled%640;

    if(!(new_x > 640-150))
        g.set_position(new_x, new_y);
    
    if(g.x_velocity <= 0){
        g.stop();
        g.x_velocity = 0;
    }
}