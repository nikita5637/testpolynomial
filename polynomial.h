#include <map>
#ifndef POLY_H
#define POLY_H

typedef unsigned int UI;

/*Вычисление производной*/
std::string Derivative(std::string polynomial);

/*Преобразование строки в корректный полином*/
std::string Polynom(std::string polynomial);

/*Парсинг строки на отдельные одночлены*/
static std::map<UI, int> Parse(std::string str);

/*Парсинг отдельного одночлена в пару <степень, множитель>*/
static std::pair<UI, int> ParseTerm(std::string term);

/*Проверка отдельного одночлена на корректность*/
static unsigned short int CheckTerm(std::string term);

#endif
