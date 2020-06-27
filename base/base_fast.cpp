#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <time.h>

#define WITH_AUDIO 0

const int screen_w = 640;
const int screen_h = 480;

SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
SDL_Event event;

void init_SDL(){
	SDL_Init( SDL_INIT_VIDEO );
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
	window = SDL_CreateWindow( "Fast Base", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		screen_w, screen_h, SDL_WINDOW_SHOWN );
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

	TTF_Init();
    IMG_Init(IMG_INIT_PNG);
#if WITH_AUDIO
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
#endif
}

void close_SDL(){
	SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
#if WITH_AUDIO
    Mix_Quit();
#endif
	IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}


int main(int argc, char* args[])
{
	(void)argc;
	(void)args;
	init_SDL();

	Uint32 initial_frames; //SDL_GetTicks();
	
	const float FPS = 24.0f; 
	const int desired_frame_time = (1.0f/FPS)*1000.0f;
	printf("frame_time: %d\n", desired_frame_time);

	SDL_Rect r = {100,100,16,16};
	int vx = 5, vy = 5;
	bool running = true;
	while(running){
		initial_frames = SDL_GetTicks();
		SDL_RenderClear(renderer);

		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
			}
		}

		SDL_SetRenderDrawColor( renderer, 0,0,0,255 );
		SDL_RenderFillRect(renderer, &r);
		r.x+=vx;
		r.y+=vy;

		if(r.x > screen_w) vx = -5;
		if(r.x < 0) vx = 5;
		if(r.y < 0) vy = 5;
		if(r.y > screen_h) vy = -5;

		SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);
		SDL_RenderPresent(renderer);

		int finish_ftime = SDL_GetTicks()-initial_frames;
		if(finish_ftime > desired_frame_time){
			SDL_Delay(finish_ftime-desired_frame_time);
		}
	}

	close_SDL();
}