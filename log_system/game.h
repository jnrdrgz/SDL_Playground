#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class Game
{
public:
    void init(const char* title, int screenw, int screenh);
    void close();
	SDL_Renderer *renderer = NULL;
	SDL_Event event;
    bool running = false;
	
private:
	SDL_Window *window = NULL;
};

#endif