/*# SuperCalc – Архитектура (v2.8ref). Закомментированный конспект, пытающийся охватить работу и логику.

## Общее описание
SuperCalc — инженерный калькулятор с расширенными возможностями (гипероператоры, комбинаторика, теория чисел, специальные функции).
Работает в консоли, поддерживает русский и английский языки.
**Текущее состояние:** вычисления на `double`, BigNumber отложен (заглушка в `bignumber.hpp/.cpp`).

## Основные улучшения по сравнению с v2.7
- Внедрена структура `Session` для хранения состояния (точность вывода, последний результат), передаётся в функции UI.
- Улучшена обработка унарных цепочек: корректно интерпретируются комбинации `++`, `--`, `~`, `!` с чередованием знаков, включая сложные выражения типа `---!++~~--!5`.
- Парсинг чисел: поддержка масштабирования по ведущим нулям (`0005` → `0.005`), исправлены баги.
- Evaluator переписан с использованием хелперов `ensureMinSize`, `pop1`, `pop2`, `unaryOpFunc`, `binaryOpFunc`, `comparisonFunc` – исключено дублирование кода.
- Функции в `mathfunc` структурированы, изолированы в отдельные именованные функции (например, `factFunc`, `isPrimeFunc`), добавлены внутренние утилиты проверки аргументов.
- Команды (`commands.cpp`) распределены по отдельным обработчикам (`handleLang`, `handlePrecision` и т.д.), главный `processCommand` стал компактным диспетчером.
- В `main.cpp` добавлена фильтрация пустого ввода (после удаления комментариев и пробелов), что избавляет от ложных ошибок.
- Убраны лишние инклюды в ряде файлов.

## Поток вычислений
1. **Ввод:** строка выражения или команда (`help`, `factor`, `egcd` и т.д.)
2. **Обработка команд:** `commands.cpp` — если строка распознана как команда, выполняется действие.
3. **Парсинг:** `Parser::parse()` → вектор токенов в инфиксной форме → `Parser::toRPN()` → RPN.
4. **Вычисление:** `Evaluator::evaluateRPN()` выполняет RPN с помощью стека `double`.
5. **Вывод:** результат форматируется с учётом `Session::outputPrecision`.

## Ключевые модули

### Ядро вычислений
- `token.hpp` — структура `Token` (тип, значение/оператор, арность).
- `parser.hpp/cpp` — токенизация, преобразование в RPN, обработка унарных цепочек, неявное умножение, парсинг чисел с ведущими нулями.
- `evaluator.hpp/cpp` — вычисление RPN, обработчики операторов и функций.
- `mathfunc.hpp/cpp` — все математические функции (`sin`, `log`, `gcd`, `zeta` и т.д.), сгруппированы по арности (1, 2, N). Константы `PI`, `E`, `PHI`.
- `calculator.hpp/cpp` — класс `Calculator` (обёртка над `Parser + Evaluator`), метод `getRPNString` для отладки.

### UI и команды
- `main.cpp` — главный цикл, подстановка `ans`, отладочный вывод RPN, фильтрация пустых строк.
- `commands.hpp/cpp` — структура `Session`, диспетчер команд (`processCommand` → `handleLang`, `handlePrecision`, `handleHelp`, `handleEgcd`, `handleFactor`, `handleCrt`).
- `console.hpp/cpp` — цветной вывод, функции помощи, диалоговые команды (без зависимости от `Calculator`).
- `helpdata.hpp/cpp` — детальные справки по функциям/операторам.
- `translations.hpp/cpp` — строки интерфейса на русском и английском, функция `tr()`.

### Отложено
- `bignumber.hpp/cpp` — класс `BigNumber` (PLAIN/TOWER/LEVEL_INDEX), **полностью закомментирован**. Когда будет готов, заменит `double` в Token/Evaluator/mathfunc.

## Соглашения
- Все математические функции доступны через `mathfunc::evalFunction1/2/N`.
- Арность функций задана в `getFunctionArity()` (parser.cpp).
- Новые функции добавляются в `mathfunc.cpp` (карты функций) и в `parser.cpp` (arity map).
- Имена функций и команд нечувствительны к регистру.
- Вывод результата: при `outputPrecision == -1` — авто; иначе форматированный вывод с обрезанием нулей.
- Комментарии: `//` до конца строки.
- Пустые строки и строки, состоящие только из пробелов, игнорируются.
*/

// main.cpp с выводом RPN и "мягкой" обработкой ошибок
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <cctype>
#include <algorithm>
#include "calculator.hpp"
#include "evaluator.hpp"
#include "parser.hpp"
#include "ui/console.hpp"
#include "ui/translations.hpp"
#include "ui/commands.hpp"
#include "ui/format.hpp"

using namespace std;

int main()
{
    initConsole();
    currentLang = getSystemLanguage();

    Calculator calc;
    string input;

    Session session;
    session.outputPrecision = -1;
    session.lastResult = 0.0;

    setColor(COLOR_HELP_HEADER);
    cout << "========================================\n";
    cout << "   " << tr("welcome") << "   \n";
    cout << "========================================\n";
    setColor(COLOR_HELP_TEXT);
    cout << tr("type_help") << "\n\n";

    while (true)
    {
        try
        {
            setColor(COLOR_PROMPT);
            cout << tr("prompt");
            setColor(COLOR_DEFAULT);
            if (!getline(cin, input))
                break; // конец ввода (Ctrl+Z / Ctrl+D)

            size_t commentPos = input.find("//");
            if (commentPos != string::npos)
                input = input.substr(0, commentPos);

            string inputLower = input;
            for (char &ch : inputLower)
                ch = tolower(ch);

            if (processCommand(inputLower, input, calc, session))
                continue;

            string processedInput = input;

            // замена ans
            size_t pos = 0;
            while ((pos = processedInput.find("ans", pos)) != string::npos)
            {
                bool needMulLeft = (pos > 0 && (isdigit(processedInput[pos - 1]) || isalpha(processedInput[pos - 1]) || processedInput[pos - 1] == ')'));
                size_t afterAns = pos + 3;
                bool needMulRight = (afterAns < processedInput.size() && (isdigit(processedInput[afterAns]) || isalpha(processedInput[afterAns]) || processedInput[afterAns] == '('));
                ostringstream oss;
                oss << setprecision(16) << session.lastResult;
                string replacement = (needMulLeft ? "*" : "") + oss.str() + (needMulRight ? "*" : "");
                processedInput.replace(pos, 3, replacement);
                pos += replacement.length();
            }

            // Игнорируем пустой ввод (после удаления комментариев и пробелов)
            {
                string tmp = processedInput;
                tmp.erase(remove_if(tmp.begin(), tmp.end(),
                                    [](unsigned char c)
                                    { return isspace(c); }),
                          tmp.end());
                if (tmp.empty())
                    continue;
            }

            // Отладочный вывод RPN
            auto rpn = Parser::parse(processedInput, session.greedyFunctions);
            if (session.showRPN)
            {
                cout << "[RPN] ";
                for (const auto &t : rpn)
                {
                    if (t.type == TokenType::Number)
                        cout << t.value << " ";
                    else if (t.type == TokenType::Function)
                        cout << t.value << " ";
                    else
                        cout << t.op << " ";
                }
                cout << "\n";
            }
            double result = Evaluator::evaluateRPN(rpn);
            setColor(COLOR_RESULT);
            cout << formatNumber(result, session.outputPrecision, currentLang) << endl;
            setColor(COLOR_DEFAULT);
            session.lastResult = result;
        }
        catch (const exception &e)
        {
            setColor(COLOR_ERROR);
            cerr << tr("error_prefix") << e.what() << endl;
            setColor(COLOR_DEFAULT);
        }
    }
    return 0;
}