#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../timer.h"
#include <vector>
#include <memory>
#include <string>
#include<sstream>

const int screen_w = 640;
const int screen_h = 480;
bool pressed = false;

namespace SDL{
	struct Game
	{
	public:
	    void init(const char* title, int screenw, int screenh){
		    SDL_Init( SDL_INIT_VIDEO );
		    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
		    window = SDL_CreateWindow( title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenw, screenh, SDL_WINDOW_SHOWN );
		    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
		    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		    
		    TTF_Init();
		    IMG_Init(IMG_INIT_PNG);
		    //Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

		    font = TTF_OpenFont("8-bit.ttf", 25); 

		    running = true;
		}

	    void close(){
		    SDL_DestroyWindow(window);
		    SDL_DestroyRenderer(renderer);
		    //Mix_Quit();
			IMG_Quit();
		    TTF_Quit();
		    SDL_Quit();
		}

		static SDL_Renderer *renderer;

    	static TTF_Font* font;
		SDL_Event event;
	    bool running = false;
		
	private:
		SDL_Window *window = NULL;
	};
}

SDL_Renderer* SDL::Game::renderer = NULL;
TTF_Font* SDL::Game::font = NULL;

struct Text
{
    Text(std::string text_str, int x, int y, int size) : 
    text_str{text_str},
    w{size*(int)text_str.length()},
    h{size},
    rct{x,y,w,h}
    {

    }

    Text(std::string text, int x, int y, int size, SDL_Color color, SDL_Renderer* renderer);
    
    Text(std::string text, int x, int y, int size, SDL_Color color);
        
    void draw() const{
        SDL_RenderCopy( SDL::Game::renderer, texture, NULL, &rct);
    }

    void load_texture(SDL_Color color);

    std::string text_str;
    int w,h;
    SDL_Rect rct;
    SDL_Texture* texture = NULL;
};



struct State;

namespace Pong
{
    struct MenuButton
    {
        MenuButton(std::string text, int x, int y, int w, int h) : 
        text_str{text},
        rct{x,y,w,h}, 
        selected{false},
        textured{false},
        text{text, x, y, h-20}
        {}

        MenuButton(std::string text, int x, int y, int w, int h, bool textured);

        void draw(SDL_Renderer* renderer) const{
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            if(selected) SDL_RenderFillRect(renderer, &rct);
            else SDL_RenderDrawRect(renderer, &rct);
            if(textured) text.draw();
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
            const int n_texts = 3;

            printf("%d,%d\n", container.x, container.y);
            int button_x = container.x + padding;
            int button_y = container.y + padding;
            int button_w = container.w - (padding*2);
            int button_h = (container.h/n_texts) - (padding*2);

            std::string texts[n_texts] = {"1 Player", "2 Players", "Exit"}; 

            buttons.reserve(n_texts);
            for(int i = 0; i < n_texts; i++){
                bool textured = true;
                buttons.emplace_back(texts[i],button_x,button_y,button_w,button_h,true);
                button_y += button_h + padding;
            }
            for(auto& button : buttons){
                if(button.text_str == "1 Player"){
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
        Game(int x, int y) : 
        container{x,y,400,400},
        paddle_p1{x+5, y, container.w/32, container.h/8},
        paddle_p2{container.x+container.w-paddle_p1.w-5, container.y+container.h-paddle_p1.h,
            paddle_p1.w,paddle_p1.h},
        ball{(x+container.w)/2,(y+container.h)/2,container.w/35,container.w/35},
        n_dividers{10},
        ballvx{4}, ballvy{4}, base_v{4},
        padv{6}, pad2v{6},
        points1{0},points2{0},
        container_center_x{(container.w/2)+container.x},
        container_center_y{(container.h/2)+container.y},
        points1text{"0", container_center_x-((paddle_p1.h/2)+10), container.y, paddle_p1.h/2, {0,0,0,0}},
        points2text{"0", container_center_x+((paddle_p1.h/2)-10), container.y, paddle_p1.h/2, {0,0,0,0}}
        {
            int diveder_w = paddle_p1.w/2;            

            int distance_between_containers = container.h/10;
            int divider_y = container.y;

            dividers = (SDL_Rect*)malloc(sizeof(SDL_Rect)*10);

            for(int i = 0; i < n_dividers; i++){
                dividers[i] = {container_center_x,divider_y,diveder_w,paddle_p1.h/2};
                divider_y += distance_between_containers;
            }
        }

        ~Game(){
            free(dividers);
        }
        
        void handle_input(){
            const Uint8 *kbstate = SDL_GetKeyboardState(NULL);

            padv = 0;
            if(kbstate[SDL_SCANCODE_UP]){
                padv = -6;
            }
            
            if(kbstate[SDL_SCANCODE_DOWN]){
                padv = 6;
            }

            if(kbstate[SDL_SCANCODE_P]){
                paused = true;
            }
            if(kbstate[SDL_SCANCODE_U]){
                paused = false;
            }
        }
        
        void update(){
            if(!paused){
                //vel update
                ball.x += ballvx;
                ball.y += ballvy;
                paddle_p1.y += padv;

                //pad collision
                if(rct_collide(paddle_p1,ball)){
                    ballvx = base_v;
                }
                if(rct_collide(paddle_p2,ball)){
                    ballvx = -base_v;
                }

                //enemy AI
                if(ball.x > container.x+(container.w/2) && ballvx > 0){
                    if(ball.y > paddle_p2.y){
                        paddle_p2.y += pad2v;
                    }

                    if(ball.y < paddle_p2.y){
                        paddle_p2.y -= pad2v;
                    }
                }

                //check of someone lose
                if(ball.x < container.x){
                    ballvx = base_v;
                    ball.x = (container.x + container.w)/2;
                    ball.y = (container.y + container.h)/2;
                    printf("lose 1\n");
                    points2++;
                    std::stringstream _score;
                    _score << points2;
                    points2text.text_str = _score.str();
                    points2text.load_texture({0,0,0,0});
                }  
                
                if(ball.x + ball.w > container.x+container.w){
                    ballvx = -base_v;
                    ball.x = (container.x + container.w)/2;
                    ball.y = (container.y + container.h)/2;
                    printf("lose 2\n");
                    points1++;
                    std::stringstream _score;
                    _score << points1;
                    points1text.text_str = _score.str();
                    points1text.load_texture({0,0,0,0});
                }
                
                //check ball dont trepass boundaries
                if(ball.y+ball.h > container.y+container.h) ballvy = -base_v;
                if(ball.y < container.y) ballvy = base_v; 

                //check paddles dont trepass boundaries
                if(paddle_p1.y < container.y) paddle_p1.y = container.y; 
                if(paddle_p1.y+paddle_p1.h > container.y+container.h) paddle_p1.y = container.y+container.h-paddle_p1.h;

                if(paddle_p2.y < container.y) paddle_p2.y = container.y; 
                if(paddle_p2.y+paddle_p2.h > container.y+container.h) paddle_p2.y = container.y+container.h-paddle_p2.h;
            }
        }

        void draw(SDL_Renderer* renderer) const{
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRects(renderer, dividers, n_dividers);
            points1text.draw();
            points2text.draw();
            SDL_RenderDrawRect(renderer, &container);
            SDL_RenderFillRect(renderer, &paddle_p1);
            SDL_RenderFillRect(renderer, &paddle_p2);
            SDL_RenderFillRect(renderer, &ball);
        }

        SDL_Rect container, paddle_p1, paddle_p2, ball, divider;
        const int n_dividers;
        SDL_Rect* dividers = NULL;
        int ballvx,ballvy,base_v;
        int padv, pad2v;
        int points1, points2;
        const int container_center_x, container_center_y; 
        Text points1text,points2text;

        bool ended{false}, paused{false};

    private:
        bool rct_collide(SDL_Rect a, SDL_Rect b){
            if( a.x < b.x + b.w &&
                a.x + a.w > b.x &&
                a.y < b.y + b.h &&
                a.y + a.h > b.y)
            {
                return true;
            }

                return false;
            }
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
    menu{200, 100, 200, 130, 10}
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
    GamePlayingState() : game{120,50}{
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

std::unique_ptr<State> Pong::Menu::handle_input(){
    const Uint8 *kbstate = SDL_GetKeyboardState(NULL);

    int n_buttons = 3;
    if(kbstate[SDL_SCANCODE_DOWN] && !pressed){
        for(int i = 0; i < n_buttons; i++){
            if(buttons[i].selected){
                if(i != n_buttons-1){
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
        for(int i = 0; i < n_buttons; i++){
            if(buttons[i].selected){
                if(i != 0){
                    buttons[i].selected = false;
                    buttons[i-1].selected = true;
                } else {
                    buttons[i].selected = false;
                    buttons[n_buttons-1].selected = true;
                }
                break;
            }
        }

        pressed = true;   
    }

    if(kbstate[SDL_SCANCODE_RETURN]){
        for(int i = 0; i < 4; i++){
            if(buttons[i].selected){
                if(i == 0) return std::make_unique<GamePlayingState>();//std::make_unique<GamePlayingState>(1);
                if(i == 1) return nullptr;//std::make_unique<GamePlayingState>(2);
                if(i == 2) return std::make_unique<GameExitState>();
            }
        }
    }

    return nullptr;
}


void Text::load_texture(SDL_Color color){        
    if(!SDL::Game::renderer){
    	printf("ERROR NOT RENDERER LOADED\n");
    	printf("error in font: %s\n", SDL_GetError());
    }
    if(texture){
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
    
    if(!SDL::Game::font){
    	printf("Error loading font\n");
    	printf("error: %s\n", SDL_GetError());
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid( SDL::Game::font, text_str.c_str(), color );
    if(!textSurface) printf("error in surface: %s\n", SDL_GetError());
    
    if(SDL::Game::renderer) texture = SDL_CreateTextureFromSurface( SDL::Game::renderer, textSurface );
    if(!texture) printf("error in texture: %s\n", SDL_GetError());        

    SDL_FreeSurface(textSurface);
}

Text::Text(std::string text, int x, int y, int size, SDL_Color color, SDL_Renderer* renderer) : 
Text(text, x, y, size)
{
    if(renderer) load_texture(color);
}

Text::Text(std::string text, int x, int y, int size, SDL_Color color) : 
Text(text, x, y, size)
{
    load_texture(color);
}

Pong::MenuButton::MenuButton(std::string text, int x, int y, int w, int h, bool textured) : 
    text_str{text},
    rct{x,y,w,h}, 
    selected{false},
    textured{true},
    text{text, x+5, y+5, h-10, {255,0,0,0}, SDL::Game::renderer}
{}

int main(int argc, char* args[])
{
	using namespace SDL;
    Game game;
    game.init("test", screen_w, screen_h);

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
