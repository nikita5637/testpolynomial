#include <map>
#ifndef POLY_H
#define POLY_H

typedef unsigned int Tstep;
typedef int Tcoef;

/*Замена текущего состояния*/
static unsigned char ChangeState(unsigned char currentState, char str);

/*Вычисление производной*/
std::string Derivative(std::string polynomial);

/*Парсинг строки на отдельные одночлены*/
static std::map<Tstep, Tcoef> Parse(std::string str);

/*Парсинг отдельного одночлена в пару <степень, множитель>*/
static std::pair<Tstep, Tcoef> ParseTerm(std::string term);

#endif
