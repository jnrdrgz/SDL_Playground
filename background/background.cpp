#include "background.h"

Background::Background(){

}

Background::~Background(){
	
}

Background::add_layer(Layer layer){
	layer.push_back(layer);
}
