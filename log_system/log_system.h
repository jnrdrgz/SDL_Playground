#ifndef LOG_SYSMTEM_H
#define LOG_SYSMTEM_H

#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <string>
#include "text.h"

class LogSystem
{
public:
	TTF_Font *font = NULL;
	int size = 15;
	std::unordered_map<std::string, Text*> text_umap;// = std::unordered_map<std::string, Text>();
	SDL_Color color;
	int x = 0, y = 0;

	LogSystem(){
		
	}

	void init(){
		font = TTF_OpenFont( "fonts/Lato-Bold.ttf", size );
		if(!font){
			printf("fatal error creating log system: font not loaded correctly: %s\n", TTF_GetError());
		}
		color = {255,50,50};
	}

	void add_text(std::string reference, std::string text, SDL_Renderer* renderer){
		text_umap[reference] =  new Text(text, color, renderer, font);
	}

	void update_text(std::string text_name, std::string reference, std::string text){

	}

	void draw(SDL_Renderer* renderer){
		int pos = 0;
		for(auto t : text_umap){
			t.second->render(this->x, this->y+(pos*size), renderer);
			pos++;
		}
	}
};
	
#endif