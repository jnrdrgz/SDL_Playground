#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "layer.h"
#include "background_constants.h"


class Background
{
private:
	std::vector<Layer> layers;
	SDL_Rect* reference = nullptr; //the object to follow
	SDL_Rect dst_rct;
	CameraType cameraType;

public:
	Background();
	~Background();
	void add_layer(Layer layer);
	void draw(SDL_Renderer* renderer);
	void set_size(int w, int h);

	void set_reference(SDL_Rect *r){
		reference = r;
	}
	
	void set_camera_static(){
		cameraType = CameraType::STATIC;
	}
	void set_camera_scrolling(/*reference*/){
		cameraType = CameraType::SCROLLING;
	}
	void set_camera_fixed(){
		cameraType = CameraType::FIXED;
	}
	void set_camera_looper(){
		cameraType = CameraType::LOOPER;
	}
};

#endif