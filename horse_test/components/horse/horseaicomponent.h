#ifndef HORSEAICOMPONENT_H
#define HORSEAICOMPONENT_H

#include "../base/updatecomponent.h"
#include "../../gameobject.h"
#include <stdio.h>
#include "horsedata.h"

class HorseAIComponent : public UpdateComponent
{
private:
	int acum = 0;
	int max_vel = 0;
	bool runned = false;

/*	
	int meters_to_finnish = 0;
	int laps_to_finnish = 0;
    int position = 0;
    int meters_to_travelled = 0;
*/
public:
	HorseData* data = nullptr;

    HorseAIComponent(HorseData* data);
    void update(GameObject& g, Uint32 dt) override;
};

#endif