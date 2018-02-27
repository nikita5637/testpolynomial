#include "polynomial.h"
#include <string.h>
#include <iostream>
//std::string str = "x^2+x";
std::string str = "2*x^10-100*x^9+x^8-x^7+5*x-3*x+10-5";
//std::string str = "x^10000+1+x";
//std::string str = "-x^2-x^3";
//std::string str = "x+x+x+x+x+x+x+x+x";
//std::string str = "-notvalidstd::string++++++x^2+-13xx^2321.sd-4*x^2+4x^2";
//std::string str = "2*x";
//std::string str = "+00001*x+1";
//std::string str = "";

static std::pair<Tstep, Tcoef> ParseTerm(std::string term) {
    Tstep uintStep = 1; //степень одночлена
    Tcoef intCoef = 1; //множитель одночлена
    char charSign = 1; //по умолчанию одночлен положительный, иначе sign = -1
    std::string strCoef = ""; //множитель как строка
    std::string strStep = ""; //степень как строка
    auto strIt = term.begin(); //начало одночлена-строки

    if (*strIt == '+') charSign = 1; //если первый символ +, то знак +
    else if (*strIt == '-') charSign = -1;//иначе, если первый символ -, то знак -, иначе невалид

    strIt++; //переходим к следующему символу
    while (*strIt != 'x') {
        /*пока не найдем символ 'x'*/
        if (strIt == term.end()) return std::pair<Tstep, Tcoef>(1, 0); //если прошли всю строку, а 'x' нет, то это просто число, например +5 или -7
        else strCoef += *strIt++; //иначе посимвольно считываем множитель
    }

    strIt++; //переходим на следующий символ за символом 'x'
    if (strCoef == "") { //если множитель пустой, например строка вида +x^3 или -x^5
        strCoef = "1*"; //множитель равен "1*"
    }

    if (strIt == term.end()) { //если это конец строки, то степени нет
        strStep = "^1"; //степень равна "^1"
        strCoef.erase(strCoef.end() - 1); //удаляем последний символ == * из множителя
        intCoef = std::stoi(strCoef) * charSign; //преобразуем множитель из строки в число и перемножаем на 1 или -1 в зависимости от первого символа одночлена
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
    return std::pair<Tstep, Tcoef>(uintStep, intCoef);
}

static unsigned char ChangeState(unsigned char currentState, char str) {
    /*смена текущего состояния конечного автомата
    в зависимости от текущего состояния и нового символа
    обрабатываем новый символ и переходим к новому состоянию*/

    std::string alph1 = "+-"; //00001
    std::string alph2 = "0123456789"; //00010
    std::string alph3 = "x"; //00100
    std::string alph4 = "*"; //01000
    std::string alph5 = "^"; //10000

    if (currentState == 0x1) {
        /*Как только начали обрабатывать строку с многочленом, ожидаем символ + или -*/
        if (alph1.find(str) != std::string::npos) return 0x6; //если нашли, то дальше ожидаем увидить либо цифру, либо символ 'x'
        return 0x0; //невалид
    }

    if (currentState == 0x2) {
        /*ожидаем только цифру*/
        if (alph2.find(str) != std::string::npos) return 0x3; //если нашли, то ожидаем либо цифру, либо +-
        return 0x0; //невалид
    }

    if (currentState == 0x3) {
        /*ожидаем цифру, либо +-*/
        if (alph2.find(str) != std::string::npos) return 0x3; //нашли цифру, ожинаем цифру, либо +-
        if (alph1.find(str) != std::string::npos) return 0x6; //нашли +-, ожидаем цифру, либо x
        return 0x0; //невалид
    }

    if (currentState == 0x4) {
        /*ожидаем 'x'*/
        if (alph3.find(str) != std::string::npos) return 0x11; //если нашли 'x', то ожидаем +-, либо '^'
        return 0x0; //невалид
    }

    if (currentState == 0x6) {
        /*ожидаем либо цифру, либо 'x'*/
        if (alph2.find(str) != std::string::npos) return 0xB; //если нашли цифру, то ожидаем увидеть или цифру, или '+', или '-', или '*'
        if (alph3.find(str) != std::string::npos) return 0x11; //если нашли 'x', то ожидаем или '^', или '+', или '-'
        return 0x0; //невалид
    }

    if (currentState == 0xB) {
        /*ожидаем цифру, или '-', или '+' или '*'*/
        if (alph1.find(str) != std::string::npos) return 0x6; //если нашли '+' или '-', то ожидаем либо цифру, либо 'x'
        if (alph2.find(str) != std::string::npos) return 0xB; //если нашли цифру, то ожидаем либо цифру, либо +-, либо *
        if (alph4.find(str) != std::string::npos) return 0x4; //если нашли '*', то ожидаем 'x'
        return 0x0; //невалид
    }

    if (currentState == 0x11) {
        /*ожидаем +-, либо '^'*/
        if (alph1.find(str) != std::string::npos) return 0x6; //если нашли "+-", то ожидаем цифру, или 'x'
        if (alph5.find(str) != std::string::npos) return 0x2; //если нашли '^', то ожидаем только цифру
        return 0x0; //невалид
    }

    return 0x0; //невалид
}

static std::map<Tstep, Tcoef> Parse(std::string str) {
    std::map <Tstep, Tcoef> termMap; //Карта, в которой хранятся одночлены <степень, множитель>
    std::string term = ""; //Очередной одночлен как строка
    std::pair<Tstep, Tcoef> termPair; //Очередной одночлен как пара <степень, множитель>
    auto strIt = str.begin(); //Символ строки
    auto mapIt = termMap.begin(); //Пара <степень, множитель> в карте
    unsigned char currentState = 0x1;

    if ((*strIt != '+') && (*strIt != '-')) {
        /*если в начале строки нет вообще никаких знаков, то добавляем в начало строки символ '+'*/
        str = "+" + str;
        strIt = str.begin();
    }

    while (strIt != str.end()) {
        /*проходим всю строку*/

        if (!(currentState = ChangeState(currentState, *strIt))) {
            /*если символ невалидный*/
            termMap.insert(std::pair<Tstep, Tcoef>(0, 0)); //добавляем в карту пару <0, 0>, что является невалидной парой
            return termMap;
        }

        if ((*strIt == '+' || *strIt == '-') && strIt != str.begin()) {
            /*если встретили символ '+' или '-', то заканчиваем считывать очередной одночлен,
            при условии, что это не самый первый символ строки*/

            termPair = ParseTerm(term); //парсим полученный одночлен-строку в пару

            mapIt = termMap.find(termPair.first); //ищем в карте ключ, который является степенью одночлена
            if (mapIt == termMap.end()) termMap.insert(termPair); //если не нашли, то вставляем в карту нашу новую пару
            else mapIt->second += termPair.second; //если нашли, то суммируем множители

            term = ""; //обнуляем очередной одночлен-строку
        }

        term += *strIt; //конкатенируем одночлен-строку с очередным символом
        strIt++; //след символ
    }

    if (currentState == 0x2 || currentState == 0x6 || currentState == 0x1 || currentState == 0x4) {
        /*если по окончанию строки автомат ожидает либо только цифру(значит последний символ '^')
        либо цифру или 'x'(значит последний символ '+' или '-')
        либо только +-(значит пустая строка)
        либо только 'x'(значит последний символ '*'*/
        termMap.insert(std::pair<Tstep, Tcoef>(0, 0)); //добавляем невалидную пару
        return termMap;
    }

    /* т.к. в конце строки нету символов '+' или '-', то допарсиваем последний одночлен-строку вне цикла */
    termPair = ParseTerm(term); //парсим полученный одночлен-строку в пару

    mapIt = termMap.find(termPair.first); //ищем в карте ключ, который является степенью одночлена
    if (mapIt == termMap.end()) termMap.insert(termPair); //если не нашли, то вставляем в карту нашу новую пару
    else mapIt->second += termPair.second; //если нашли, то суммируем множители

    return termMap;
}

std::string Derivative(std::string polynomial) {
    std::map<Tstep, Tcoef> termMap = Parse(polynomial); //карта пар одночленов полинома

    if (termMap.begin()->first == 0 ) {
        /*если степень == 0(чего в реализации быть не может), то невалидная строка*/
        return "Not valid!";
    }

    std::string str = ""; //результирующая строка
    for (auto it = termMap.rbegin(); it != termMap.rend(); it++) {
        Tcoef newcoef = (*it).first * (*it).second; //новый множитель, равный перемножению старого и степени
        Tstep newstep = (*it).first - 1; //степень = степень - 1
        if ((newcoef >= 1) && it != termMap.rbegin()) str += "+";
        if ((newcoef > 1 || newcoef < -1) && newstep != 0) str += std::to_string(newcoef) + "*";
        else if (newcoef != 0) str += std::to_string(newcoef);
        if (newstep != 0) str += "x";
        if (newstep > 1) str += "^" + std::to_string(newstep);
    }
    return str;
}

