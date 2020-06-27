#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "timer.h"

#define WITH_AUDIO 0
#define DEBUG 1

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

struct Atlas
{
	Atlas(const char* path)
	: texture{nullptr}
	{	
		load_texture(path);
	}

	Atlas(const char* path, SDL_Color color_key)
	: texture{nullptr}
	{	
		load_texture(path, true, color_key);
	}

	~Atlas(){
		SDL_DestroyTexture(texture);
	}

	void load_texture(const char* path, bool color_key = false, SDL_Color ckey = {0,0,0,0})
	{
		SDL_Surface* tmp_surface = IMG_Load(path);
		if(!tmp_surface) printf("error loading surface\n");

		SDL_SetColorKey(tmp_surface, SDL_TRUE, SDL_MapRGB(tmp_surface->format, ckey.r, ckey.g, ckey.b));
		w = tmp_surface->w;
		h = tmp_surface->h;

		texture = SDL_CreateTextureFromSurface(SDL::Context::renderer, tmp_surface);
		if(!texture) printf("error loading texture\n");

		SDL_FreeSurface(tmp_surface);
	}

	SDL_Texture* texture;
	int w,h;
};

struct Animation
{
	Animation(const Atlas &atlas, int x, int y, 
				int frames, int w, int h, int limit, Uint32 time)
	: atlas{atlas}, x{x}, y{y}, frames{frames}, rct{x,y,w,h}, limit{limit}
	{
		if(limit == 0) this->limit = atlas.w;
		step = (int)time/frames;
		timer.start();
#if DEBUG
		printf("limit %d\n", limit);
		printf("step %d\n", step);
#endif
	}

	void calculate_frame(){
		rct.x = frame*rct.w + x;
		if(rct.x >= limit){
			rct.x -= limit;
			rct.y += rct.h;
#if DEBUG
			printf("adentro\n");
#endif
		}
#if DEBUG
		printf("x:%d,y:%d,w:%d,h:%d\n", rct.x,rct.y,rct.w,rct.h);
#endif
	}

	void update(){
		if(timer.getTicks() >= step){
			frame++;
			if(frame > frames) frame = 0;
				
			calculate_frame();	
			timer.restart();
		}
	}

	Timer timer;
	Uint32 step;
	int frame, frames, x, y, limit; //first frame is 0, not 1
	SDL_Rect rct;
	const Atlas& atlas;
};

struct GameObject
{
	GameObject(int x, int y, int w, int h) : rct{x,y,w,h}
	{}

	SDL_Rect rct;
};

/*
atlas
x in atlas
y in atlas
hframes
vframes
w
h
time
timer

Animation animation(&atlas, 45, 45, 6, 2, 32, 32, 400);


part of general state¿¿??
struct Player
{
	vector<Animation> animations ¿¿??

	std::unique_ptr current_animation

	switch state
		curent_animation.move(animations[0])
		curent_animation.move(animations[1])
		curent_animation.move(animations[2])
}

*/

int main(int argc, char* args[])
{
	(void)argc;
	(void)args;

	SDL::Context sdl_context("test");
   
	Uint32 initial_frames; //
	
	const float FPS = 24.0f; 
	const int desired_frame_time = (1.0f/FPS)*1000.0f;

	Atlas atlas("sp_sheet1.png",{0,255,0,0});

	Animation animation(atlas, 0, 0, 10, 30, 45, 0, 3000);
	
	SDL_Rect rct = {0,0,screen_w,screen_h};
	
	SDL_Rect animation_rct = {50,50,30,65};

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



        //SDL_RenderCopy(SDL::Context::renderer, atlas.texture, nullptr, &rct);

        animation.update();
        SDL_RenderCopy(SDL::Context::renderer, atlas.texture, &animation.rct, &animation_rct);
    	
        SDL_SetRenderDrawColor( SDL::Context::renderer, 255, 255, 255, 255);
        SDL_RenderPresent(SDL::Context::renderer);

        int finish_ftime = SDL_GetTicks()-initial_frames;
		if(finish_ftime > desired_frame_time){
			SDL_Delay(finish_ftime-desired_frame_time);
		}
    }
}