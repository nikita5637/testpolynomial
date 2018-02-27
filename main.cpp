#include "polynomial.h"
#include <iostream>

const std::string str = "2*x^10-100*x^9+x^8-x^7+5*x-3*x+10-5";

int main(int argc, const char *argv[]) {
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			std::string tmpStr=argv[i];
			std::cout << "String = " << tmpStr << std::endl;
			std::cout << "Derivative = " << Derivative(tmpStr) << std::endl << std::endl << std::endl;
		}
	} else {
		std::cout << "String = " << str << std::endl;
		std::cout << "Derivative = " << Derivative(str) << std::endl << std::endl << std::endl;
	}
	return 0;
}
