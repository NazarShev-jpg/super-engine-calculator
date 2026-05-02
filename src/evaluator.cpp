#include "evaluator.hpp"
#include "mathfunc.hpp"
#include <stack>
#include <stdexcept>
#include <string>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <cstdlib>
#include "ui/translations.hpp"

using namespace std;

// ------------------ helpers ------------------
static void ensureMinSize(stack<double>& s, size_t n, const string& op) {
    if (s.size() < n)
        throw runtime_error(tr("error_not_enough_operands") + op);
}

static double pop1(stack<double>& s) {
    double v = s.top(); s.pop();
    return v;
}

static pair<double,double> pop2(stack<double>& s) {
    double b = s.top(); s.pop();
    double a = s.top(); s.pop();
    return {a,b};
}

static void unaryOpFunc(stack<double>& s, const string& opName, function<double(double)> f) {
    ensureMinSize(s, 1, opName);
    s.push(f(pop1(s)));
}

static void binaryOpFunc(stack<double>& s, const string& opName, function<double(double,double)> f) {
    ensureMinSize(s, 2, opName);
    auto [a,b] = pop2(s);
    s.push(f(a,b));
}

static void comparisonFunc(stack<double>& s, function<bool(double,double)> cmp) {
    ensureMinSize(s, 2, "comparison");
    auto [a,b] = pop2(s);
    s.push(cmp(a,b) ? 1.0 : 0.0);
}

static long long asInt(double d, const string& errMsg) {
    if (fabs(d - round(d)) > 1e-9) throw runtime_error(errMsg);
    return static_cast<long long>(round(d));
}

// ------------------ evaluator ------------------
Evaluator::Value Evaluator::evaluateRPN(const vector<Token>& rpn) {
    stack<double> values;

    const unordered_map<string, function<void(stack<double>&)>> opHandlers = {
        // унарные
        {"u",  [](auto& s){ unaryOpFunc(s, "u", negate<double>{}); }},
        {"u+", [](auto& s){ ensureMinSize(s,1,"u+"); }},
        {"~",  [](auto& s){
            ensureMinSize(s,1,"~");
            double d = pop1(s);
            long long v = asInt(d, tr("error_bitwise_not_int"));
            s.push(static_cast<double>(~v));
        }},
        {"u!", [](auto& s){
            if (s.empty()) s.push(0.0);
            else { double d = pop1(s); s.push(d != 0.0 ? 0.0 : 1.0); }
        }},
        {"++", [](auto& s){ unaryOpFunc(s, "++", [](double x){ return x+1.0; }); }},
        {"--", [](auto& s){ unaryOpFunc(s, "--", [](double x){ return x-1.0; }); }},

        // постфиксные
        {"!",  [](auto& s){
            ensureMinSize(s,1,"!");
            double d = pop1(s);
            int n = static_cast<int>(d);
            if (n < 0 || d != n) throw runtime_error(tr("error_factorial_neg"));
            double res = 1.0;
            for (int i = 2; i <= n; ++i) res *= i;
            s.push(res);
        }},
        {"!!", [](auto& s){
            ensureMinSize(s,1,"!!");
            double d = pop1(s);
            int n = static_cast<int>(d);
            if (n < 0 || d != n) throw runtime_error(tr("error_double_factorial_neg"));
            double res = 1.0;
            for (int i = n; i > 0; i -= 2) res *= i;
            s.push(res);
        }},
        {"%",  [](auto& s){
            ensureMinSize(s,1,"%");
            s.push(pop1(s)/100.0);
        }},

        // бинарные арифметические
        {"+",  [](auto& s){ binaryOpFunc(s, "+", plus<double>{}); }},
        {"-",  [](auto& s){ binaryOpFunc(s, "-", minus<double>{}); }},
        {"*",  [](auto& s){ binaryOpFunc(s, "*", multiplies<double>{}); }},
        {"/",  [](auto& s){
            ensureMinSize(s,2,"/");
            auto [a,b] = pop2(s);
            if (b == 0.0) throw runtime_error(tr("error_div_zero"));
            s.push(a/b);
        }},
        {"^",  [](auto& s){ binaryOpFunc(s, "^", [](double a,double b){ return pow(a,b); }); }},
        {"%%", [](auto& s){
            ensureMinSize(s,2,"%%");
            auto [a,b] = pop2(s);
            if (b == 0.0) throw runtime_error(tr("error_mod_zero"));
            s.push(fmod(a,b));
        }},

        // гипероператоры
        {"^^",  [](auto& s){
            ensureMinSize(s,2,"^^");
            auto [a,b] = pop2(s);
            s.push(mathfunc::tetration(a, static_cast<int>(b)));
        }},
        {"^^^", [](auto& s){
            ensureMinSize(s,2,"^^^");
            auto [a,b] = pop2(s);
            s.push(mathfunc::pentation(a, static_cast<int>(b)));
        }},

        // побитовые / сдвиги
        {"<<", [](auto& s){
            ensureMinSize(s,2,"<<");
            auto [a,b] = pop2(s);
            long long la = asInt(a, tr("error_shift_int"));
            long long lb = asInt(b, tr("error_shift_int"));
            s.push(static_cast<double>(la << lb));
        }},
        {">>", [](auto& s){
            ensureMinSize(s,2,">>");
            auto [a,b] = pop2(s);
            long long la = asInt(a, tr("error_shift_int"));
            long long lb = asInt(b, tr("error_shift_int"));
            s.push(static_cast<double>(la >> lb));
        }},
        {"&",  [](auto& s){
            ensureMinSize(s,2,"&");
            auto [a,b] = pop2(s);
            long long la = asInt(a, tr("error_bitwise_and"));
            long long lb = asInt(b, tr("error_bitwise_and"));
            s.push(static_cast<double>(la & lb));
        }},
        {"|",  [](auto& s){
            ensureMinSize(s,2,"|");
            auto [a,b] = pop2(s);
            long long la = asInt(a, tr("error_bitwise_or"));
            long long lb = asInt(b, tr("error_bitwise_or"));
            s.push(static_cast<double>(la | lb));
        }},

        // сравнения
        {"==", [](auto& s){ comparisonFunc(s, [](double a,double b){ return fabs(a-b)<1e-9; }); }},
        {"!=", [](auto& s){ comparisonFunc(s, [](double a,double b){ return fabs(a-b)>=1e-9; }); }},
        {"<",  [](auto& s){ comparisonFunc(s, less<double>{}); }},
        {">",  [](auto& s){ comparisonFunc(s, greater<double>{}); }},
        {"<=", [](auto& s){ comparisonFunc(s, [](double a,double b){ return a <= b+1e-9; }); }},
        {">=", [](auto& s){ comparisonFunc(s, [](double a,double b){ return a+1e-9 >= b; }); }},

        // логические
        {"&&", [](auto& s){ binaryOpFunc(s, "&&", [](double a,double b){ return (a&&b)?1.0:0.0; }); }},
        {"||", [](auto& s){ binaryOpFunc(s, "||", [](double a,double b){ return (a||b)?1.0:0.0; }); }},

        // тернарный
        {"?:", [](auto& s){
            ensureMinSize(s,3,"?:");
            double falseVal = pop1(s);
            double trueVal  = pop1(s);
            double cond = pop1(s);
            s.push(cond != 0.0 ? trueVal : falseVal);
        }}
    };

    // цикл обработки токенов (без изменений)
    for (const Token& tok : rpn) {
        if (tok.type == TokenType::Number) {
            char* end;
            double val = strtod(tok.value.c_str(), &end);
            if (end == tok.value.c_str())
                throw runtime_error(tr("error_invalid_number") + tok.value);
            values.push(val);
        }
        else if (tok.type == TokenType::Function) {
            const string& name = tok.value;
            int arity = tok.arity;
            if ((int)values.size() < arity)
                throw runtime_error(tr("error_not_enough_args") + name);

            vector<double> args(arity);
            for (int i = arity - 1; i >= 0; --i)
                args[i] = pop1(values);

            if (arity == 1)
                values.push(mathfunc::evalFunction1(name, args[0]));
            else if (arity == 2)
                values.push(mathfunc::evalFunction2(name, args[0], args[1]));
            else
                values.push(mathfunc::evalFunctionN(name, args));
        }
        else if (tok.type == TokenType::Operator) {
            auto it = opHandlers.find(tok.op);
            if (it != opHandlers.end())
                it->second(values);
            else
                throw runtime_error(tr("error_unknown_operator") + tok.op);
        }
    }

    if (values.size() != 1)
        throw runtime_error(tr("error_invalid_expression"));
    return values.top();
}