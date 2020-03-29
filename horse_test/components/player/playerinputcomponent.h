#ifndef PLAYERINPUTCOMPONENT_H
#define PLAYERINPUTCOMPONENT_H

#include "../base/inputcomponent.h"
#include "../../gameobject.h"
#include <stdio.h>

class PlayerInputComponent : public InputComponent
{
public:
    PlayerInputComponent(){}
    void handle_input(GameObject& g, SDL_Event event);
private:
    int jumps_left = 2;
    bool jumping = false;
    int max_vel = 20;
};

#endif