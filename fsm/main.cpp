#include <SDL2/SDL.h>
#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>

static LogSystem log_system = LogSystem();

const int screen_w = 640;
const int screen_h = 480;

struct State
{
public: 
    virtual State* update() = 0;
    virtual State* handle_input() = 0;
    virtual State* draw(SDL_Renderer* renderer) = 0;
};

struct GameMenuState : State 
{
public: 
    State* update() override{
        return nullptr;
    }
    State* handle_input() override{
        return nullptr;
    }
    State* draw(SDL_Renderer* renderer) override{
        return nullptr;
    }
};

struct GamePlayingState : State 
{
public: 
    State* update() override{
        return nullptr;
    }
    State* handle_input() override{
        return nullptr;
    }
    State* draw(SDL_Renderer* renderer) override{
        return nullptr;
    }
};


struct PlayerStandingState : State 
{
public: 
    State* update() override{
        return nullptr;
    }
    State* handle_input() override{
        return nullptr;
    }
    State* draw(SDL_Renderer* renderer) override{
        return nullptr;
    }
};

struct PlayerAttakingState : State 
{
public: 
    State*  update() override{
        return nullptr;
    }
    State* handle_input() override{
        return nullptr;
    }
    State*  draw(SDL_Renderer* renderer) override{
        return nullptr;
    }
};

struct PlayerWalkingState : State 
{
public: 
    State*  update() override{
        return nullptr;
    }
    State* handle_input() override{
        return nullptr;
    }
    State*  draw(SDL_Renderer* renderer) override{
        return nullptr;
    }
};

struct PlayerDyingState : State 
{
public: 
    State*  update() override{
        return nullptr;
    }
    State*  handle_input() override{
        return nullptr;
    }
    State*  draw(SDL_Renderer* renderer) override{
        return nullptr;
    }
};

struct GameObject
{
    void update(){
        State* n_state = state->update();
        if(n_state) state = n_state;
    }
    void handle_input() {
        State* n_state = state->handle_input();
        if(n_state) state = n_state;
    }
    void draw(SDL_Renderer* renderer) {
        State* n_state = state->draw(renderer);
        if(n_state) state = n_state;
    }

    State* state = nullptr;
    SDL_Rect rct;
};

struct MainGame
{
    void update(){
        State* n_state = state->update();
        if(n_state){
            //delete state;
            //state = nullptr;
            state = n_state;
        }
    }
    void handle_input() {
        State* n_state = state->handle_input();
        if(n_state) state = n_state;
    }
    void draw(SDL_Renderer* renderer) {
        State* n_state = state->draw(renderer);
        if(n_state) state = n_state;
    }

    State* state = nullptr;
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
