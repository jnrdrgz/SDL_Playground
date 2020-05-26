#include <iostream>
#include <fstream>

int main(){
	std::ofstream file;
	
	file.open("level");
	//std::cout << file << "\n";
	file << "test write\n";
	file.close();
	
	return 0;
}