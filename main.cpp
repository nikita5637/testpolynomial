#include "polynomial.h"
#include <iostream>

int main(int argc, const char *argv[]) {
	extern std::string str;
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			str=argv[i];
			std::cout << "String = " << str << std::endl;
			std::cout << "Derivative = " << Derivative(str) << std::endl << std::endl << std::endl;
		}
	} else {
		std::cout << "String = " << str << std::endl;
		std::cout << "Derivative = " << Derivative(str) << std::endl << std::endl << std::endl;
	}
	return 0;
}
