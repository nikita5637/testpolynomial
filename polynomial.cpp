#include "polynomial.h"
#include <cstring>
#include <stdio.h>
#include <regex>
#include <map>
#include <math.h>
#include <iostream>

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
std::string str = "2*x^10-100*x^9+x^8-x^7+5*x-3*x+10-5";
//std::string str = "x^10000+x+1";
//std::string str = "-x^2-x^3";
//std::string str = "x+x+x+x+x+x+x+x+x";
//std::string str = "-notvalidstd::string++++++x^2+-13xx^2321.sd-4*x^2+4x^2";
//std::string str = "2*x";
//std::string str = "+00001*x-";
//std::string str = "+";

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

static std::pair<Tstep, Tcoef> ParseTerm(std::string term) {
	Tstep uintStep = 1; //степень одночлена
	Tcoef intCoef = 1; //множитель одночлена
	char charSign = 1; //по умолчанию одночлен положительный, иначе sign = -1
	std::string strCoef = ""; //множитель как строка
	std::string strStep = ""; //степень как строка
	auto strIt = term.begin(); //начало одночлена-строки
	std::cout << "term: " << term << std::endl;
	if (*strIt == '+') charSign = 1; //если первый символ +, то знак +
	else if (*strIt == '-') charSign = -1;//иначе, если первый символ -, то знак -, иначе невалид
	strIt++; //переходим к следующему символу
	while (*strIt != 'x') {
		/*пока не найдем символ 'x'*/
		if (strIt == term.end()) {
			//strCoef.erase(strCoef.end() - 1); //удаляем последний символ == * из множителя
			//std::cout << strCoef << std::endl;
			//if (isdigit(atoi(strCoef.c_str()))){
				return std::pair<Tstep, Tcoef>(1, 0); //если прошли всю строку, а 'x' нет, то это просто число, например +5 или -7
			//} else {
				//std::cout << "HUUUUY";
				//return std::pair<Tstep, Tcoef>(0, 0); //если прошли всю строку, а 'x' нет, то это просто число, например +5 или -7
			//}
		}
		else strCoef += *strIt++; //иначе посимвольно считываем множитель
	}
	strIt++; //переходим на следующий символ за символом 'x'
	if (strCoef == "") { //если множитель пустой, например строка вида +x^3 или -x^5
		strCoef = "1*"; //множитель равен "1*"
	}
	if (strIt == term.end()) { //если это конец строки, то степени нет
		strStep = "^1"; //степень равна "^1" 
		strCoef.erase(strCoef.end() - 1); //удаляем последний символ == * из множителя
		intCoef = std::stol(strCoef) * charSign; //преобразуем множитель из строки в число и перемножаем на 1 или -1 в зависимости от первого символа одночлена
		return std::pair<Tstep, Tcoef>(1, intCoef); //возвращаем пару для выражений вида +5*x или -x
	}
	while (strIt != term.end()) {
		/*пока не дойдем до конца строки*/
		strStep += *strIt++; //посимвольно считываем степень
	}
	
	strCoef.erase(strCoef.end() - 1); //удаляем последний символ == * из множителя
	intCoef = std::stol(strCoef) * charSign; //преобразуем множитель из строки в число и перемножаем на 1 или -1 в зависимости от первого символа одночлена
	strStep.erase(strStep.begin()); //удаляем первый символ == ^ из степени
	uintStep = std::stol(strStep); //преобразуем степень в число
	//std::cout << "strCoef: " << strCoef << "   " << "strStep: " << strStep << std::endl;
	//std::cout << "intCoef: " << intCoef << "   " << "uintStep: " << uintStep << std::endl << std::endl;
	return std::pair<Tstep, Tcoef>(uintStep, intCoef);
}

static std::map<Tstep, Tcoef> Parse(std::string str) {
	std::map <Tstep, Tcoef> termMap; //Карта, в которой хранятся одночлены <степень, множитель>
	std::string term = ""; //Очередной одночлен как строка
	std::pair<Tstep, Tcoef> termPair; //Очередной одночлен как пара <степень, множитель>
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
				if (termPair.first == 0 && termPair.second == 0) {
					std::cout << termPair.first << " : " << termPair.second << std::endl;
					std::cout << "NOT VALID!" << std::endl;
				}
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
	std::map<Tstep, Tcoef> termMap = Parse(polynomial); //карта пар одночленов полинома
	std::string str = ""; //результирующая строка
	for (auto it = termMap.rbegin(); it != termMap.rend(); it++) {
		Tcoef newcoef = (*it).first * (*it).second; //новый множитель, равный перемножению старого и степени
		Tstep newstep = (*it).first == 0 ? 0 : (*it).first - 1; //степень = степень - 1, если она не равна 0
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
	std::map<Tstep, Tcoef> termMap = Parse(polynomial); //карта пар одночленов полинома
	std::string str = ""; //результирующая строка
	for (auto it = termMap.rbegin(); it != termMap.rend(); it++) {
		Tcoef newcoef = (*it).second; //новый множитель, равный перемножению старого и степени
		Tstep newstep = (*it).first; //степень = степень - 1, если она не равна 0
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
