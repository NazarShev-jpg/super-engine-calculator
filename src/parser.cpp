#include "parser.hpp"
#include "mathfunc.hpp"
#include <cctype>
#include <stack>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <map>
#include <cmath>
#include "ui/translations.hpp"

using namespace std;

// ========== Вспомогательные статические таблицы ==========
namespace
{
    int getFunctionArity(const string &name)
    {
        static const map<string, int> arityMap = {
            {"sin", 1},
            {"cos", 1},
            {"tan", 1},
            {"cot", 1},
            {"sec", 1},
            {"csc", 1},
            {"sind", 1},
            {"cosd", 1},
            {"tand", 1},
            {"cotd", 1},
            {"secd", 1},
            {"cscd", 1},
            {"asin", 1},
            {"acos", 1},
            {"atan", 1},
            {"acot", 1},
            {"asec", 1},
            {"acsc", 1},
            {"asind", 1},
            {"acosd", 1},
            {"atand", 1},
            {"acotd", 1},
            {"asecd", 1},
            {"acscd", 1},
            {"sinh", 1},
            {"cosh", 1},
            {"tanh", 1},
            {"coth", 1},
            {"sech", 1},
            {"csch", 1},
            {"asinh", 1},
            {"acosh", 1},
            {"atanh", 1},
            {"acoth", 1},
            {"asech", 1},
            {"acsch", 1},
            {"sqrt", 1},
            {"cbrt", 1},
            {"abs", 1},
            {"round", 1},
            {"floor", 1},
            {"ceil", 1},
            {"exp", 1},
            {"ln", 1},
            {"log10", 1},
            {"gamma", 1},
            {"fact", 1},
            {"isPrime", 1},
            {"sign", 1},
            {"fib", 1},
            {"erf", 1},
            {"zeta", 1},
            {"deg2rad", 1},
            {"rad2deg", 1},
            {"subfact", 1},
            {"catalan", 1},
            {"bell", 1},
            {"nextprime", 1},
            {"prevprime", 1},
            {"eulerphi", 1},
            {"moebius", 1},
            {"sigma", 1},
            {"isperfect", 1},
            {"primitiveroot", 1},
            {"isarmstrong", 1},
            {"issuperprime", 1},
            {"log", 2},
            {"root", 2},
            {"hypot", 2},
            {"atan2", 2},
            {"beta", 2},
            {"gcd", 2},
            {"lcm", 2},
            {"ncr", 2},
            {"npr", 2},
            {"tet", 2},
            {"pent", 2},
            {"stirling1", 2},
            {"stirling2", 2},
            {"clamp", 3},
            {"map", 5},
            {"modpow", 3},
            {"egcd", 2},
            {"crt", -1},
            {"factor", 1},
        };
        auto it = arityMap.find(name);
        return it != arityMap.end() ? it->second : 1;
    }

    int getPrecedence(const string &op, bool unary = false)
    {
        // Порядок приоритетов (от низшего к высшему):
        // ||(-1) < &&(0) < ==/!=(1) < </>/<=/>=(2) < |(3) < &(4)
        // < <</>>(5) < +/-(6) < *//(7) < %%(7) < ^(8) < ^^(9) < ^^^(10)
        // < постфикс(11) < унарный(12) < функция без скобок(13)
        //
        // Битовые & и | намеренно ВЫШЕ сравнений, чтобы
        // 5 & 3 == 1  →  (5&3)==1 = 1  (интуитивно для калькулятора)

        // Унарные
        if (op == "u" || op == "u+" || op == "u!")
            return 13;
        if (op == "++" || op == "--")
            return unary ? 12 : 11;
        if (unary && (op == "-" || op == "~"))
            return 12;

        // Логические
        if (op == "||")  return -1;
        if (op == "&&")  return 0;

        // Сравнения (ниже битовых)
        if (op == "==" || op == "!=")                            return 1;
        if (op == "<" || op == ">" || op == "<=" || op == ">=") return 2;

        // Битовые (выше сравнений)
        if (op == "|")   return 3;
        if (op == "&")   return 4;

        // Сдвиги
        if (op == "<<" || op == ">>")  return 5;

        // Постфиксные
        if (op == "!" || op == "!!" || op == "%")  return 11;

        // Гипероператоры
        if (op == "^^")   return 9;
        if (op == "^^^")  return 10;

        // Арифметика
        if (op == "%%")  return 7;
        if (op.length() == 1)
        {
            switch (op[0])
            {
            case '+': case '-': return 6;
            case '*': case '/': return 7;
            case '^':           return 8;
            default:            return 0;
            }
        }
        return 0;
    }

    bool isRightAssociative(const string &op)
    {
        return op == "^" || op == "^^" || op == "^^^" || op == "u" || op == "~" || op == "u!" || op == "?" || op == "?:";
    }
}

// ========== Публичные методы ==========
vector<Token> Parser::parse(const string &expr, bool greedyFunctions)
{
    Parser p;
    p.m_expr = expr;
    p.m_pos = 0;
    p.m_lastWasOperand = false;
    vector<Token> infix = p.tokenize();
    return toRPN(infix);
}

vector<Token> Parser::toRPN(const vector<Token> &tokens, bool greedyFunctions)
{
    vector<Token> output;
    stack<Token> opStack;
    bool expectOperand = true;

    auto getPrec = [&](const Token &tok) {
    if (tok.type == TokenType::Function) {
        if (tok.precedence > 0) return tok.precedence;
        else return 5; // приоритет функций со скобками
    }
    if (tok.type == TokenType::Operator) {
        bool unary = (tok.op == "u" || tok.op == "u+" || tok.op == "~" || tok.op == "u!");
        return getPrecedence(tok.op, unary);
    }
    return 0;
    };

    for (const Token &tok : tokens)
    {
        switch (tok.type)
        {
        case TokenType::Number:
            output.push_back(tok);
            expectOperand = false;
            break;
        case TokenType::Function:
        {
            if (tok.precedence > 0)
            {
                // Унарная функция без скобок: обрабатываем как правоассоциативный оператор
                int prec = tok.precedence;
                while (!opStack.empty())
                {
                    Token top = opStack.top();
                    if (top.type == TokenType::LeftParen) break;
                    int topPrec = getPrec(top);
                    // для правой ассоциативности выталкиваем только операторы с большим приоритетом
                    if (prec < topPrec)
                    {
                        output.push_back(top);
                        opStack.pop();
                    }
                    else
                    {
                        break;
                    }
                }
                opStack.push(tok);
                expectOperand = true;
            }
            else
            {
                // Обычная функция со скобками (или многоаргументная без скобок – не бывает)
                opStack.push(tok);
                expectOperand = true;
            }
            break;
        }
        case TokenType::LeftParen:
            opStack.push(tok);
            expectOperand = true;
            break;
        case TokenType::RightParen:
        {
            while (!opStack.empty() && opStack.top().type != TokenType::LeftParen)
            {
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (opStack.empty())
                throw runtime_error(tr("error_mismatched_parens"));
            opStack.pop(); // удаляем '('
            if (!opStack.empty() && opStack.top().type == TokenType::Function)
            {
                output.push_back(opStack.top());
                opStack.pop();
            }
            expectOperand = false;
            break;
        }
        case TokenType::Semicolon:
            while (!opStack.empty() && opStack.top().type != TokenType::LeftParen)
            {
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (opStack.empty())
                throw runtime_error(tr("error_misplaced_semicolon"));
            expectOperand = true;
            break;
        case TokenType::Operator:
        {
            const string &opStr = tok.op;

            // Постфиксные операторы: применяются сразу, только если перед ними был операнд
            if ((opStr == "!" || opStr == "!!" || opStr == "%" || opStr == "++" || opStr == "--") && !expectOperand)
            {
                output.push_back(tok);
                expectOperand = false;
                break;
            }
            // Тернарный оператор — особая логика
            if (opStr == "?")
            {
                while (!opStack.empty() && opStack.top().type != TokenType::LeftParen)
                {
                    int prec = getPrecedence("?");
                    int topPrec = getPrec(opStack.top());
                    if ((isRightAssociative("?") && prec < topPrec) ||
                        (!isRightAssociative("?") && prec <= topPrec))
                    {
                        output.push_back(opStack.top());
                        opStack.pop();
                    }
                    else
                        break;
                }
                opStack.push(Token{TokenType::Operator, "", "?"});
                expectOperand = true;
                break;
            }
            if (opStr == ":")
            {
                while (!opStack.empty() && opStack.top().op != "?")
                {
                    output.push_back(opStack.top());
                    opStack.pop();
                }
                if (opStack.empty())
                    throw runtime_error(tr("error_misplaced_colon"));
                opStack.top().op = "?:";
                expectOperand = true;
                break;
            }

            // Определяем, является ли оператор унарным в данном контексте
            bool unary = ((opStr == "-" || opStr == "+" || opStr == "~" || opStr == "u!" || opStr == "++" || opStr == "--") && expectOperand);
            string actualOp = unary ? (opStr == "-" ? "u" : (opStr == "+" ? "u+" : opStr)) : opStr;
            int prec = getPrecedence(actualOp, unary);

            if (!unary)
            {
                while (!opStack.empty())
                {
                    Token top = opStack.top();
                    if (top.type == TokenType::LeftParen)
                        break;
                    int topPrec = getPrec(top);
                    if ((isRightAssociative(actualOp) && prec < topPrec) ||
                        (!isRightAssociative(actualOp) && prec <= topPrec))
                    {
                        output.push_back(top);
                        opStack.pop();
                    }
                    else
                        break;
                }
            }

            if (actualOp != "u+")
            {
                opStack.push(Token{TokenType::Operator, "", actualOp});
            }
            expectOperand = (actualOp == "!" || actualOp == "!!" || actualOp == "%") ? false : true;
            break;
        }
        default:
            break;
        }
    }

    while (!opStack.empty())
    {
        Token top = opStack.top();
        if (top.type == TokenType::LeftParen)
            throw runtime_error(tr("error_mismatched_parens"));
        output.push_back(top);
        opStack.pop();
    }
    return output;
}

// ========== Приватные методы токенизации ==========
void Parser::skipSpaces()
{
    while (m_pos < m_expr.length() && isspace(m_expr[m_pos]))
        ++m_pos;
}

Token Parser::parseNumber()
{
    string num;
    bool hasDot = false, hasExp = false;
    int leadingZeros = 0;       // количество ведущих нулей
    bool inLeadingZeros = true; // true, пока не встретили ненулевую цифру или точку

    while (m_pos < m_expr.length())
    {
        char ch = m_expr[m_pos];
        if (isdigit(ch) || ch == '.' || ch == ',')
        {
            if (ch == '.' || ch == ',')
            {
                if (hasDot)
                    throw runtime_error(tr("error_multiple_separators")); // сообщение теперь "Multiple decimal separators"
                hasDot = true;
                inLeadingZeros = false;
                leadingZeros = 0;
                num += '.'; // всегда сохраняем точку для внутреннего представления
                ++m_pos;
                continue;
            }
            // Цифра
            if (inLeadingZeros)
            {
                if (ch == '0')
                {
                    ++leadingZeros; // считаем ведущие нули
                    ++m_pos;
                    continue; // не добавляем '0' в num
                }
                else
                {
                    inLeadingZeros = false; // первая ненулевая цифра
                }
            }
            num += ch; // после ведущих нулей добавляем все цифры
            ++m_pos;
        }
        else if ((ch == 'e' || ch == 'E') && !hasExp)
        {
            if (m_pos + 1 < m_expr.length())
            {
                char next = m_expr[m_pos + 1];
                if (isdigit(next) || next == '+' || next == '-')
                {
                    hasExp = true;
                    num += 'e';
                    ++m_pos;
                    if (m_expr[m_pos] == '+' || m_expr[m_pos] == '-')
                    {
                        num += m_expr[m_pos];
                        ++m_pos;
                    }
                    continue;
                }
            }
            break;
        }
        else
            break;
    }

    // Случай, когда всё число состояло из одних ведущих нулей ("000")
    if (num.empty() && leadingZeros > 0)
    {
        return Token(TokenType::Number, "0", "");
    }

    // Если были ведущие нули и точки/экспоненты ещё нет — строим "0.000...digits"
    // leadingZeros-1 нулей после точки, затем значащие цифры.
    // Примеры: 025→0.25, 0025→0.025, 0123→0.123, 05→0.5
    if (leadingZeros > 0 && !hasDot && !hasExp)
    {
        num = "0." + string(leadingZeros - 1, '0') + num;
    }

    char *end;
    double val = strtod(num.c_str(), &end);
    if (end == num.c_str())
        throw runtime_error(tr("error_invalid_number") + num);

    return Token(TokenType::Number, num, "");
}

Token Parser::parseIdentifier()
{
    string name;
    while (m_pos < m_expr.length() && (isalnum(m_expr[m_pos]) || m_expr[m_pos] == '_'))
    {
        name += m_expr[m_pos++];
    }
    string nameLower = name;
    for (char &ch : nameLower)
        ch = tolower(ch);

    // Константы
    if (nameLower == "pi" || nameLower == "e" || nameLower == "phi")
    {
        ostringstream oss;
        oss << setprecision(16);
        if (nameLower == "pi")
            oss << mathfunc::PI;
        else if (nameLower == "e")
            oss << mathfunc::E;
        else
            oss << mathfunc::PHI;
        return Token(TokenType::Number, oss.str(), "");
    }

    // Функция?
    int arity = getFunctionArity(nameLower);
    skipSpaces();
    if (m_pos < m_expr.length() && m_expr[m_pos] == '(')
    {
        ++m_pos; // проглатываем '('

        // Подсчёт количества аргументов (правильный, без учёта пробелов)
        size_t savedPos = m_pos;
        int argsCount = 0;
        int parenDepth = 1;
        bool argStarted = false;
        while (m_pos < m_expr.length() && parenDepth > 0)
        {
            char ch = m_expr[m_pos];
            if (ch == '(') parenDepth++;
            else if (ch == ')') parenDepth--;
            else if (parenDepth == 1 && ch == ';')
            {
                if (!argStarted)
                    throw runtime_error(tr("error_empty_arg") + name);
                argsCount++;
                argStarted = false;
                m_pos++;
                continue;
            }
            if (parenDepth == 1 && !isspace(ch))
            {
                argStarted = true;
            }
            m_pos++;
        }
        if (parenDepth != 0)
            throw runtime_error(tr("error_mismatched_parens"));
        if (argStarted) argsCount++;
        m_pos = savedPos;

        int actualArity = (arity == -1) ? argsCount : arity;
        if (arity > 0 && argsCount != arity)
        {
            throw runtime_error(
                tr("error_not_enough_args") + name + " " +
                tr("error_func_arity_expects") + " " + to_string(arity) + " " +
                tr("error_func_arity_args") + ", " +
                tr("error_func_arity_but") + " " + to_string(argsCount) + " " +
                tr("error_func_arity_provided"));
        }

        return Token(TokenType::Function, nameLower, "", actualArity);
    }
    else
    {
        // Без скобок – только для одноаргументных функций
        if (arity == 1)
        {
            Token tok(TokenType::Function, nameLower, "", 1);
            tok.precedence = 12;  // выше унарного (12), как и положено функции без скобок
            return tok;
        }
        else
        {
            throw runtime_error(tr("error_expected_lparen") + name);
        }
    }
}

Token Parser::parseOperator()
{
    char c = m_expr[m_pos];

    // Степени и гипероператоры
    if (c == '^')
    {
        string op;
        while (m_pos < m_expr.length() && m_expr[m_pos] == '^')
        {
            op += '^';
            ++m_pos;
        }
        if (op != "^" && op != "^^" && op != "^^^")
            throw runtime_error(tr("error_invalid_operator") + op);
        return Token(TokenType::Operator, "", op);
    }

    // Сравнения и сдвиги
    if (c == '=' || c == '!' || c == '<' || c == '>')
    {
        if (c == '<' && m_pos + 1 < m_expr.length() && m_expr[m_pos + 1] == '<')
        {
            m_pos += 2;
            return Token(TokenType::Operator, "", "<<");
        }
        if (c == '>' && m_pos + 1 < m_expr.length() && m_expr[m_pos + 1] == '>')
        {
            m_pos += 2;
            return Token(TokenType::Operator, "", ">>");
        }
        if (m_pos + 1 < m_expr.length())
        {
            char next = m_expr[m_pos + 1];
            if (c == '=' && next == '=')
            {
                m_pos += 2;
                return Token(TokenType::Operator, "", "==");
            }
            if (c == '!' && next == '=')
            {
                m_pos += 2;
                return Token(TokenType::Operator, "", "!=");
            }
            if (c == '<' && next == '=')
            {
                m_pos += 2;
                return Token(TokenType::Operator, "", "<=");
            }
            if (c == '>' && next == '=')
            {
                m_pos += 2;
                return Token(TokenType::Operator, "", ">=");
            }
        }
        if (c == '<' || c == '>')
        {
            ++m_pos;
            return Token(TokenType::Operator, "", string(1, c));
        }
        if (c == '=')
            throw runtime_error(tr("error_unknown_character") + "=");
    }

    // Логические && и ||
    if (c == '&' && m_pos + 1 < m_expr.length() && m_expr[m_pos + 1] == '&')
    {
        m_pos += 2;
        return Token(TokenType::Operator, "", "&&");
    }
    if (c == '|' && m_pos + 1 < m_expr.length() && m_expr[m_pos + 1] == '|')
    {
        m_pos += 2;
        return Token(TokenType::Operator, "", "||");
    }

    // Постфиксные ! и !!
    if (c == '!' && m_lastWasOperand)
    {
        if (m_pos + 1 < m_expr.length() && m_expr[m_pos + 1] == '!')
        {
            m_pos += 2;
            return Token(TokenType::Operator, "", "!!");
        }
        else
        {
            ++m_pos;
            return Token(TokenType::Operator, "", "!");
        }
    }

    // Остальные одиночные символы
    switch (c)
    {
    case ')':
        ++m_pos;
        return Token(TokenType::RightParen, "", ")");
    case ';':
        ++m_pos;
        return Token(TokenType::Semicolon, "", ";");
    case '%':
        if (m_pos + 1 < m_expr.length() && m_expr[m_pos + 1] == '%')
        {
            size_t ahead = m_pos + 2;
            while (ahead < m_expr.length() && isspace(m_expr[ahead]))
                ++ahead;
            if (ahead < m_expr.length() &&
                (isdigit(m_expr[ahead]) || m_expr[ahead] == '(' || isalpha(m_expr[ahead])))
            {
                m_pos += 2;
                return Token(TokenType::Operator, "", "%%");
            }
            else
            {
                ++m_pos;
                return Token(TokenType::Operator, "", "%");
            }
        }
        else
        {
            ++m_pos;
            return Token(TokenType::Operator, "", "%");
        }
    case '+':
    case '-':
    {
        if (m_lastWasOperand && m_pos + 1 < m_expr.length() && m_expr[m_pos + 1] == c)
        {
            size_t ahead = m_pos + 2;
            while (ahead < m_expr.length() && isspace(m_expr[ahead]))
                ++ahead;
            char nextCh = (ahead < m_expr.length()) ? m_expr[ahead] : '\0';
            bool isPostfix = true;
            if (isdigit(nextCh) || isalpha(nextCh) || nextCh == '(')
            {
                isPostfix = false;
            }
            else if (nextCh == '+' || nextCh == '-')
            {
                size_t temp = ahead;
                while (temp < m_expr.length() && (m_expr[temp] == '+' || m_expr[temp] == '-'))
                    ++temp;
                while (temp < m_expr.length() && isspace(m_expr[temp]))
                    ++temp;
                if (temp < m_expr.length() && (isdigit(m_expr[temp]) || isalpha(m_expr[temp]) || m_expr[temp] == '('))
                {
                    isPostfix = false;
                }
            }
            if (isPostfix)
            {
                m_pos += 2;
                return Token(TokenType::Operator, "", string(1, c) + string(1, c));
            }
        }
        ++m_pos;
        return Token(TokenType::Operator, "", string(1, c));
    }
    case '~':
        if (m_lastWasOperand)
        {
            throw runtime_error(tr("error_bitwise_not_postfix"));
        }
        ++m_pos;
        return Token(TokenType::Operator, "", "~");
    case '*':
    case '/':
    case '&':
    case '|':
    case '?':
    case ':':
        ++m_pos;
        return Token(TokenType::Operator, "", string(1, c));
    default:
        throw runtime_error(tr("error_unknown_character") + string(1, c));
    }
}

// ======================== ИСПРАВЛЕННАЯ УНАРНАЯ ЦЕПОЧКА ========================
std::vector<Token> Parser::parseUnaryChain()
{
    size_t savedPos = m_pos;
    std::vector<Token> chain;          // будет заполняться в порядке исходной строки
    std::vector<Token> innerOperators; // временный сборщик для +/-/!

    while (m_pos < m_expr.length())
    {
        skipSpaces();
        if (m_pos >= m_expr.length())
            break;

        char c = m_expr[m_pos];
        char next = (m_pos + 1 < m_expr.length()) ? m_expr[m_pos + 1] : '\0';

        // Обработка '!'
        if (c == '!')
        {
            chain.push_back(Token(TokenType::Operator, "", "u!"));
            ++m_pos;
            continue;
        }
        // Обработка '~'
        if (c == '~')
        {
            chain.push_back(Token(TokenType::Operator, "", "~"));
            ++m_pos;
            continue;
        }
        // Обработка '+' и '-'
        if (c == '+' || c == '-')
        {
            size_t start = m_pos;
            char sign = c;
            while (m_pos < m_expr.length() && m_expr[m_pos] == sign)
                ++m_pos;
            size_t count = m_pos - start; // количество подряд идущих одинаковых знаков

            if (sign == '+')
            {
                size_t incCount = count / 2; // каждые два плюса → префиксный ++
                for (size_t i = 0; i < incCount; ++i)
                {
                    innerOperators.push_back(Token(TokenType::Operator, "", "++"));
                }
                // нечётный плюс — унарный плюс (игнорируется)
            }
            else
            { // sign == '-'
                if (count % 2 == 1)
                {
                    innerOperators.push_back(Token(TokenType::Operator, "", "u")); // один унарный минус
                }
                size_t decCount = count / 2; // каждые два минуса → префиксный --
                for (size_t i = 0; i < decCount; ++i)
                {
                    innerOperators.push_back(Token(TokenType::Operator, "", "--"));
                }
            }
            continue; // не прерываем цикл, т.к. внутри while мы уже продвинули m_pos
        }
        break; // любой другой символ — конец цепочки
    }

    // Добавляем innerOperators в chain **в том порядке, как они были собраны (слева направо)**
    for (const auto &tok : innerOperators)
    {
        chain.push_back(tok);
    }

    skipSpaces();
    if (m_pos < m_expr.length() &&
        (isdigit(m_expr[m_pos]) || m_expr[m_pos] == '(' || isalpha(m_expr[m_pos])))
    {
        return chain;
    }
    else
    {
        m_pos = savedPos;
        return {};
    }
}

void Parser::addImplicitMultiplication(vector<Token> &tokens)
{
    if (m_lastWasOperand)
    {
        tokens.push_back(Token(TokenType::Operator, "", "*"));
        m_lastWasOperand = false;
    }
}

vector<Token> Parser::tokenize()
{
    vector<Token> tokens;
    m_pos = 0;
    m_lastWasOperand = false;

    while (m_pos < m_expr.length())
    {
        skipSpaces();
        if (m_pos >= m_expr.length())
            break;
        char c = m_expr[m_pos];

        // Число
        if (isdigit(c) || c == '.' || c == ',')
        {
            addImplicitMultiplication(tokens);
            tokens.push_back(parseNumber());
            m_lastWasOperand = true;
            continue;
        }

        // Идентификатор (функция или константа)
        if (isalpha(c))
        {
            addImplicitMultiplication(tokens);
            Token tok = parseIdentifier();
            tokens.push_back(tok);
            m_lastWasOperand = (tok.type != TokenType::Function);
            if (tok.type == TokenType::Function && tok.precedence <= 0)
            {
                tokens.push_back(Token(TokenType::LeftParen, "", "("));
            }
            continue;
        }

        // Открывающая скобка — вставляем неявное умножение, если нужно
        if (c == '(')
        {
            addImplicitMultiplication(tokens);
            tokens.push_back(Token(TokenType::LeftParen, "", "("));
            ++m_pos;
            m_lastWasOperand = false;
            continue;
        }

        // Унарная цепочка
        if (!m_lastWasOperand && (c == '+' || c == '-' || c == '~' || c == '!'))
        {
            size_t savedPos = m_pos;
            vector<Token> unary = parseUnaryChain();
            if (m_pos != savedPos)
            {
                tokens.insert(tokens.end(), unary.begin(), unary.end());
                continue;
            }
        }

        // Остальные операторы и скобки
        Token tok = parseOperator();
        tokens.push_back(tok);
        if (tok.type == TokenType::Number || tok.type == TokenType::RightParen)
            m_lastWasOperand = true;
        else if (tok.type == TokenType::Operator)
        {
            if (tok.op == "!" || tok.op == "!!" || tok.op == "%" || tok.op == "++" || tok.op == "--")
                m_lastWasOperand = true; // постфиксный оператор: после него снова ожидаем операнд
            else
                m_lastWasOperand = false;
        }
        else
        {
            m_lastWasOperand = false;
        }
    }
    return tokens;
}