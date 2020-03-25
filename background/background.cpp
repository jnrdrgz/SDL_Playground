#include "background.h"

Background::Background(){

}

Background::~Background(){
	
}

void Background::add_layer(Layer layer){
	layers.push_back(layer);
}

void Background::draw(SDL_Renderer *renderer){
	for(auto layer : layers){
		layer.draw(renderer);
	}
}

void Background::set_size(int w, int h){
	for(auto layer : layers){
		layer.set_size(w,h);
	}
}