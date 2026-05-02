#include "calculator.hpp"
#include "parser.hpp"
#include "evaluator.hpp"
#include <string>

double Calculator::calculate(const std::string& expression) {
    auto rpn = Parser::parse(expression);
    return Evaluator::evaluateRPN(rpn);
}

std::string Calculator::getRPNString(const std::string& expression) {
    auto rpn = Parser::parse(expression);
    std::string out;
    for (const Token& t : rpn) {
        if (t.type == TokenType::Number) out += t.value + " ";
        else if (t.type == TokenType::Function) out += t.value + " ";
        else out += t.op + " ";
    }
    return out;
}