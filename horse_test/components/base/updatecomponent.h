#ifndef UPDATECOMPONENT_H
#define UPDATECOMPONENT_H

#include "base.h"

class UpdateComponent
{
public:
	bool active;
    virtual void update(GameObject& g, Uint32 dt) = 0;
};

#endif