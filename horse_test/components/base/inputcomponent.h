#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include <SDL2/SDL.h>
#include "base.h"

class InputComponent
{
public:
    virtual void handle_input(GameObject& g, SDL_Event event) = 0;
};
#endif