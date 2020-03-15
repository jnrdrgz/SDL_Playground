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
	bool inited = false;

	LogSystem();

	void init();

	void add_text(std::string reference, std::string text, SDL_Renderer* renderer);

	void delete_text(std::string reference);

	void update_text(std::string reference, std::string text, SDL_Renderer* renderer);

	void draw(SDL_Renderer* renderer);
};
	
#endif