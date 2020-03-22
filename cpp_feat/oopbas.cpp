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

int main(){

	Obj o1("object 1");
	Obj o2("object 2");
	Obj o3;

	printf("%s\n", o1.prop.c_str());

	o3 = Obj("Object3 created lately");
	printf("%s\n", o3.prop.c_str());

	o3 = o1;
	printf("should be object1--> %s\n", o3.prop.c_str());
	return 0;
}