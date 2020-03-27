#include <stdio.h>
#include <string>

class Obj
{
public:
	Obj(){}
	Obj(std::string s){
		prop = s;
	}
	std::string prop;
};

int distance(int target, int guess){
    if(target <= guess){
        return guess-target;
    } else {
        return target-guess;
    }
}

int get_value(int target, int guess, int maxv){
    return distance(target, guess)/(target/maxv);  
}

int main(){

	Obj o1("object 1");
	Obj o2("object 2");
	Obj o3;

	printf("%s\n", o1.prop.c_str());

	o3 = Obj("Object3 created lately");
	printf("%s\n", o3.prop.c_str());

	o3 = o1;
	printf("should be object1--> %s\n", o3.prop.c_str());

	int target = 125000;
	int guess = 249999;
	int maxv = 255;

	printf("guess: %d, value: %d\n", guess, get_value(target,guess,maxv));

	return 0;
}