#include "horseaicomponent.h"

HorseAIComponent::HorseAIComponent(int meters_to_finnish, int laps_to_finnish, int max_vel){
    this->meters_to_finnish = meters_to_finnish;
    this->laps_to_finnish = laps_to_finnish;
    this->max_vel = max_vel;
}
    

void HorseAIComponent::update(GameObject& g, Uint32 dt){
	if(!runned){
		g.run();
		runned = true;
	}

	int new_x = g.get_x()+g.x_velocity;
    int new_y = g.get_y()+g.y_velocity;

    
    meters_to_finnish -= g.x_velocity;
    laps_to_finnish = meters_to_finnish/640;
    position = meters_to_finnish%640;


    printf("laps_to_finnish: %d\n", laps_to_finnish);
    printf("meters_to_finnish: %d\n", meters_to_finnish);

    
    //if(!(new_x > 640-150))
    g.set_position(position, new_y);

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