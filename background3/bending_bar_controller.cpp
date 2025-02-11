#include "bending_bar_controller.h"

BendingBarController::BendingBarController(int x, int y, int w, int h){
    bar.x = x;
    bar.y = y;
    bar.w = w;
    bar.h = h;

    goal_bar.w = w/2;
    goal_bar.h = h;

    goal_bar.x = x+(w/4);
    goal_bar.y = y;

    bender.x = x;
    bender.y = y;
    bender.w = w/10;
    bender.h = h;
    
}

void BendingBarController::set_vel(int v){
    bend_vel = v;
}

BendingBarController::BendingBarController(){

}

void BendingBarController::draw(SDL_Renderer* renderer){

    SDL_SetRenderDrawColor( renderer, 255,0,0, 255);
    SDL_RenderFillRect(renderer, &bar);
    
    SDL_SetRenderDrawColor( renderer, 0,255,0, 255);
    SDL_RenderFillRect(renderer, &goal_bar);

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
        bend_vel = -acc;
    } else if(bender.x < bar.x){
        bend_vel = acc;
        
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
            acc = 2;
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
    acc = 2;
    bend_vel = acc;
}
void BendingBarController::start(int v){
    acc = v;
}

void BendingBarController::speed_up(int n){
    acc += n;
}

void BendingBarController::slow_down(int n){
    acc -= n;
}