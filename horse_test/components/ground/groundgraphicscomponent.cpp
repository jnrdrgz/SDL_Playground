#include "groundgraphicscomponent.h"

void GroundGraphicsComponent::draw(GameObject& g, SDL_Renderer *renderer){
        SDL_SetRenderDrawColor(renderer,13,123,40,255);
        SDL_RenderFillRect(renderer, &g.dst);        
}