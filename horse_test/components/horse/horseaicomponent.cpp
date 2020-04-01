#include "horseaicomponent.h"

HorseAIComponent::HorseAIComponent(HorseData* data){
    this->data = data;
}
    

void HorseAIComponent::update(GameObject& g, Uint32 dt){
	if(!runned){
		g.run();
		runned = true;
	}

	int new_x = g.get_x()+g.x_velocity;
    int new_y = g.get_y()+g.y_velocity;
    
    data->meters_to_finnish -= g.x_velocity;
    data->meters_to_travelled += g.x_velocity;
    data->laps_to_finnish = data->meters_to_finnish/(640*2);
    data->position = data->meters_to_travelled%640;


    //printf("laps_to_finnish: %d\n", laps_to_finnish);
    //printf("meters_to_finnish: %d\n", meters_to_finnish);

    
    //if(!(new_x > 640-150))
    g.set_position(data->position, new_y);

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