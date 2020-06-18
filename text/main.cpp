#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../SDL_Needs/game.h"
#include "../timer.h"
#include <vector>
#include <string>

const int screen_w = 640;
const int screen_h = 480;

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
        
    void draw(SDL_Renderer* renderer){
        SDL_RenderCopy( renderer, texture, NULL, &rct);
    }

    void load_texture(SDL_Color color, SDL_Renderer* renderer, TTF_Font* font){        
        if(texture){
            SDL_DestroyTexture(texture);
            texture = NULL;
        }
        
        if(!font){
            printf("fatal error: %s\n", SDL_GetError());
        }

        SDL_Surface* textSurface = TTF_RenderText_Solid( font, text.c_str(), color );
        
        if(!textSurface){
            printf("fatal error: %s\n", SDL_GetError());
        }

        texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        
        if(!texture){
            printf("fatal error: %s\n", SDL_GetError());
        }

        SDL_FreeSurface(textSurface);
    }

    std::string text;
    int w,h;
    SDL_Rect rct;
    SDL_Texture* texture = NULL;
    
};

int main(int argc, char* args[])
{
    Game game;
    game.init("test", screen_w, screen_h);
    
    TTF_Font* font = TTF_OpenFont("8-bit.ttf", 25);

    Text t{"4", 100,100,11};
    t.load_texture({255,0,0,0}, game.renderer, font);

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

        t.draw(game.renderer);    
        SDL_SetRenderDrawColor( game.renderer, 255, 255, 255, 255);
        SDL_RenderPresent(game.renderer);

       
    }
    
    
    game.close();
}
