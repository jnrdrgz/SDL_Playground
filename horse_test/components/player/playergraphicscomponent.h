#ifndef PLAYERGRAPHICSCOMPONENT_H
#define PLAYERGRAPHICSCOMPONENT_H

#include "../base/graphicscomponent.h"
#include "../../gameobject.h"
#include <stdio.h>

class PlayerGraphicsComponent : public GraphicsComponent
{
public:
    PlayerGraphicsComponent(){}
    void draw(GameObject& g, SDL_Renderer *renderer);
};

#endif