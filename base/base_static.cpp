#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define WITH_AUDIO 0

const int screen_w = 640;
const int screen_h = 480;
bool pressed = false;

namespace SDL{
	struct Context
	{
	public:
		Context(const char* title){
			SDL_Init( SDL_INIT_VIDEO );
		    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
		    window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_w, screen_h, SDL_WINDOW_SHOWN );
		    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
		    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		    
		    TTF_Init();
		    IMG_Init(IMG_INIT_PNG);
#if WITH_AUDIO
    		Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
#endif

		    //font = TTF_OpenFont("8-bit.ttf", 25); 
		}
	    
	    ~Context(){
	    	SDL_DestroyWindow(window);
		    SDL_DestroyRenderer(renderer);
#if WITH_AUDIO
		    Mix_Quit();
#endif
			IMG_Quit();
		    TTF_Quit();
		    SDL_Quit();
	    }

		static SDL_Renderer *renderer;
    	//static TTF_Font* font;
		static SDL_Event event;
	    
	private:
		SDL_Window *window = NULL;
	};
}

SDL_Renderer* SDL::Context::renderer = NULL;
//TTF_Font* SDL::Context::font = NULL;
SDL_Event SDL::Context::event;

int main(int argc, char* args[])
{
	(void)argc;
	(void)args;

	SDL::Context sdl_context("test");
   
	Uint32 initial_frames; //
	
	const float FPS = 24.0f; 
	const int desired_frame_time = (1.0f/FPS)*1000.0f;

    bool running = true;
    while(running){
    	initial_frames = SDL_GetTicks();
        
        SDL_RenderClear(SDL::Context::renderer);

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_E]){
            printf("Pressing E\n");
        }

        while(SDL_PollEvent(&SDL::Context::event)){
            if(SDL::Context::event.type == SDL_QUIT){
                running = false;
            }
            if(SDL::Context::event.type == SDL_MOUSEMOTION){
                
            }
            if(SDL::Context::event.type == SDL_KEYDOWN){
                if(SDL::Context::event.key.keysym.sym == SDLK_RIGHT){
                
                }
            }
            if(SDL::Context::event.type == SDL_KEYUP){
            	pressed = false;
                //switch(SDL::Context::event.key.keysym.sym){
                //    case SDLK_UP:
                //    case SDLK_DOWN:
                //        pressed = false;
                //}
            }
        }
    
        SDL_SetRenderDrawColor( SDL::Context::renderer, 255, 255, 255, 255);
        SDL_RenderPresent(SDL::Context::renderer);

        int finish_ftime = SDL_GetTicks()-initial_frames;
		if(finish_ftime > desired_frame_time){
			SDL_Delay(finish_ftime-desired_frame_time);
		}
    }
}