#include "polynomial.h"
#include <iostream>

std::string Generator() {
	std::string str= "";
	char alph[] = "01234-+*^56x789";
	int countSymbols = rand() % 30; //количество символов в выходной строке
	for ( ; countSymbols; countSymbols--) {
		str += alph[rand() % sizeof(alph)];
	}
	return str;
}

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
