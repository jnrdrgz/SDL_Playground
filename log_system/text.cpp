#include "text.h"
Text::Text(std::string text, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font){
    load_text_texture(text, color, renderer, font);
}

Text::~Text(){
    if(textTexture){
        SDL_DestroyTexture(textTexture);
        textTexture = NULL;
    }
}

void Text::update(std::string text, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font){
    load_text_texture(text, color, renderer, font);
}

void Text::load_text_texture(std::string text, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font){
    
    //log
    //printf("loading text texture\n");
    

    if(textTexture){
        SDL_DestroyTexture(textTexture);
        textTexture = NULL;
    }
    //log
    //printf("loading surface\n");
        
    if(!font){
        printf("fatal error: %s\n", SDL_GetError());
    }


    SDL_Surface* textSurface = TTF_RenderText_Solid( font, text.c_str(), color );
    
    if(!textSurface){
        printf("fatal error: %s\n", SDL_GetError());
    }

    len = text.length();

    //log
    //printf("loading texture\n");
    textTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
    
    if(!textTexture){
        printf("fatal error: %s\n", SDL_GetError());
    }


    w = textSurface->w;
    h = textSurface->h;
    SDL_FreeSurface(textSurface);

    this->color = color;
    this->font = font;
}

void Text::render(int x, int y, SDL_Renderer* renderer){
    src.x = 0; 
    src.y = 0;
    src.w = w;
    src.h = h;

    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    
    SDL_RenderCopy( renderer, textTexture, &src, &dst);
}