#ifndef BENDING_BAR_CONTROLLER_H
#define BENDING_BAR_CONTROLLER_H

#include <SDL2/SDL.h>

class BendingBarController
{
private:
    SDL_Rect bar;
    SDL_Rect bender;
    int bend_vel = 0;

    int distance(int target, int guess){
        if(target <= guess){
            return guess-target;
        } else {
            return target-guess;
        }
    }

    int get_value(int target, int guess, int maxv){
        return distance(target, guess)/(target/maxv);
    }

    int bar_stop_n(){
        return (bar.x+bar.w) - bar.x;
    }

    int bender_x_within_bar(){
        return (bender.x+(bender.w/2)) - bar.x;
    }

    template <typename T> 
    T t_distance(T target, T guess){ 
        return (target <= guess) ? guess-target : target-guess;
    }

    template <typename T> 
    T t_get_value(T target, T guess, T maxv){
        return t_distance(target, guess)/(target/maxv);
    } 

public:
    BendingBarController(int x, int y, int w, int h);

    void set_vel(int v);

    void draw(SDL_Renderer* renderer);

    void update();

    void handle_input(SDL_Event event);

    float get_controller_value();

    void start();
    void start(int v);

    void speed_up(int n);
    void slow_down(int n);

};

#endif