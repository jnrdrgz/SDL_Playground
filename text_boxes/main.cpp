#include <SDL2/SDL.h>
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include "text.h"
#include <vector>

TTF_Font* font = NULL;
    
struct TextBox
{
    TextBox(){}
    TextBox(SDL_Renderer* renderer){
        std::string str = "a";
        SDL_Color red = {0,0,0,255};
        text = Text(str, red, renderer, font);
        animated = false;
    }

    TextBox(SDL_Renderer* renderer, bool animated){
        std::string str = "a";
        SDL_Color red = {0,0,0,255};
        text = Text(str, red, renderer, font);
        this->animated = animated;
    }

    void update(SDL_Renderer* renderer){
        if(animated){
            if(timer.getTicks() > 100){
                incomplete_text += complete_text[current_letter];
                current_letter++;
                text.update_text(incomplete_text, renderer);
                timer.stop();
                if(current_letter != complete_text.length()) timer.start();
            }
        }
    }

    void draw(SDL_Renderer* renderer, bool fill = false){
        SDL_SetRenderDrawColor(renderer, box_color.r, box_color.g, box_color.b, box_color.a);
        if(fill){
            SDL_RenderFillRect(renderer, &box);
        } else {
            SDL_RenderDrawRect(renderer, &box);
        }

        //SDL_SetRenderDrawColor(renderer, text_color.r, text_color.g, text_color.b, text_color.a);
        text.render(box.x+margin, box.y+margin, renderer);

    }

    void change_text(std::string str, SDL_Renderer* renderer ){
        if(animated){
            complete_text = str;
            incomplete_text = str[0];
            current_letter = 1;
            timer.start();
        } else {
            text.update_text(str, renderer);
        }
    }

    SDL_Rect box;
    int margin;
    SDL_Color text_color, box_color;
    Text text;
    std::string incomplete_text, complete_text;
    Timer timer;
    int current_letter;
    bool animated;
};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", 500, 500);
    
    font = TTF_OpenFont( "./Lato-Bold.ttf", 32 );

    if(!font){
        printf("fatal error: font not loaded correctly: %s\n", TTF_GetError());
    }

    SDL_Color red = {255,0,0,255};
    SDL_Color black = {0,0,0,255};
    //std::string long_str = "A ver que tan largo es el texto que puedo escribir aaaaa_";
    //Text test_text(long_str, red, game.renderer, font);

    TextBox tb(game.renderer, true);
    tb.box.x = 10;
    tb.box.y = 390;
    tb.box.w = 480;
    tb.box.h = 100;
    tb.margin = 10;
    tb.text_color = black;
    tb.box_color = red;
    tb.change_text("Texto de pruebardaa", game.renderer);

    //std::vector<TextBox> menu;
    //menu.reserve(4);
    TextBox menu[4];

    int menux = 30;
    int menuy = 30;
    int menuw = 100;
    int menuh = 30;
    for(int i = 0; i < 4; i++){
        TextBox box(game.renderer, true);
        box.box.x = menux;
        box.box.y = menuy;
        box.box.w = menuw;
        box.box.h = menuh;
        box.margin = 10;
        box.text_color = black;
        box.box_color = red;
        if(i==0) box.change_text("PLAY", game.renderer);
        if(i==1) box.change_text("MULTI", game.renderer);
        if(i==2) box.change_text("OPIONS", game.renderer);
        if(i==3) box.change_text("EXIT", game.renderer);
        //menu.push_back(box);
        menu[i] = box;
        
        menux += menux;
        menuy += menuy;
    }

    while(game.running){        
        SDL_RenderClear(game.renderer);

        while(SDL_PollEvent(&game.event)){
            if(game.event.type == SDL_QUIT){
                game.running = false;
            }
            if(game.event.type == SDL_MOUSEMOTION){
                
            }
            if(game.event.type == SDL_KEYDOWN){
                SDL_Color red = {255,0,0,0};
                if(game.event.key.keysym.sym == SDLK_RIGHT){
                    //test_text.update_text("RIGHT", game.renderer);
                }
                if(game.event.key.keysym.sym == SDLK_LEFT){
                    //test_text.update_text("LEFT", game.renderer);
                }
                if(game.event.key.keysym.sym == SDLK_UP){
                    //test_text.update_text("UP", game.renderer);
                }
                if(game.event.key.keysym.sym == SDLK_DOWN){
                    //test_text.update_text("DOWN", game.renderer);
                }
            }
        }

        //test_text.render(100, 100, game.renderer);
        tb.update(game.renderer);
        for (auto b : menu)
        {
            b.draw(game.renderer);
        }
        
        tb.draw(game.renderer);

        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
