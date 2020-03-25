#ifndef LAYER_H
#define LAYER_H

#include <string>
#include <SDL2/SDL.h>
#include "layer.h"
#include "background_constants.h"

class Layer
{
private:
	SDL_Texture* texture = nullptr;
	SDL_Rect dst_rct, src_rct;
	int angle;

public:
	Layer(){
		dst_rct.x = 0;
		dst_rct.y = 0;
		angle = 0;
	}
	void update(){}
	void draw(SDL_Renderer* renderer){}
	void set_size(int w, int h){
		dst_rct.w = w;
		dst_rct.h = h;
	}

	void set_angle(int angle){
		this->angle = angle;
	}
};

#endif