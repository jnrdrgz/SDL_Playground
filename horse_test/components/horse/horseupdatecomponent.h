#ifndef HORSEUPDATECOMPONENT_H
#define HORSEUPDATECOMPONENT_H

#include "../base/updatecomponent.h"
#include "../../gameobject.h"
#include <stdio.h>
#include "horsedata.h"

class HorseUpdateComponent : public UpdateComponent
{
public:
	HorseData* data;
	HorseUpdateComponent(HorseData* data){this->data = data;}
    void update(GameObject& g, Uint32 dt) override;
};

#endif