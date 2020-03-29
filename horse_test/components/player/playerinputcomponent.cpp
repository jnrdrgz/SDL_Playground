#include "playerinputcomponent.h"

void PlayerInputComponent::handle_input(GameObject& g, SDL_Event event){
    b2Vec2 vel = g.boxBody->GetLinearVelocity();
    switch (event.type)
    {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_RIGHT:
                    if(vel.x < max_vel){
                        vel.x += 5;
                    }
                break;
                case SDLK_LEFT:
                    if(vel.x > -max_vel){
                        vel.x -= 5;
                    }
                break;

                case SDLK_DOWN:
                    if(vel.y > -max_vel){
                        vel.y -= 5;
                    }
                break;
                case SDLK_UP:
                // {
                //     float impulse = g.boxBody->GetMass() * 10;
                //     if(jumps_left > 0 && !jumping){
                //         g.boxBody->ApplyLinearImpulse( b2Vec2(0,impulse), g.boxBody->GetWorldCenter(), true);
                //         jumping = true;
                //         //jumps_left--;
                //     }
                // }
                    if(vel.y < max_vel){
                        vel.y += 5;
                    }
                break;

                case SDLK_r:
                    g.boxBody->SetTransform(b2Vec2(1,1), 0);
                break;

                case SDLK_v:
                {
                    float angle = g.boxBody->GetAngle();
                    g.boxBody->SetTransform(g.boxBody->GetPosition(), angle+1);
                    printf("%.4f\n", angle);
                }
                break;
                case SDLK_b:
                    printf("%.4f\n", g.boxBody->GetAngle());
                break;
            }
            
        break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
                case SDLK_RIGHT:
                    vel.x = 0;
                break;
                case SDLK_LEFT:
                    vel.x = 0;
                break;
                case SDLK_DOWN:
                    vel.y = 0;
                break;
                case SDLK_UP:
                    //jumping = false;
                    vel.y = 0;
                break;
            }
        break;

       
    }
    //g.boxBody->SetLinearVelocity(b2Vec2(vel.x,g.boxBody->GetLinearVelocity().y));
    g.boxBody->SetLinearVelocity(b2Vec2(vel.x,vel.y));
}