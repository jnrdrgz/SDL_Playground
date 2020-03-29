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
    HorseAIComponent(){}
    void update(GameObject& g, Uint32 dt);
};

#endif