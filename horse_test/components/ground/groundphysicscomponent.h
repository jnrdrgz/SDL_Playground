#ifndef GROUNDPHYSICSCOMPONENT_H
#define GROUNDPHYSICSCOMPONENT_H

#include "../base/graphicscomponent.h"
#include "../../gameobject.h"

class GroundPhysicsComponent : public PhysicsComponent
{
public:
    void init(GameObject& g, b2World* world);
    void update(GameObject& g);
};

#endif