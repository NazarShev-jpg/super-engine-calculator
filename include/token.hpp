#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum class TokenType {
    Number, Operator, LeftParen, RightParen, Semicolon, Function, End
};

struct Token {
    TokenType type;
    std::string value;
    std::string op;
    int arity = -1;
    int precedence = -1;   // для функций без скобок (>0), иначе -1

    Token() = default;
    Token(TokenType t, const std::string& v, const std::string& o, int a = -1, int prec = -1)
        : type(t), value(v), op(o), arity(a), precedence(prec) {}
};

#endif