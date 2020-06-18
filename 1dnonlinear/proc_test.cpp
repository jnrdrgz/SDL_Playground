#include <stdio.h>
#include "../particles/vector2.h"

float SmoothStart2(float t){return t*t;}
float SmoothStart3(float t){return t*t*t;}
float SmoothStart4(float t){return t*t*t*t;}

Vector2 p2p(Vector2 a, Vector2 b, float t){
	Vector2 v = a + ((b-a)*t);
	printf("v.x: %.2f, v.y: %.2f\n", v.x, v.y);

	return v;
}

int	main(int argc, char const *argv[])
{
	Vector2 p1(0,700);
	Vector2 p2(700,0);

	float seconds = 0.0f;

	while(seconds < 1.0f){
		Vector2 v = p2p(p1,p2,seconds);
		printf("SM1: %.2f\nSM2:%.2f\nSM3:%.2f\n",
		SmoothStart2(seconds),SmoothStart3(seconds),SmoothStart4(seconds) );
		seconds += 0.025f;

	}

	return 0;
}