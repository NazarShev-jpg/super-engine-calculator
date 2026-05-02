#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <string>
#include <vector>
#include "token.hpp"

class Calculator {
public:
    double calculate(const std::string& expression);
    std::string getRPNString(const std::string& expression);
};

#endif