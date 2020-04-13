#include <cstdio>
struct Ju
{
	Ju(){
		printf("constructed\n");
	}
};

int* t(){
	int wh[2]{};
	wh[0] = 1;
	wh[1] = 4;

	int* wh_ = wh;
	return wh_;
}
int main (){
	Ju ju[10][10];

	int b = t()[0];
	int c = t()[1];
	printf("b: %d\n", b);
	printf("c: %d\n", c);
}