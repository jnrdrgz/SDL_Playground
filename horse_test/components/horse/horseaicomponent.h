#ifndef HORSEAICOMPONENT_H
#define HORSEAICOMPONENT_H

#include "../base/updatecomponent.h"
#include "../../gameobject.h"
#include <stdio.h>

class HorseAIComponent : public UpdateComponent
{
private:
	int acum = 0;
	int max_vel = 0;
	bool runned = false;

public:
	int meters_to_finnish = 0;
	int laps_to_finnish = 0;
    int position = 0;
    
    HorseAIComponent(int meters_to_finnish, int laps_to_finnish, int max_vel);
    void update(GameObject& g, Uint32 dt);
};

#endif