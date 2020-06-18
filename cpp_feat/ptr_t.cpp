#include <cstdio>

int main()
{
	int* ptr_a = nullptr;
	int b = 44;
	ptr_a = &b;

	int* ptr_c = ptr_a;
	int* ptr_d = ptr_a; 
	printf("%p\n", ptr_a);
	printf("%p\n", ptr_c);
	printf("%p\n", ptr_d);	
	return 0;
}