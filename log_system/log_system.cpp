#include "log_system.h"

LogSystem::LogSystem(){
	
}

void LogSystem::init(){
	font = TTF_OpenFont( "fonts/Lato-Bold.ttf", size );
	if(!font){
		printf("fatal error creating log system: font not loaded correctly: %s\n", TTF_GetError());
	}
	color = {255,50,50};
}

void LogSystem::add_text(std::string reference, std::string text, SDL_Renderer* renderer){
	text_umap[reference] =  new Text(text, color, renderer, font);
}

void LogSystem::update_text(std::string reference, std::string text, SDL_Renderer* renderer){
	text_umap[reference]->update_text_texture(text, renderer);
}

void LogSystem::draw(SDL_Renderer* renderer){
	int pos = 0;
	for(auto t : text_umap){
		t.second->render(this->x, this->y+(pos*size), renderer);
		pos++;
	}
}