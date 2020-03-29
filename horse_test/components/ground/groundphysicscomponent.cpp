#include "groundphysicscomponent.h"

void GroundPhysicsComponent::init(GameObject& g, b2World* world){
    b2BodyDef bodyDef;
    bodyDef.position.Set(g.x, g.y);
    g.boxBody = world->CreateBody(&bodyDef);
    b2PolygonShape box;
    box.SetAsBox(g.w, g.h);
    g.boxBody->CreateFixture(&box, 0.0f);

    printf("Physics inited\n");
}

void GroundPhysicsComponent::update(GameObject& g){
    b2Vec2 pos = g.boxBody->GetPosition();
    g.set_position(pos.x,pos.y);
}