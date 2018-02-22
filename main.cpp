#include "polynomial.h"
#include <iostream>
#include <regex>

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
	std::string str = "";
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			str=argv[i];
			std::cout << "String = " << str << std::endl;
			std::cout << "Polynom = " << Polynom(str) << std::endl;
			std::cout << "Derivative = " << Derivative(str) << std::endl << std::endl << std::endl;
		}
	} else {
		while (1) {
			str = Generator();
			if (Derivative(str).length() > 5) {
				std::cout << "String = " << str << std::endl;
				std::cout << "Polynom = " << Polynom(str) << std::endl;
				std::cout << "Derivative = " << Derivative(str) << std::endl << std::endl << std::endl;
			}
		}
	}
	return 0;
}
