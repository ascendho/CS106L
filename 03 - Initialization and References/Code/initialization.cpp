// Compile and run this code using the following command:
// g++ -std=c++23 -o initialization initialization.cpp && ./initialization

#include <iostream>

int main() {
	// Uniform Initialization, will fail
	int numOne{12.0};
	float numTwo{12.0};

	std::cout << "numOne is: " << numOne << std::endl;
	std::cout << "numTwo is: " << numTwo << std::endl;
	return 0;
}