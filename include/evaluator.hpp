#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <vector>
#include "token.hpp"

class Evaluator {
public:
    // Теперь стек оперирует только double
    using Value = double;

    static Value evaluateRPN(const std::vector<Token>& rpn);
};

#endif