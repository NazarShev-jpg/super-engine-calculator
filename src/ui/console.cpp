#include "ui/console.hpp"
#include "ui/helpdata.hpp"
#include "ui/translations.hpp"
#include "ui/commands.hpp"   // нужен для Session (или можно объявить struct Session;)
#include "calculator.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <numeric>
#include <cctype>
#include "ui/format.hpp"

#ifdef _WIN32
#include <windows.h>
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
std::string getSystemLanguage() {
    LANGID langId = GetUserDefaultUILanguage();
    WORD primaryLang = PRIMARYLANGID(langId);
    return (primaryLang == LANG_RUSSIAN) ? "ru" : "en";
}
void initConsole() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#else
#include <clocale>
void setColor(const char* code) {
    std::cout << "\033[" << code << "m";
}
std::string getSystemLanguage() {
    const char* lang = std::setlocale(LC_ALL, "");
    return (lang && std::string(lang).find("ru") != std::string::npos) ? "ru" : "en";
}
void initConsole() { /* nothing */ }
#endif

using namespace std;

// Цветовые константы
const int COLOR_DEFAULT      = 7;
const int COLOR_PROMPT       = 10;
const int COLOR_RESULT       = 14;
const int COLOR_ERROR        = 12;
const int COLOR_HELP_HEADER  = 11;
const int COLOR_HELP_TEXT    = 7;

// ---------- Вспомогательные локальные функции ----------

// Вывод строки заданным цветом (без перевода строки)
static void printColored(int color, const string& msg) {
    setColor(color);
    cout << msg;
    setColor(COLOR_DEFAULT);
}
// Ошибка красным
static void printError(const string& msg) { printColored(COLOR_ERROR, msg + "\n"); }
// Результат жёлтым
static void printResult(const string& msg) { printColored(COLOR_RESULT, msg + "\n"); }

// Расширенный алгоритм Евклида (локальная копия, чтобы не зависеть от commands)
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

// ========== ФУНКЦИИ ПОМОЩИ ==========
void showGeneralHelp() {
    setColor(COLOR_HELP_HEADER);
    cout << tr("help_header");
    setColor(COLOR_HELP_TEXT);
    cout << tr("help_categories") << "\n"
         << "  " << tr("help_basic") << "\n"
         << "  " << tr("help_func") << "\n"
         << "  " << tr("help_const") << "\n"
         << "  " << tr("help_oper") << "\n"
         << "  " << tr("help_detail") << "\n\n"
         << tr("help_examples") << "\n"
         << tr("help_commands") << "\n\n"
         << "\n" << tr("special_commands") << "\n"
         << tr("cmd_egcd") << "\n"
         << tr("cmd_factor") << "\n"
         << tr("cmd_crt") << "\n"
         << tr("cmd_precision") << "\n";
    setColor(COLOR_DEFAULT);
}

void showBasicHelp() {
    setColor(COLOR_HELP_HEADER); cout << tr("basic_header");
    setColor(COLOR_HELP_TEXT);
    cout << tr("basic_numbers") << "\n"
         << tr("basic_parentheses") << "\n"
         << tr("basic_operators") << "\n"
         << tr("basic_unary") << "\n"
         << tr("basic_postfix") << "\n"
         << tr("basic_precedence") << "\n"
         << tr("basic_implicit") << "\n"
         << tr("basic_ans") << "\n"
         << tr("basic_comments") << "\n\n";
    setColor(COLOR_DEFAULT);
}

void showConstantsHelp() {
    setColor(COLOR_HELP_HEADER); cout << tr("const_header");
    setColor(COLOR_HELP_TEXT);
    cout << tr("const_pi") << "\n"
         << tr("const_e") << "\n"
         << tr("const_phi") << "\n"
         << tr("const_ans") << "\n"
         << tr("const_examples") << "\n";
    setColor(COLOR_DEFAULT);
}

void showOperatorsHelp() {
    setColor(COLOR_HELP_HEADER); cout << tr("oper_header");
    setColor(COLOR_HELP_TEXT);
    cout << tr("oper_table")
         << tr("oper_notes");
    setColor(COLOR_DEFAULT);
}

void showFunctionsHelp() {
    setColor(COLOR_HELP_HEADER); cout << tr("func_header");
    setColor(COLOR_HELP_TEXT);
    cout << tr("func_trig_rad") << "\n"
         << tr("func_trig_deg") << "\n"
         << tr("func_inv_rad") << "\n"
         << tr("func_inv_deg") << "\n"
         << tr("func_hyp") << "\n"
         << tr("func_inv_hyp") << "\n"
         << tr("func_exp_log_root") << "\n"
         << tr("func_round_sign") << "\n"
         << tr("func_comb") << "\n"
         << tr("func_special") << "\n"
         << tr("func_hyper") << "\n"
         << tr("func_multi") << "\n"
         << tr("func_conv") << "\n"
         << tr("func_more") << "\n";
    setColor(COLOR_DEFAULT);
}

void showDetailHelp(const std::string& name) {
    const auto& help = getDetailedHelp();
    string key = name;
    size_t a = key.find_first_not_of(" \t");
    size_t b = key.find_last_not_of(" \t");
    if (a != string::npos && b != string::npos)
        key = key.substr(a, b - a + 1);
    else
        key = "";

    auto it = help.find(key);
    if (it != help.end()) {
        setColor(COLOR_HELP_HEADER);
        cout << (currentLang == "ru" ? "\n--- ПОМОЩЬ: " : "\n--- HELP: ") << key << " ---\n";
        setColor(COLOR_HELP_TEXT);
        cout << (currentLang == "ru" ? it->second.descRu : it->second.descEn) << "\n\n"
             << tr("examples_label") << "\n"
             << (currentLang == "ru" ? it->second.examplesRu : it->second.examplesEn) << "\n\n";
    } else {
        printError(tr("no_detail") + name + "'.");
        cout << tr("try_list") << "\n";
    }
    setColor(COLOR_DEFAULT);
}

void searchHelp(const std::string& query) {
    const auto& help = getDetailedHelp();
    setColor(COLOR_HELP_HEADER); cout << tr("search_header") << query << "' ---\n";
    setColor(COLOR_HELP_TEXT);
    bool found = false;
    for (const auto& p : help) {
        string text = currentLang == "ru" ? p.second.descRu : p.second.descEn;
        if (text.find(query) != string::npos || p.first.find(query) != string::npos) {
            cout << "• " << p.first << " - " << text.substr(0, 60) << "...\n";
            found = true;
        }
    }
    if (!found) cout << tr("search_none") << "\n";
    setColor(COLOR_DEFAULT);
}

void showEasterEgg() {
    setColor(COLOR_RESULT);
    cout << "\n  ╔══════════════════════════════════════╗\n"
            "  ║  Congratulations! You found an egg!  ║\n"
            "  ║  Super Calculator was made with ❤️   ║\n"
            "  ║  Answer to the Ultimate Question: 42 ║\n"
            "  ╚══════════════════════════════════════╝\n\n";
    setColor(COLOR_DEFAULT);
}

void showExamples(Calculator& calc, Session& session) {
    vector<string> examples = {
        "2(3+5)", "2^^3", "5! & 7", "clamp(-2,0,10)", "fib(10)",
        "2>3 ? 10 : 20", "deg2rad(90)==pi/2", "map(5,0,10,0,100)",
        "2e^pi", "!(5&3) && 1", "hypot(3!,4!!)", "zeta(2)*beta(2,3)"
    };
    setColor(COLOR_HELP_HEADER); cout << tr("examples_header");
    setColor(COLOR_HELP_TEXT);   cout << tr("examples_list");
    cout << tr("examples_prompt");
    int choice; cin >> choice; cin.ignore();
    if (choice == 0) { printResult(tr("examples_cancelled")); return; }
    if (choice < 1 || choice > (int)examples.size()) { printError(tr("examples_invalid")); return; }
    string expr = examples[choice - 1];
    setColor(COLOR_PROMPT); cout << "> " << expr << "\n";
    try {
        double result = calc.calculate(expr);
        printResult(formatNumber(result, -1, currentLang));
        session.lastResult = result;
    } catch (const exception& e) {
        cerr << tr("error_prefix") << e.what() << "\n";
    }
    setColor(COLOR_DEFAULT);
}

// ---------- Диалоговые команды (без параметра Calculator) ----------
void handleEgcd() {
    cout << tr("egcd_prompt");
    string line; getline(cin, line);
    for (char& ch : line) if (ch == ',') ch = ' ';
    istringstream iss(line);
    long long a, b;
    if (!(iss >> a >> b)) { printError(tr("egcd_expects_two")); return; }
    long long gcd, x, y;
    extendedGcd(a, b, gcd, x, y);
    printResult(tr("gcd_label") + to_string(gcd) +
                tr("x_label") + to_string(x) +
                tr("y_label") + to_string(y));
    cout << a << "*" << x << " + " << b << "*" << y << " = " << gcd << "\n";
    setColor(COLOR_DEFAULT);
}

void handleFactor() {
    cout << tr("factor_prompt");
    string line; getline(cin, line);
    string cleaned;
    for (char ch : line) if (isdigit(ch) || ch == '-') cleaned += ch;
    if (cleaned.empty()) { printError(tr("factor_expects_one")); return; }
    long long n = stoll(cleaned);
    if (n < 2) { printResult(tr("factorization") + ": " + to_string(n)); return; }
    map<long long, int> factors;
    long long temp = n;
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
    setColor(COLOR_DEFAULT);
}

void handleCrt() {
    cout << tr("crt_prompt");
    string line; getline(cin, line);
    for (char& ch : line) if (ch == ',') ch = ' ';
    istringstream iss(line);
    vector<long long> rem, mod;
    long long r, m;
    while (iss >> r >> m) { rem.push_back(r); mod.push_back(m); }
    if (rem.size() < 2) { printError(tr("crt_at_least_two")); return; }
    for (size_t i = 0; i < mod.size(); ++i)
        for (size_t j = i + 1; j < mod.size(); ++j)
            if (gcd(mod[i], mod[j]) != 1) { printError(tr("crt_coprime")); return; }
    long long prod = 1;
    for (long long m : mod) prod *= m;
    long long result = 0;
    for (size_t i = 0; i < rem.size(); ++i) {
        long long pp = prod / mod[i];
        long long inv, dummy;
        extendedGcd(pp, mod[i], dummy, inv, dummy);
        inv = (inv % mod[i] + mod[i]) % mod[i];
        result = (result + rem[i] * pp % prod * inv) % prod;
    }
    printResult(tr("crt_x_prefix") + to_string(result) +
                tr("crt_mod_infix") + to_string(prod) +
                tr("crt_mod_suffix"));
    setColor(COLOR_DEFAULT);
}