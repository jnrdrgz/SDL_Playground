#ifndef MYRANDOM_H
#define MYRANDOM_H

#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

int random_between(int mn, int mx){
    int n = rand()%(mx-mn)+mn;
    return n;
}

//template<typename T> 
float random_betweenf(float mn, float mx){
    float n = (float)rand() / (float)RAND_MAX;

    return (mn + (mx - mn) * n);
}

SDL_Color get_random_rainbow_color(){
    //Uint8 numbers[3] = {255,127,0};
    std::vector<Uint8> numbers = {255,127,0};
 
    std::random_shuffle(numbers.begin(), numbers.end());

    Uint8 r = numbers[0];
    Uint8 g = numbers[1];
    Uint8 b = numbers[2];
    /*if(r == 0 && g == 0 && b == 0){
        r = numbers[random()%2];
        g = numbers[random()%2];
        b = numbers[random()%2];
    }*/

    SDL_Color color = {r,g,b,255};

    return color;
}

#endif //my_random