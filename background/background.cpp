#include "background.h"

Background::Background(){

}

Background::~Background(){
	
}

void Background::add_layer(Layer layer){
	layers.push_back(layer);
}
