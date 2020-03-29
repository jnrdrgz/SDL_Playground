#include "horseaicomponent.h"

void HorseAIComponent::update(GameObject& g, Uint32 dt){
	if(!runned){
		g.run();
		runned = true;
	}

	int new_x = g.get_x()+g.x_velocity;
    int new_y = g.get_y()+g.y_velocity;
    
    if(!(new_x > 640-150))
        g.set_position(new_x, new_y);

    if(g.x_velocity < 7 && acum < 2000){
    	g.x_velocity += 1;
    }
    
    if(g.x_velocity <= 0){
        g.stop();
        g.x_velocity = 0;
    }

    if(dt < 1000)
    	acum += dt;
}