#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>
#include <memory>

static LogSystem log_system = LogSystem();

const int screen_w = 640;
const int screen_h = 480;

struct State
{
public: 
    virtual std::unique_ptr<State> update() = 0;
    virtual std::unique_ptr<State> handle_input() = 0;
    virtual std::unique_ptr<State> draw(SDL_Renderer* renderer) = 0;
};

struct GameMenuState : State 
{
public: 
    std::unique_ptr<State> update() override{
        return nullptr;
    }
    std::unique_ptr<State> handle_input() override{
        return nullptr;
    }
    std::unique_ptr<State> draw(SDL_Renderer* renderer) override{
        return nullptr;
    }
};

struct GamePlayingState : State 
{
public: 
    std::unique_ptr<State> update() override{
        return nullptr;
    }
    std::unique_ptr<State> handle_input() override{
        return nullptr;
    }
    std::unique_ptr<State> draw(SDL_Renderer* renderer) override{
        return nullptr;
    }
};

struct MainGame
{
    void update(){
        auto n_state = state->update();
        if(n_state){
            state = std::move(n_state);
        }
    }
    void handle_input() {
        auto n_state = state->update();
        if(n_state){
            state = std::move(n_state);
        }
    }
    void draw(SDL_Renderer* renderer) {
        auto n_state = state->update();
        if(n_state){
            state = std::move(n_state);
        }
    }

    std::unique_ptr<State> state = std::make_unique<State>();
};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", screen_w, screen_h);
    log_system.init();

    
    while(game.running){
        
        SDL_RenderClear(game.renderer);

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_M]){
            printf("Pressing M\n");
        }

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                if(game.event.key.keysym.sym == SDLK_RIGHT){
                
                }
            }
        }

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
