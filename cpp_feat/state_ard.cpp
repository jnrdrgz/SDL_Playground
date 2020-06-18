#include <stdio.h>

int sprite[8] = {1,1,1,1,1,1,1,1};

struct State
{
	char* name = NULL;
};

struct State1 : State
{
	State1(){
		printf("state1 constructed\n");
	}	
};

struct State2 : State
{
	State2(){
		printf("state2 constructed\n");
	}	
};

struct State3 : State
{
	State3(){
		printf("state3 constructed\n");
	}	
};

struct Game
{
	Game(int id) : id(id)
	{
		printf("game %d created\n", id);
	}
	
	~Game(){
		printf("game %d deleted\n", id);
		delete state;
		state = NULL;
	}

	void change_state(int s){
		if(state){
			delete state;
			state = NULL;
		}

		if(s == 1) state = new State1();
		else if(s == 2) state = new State2();
		else state = new State3();
	}

	int id;
	State* state = NULL;
};

int main(){

	Game game(1);

	game.change_state(1);
	game.change_state(2);
	game.change_state(3);

	return 0;
}