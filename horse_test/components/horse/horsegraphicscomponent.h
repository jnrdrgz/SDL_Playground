#ifndef HORSEGRAPHICSCOMPONENT_H
#define HORSEGRAPHICSCOMPONENT_H

#include "../base/graphicscomponent.h"
#include "../../gameobject.h"
#include <stdio.h>

class HorseGraphicsComponent : public GraphicsComponent
{
public:
    HorseGraphicsComponent(){}
    void load(GameObject& g, SDL_Renderer *renderer) override;
    void draw(GameObject& g, SDL_Renderer *renderer, Uint32 dt) override;
};

#endif