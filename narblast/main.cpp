#include <SDL2/SDL.h>
#include <vector>
#include <cstdint>

namespace Narblast
{
    struct RenderSystem
    {
    public:
        RenderSystem(const char* title, int sw, int sh) : screen_width{sw}, screen_height{sh}{
            SDL_Init( SDL_INIT_VIDEO );
            SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
            window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN );
            renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );    
        }

        ~RenderSystem(){
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
        }

        void clear(){
            SDL_RenderClear(renderer);
        }
        void present(){
            SDL_RenderPresent(renderer);
        }



        void set_renderer_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
            SDL_SetRenderDrawColor( renderer, r, g, b, a);
        }

        SDL_Renderer *renderer = NULL;

    private:
        SDL_Window *window = NULL;
        const int screen_width{0}, screen_height{0};

    };

    struct InputSystem
    {
        InputSystem() = default;
        void handle_events(bool& running){
            const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
            if(kbstate[SDL_SCANCODE_M]){
                printf("Pressing M\n");
            }
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT){
                    running = false;
                }
            }
        }
        SDL_Event event;    
    };


}

struct Game
{
public:
    Game() : renderSystem{Narblast::RenderSystem{"My Game", 640, 480}},
             running{true}
    {

    }
    ~Game(){
        SDL_Quit();
    }

    void loop(){
        while(running){
            renderSystem.clear();
            inputSystem.handle_events(running);
            renderSystem.set_renderer_color(255, 255, 255, 255);
            renderSystem.present();
        }
    }
    
private:
    Narblast::RenderSystem renderSystem{"",0,0};
    Narblast::InputSystem inputSystem{};
    bool running;
};

int main(int argc, char* args[])
{
    Game game;
    game.loop();
}
