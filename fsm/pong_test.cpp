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
bool pressed;

struct Text
{
    Text(std::string text, int x, int y, int size) : 
    text{text},
    w{size*(int)text.length()},
    h{size},
    rct{x,y,w,h}
    {

    }

    Text(std::string text, int x, int y, int size, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font) : 
    Text(text, x, y, size)
    {
        load_texture(color, renderer, font);
    }
        
    void draw(SDL_Renderer* renderer) const{
        SDL_RenderCopy( renderer, texture, NULL, &rct);
    }

    void load_texture(SDL_Color color, SDL_Renderer* renderer, TTF_Font* font){        
        if(texture){
            SDL_DestroyTexture(texture);
            texture = NULL;
        }
        
        if(!font){
            printf("error in font: %s\n", SDL_GetError());
        }

        SDL_Surface* textSurface = TTF_RenderText_Solid( font, text.c_str(), color );
        
        if(!textSurface){
            printf("error in surface: %s\n", SDL_GetError());
        }

        texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        
        if(!texture){
            printf("error in texture: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(textSurface);
    }

    std::string text;
    int w,h;
    SDL_Rect rct;
    SDL_Texture* texture = NULL;
    
};

struct State;

namespace Pong
{
    TTF_Font* font = TTF_OpenFont("8-bit.ttf", 25);

    struct MenuButton
    {
        MenuButton(std::string text, int x, int y, int w, int h) 
        : text_str{text},
        rct{x,y,w,h}, 
        selected{false},
        textured{false},
        text{text, x, y, h, {255,0,0,0}, NULL, font}
        {}

        MenuButton(std::string text, int x, int y, int w, int h, SDL_Renderer* renderer) 
        : text_str{text},
        rct{x,y,w,h}, 
        selected{false},
        textured{true},
        text{text, x, y, h, {255,0,0,0}, renderer, font}
        {}

        void draw(SDL_Renderer* renderer) const{
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            if(selected) SDL_RenderFillRect(renderer, &rct);
            else SDL_RenderDrawRect(renderer, &rct);
            if(textured) text.draw(renderer);
        }
        //std::unique_ptr<State> handle_input();
        std::string text_str;
        SDL_Rect rct;
        Text text;
        bool selected;
        bool textured;
    };

    struct Menu
    {
        Menu(int x, int y, int w, int h, int padding)
        : container{x,y,w,h}
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
                if(button.text_str == "Play"){
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
        Uint8 last_state = 0;
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
    std::string tag;
};

struct GameExitState : State 
{
public: 
    GameExitState() 
    {
        tag = "Exit";
    }

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


struct GameMenuState : State 
{
public: 
    GameMenuState() : 
    menu{200, 100, 200, 160, 10}
    {
        tag = "Menu";
    }

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
    GamePlayingState(){
        tag = "Menu";
    }
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
    if(kbstate[SDL_SCANCODE_A]){
        printf("Pressing A in Menu State\n");
        return std::make_unique<GamePlayingState>();
    }
    if(kbstate[SDL_SCANCODE_M]){
        printf("Pressing M in Menu State\n");
    }
    return menu.handle_input();
}

struct MainGame
{
    bool update(){
        auto n_state = state->update();
        if(n_state){
            state = std::move(n_state);
        }

        if(state->tag == "Exit"){
            return false;
        } else {
            return true;
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
    
    //printf("kb_s_dow: %d,l_s: %d\n", kbstate[SDL_SCANCODE_DOWN],last_state);
    //printf("kb_s_up: %d,l_s: %d\n", kbstate[SDL_SCANCODE_UP],last_state);
    if(kbstate[SDL_SCANCODE_DOWN] && !pressed){
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

        pressed = true;
    }

    if(kbstate[SDL_SCANCODE_UP] && !pressed){
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

        pressed = true;   
    }

    if(kbstate[SDL_SCANCODE_RETURN]){
        for(int i = 0; i < 4; i++){
            if(buttons[i].selected){
                printf("button %d selected\n", i);
                if(i == 0){
                    return std::make_unique<GamePlayingState>();
                }
                if(i == 1){
                    //return std::make_unique<GameOptionsState>();
                    return nullptr;
                }
                if(i == 2){
                    //return std::make_unique<GameHighScoresState>();
                    return nullptr;
                }
                if(i == 3){
                    return std::make_unique<GameExitState>();
                }
            }
        }
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
            if(game.event.type == SDL_KEYUP){
                switch(game.event.key.keysym.sym){
                    case SDLK_UP:
                    case SDLK_DOWN:
                        pressed = false;
                }
            }
        }

        if(!mainGame.update()) game.running = false;
        mainGame.draw(game.renderer);

    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
