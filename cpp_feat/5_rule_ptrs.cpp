#include <cstdio>
#include <memory>



struct State
{
	int number;
};

struct GObj
{
	int id;
	int numbers[1000];	
	State* state = nullptr;
};

int main(){
	printf("!!!!!!!!!!\n");
	int* ptr = nullptr;

	int r_ = 1;
	int g_ = 3;
	int b_ = 2;
	
	ptr = &r_;
	printf("ptr now is %d\n", *ptr);
	ptr = &g_;
	printf("ptr now is %d\n", *ptr);
	ptr = &b_;
	printf("ptr now is %d\n", *ptr);
	
	
	ptr = new int{33};
	printf("ptr now is %d\n", *ptr);
	delete ptr;
	ptr = nullptr;

	ptr = &b_;
	printf("ptr now is %d\n", *ptr);

	GObj a;
	GObj* ptr_a = nullptr;
	ptr_a = &a;
	//delete ptr_a;
	ptr_a = nullptr;

	auto u_ptr = std::make_unique<GObj>();
	printf("u_ptr declared\n");
	u_ptr->id = 3;
	printf("u_ptr id: %d\n", u_ptr->id);

	ptr_a = u_ptr.get();
	ptr_a = nullptr;

	auto u_ptr2 = std::move(u_ptr);
	if(u_ptr) printf("u_ptr survived with id: %d\n", u_ptr->id);
	if(u_ptr2) printf("u_ptr2 survived with id: %d\n", u_ptr2->id);
}