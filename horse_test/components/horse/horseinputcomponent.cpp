#include "horseinputcomponent.h"

void HorseInputComponent::handle_input(GameObject &g, SDL_Event event){
	if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_RIGHT){
            //
            //
        }
        if(event.key.keysym.sym == SDLK_LEFT){
            //
            //
        }
        if(event.key.keysym.sym == SDLK_SPACE){
            //
            g.run();
            //
        }
        if (event.key.keysym.sym == SDLK_UP){
            
            //printf("pressing up in horse ecs input component\n");
            g.run();
            
            g.sum_velocity(1,0);
            g.anim_vel -= 50;
            g.set_anim_vel(g.anim_vel);
        }
        if (event.key.keysym.sym == SDLK_DOWN){
            g.sum_velocity(-1,0);
            g.anim_vel += 50;
			g.set_anim_vel(g.anim_vel);
        }
    }
}