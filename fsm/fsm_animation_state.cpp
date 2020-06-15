#include <SDL2/SDL.h>
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>
#include <memory>

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
    std::unique_ptr<State> handle_input() override;
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
        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_A]){
            printf("Pressing A in Playing Game State\n");
        }

        return nullptr;
    }
    std::unique_ptr<State> draw(SDL_Renderer* renderer) override{
        
        return nullptr;
    }

};

std::unique_ptr<State> GameMenuState::handle_input(){
    const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
    if(kbstate[SDL_SCANCODE_A]){
        printf("Pressing A in Menu State\n");
        return std::make_unique<GamePlayingState>();
    }
    if(kbstate[SDL_SCANCODE_M]){
        printf("Pressing M in Menu State\n");
    }
    return nullptr;
}

struct MainGame
{
    void update(){
        auto n_state = state->update();
        if(n_state){
            state = std::move(n_state);
        }
    }
    void handle_input() {
        auto n_state = state->handle_input();
        if(n_state){
            state = std::move(n_state);
        }
    }
    void draw(SDL_Renderer* renderer) {
        auto n_state = state->draw(renderer);
        if(n_state){
            state = std::move(n_state);
        }
    }

    std::unique_ptr<State> state = nullptr;
};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", screen_w, screen_h);
    //log_system.init();

    MainGame mainGame;
    mainGame.state = std::make_unique<GameMenuState>();

    //Animation(game.renderer, "assets/img.png", 3, 2, 3, 16, 16, 600)
    
    while(game.running){
        
        SDL_RenderClear(game.renderer);

        mainGame.handle_input();

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_E]){
            printf("Pressing E\n");
        }
        if(kbstate[SDL_SCANCODE_ESCAPE]){
            game.running = false;
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

        mainGame.update();
        mainGame.draw(game.renderer);

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
