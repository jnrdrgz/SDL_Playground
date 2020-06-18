#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Text
{
public:
    //NO CUMPLE REGLA DE 5
    Text(){}
    Text(std::string text, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font);
    ~Text();
    void update(std::string text, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font);
    void load_text_texture(std::string text, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font);
    
    //test
	void update_text(std::string text, SDL_Renderer* renderer){
        load_text_texture(text, this->color, renderer, this->font);
    }
    //test

    void render(int x, int y, SDL_Renderer* renderer);
private:
    SDL_Texture* textTexture = NULL;
    SDL_Rect src;
    SDL_Rect dst;
    
    SDL_Color color;
    TTF_Font* font;
    
    int w,h,len;
};

#endif