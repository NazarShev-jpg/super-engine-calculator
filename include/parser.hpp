#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include "token.hpp"

class Parser {
public:
    static std::vector<Token> parse(const std::string& expr);
    static std::vector<Token> toRPN(const std::vector<Token>& tokens);

private:
    std::string m_expr;
    size_t m_pos = 0;
    bool m_lastWasOperand = false;

    void skipSpaces();
    std::vector<Token> tokenize();
    Token parseNumber();
    Token parseIdentifier();
    Token parseOperator();
    std::vector<Token> parseUnaryChain();
    void addImplicitMultiplication(std::vector<Token>& tokens);
};

#endif