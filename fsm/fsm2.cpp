#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <memory>

const int screen_w = 640;
const int screen_h = 480;

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
		    //Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

		    font = TTF_OpenFont("8-bit.ttf", 25); 
		}
	    
	    ~Context(){
	    	SDL_DestroyWindow(window);
		    SDL_DestroyRenderer(renderer);
		    //Mix_Quit();
			IMG_Quit();
		    TTF_Quit();
		    SDL_Quit();
	    }

		static SDL_Renderer *renderer;
    	static TTF_Font* font;
		static SDL_Event event;
	    
	private:
		SDL_Window *window = NULL;
	};
}

SDL_Renderer* SDL::Context::renderer = NULL;
TTF_Font* SDL::Context::font = NULL;
SDL_Event SDL::Context::event;


struct State
{
public: 
    virtual void update() = 0;
    virtual void handle_input() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
    std::string tag;
};

struct MenuState : State
{
public: 
	MenuState(){
		tag = "Menu";
	}
    void update() override{}
    void handle_input() override;
    void draw(SDL_Renderer* renderer) override{}
};

struct PlayingState : State
{
public: 
	PlayingState(){
		tag = "Playing";
	} 
    void update() override{}
    void handle_input() override{
    	const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_E]){
            printf("Pressing e in playing\n");
        }
    }
    void draw(SDL_Renderer* renderer) override{}
};

namespace Jumper
{
	struct Game
	{
		Game() : 
		running{true},
		menuState{std::make_unique<MenuState>()},
		playingState{std::make_unique<PlayingState>()},
		currentState{nullptr}
		{}

		void update(){}
		void handle_input(){
			currentState->handle_input();
		}
		void draw(){}

		bool running;

		std::unique_ptr<MenuState> menuState;
		std::unique_ptr<PlayingState> playingState;
		std::unique_ptr<State> currentState;
	};
}

void MenuState::handle_input(){
	const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
    if(kbstate[SDL_SCANCODE_E]){
        printf("Pressing e in menu\n");
    }

}

int main(int argc, char* args[])
{
	(void)argc;
	(void)args;

	SDL::Context sdl_context("test");
   
    Jumper::Game game;
    
    while(game.running){
        
        SDL_RenderClear(SDL::Context::renderer);

        game.handle_input();

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_E]){
            printf("Pressing E\n");
        }

        while(SDL_PollEvent(&SDL::Context::event)){
            if(SDL::Context::event.type == SDL_QUIT){
                game.running = false;
            }
            if(SDL::Context::event.type == SDL_MOUSEMOTION){
                
            }
            if(SDL::Context::event.type == SDL_KEYDOWN){
                if(SDL::Context::event.key.keysym.sym == SDLK_RIGHT){
                
                }
            }
            if(SDL::Context::event.type == SDL_KEYUP){
                switch(SDL::Context::event.key.keysym.sym){
                    //case SDLK_UP:
                    //case SDLK_DOWN:
                    //    pressed = false;
                }
            }
        }

        game.update();
        game.draw();
    
        SDL_SetRenderDrawColor( SDL::Context::renderer, 255, 255, 255, 255);
        SDL_RenderPresent(SDL::Context::renderer);
    }
}