#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include <SDL2/SDL.h>
#include "base.h"

class AIComponent
{
public:
    virtual void update(GameObject& g) = 0;
};
#endif