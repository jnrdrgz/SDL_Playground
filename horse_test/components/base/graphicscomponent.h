#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

#include <SDL2/SDL.h>
#include "base.h"

class GraphicsComponent
{
public:
	virtual void init(GameObject& g, SDL_Renderer *renderer) = 0;
    virtual void draw(GameObject& g, SDL_Renderer *renderer) = 0;
};
#endif