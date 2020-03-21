#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <SDL2/SDL.h>
#include "layer.h"

class Layer
{
private:
	SDL_Texture* texture = nullptr;
	SDL_Rect dst_rct,src_rct;
public:
	virtual void update();
	virtual void draw();
};

#endif