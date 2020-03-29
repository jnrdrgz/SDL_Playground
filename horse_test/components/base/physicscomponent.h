#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "base.h"

class PhysicsComponent
{
public:
    virtual void init(GameObject& g/*, physicsworld world*/) = 0;
    virtual void update(GameObject& g) = 0;
};

#endif