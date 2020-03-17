#include <stdio.h>

class MyObj
{
public:
	MyObj(){
		printf("my obj created!\n");
	}
	MyObj(int a){
		printf("my obj created! with param %d\n", a);
	}
	~MyObj(){

	}
};

int main(int argc, char* args[])
{
	MyObj myObj;
	MyObj myObj2(1);
}
