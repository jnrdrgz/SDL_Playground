#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <vector>
#include <string>
#include "layer.h"

class Background
{
private:
	std::vector<Layer> layers;
	int reference; //the object to follow
public:
	Background();
	~Background();
	void add_layer(Layer layer);
};

#endif