#include "log_system.h"

LogSystem::LogSystem(){
	
}

void LogSystem::init(){
	font = TTF_OpenFont( "../log_system/fonts/Lato-Bold.ttf", size );
	if(!font){
		printf("fatal error creating log system: font not loaded correctly: %s\n", TTF_GetError());
	}
	color = {255,50,50,255};
	inited = true;
}

void LogSystem::add_text(std::string reference, std::string text, SDL_Renderer* renderer){
	if(!inited){printf("WARNING:LOG SYSTEM NOT INITED\n");}
	//text_umap[reference] =  new Text(text, color, renderer, font);
	text_umap[reference] =  new Text(reference+": "+text, color, renderer, font);
}

void LogSystem::delete_text(std::string reference){
	if(!inited){printf("WARNING:LOG SYSTEM NOT INITED\n");}
	std::unordered_map<std::string,Text*>::const_iterator text_in_pile = text_umap.find(reference);
	if(text_in_pile == text_umap.end()){
		printf("Text to delete not found in log system\n");
	} else {
		text_umap.erase(reference);
}	}

void LogSystem::update_text(const std::string reference, std::string text, SDL_Renderer* renderer){	
	if(!inited){printf("WARNING:LOG SYSTEM NOT INITED\n");}
	//text_umap[reference]->update_text_texture(text, renderer);
	text_umap[reference]->update_text_texture(reference+": "+text, renderer);
}

void LogSystem::draw(SDL_Renderer* renderer){
	if(!inited){printf("WARNING:LOG SYSTEM NOT INITED\n");}
	int pos = 0;
	for(auto t : text_umap){
		t.second->render(this->x, this->y+(pos*size), renderer);
		pos++;
	}
}