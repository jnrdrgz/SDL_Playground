#include "playergraphicscomponent.h"

void PlayerGraphicsComponent::draw(GameObject& g, SDL_Renderer *renderer){
        if(g.texture){
                //SDL_RenderCopyEx(renderer, g.texture, &g.src, &g.dst, g.boxBody->GetAngle()*RAD2DEG*-1, 0, SDL_FLIP_NONE);
                SDL_RenderCopyEx(renderer, g.texture, &g.src, &g.dst, g.boxBody->GetAngle()*RAD2DEG*-1, 0, SDL_FLIP_NONE);
        } else {
                SDL_SetRenderDrawColor(renderer,123,0,200,255);
                SDL_RenderFillRect(renderer, &g.dst);
        }
}