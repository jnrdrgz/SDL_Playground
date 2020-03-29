#ifndef PLAYERPHYSICSCOMPONENT_H
#define PLAYERPHYSICSCOMPONENT_H

#include "../base/physicscomponent.h"
#include "../../gameobject.h"
#include <stdio.h>

class PlayerPhysicsComponent : public PhysicsComponent
{
public:
    void init(GameObject& g, b2World* world);
    void update(GameObject& g);
};

#endif