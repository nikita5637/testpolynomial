#include "polynomial.h"
#include <cstring>
#include <stdio.h>
#include <regex>
#include <map>
#include <math.h>

/*Массив регулярных выражений для парсинга отдельных одночленов многочлена*/
static std::regex reg1 ("^\\+[[:digit:]]+\\*x\\^[[:digit:]]+"), //Выражение для формата +3*x^2
				reg2 ("^\\-[[:digit:]]+\\*x\\^[[:digit:]]+"), //Выражение для формата -3*x^2
				reg3 ("^\\+[[:digit:]]+\\*x"), //Выражение для формата +3*x
				reg4 ("^\\-[[:digit:]]+\\*x"), //Выражение для формата -3*x
				reg5 ("^\\+x\\^[[:digit:]]+"), //Выражение для формата +x^2
				reg6 ("^\\-x\\^[[:digit:]]+"), //Выражение для формата -x^2
				reg7 ("^\\+x"), //Выражение для формата +x
				reg8 ("^\\-x"), //Выражение для формата -x
				reg9 ("^\\+[[:digit:]]+"), //Выражение для формата +1
				reg10 ("^\\-[[:digit:]]+"); //Выражение для формата -1

//std::string str = "x^2+x";
//std::string str = "2*x^100+100*x^2";
//std::string str = "x^10000+x+1";
//std::string str = "-x^2-x^3";
//std::string str = "x+x+x+x+x+x+x+x+x";
//std::string str = "-notvalidstd::string++++++x^2+-13xx^2321.sd-4*x^2+4x^2";
//std::string str = "2*x";
//std::string str = "+00001*x-";

static unsigned short int CheckTerm(std::string term) {
	/*Проверка одночлена-строки, если не валидный, то возвращается 0,
	иначе число, с установленной 1 в бите с соответствующим номером regN*/
	unsigned short int mask = 0x0;
	if (std::regex_match(term, reg1)) mask |= 0x1;
	if (std::regex_match(term, reg2)) mask |= 0x2;
	if (std::regex_match(term, reg3)) mask |= 0x4;
	if (std::regex_match(term, reg4)) mask |= 0x8;
	if (std::regex_match(term, reg5)) mask |= 0x10;
	if (std::regex_match(term, reg6)) mask |= 0x20;
	if (std::regex_match(term, reg7)) mask |= 0x40;
	if (std::regex_match(term, reg8)) mask |= 0x80;
	if (std::regex_match(term, reg9)) mask |= 0x100;
	if (std::regex_match(term, reg10)) mask |= 0x200;
	//std::cout << mask << std::endl;
	return mask;
}

static std::pair<UI, int> ParseTerm(std::string term) {
	UI step = 1; //степень одночлена
	int coef = 1; //множитель одночлена
	//std::cout << "term: " << term << std::endl;
	if (std::regex_match(term, reg1)) sscanf(term.c_str(), "+%d*x^%d", &coef, &step);
	if (std::regex_match(term, reg2)) { sscanf(term.c_str(), "-%d*x^%d", &coef, &step); coef *= -1;};
	if (std::regex_match(term, reg3)) sscanf(term.c_str(), "+%d*x", &coef);
	if (std::regex_match(term, reg4)) { sscanf(term.c_str(), "-%d*x", &coef); coef *= -1;};
	if (std::regex_match(term, reg5)) sscanf(term.c_str(), "+x^%d", &step);
	if (std::regex_match(term, reg6)) { sscanf(term.c_str(), "-x^%d", &step); coef *= -1;};
	if (std::regex_match(term, reg7)) {coef = 1; step = 1;};
	if (std::regex_match(term, reg8)) {coef = -1; step = 1;};
	if (std::regex_match(term, reg9)) {coef = 0; step = 0;};
	if (std::regex_match(term, reg10)) {coef = 0; step = 0;};
	//std::cout << "coef = " << coef << " step = " << step << std::endl;
	return std::pair<UI, int>(step, coef);
}

static std::map<UI, int> Parse(std::string str) {
	std::map <UI, int> termMap; //Карта, в которой хранятся одночлены <степень, множитель>
	std::string term = ""; //Очередной одночлен как строка
	std::pair<UI, int> termPair; //Очередной одночлен как пара <степень, множитель>
	auto strIt = str.begin(); //Символ строки
	auto mapIt = termMap.begin(); //Пара <степень, множитель> в карте
	if ((*strIt != '+') && (*strIt != '-')) {
		/*если в начале строки нет вообще никаких знаков, то добавляем в первый одночлен символ '+'*/
		term = "+";
	}
	while (strIt != str.end()) {
		/*проходим всю строку*/
		if ((*strIt == '+' || *strIt == '-') && strIt != str.begin()) {
			/*если встретили символ '+' или '-', то заканчиваем считывать очередной одночлен,
			при условии, что это не самый первый символ строки*/
			if (CheckTerm(term)) {
				/*если наш одночлен синтаксически валидный*/
				termPair = ParseTerm(term); //парсим полученный одночлен-строку в пару
				mapIt = termMap.find(termPair.first); //ищем в карте ключ, который является степенью одночлена
				if (mapIt == termMap.end()) {
					termMap.insert(termPair); //если не нашли, то вставляем в карту нашу новую пару
				} else {
					mapIt->second += termPair.second; //если нашли, то суммируем множители
				}
			}
			term = ""; //обнуляем очередной одночлен-строку
		}
		term += *strIt; //конкатенируем одночлен-строку с очередным символом
		strIt++; //след символ
	}
	/* т.к. в конце строки нету символов '+' или '-', то допарсиваем последний одночлен-строку вне цикла */
	if (CheckTerm(term)) {
		/*если наш одночлен синтаксически валидный*/
		termPair = ParseTerm(term); //парсим полученный одночлен-строку в пару
		mapIt = termMap.find(termPair.first); //ищем в карте ключ, который является степенью одночлена
		if (mapIt == termMap.end()) {
			termMap.insert(termPair); //если не нашли, то вставляем в карту нашу новую пару
		} else {
			mapIt->second += termPair.second; //если нашли, то суммируем множители
		}
	}
	return termMap;
}

std::string Derivative(std::string polynomial) {
	std::map<UI, int> termMap = Parse(polynomial); //карта пар одночленов полинома
	std::string str = ""; //результирующая строка
	for (auto it = termMap.rbegin(); it != termMap.rend(); it++) {
		int newcoef = (*it).first * (*it).second; //новый множитель, равный перемножению старого и степени
		UI newstep = (*it).first == 0 ? 0 : (*it).first - 1; //степень = степень - 1, если она не равна 0
		//std::cout << std::endl << (*it).first << " : " << (*it).second << std::endl;
		//std::cout << std::endl << newcoef << " : " << newstep << std::endl;
		if ((newcoef >= 1) && it != termMap.rbegin()) str += "+";
		if ((newcoef > 1 || newcoef < -1) && newstep != 0) str += std::to_string(newcoef) + "*";
		else if (newcoef != 0) str += std::to_string(newcoef);
		if (newstep != 0) str += "x";
		if (newstep > 1) str += "^" + std::to_string(newstep);
	}
	return str;
}

std::string Polynom(std::string polynomial) {
	std::map<UI, int> termMap = Parse(polynomial); //карта пар одночленов полинома
	std::string str = ""; //результирующая строка
	for (auto it = termMap.rbegin(); it != termMap.rend(); it++) {
		int newcoef = (*it).second; //новый множитель, равный перемножению старого и степени
		UI newstep = (*it).first; //степень = степень - 1, если она не равна 0
		//std::cout << std::endl << (*it).first << " : " << (*it).second << std::endl;
		//std::cout << std::endl << newcoef << " : " << newstep << std::endl;
		if ((newcoef >= 1) && it != termMap.rbegin()) str += "+";
		if ((newcoef > 1 || newcoef < -1) && newstep != 0) str += std::to_string(newcoef) + "*";
		else if (newcoef != 0) str += std::to_string(newcoef);
		if (newstep != 0) str += "x";
		if (newstep > 1) str += "^" + std::to_string(newstep);
	}
	return str;
}
