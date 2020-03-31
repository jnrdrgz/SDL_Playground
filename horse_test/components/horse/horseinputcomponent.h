#ifndef HORSEINPUTCOMPONENT_H
#define HORSEINPUTCOMPONENT_H

#include "../base/inputcomponent.h"
#include "../../gameobject.h"
#include <stdio.h>

class HorseInputComponent : public InputComponent
{
public:
    HorseInputComponent(){}
    void handle_input(GameObject& g, SDL_Event event) override;
};

#endif