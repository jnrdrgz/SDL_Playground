#ifndef HORSEUPDATECOMPONENT_H
#define HORSEUPDATECOMPONENT_H

#include "../base/updatecomponent.h"
#include "../../gameobject.h"
#include <stdio.h>

class HorseUpdateComponent : public UpdateComponent
{
public:
    HorseUpdateComponent(){}
    void update(GameObject& g, Uint32 dt);
};

#endif