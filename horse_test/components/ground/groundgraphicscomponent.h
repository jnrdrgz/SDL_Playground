#ifndef GROUNDGRAPHICSCOMPONENT_H
#define GROUNDGRAPHICSCOMPONENT_H

#include "../base/graphicscomponent.h"
#include "../../gameobject.h"

class GroundGraphicsComponent : public GraphicsComponent
{
public:
    GroundGraphicsComponent(){}
    void draw(GameObject& g, SDL_Renderer *renderer);
};

#endif