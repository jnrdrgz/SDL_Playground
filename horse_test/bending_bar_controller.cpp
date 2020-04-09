#include "bending_bar_controller.h"

BendingBarController::BendingBarController(int x, int y, int w, int h){
    bar.x = x;
    bar.y = y;
    bar.w = w;
    bar.h = h;

    bender.x = x;
    bender.y = y;
    bender.w = w/10;
    bender.h = h;
    
}

void BendingBarController::set_vel(int v){
    bend_vel = v;
}

void BendingBarController::draw(SDL_Renderer* renderer){

    SDL_SetRenderDrawColor( renderer, 255,0,0, 255);
    SDL_RenderFillRect(renderer, &bar);


    SDL_SetRenderDrawColor( renderer, 0,0,0, 255);
    SDL_RenderFillRect(renderer, &bender);

}

void BendingBarController::update(){
    /*if(bender.x + bender.w > bar.x + bar.w){
        bend_vel = -2;
    } else if(bender.x < bar.x){
        bend_vel = 2;
    }*/

    if(bender.x + bender.w > bar.x + bar.w){
        bend_vel = -2;
    } else if(bender.x < bar.x){
        bend_vel = 2;
    }

    bender.x += bend_vel;
}

float BendingBarController::get_controller_value(){
    return t_get_value<float>(50.0, (float)bender_x_within_bar(), 3);
}

void BendingBarController::handle_input(SDL_Event event){
    if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_i){
            printf("change\n");
            bend_vel = 2;
        }
        if(event.key.keysym.sym == SDLK_UP){
            if(bend_vel == 0)
                bend_vel = 2;
        }
        if(event.key.keysym.sym == SDLK_SPACE){
            printf("%d\n", get_value(50, bender_x_within_bar(), 3));
            printf("%.2f\n", t_get_value<float>(50.0, (float)bender_x_within_bar(), 3));
        }
    }  
}


void BendingBarController::start(){
    bend_vel = 2;
}

void BendingBarController::start(int v){
    bend_vel = v;
}