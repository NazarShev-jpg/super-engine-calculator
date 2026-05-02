#ifndef UI_CONSOLE_HPP
#define UI_CONSOLE_HPP

#include <string>

// Цвета
extern const int COLOR_DEFAULT;
extern const int COLOR_PROMPT;
extern const int COLOR_RESULT;
extern const int COLOR_ERROR;
extern const int COLOR_HELP_HEADER;
extern const int COLOR_HELP_TEXT;

// Функции цветов и языка
void setColor(int color);
std::string getSystemLanguage();

// Структура сессии (объявлена в commands.hpp, но нужна здесь)
struct Session;

// Класс Calculator необходим для showExamples
class Calculator;

// Команды интерфейса
void showGeneralHelp();
void showBasicHelp();
void showConstantsHelp();
void showOperatorsHelp();
void showFunctionsHelp();
void showDetailHelp(const std::string& name);
void searchHelp(const std::string& query);
void showEasterEgg();
void showExamples(Calculator& calc, Session& session);
void handleEgcd();
void handleFactor();
void handleCrt();

void initConsole();

#endif