#include <SDL2/SDL.h>
//#include "../log_system/log_system.h"
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>
#include <memory>
#include <string>

//static LogSystem log_system = LogSystem();

const int screen_w = 640;
const int screen_h = 480;


struct State;

namespace Pong
{
    struct MenuButton
    {
        MenuButton(std::string text, int x, int y, int w, int h) 
        : text{text},
        rct{x,y,w,h}, 
        selected{false}
        {}

        void draw(SDL_Renderer* renderer) const{
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            if(selected) SDL_RenderFillRect(renderer, &rct);
            else SDL_RenderDrawRect(renderer, &rct);
        }
        //std::unique_ptr<State> handle_input();
        std::string text;
        SDL_Rect rct;
        //text text
        bool selected;
    };

    struct Menu
    {
        Menu(int x, int y, int w, int h, int padding)
        : container{x,y,w,h},
        pressed{false}
        {
            printf("%d,%d\n", container.x, container.y);
            int button_x = container.x + padding;
            int button_y = container.y + padding;
            int button_w = container.w - (padding*2);
            int button_h = (container.h/4) - (padding*2);

            const int n_texts = 4;
            std::string texts[n_texts] = {"Play", "???", "Options", "Exit"}; 

            buttons.reserve(n_texts);
            for(int i = 0; i < n_texts; i++){
                buttons.emplace_back(texts[i],button_x,button_y,button_w,button_h);
                button_y += button_h + padding;
            }
            for(auto& button : buttons){
                if(button.text == "Play"){
                    button.selected = true;
                }
            }

        }

        std::unique_ptr<State> handle_input();
        std::unique_ptr<State> update();

        void draw(SDL_Renderer* renderer) const{
            for(auto& button : buttons){
                button.draw(renderer);
            }
        }

        SDL_Rect container;
        std::vector<MenuButton> buttons;
        bool pressed;
    };
    struct Game
    {
        Game(){}
        
        std::unique_ptr<State> handle_input();
        
        std::unique_ptr<State> update();

        void draw(SDL_Renderer* renderer) const{
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &paddle_p1);
            SDL_RenderFillRect(renderer, &paddle_p2);
            SDL_RenderFillRect(renderer, &ball);
        }

        SDL_Rect paddle_p1{0,0,25,100}, paddle_p2{screen_w-25,screen_h-100,25,100}, ball{200,200,25,25};
        bool ended{false}, paused{true};
    };

    
}

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
    GameMenuState() : 
    menu{200, 100, 200, 160, 10}
    {}

    std::unique_ptr<State> update() override{
        return nullptr;
    }
    std::unique_ptr<State> handle_input() override;
    std::unique_ptr<State> draw(SDL_Renderer* renderer) override{
        menu.draw(renderer);
        return nullptr;
    }

    Pong::Menu menu;
};

struct GamePlayingState : State 
{
public: 
    std::unique_ptr<State> update() override{
        game.update();
        if(game.ended){
            return std::make_unique<GameMenuState>();
        }
        return nullptr;
    }
    std::unique_ptr<State> handle_input() override{
        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        game.handle_input();
        if(kbstate[SDL_SCANCODE_A]){
            printf("Pressing A in Playing Game State\n");
        }

        if(kbstate[SDL_SCANCODE_V]){
            printf("Pressing V in Playing Game State\n");
            printf("Pong migth end\n");

            game.ended = true;
        }

        return nullptr;
    }

    std::unique_ptr<State> draw(SDL_Renderer* renderer) override{
        game.draw(renderer);
        return nullptr;
    }

    Pong::Game game;
};

std::unique_ptr<State> GameMenuState::handle_input(){
    const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
    menu.handle_input();
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

    std::unique_ptr<State> state{std::make_unique<GameMenuState>()};
};

std::unique_ptr<State> Pong::Game::update(){
    if(!paused){
        ball.x += 2;
        ball.y += 2;
    }
    return nullptr;
}

std::unique_ptr<State> Pong::Game::handle_input(){
    const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
    if(kbstate[SDL_SCANCODE_P]){
        printf("Pressing P in Pong::Game::handle_input\n");
        paused = false;
    }


    return nullptr;
}


std::unique_ptr<State> Pong::Menu::handle_input(){
    const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
    if(kbstate[SDL_SCANCODE_DOWN]){
        for(int i = 0; i < 4; i++){
            if(buttons[i].selected){
                if(i != 3){
                    buttons[i].selected = false;
                    buttons[i+1].selected = true;
                } else {
                    buttons[i].selected = false;
                    buttons[0].selected = true;
                }
                break;
            }
        }

        //pressed = true;
    }
    if(kbstate[SDL_SCANCODE_UP]){
        for(int i = 0; i < 4; i++){
            if(buttons[i].selected){
                if(i != 0){
                    buttons[i].selected = false;
                    buttons[i-1].selected = true;
                } else {
                    buttons[i].selected = false;
                    buttons[3].selected = true;
                }
                break;
            }
        }

        //pressed = true;   
    }


    return nullptr;
}


int main(int argc, char* args[])
{
    Game game;
    game.init("test", screen_w, screen_h);
    //log_system.init();

    MainGame mainGame;
    //mainGame.state = std::make_unique<GameMenuState>();
    
    while(game.running){
        
        SDL_RenderClear(game.renderer);

        mainGame.handle_input();

        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if(kbstate[SDL_SCANCODE_E]){
            printf("Pressing E\n");
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
