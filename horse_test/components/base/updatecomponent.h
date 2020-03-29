#ifndef UPDATECOMPONENT_H
#define UPDATECOMPONENT_H

#include "base.h"

class UpdateComponent
{
public:
    virtual void update(GameObject& g) = 0;
};

#endif