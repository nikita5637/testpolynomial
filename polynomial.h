#include <map>
#ifndef POLY_H
#define POLY_H

typedef unsigned int Tstep;
typedef int Tcoef;

/*Вычисление производной*/
std::string Derivative(std::string polynomial);

/*Преобразование строки в корректный полином*/
std::string Polynom(std::string polynomial);

/*Парсинг строки на отдельные одночлены*/
static std::map<Tstep, Tcoef> Parse(std::string str);

/*Парсинг отдельного одночлена в пару <степень, множитель>*/
static std::pair<Tstep, Tcoef> ParseTerm(std::string term);

/*Проверка отдельного одночлена на корректность*/
static unsigned short int CheckTerm(std::string term);

#endif
