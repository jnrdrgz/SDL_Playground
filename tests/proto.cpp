#include <stdio.h>

class Game
{
	Game(){
		SDL_Init();
		load();
		update();
	}

	void load(){
		Background HorseGameBackground();	
	}
};

int main()
{
	Game game();
	return 0;
}