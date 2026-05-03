#ifndef UI_COMMANDS_HPP
#define UI_COMMANDS_HPP

#include <string>

class Calculator;

// Состояние сессии пользователя (точность вывода, последний результат)
struct Session {
    int outputPrecision = -1;
    double lastResult = 0.0;
    bool showRPN = false;
    bool greedyFunctions = false;  // false = strict, true = normal
};

// Обрабатывает ввод как команду калькулятора.
// Возвращает true, если строка была командой (выход, help, egcd, factor, crt, lang и т.д.).
// Если false — значит это математическое выражение.
bool processCommand(const std::string& inputLower, const std::string& originalInput,
                    Calculator& calc, Session& session);

#endif