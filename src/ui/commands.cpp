#include "ui/commands.hpp"
#include "ui/console.hpp"
#include "ui/translations.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <numeric>
#include <cctype>
#include <optional>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ======================== Вспомогательные утилиты ========================

// Извлекает целые числа из аргументов команды (после первого пробела).
// Возвращает пустой optional, если аргументы отсутствуют.
static optional<vector<long long>> extractArgs(const string& originalInput) {
    size_t spacePos = originalInput.find(' ');
    if (spacePos == string::npos) return nullopt;
    string argsStr = originalInput.substr(spacePos + 1);
    // меняем запятые на пробелы для удобства
    for (char& ch : argsStr)
        if (ch == ',' || ch == ';') ch = ' ';
    istringstream iss(argsStr);
    vector<long long> nums;
    long long num;
    while (iss >> num)
        nums.push_back(num);
    return nums;
}

// Выводит сообщение заданным цветом (без добавления перевода строки)
static void printColor(int color, const string& msg) {
    setColor(color);
    cout << msg;
    setColor(COLOR_DEFAULT);
}

// Вывод ошибки (красный) и результата (жёлтый)
static void printError(const string& msg) { printColor(COLOR_ERROR, msg + "\n"); }
static void printResult(const string& msg) { printColor(COLOR_RESULT, msg + "\n"); }

// Расширенный алгоритм Евклида: a*x + b*y = gcd
static void extendedGcd(long long a, long long b,
                        long long& gcd, long long& x, long long& y) {
    long long x1 = 1, y1 = 0, x2 = 0, y2 = 1;
    while (b != 0) {
        long long q = a / b;
        long long t = b; b = a % b; a = t;
        t = x2; x2 = x1 - q * x2; x1 = t;
        t = y2; y2 = y1 - q * y2; y1 = t;
    }
    gcd = a; x = x1; y = y1;
}

// ======================== Обработчики отдельных команд ========================

static bool handleLang(const string& inputLower) {
    if (inputLower == "lang en" || inputLower == "language english") {
        currentLang = "en";
        printResult(tr("lang_changed"));
        return true;
    }
    if (inputLower == "lang ru" || inputLower == "language russian") {
        currentLang = "ru";
        printResult(tr("lang_changed"));
        return true;
    }
    if (inputLower == "lang" || inputLower == "language") {
        string msg = (currentLang == "ru")
            ? "Текущий язык: Русский. Используйте 'lang ru' или 'lang en' для смены."
            : "Current language: English. Use 'lang ru' or 'lang en' to change.";
        printResult(msg);
        return true;
    }
    return false;
}

static bool handlePrecision(const string& inputLower, Session& session) {
    if (inputLower.rfind("precision", 0) != 0) return false;
    size_t spacePos = inputLower.find(' ');
    if (spacePos != string::npos) {
        int prec = -1;
        try {
            prec = stoi(inputLower.substr(spacePos + 1));
        } catch (...) {
            printError(tr("invalid_precision"));
            return true;
        }
        if (prec >= 0 && prec <= 15) {
            session.outputPrecision = prec;
            printResult(tr("precision_set") + " " + to_string(prec));
        } else {
            printError(tr("precision_range"));
        }
    } else {
        if (session.outputPrecision >= 0)
            printResult(tr("current_precision") + " " + to_string(session.outputPrecision));
        else
            printResult(tr("precision_auto"));
    }
    return true;
}

static bool handleExit(const string& inputLower) {
    if (inputLower == "exit" || inputLower == "quit") {
        printResult(tr("goodbye"));
        exit(0);
    }
    return false;
}

static bool handleClear(const string& inputLower) {
    if (inputLower == "clear" || inputLower == "cls") {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return true;
    }
    return false;
}

static bool handleHelp(const string& inputLower, const string& originalInput,
                       Calculator& calc, Session& session) {
    if (inputLower == "help" || inputLower == "?") {
        showGeneralHelp();
        return true;
    }
    if (inputLower.rfind("help ", 0) == 0) {
        string topic = originalInput.substr(5);
        // удаляем пробелы
        size_t start = topic.find_first_not_of(" \t");
        size_t end = topic.find_last_not_of(" \t");
        if (start != string::npos && end != string::npos)
            topic = topic.substr(start, end - start + 1);
        else
            topic = "";
        string topicLower = topic;
        for (char& ch : topicLower) ch = tolower(ch);

        if (topicLower == "basic") showBasicHelp();
        else if (topicLower == "func") showFunctionsHelp();
        else if (topicLower == "const") showConstantsHelp();
        else if (topicLower == "oper") showOperatorsHelp();
        else if (topicLower == "examples") showExamples(calc, session);   // <-- изменено
        else if (topicLower == "easter") showEasterEgg();
        else if (topicLower.rfind("search ", 0) == 0) searchHelp(topic.substr(7));
        else showDetailHelp(topicLower);
        return true;
    }
    return false;
}

static bool handleEgcd(const string& inputLower, const string& originalInput,
                       Calculator& calc) {
    if (inputLower.rfind("egcd", 0) != 0) return false;
    if (originalInput.find('(') != string::npos) return false;

    auto args = extractArgs(originalInput);
    if (!args) {
        handleEgcd();    // <-- без calc
        return true;
    }
    if (args->size() != 2) {
        printError(tr("egcd_expects_two"));
        return true;
    }
    long long a = (*args)[0], b = (*args)[1];
    long long gcd, x, y;
    extendedGcd(a, b, gcd, x, y);
    printResult(tr("gcd_label") + to_string(gcd) +
                tr("x_label") + to_string(x) +
                tr("y_label") + to_string(y));
    cout << a << "*" << x << " + " << b << "*" << y << " = " << gcd << endl;
    return true;
}

static bool handleFactor(const string& inputLower, const string& originalInput,
                         Calculator& calc) {
    if (inputLower.rfind("factor", 0) != 0) return false;
    if (originalInput.find('(') != string::npos) return false;

    auto args = extractArgs(originalInput);
    if (!args) {
        handleFactor();   // <-- без calc
        return true;
    }
    if (args->size() != 1) {
        printError(tr("factor_expects_one"));
        return true;
    }
    long long n = (*args)[0];
    if (n < 2) {
        printResult(tr("factorization") + ": " + to_string(n));
        return true;
    }
    long long temp = n;
    map<long long, int> factors;
    for (long long p = 2; p * p <= temp; ++p)
        while (temp % p == 0) { factors[p]++; temp /= p; }
    if (temp > 1) factors[temp]++;

    ostringstream oss;
    oss << n << tr("factor_eq");
    bool first = true;
    for (auto& kv : factors) {
        if (!first) oss << tr("factor_mul");
        oss << kv.first;
        if (kv.second > 1) oss << tr("factor_pow") << kv.second;
        first = false;
    }
    printResult(oss.str());
    return true;
}

static bool handleCrt(const string& inputLower, const string& originalInput,
                      Calculator& calc) {
    if (inputLower.rfind("crt", 0) != 0) return false;
    if (originalInput.find('(') != string::npos) return false;

    auto args = extractArgs(originalInput);
    if (!args) {
        handleCrt();      // <-- без calc
        return true;
    }
    if (args->size() < 4 || args->size() % 2 != 0) {
        printError(tr("crt_expects_pairs"));
        return true;
    }
    size_t pairs = args->size() / 2;
    vector<long long> rem(pairs), mod(pairs);
    for (size_t i = 0; i < pairs; ++i) {
        rem[i] = (*args)[2 * i];
        mod[i] = (*args)[2 * i + 1];
    }

    for (size_t i = 0; i < pairs; ++i)
        for (size_t j = i + 1; j < pairs; ++j)
            if (gcd(mod[i], mod[j]) != 1) {
                printError(tr("crt_coprime"));
                return true;
            }

    long long prod = 1;
    for (long long m : mod) prod *= m;
    long long result = 0;
    for (size_t i = 0; i < pairs; ++i) {
        long long pp = prod / mod[i];
        long long inv, dummy;
        extendedGcd(pp, mod[i], dummy, inv, dummy);
        inv = (inv % mod[i] + mod[i]) % mod[i];
        result = (result + rem[i] * pp % prod * inv) % prod;
    }
    printResult(tr("crt_x_prefix") + to_string(result) +
                tr("crt_mod_infix") + to_string(prod) +
                tr("crt_mod_suffix"));
    return true;
}

static bool handleRpn(const string& cmd, Session& session) {
    if (cmd == "rpn on") {
        session.showRPN = true;
        printResult(tr("rpn_on"));
        return true;
    }
    if (cmd == "rpn off") {
        session.showRPN = false;
        printResult(tr("rpn_off"));
        return true;
    }
    if (cmd == "rpn") {
        session.showRPN = !session.showRPN;
        printResult(session.showRPN ? tr("rpn_on") : tr("rpn_off"));
        return true;
    }
    return false;
}

static bool handleFuncMode(const string& cmd, Session& session) {
    if (cmd == "funcmode normal") {
        session.greedyFunctions = true;
        printResult(tr("funcmode_normal"));
        return true;
    }
    if (cmd == "funcmode strict") {
        session.greedyFunctions = false;
        printResult(tr("funcmode_strict"));
        return true;
    }
    return false;
}

// ======================== Главный диспетчер ========================
bool processCommand(const string& inputLower, const string& originalInput,
                    Calculator& calc, Session& session) {
    // Обрезаем начальные и конечные пробелы для корректного распознавания команд
    string cmd = inputLower;
    size_t start = cmd.find_first_not_of(" \t");
    if (start == string::npos) return false;   // одни пробелы
    size_t end = cmd.find_last_not_of(" \t");
    cmd = cmd.substr(start, end - start + 1);

    // Далее везде используем cmd вместо inputLower
    if (handleLang(cmd)) return true;
    if (handlePrecision(cmd, session)) return true;
    if (handleExit(cmd)) return true;
    if (handleClear(cmd)) return true;
    if (handleHelp(cmd, originalInput, calc, session)) return true;
    if (handleEgcd(cmd, originalInput, calc)) return true;
    if (handleFactor(cmd, originalInput, calc)) return true;
    if (handleCrt(cmd, originalInput, calc)) return true;
    if (handleRpn(cmd, session)) return true;
    if (handleFuncMode(cmd,session)) return true;
    return false;
}