#include "playerphysicscomponent.h"

void PlayerPhysicsComponent::init(GameObject& g, b2World* world){
    b2BodyDef bodyDef;
    bodyDef.position.Set(g.x, g.y);
    bodyDef.type = b2_dynamicBody;
    g.boxBody = world->CreateBody(&bodyDef);
    b2PolygonShape box;
    box.SetAsBox(g.w, g.h);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.restitution = 0.0f;
    fixtureDef.density = 2.0f;
    fixtureDef.friction = 0.3f;
    g.boxBody->CreateFixture(&fixtureDef);

    g.boxBody->SetFixedRotation(true);
    printf("Physics inited\n");
}

void PlayerPhysicsComponent::update(GameObject& g){
    b2Vec2 pos = g.boxBody->GetPosition();
    g.set_position(pos.x,pos.y);
}