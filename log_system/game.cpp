#include "game.h"

void Game::init(const char* title, int screenw, int screenh){
    SDL_Init( SDL_INIT_VIDEO );
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenw, screenh, SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    
    if( TTF_Init() == -1 ){
        printf( "Error starting SDL_TTF: %s\n", TTF_GetError() );
    }

    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

    running = true;
}

void Game::close(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    Mix_Quit();
	IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}