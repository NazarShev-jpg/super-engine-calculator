#include "ui/translations.hpp"
#include <map>

std::string currentLang = "en";
const std::string VERSION = "2.8ref";

static const std::map<std::string, std::map<std::string, std::string>>& getTranslations() {
    static const std::map<std::string, std::map<std::string, std::string>> translations = {
        {"en", {
            {"welcome", "Super Calculator v" + VERSION + " by Nazar Shev"},
            {"type_help", "Type 'help' for commands, 'exit' to quit."},
            {"goodbye", "Goodbye!"},
            {"prompt", "> "},
            {"error_prefix", "Error: "},
            {"help_header", "\n====== SUPER CALCULATOR HELP ======\n\n"},
            {"help_categories", "Type 'help <category>' for more details:"},
            {"help_basic", "help basic   - basic syntax and operators"},
            {"help_func", "help func    - mathematical functions"},
            {"help_const", "help const   - built-in constants"},
            {"help_oper", "help oper    - all operators with precedence"},
            {"help_detail", "help <name>  - detailed info about a specific function/operator"},
            {"help_examples", "Examples: help sin, help ^^, help clamp, help ?:"},
            {"help_commands", "Commands: exit, quit, clear, cls, ans, lang en/ru, help, egcd, factor, crt, precision, rpn on/off (toggle Reverse Polish Notation (RPN) debug output)"},
            {"basic_header", "\n--- BASIC SYNTAX ---\n"},
            {"basic_numbers", "  Numbers: 123, 3.14, .5, 1.2e-5, 2E3"},
            {"basic_parentheses", "  Parentheses: ( ) for grouping (spaces are ignored)"},
            {"basic_operators", "  Operators: + - * / ^ (power) %% (modulo)"},
            {"basic_unary", "  Unary: -5, +5, !5 (logical NOT)"},
            {"basic_postfix", "  Postfix: 5! (factorial), 5!!, 20%"},
            {"basic_precedence", "  Precedence (high to low): postfix, unary, ^, * / %%, + -, comparison, logical"},
            {"basic_implicit", "  Implicit multiplication: 2(3+5), 2pi, (1+2)(3+4)"},
            {"basic_ans", "  Last result: ans (e.g., ans*2, 2ans)"},
            {"basic_comments", "  Comments: // comment (everything after // is ignored)"},
            {"const_header", "\n--- CONSTANTS ---\n"},
            {"const_pi", "  pi  = 3.141592653589793  (ratio of circle circumference to diameter)"},
            {"const_e",  "  e   = 2.718281828459045  (base of natural logarithm)"},
            {"const_phi","  phi = 1.618033988749895  (golden ratio (1+√5)/2)"},
            {"const_ans","  ans = last computed result (use in expressions like ans*2, 2ans)"},
            {"const_examples", "\nExamples:\n  sin(pi/2) = 1\n  ln(e) = 1\n  phi^2 = 2.618...\n  ans after 2+3: ans*2 = 10\n"},
            {"examples_header", "\n--- COOL EXAMPLES ---\n"},
            {"examples_list",
            " 1. 2(3+5)           → 16        (implicit multiplication)\n"
            " 2. 2^^3             → 16        (tetration)\n"
            " 3. 5! & 7           → 0         (bitwise AND)\n"
            " 4. clamp(-2,0,10)   → 0         (clamping)\n"
            " 5. fib(10)          → 55        (Fibonacci)\n"
            " 6. 2>3 ? 10 : 20    → 20        (ternery)\n"
            " 7. deg2rad(90)==pi/2 → 1        (comparison with epsilon)\n"
            " 8. map(5,0,10,0,100)→ 50        (linear mapping)\n"
            " 9. 2e^pi            → 46.28     (constants)\n"
            "10. !(5&3) && 1      → 1         (logic)\n"
            "11. hypot(3!,4!!)    → 10        (mixed functions)\n"
            "12. zeta(2)*beta(2,3)→ 0.137     (special functions)\n"},
            {"examples_prompt", "Enter number to run (0 to cancel): "},
            {"examples_cancelled", "Cancelled.\n"},
            {"examples_invalid", "Invalid choice.\n"},
            {"oper_header", "\n--- OPERATORS (precedence high to low) ---\n"},
            {"oper_table", 
            "  Oper (name)              | Prec | Assoc | Example\n"
            "  -------------------------+------+-------+------------\n"
            "  ! (postfix fact), !!     |  7   | left  | 5! = 120\n"
            "  % (postfix percent)      |  7   | left  | 20% = 0.2\n"
            "  ++ (postfix increment)   |  7   | left  | 5++ = 6\n"
            "  -- (postfix decrement)   |  7   | left  | 5-- = 4\n"
            "  ^^ (tetration)           |  5   | right | 2^^3 = 16\n"
            "  ^^^ (pentation)          |  6   | right | 3^^^2 ≈ 7.6e12\n"
            "  ++у (prefix increment)   |  8   | right | ++5 = 6\n"
            "  --у (prefix decrement)   |  8   | right | --5 = 4\n"
            "  -u (unary minus)         |  8   | right | -5\n"
            "  +u (unary plus)          |  8   | right | +5\n"
            "  ~ (bitwise NOT)          |  8   | right | ~0 = -1\n"
            "  !u (prefix logical NOT)  |  8   | right | !5 = 0\n"
            "  ^ (power)                |  3   | right | 2^3^2 = 512\n"
            "  *  /  %% (modulo)        |  2   | left  | 10%%3 = 1\n"
            "  +  -                     |  1   | left  | 2+3 = 5\n"
            "  << >> (bitwise shifts)   |  2   | left  | 1<<3 = 8\n"
            "  & (bitwise AND)          |  1   | left  | 5&3 = 1\n"
            "  | (bitwise OR)           |  0   | left  | 5|3 = 7\n"
            "  == != < > <= >=          |  1   | left  | 2<3 = 1\n"
            "  && (logical AND)         |  0   | left  | 1&&0 = 0\n"
            "  || (logical OR)          |  -1  | left  | 1||0 = 1\n"
            "  ?: (ternery)             |  -2  | right | 2>3?10:20 = 20\n"},
            {"oper_notes", 
            "\nNotes:\n"
            "  - Precedence: higher number = evaluated earlier.\n"
            "  - Postfix ! is factorial; prefix ! is logical NOT.\n"
            "  - Postfix % divides by 100; binary %% is modulo.\n"
            "  - ++ and -- can be prefix (++5) or postfix (5++) — increment/decrement.\n"
            "  - Bitwise operators require integer operands.\n"
            "  - Ternery ?: is right-associative and lowest precedence.\n"
            "  - Results < 1e-12 are shown as 0.\n"},
            {"func_header", "\n--- FUNCTIONS ---\n"},
            {"func_trig_rad", "  Trigonometric (radians):\n    sin, cos, tan, cot, sec, csc"},
            {"func_trig_deg", "  Trigonometric (degrees):\n    sind, cosd, tand, cotd, secd, cscd"},
            {"func_inv_rad", "  Inverse trigonometric (radians):\n    asin, acos, atan, acot, asec, acsc"},
            {"func_inv_deg", "  Inverse trigonometric (degrees):\n    asind, acosd, atand, acotd, asecd, acscd"},
            {"func_hyp", "  Hyperbolic:\n    sinh, cosh, tanh, coth, sech, csch"},
            {"func_inv_hyp", "  Inverse hyperbolic:\n    asinh, acosh, atanh, acoth, asech, acsch"},
            {"func_exp_log_root", "  Exponential, logarithmic, roots:\n    exp, ln, log10, log(base,x), sqrt, cbrt, root(x,n)"},
            {"func_round_sign", "  Rounding and sign:\n    round, floor, ceil, abs, sign"},
            {"func_comb", "  Combinatorics & number theory:\n    fact, subfact, nCr, nPr, gcd, lcm, isPrime, fib, catalan, bell, stirling1, stirling2, nextPrime, prevPrime, eulerPhi, moebius, sigma, isPerfect, primitiveRoot, isArmstrong, isSuperPrime"},
            {"func_special", "  Special functions:\n    gamma, beta, erf, zeta"},
            {"func_hyper", "  Hyperoperators:\n    tet(a,b), pent(a,b)"},
            {"func_multi", "  Multi-argument:\n    clamp(x,min,max), map(val,in_min,in_max,out_min,out_max), modpow(a,b,m)"},
            {"func_conv", "  Converters:\n    deg2rad, rad2deg"},
            {"cmd_egcd", "  egcd       - extended Euclidean algorithm (gcd and Bezout coefficients)"},
            {"cmd_factor", "  factor     - prime factorization of an integer"},
            {"cmd_crt", "  crt        - Chinese Remainder Theorem (enter pairs: r1 m1 r2 m2 ...)"},
            {"func_nextprime", "nextPrime(n): smallest prime >= n"},
            {"func_prevprime", "prevPrime(n): largest prime <= n"},
            {"func_eulerphi", "eulerPhi(n): Euler's totient function"},
            {"func_moebius", "moebius(n): Möbius function μ(n)"},
            {"func_more", "\nType 'help <function>' for details and examples.\n"},
            {"error_tetration_int_exp", "Tetration height must be a non-negative integer"},
            {"error_tetration_height_too_large", "Tetration height too large to represent as integer"},
            {"error_tetration_height_inf", "Tetration height must be a finite integer"},
            {"error_pentation_int_exp", "Pentation height must be a non-negative integer"},
            {"error_pentation_height_too_large", "Pentation height too large to represent as integer"},
            {"error_pentation_height_inf", "Pentation height must be a finite integer"},
            {"no_detail", "No detailed help available for '"},
            {"try_list", "Try 'help func' or 'help oper' for lists."},
            {"lang_changed", "Language changed to English."},
            {"search_header", "\n--- SEARCH RESULTS FOR '"},
            {"search_none", "No results found."},
            {"error_div_zero", "Division by zero"},
            {"error_mismatched_parens", "Mismatched parentheses"},
            {"error_misplaced_semicolon", "Misplaced semicolon"},
            {"error_factorial_neg", "Factorial requires non-negative integer"},
            {"error_double_factorial_neg", "Double factorial requires non-negative integer"},
            {"error_not_enough_operands", "Not enough operands for "},
            {"error_unknown_function", "Unknown function: "},
            {"error_invalid_number", "Invalid number format: "},
            {"error_invalid_expression", "Invalid expression format"},
            {"error_bitwise_and", "Bitwise AND requires integer operands"},
            {"error_bitwise_or",  "Bitwise OR requires integer operands"},
            {"error_shift_int", "Shift requires integer operands"},
            {"error_unary_minus_missing", "Missing operand for unary minus"},
            {"error_unary_plus_missing", "Missing operand for unary plus"},
            {"error_bitwise_not_int", "Bitwise NOT requires integer operand"},
            {"error_tetration_neg_exp", "Tetration requires non-negative integer exponent"},
            {"error_pentation_neg_exp", "Pentation requires non-negative integer exponent"},
            {"error_zeta_domain", "zeta(x) requires x > 1"},
            {"error_primitive_root_prime", "primitiveRoot requires prime > 2"},
            {"error_euler_phi_positive", "eulerPhi requires positive integer"},
            {"error_moebius_positive", "moebius requires positive integer"},
            {"error_sigma_positive", "sigma requires positive integer"},
            {"error_subfact_neg", "subfactorial requires non-negative integer"},
            {"error_catalan_neg", "catalan requires non-negative integer"},
            {"error_bell_neg", "bell requires non-negative integer"},
            {"error_fib_neg", "fib requires non-negative integer"},
            {"error_clamp_min_max", "clamp: min > max"},
            {"error_map_denom", "map: in_min == in_max"},
            {"error_modpow_mod_positive", "modpow: modulus must be positive"},
            {"error_crt_even_args", "crt expects an even number of arguments (remainder modulus ...)"},
            {"error_crt_coprime", "crt: moduli must be pairwise coprime"},
            {"error_stack_underflow", "Stack underflow"},
            {"error_invalid_arity", "Invalid function arity for "},
            {"error_not_enough_args", "Not enough arguments for "},
            {"error_empty_arg", "Empty argument in function "},
            {"error_expected_lparen", "Expected '(' after function name: "},
            {"error_unknown_character", "Unknown character: "},
            {"error_multiple_dots", "Multiple dots in number"},
            {"error_invalid_operator", "Invalid operator: "},
            {"error_misplaced_colon", "Misplaced ':' without '?'"},
            {"error_mod_zero", "Modulo by zero"},
            {"error_func_arity_expects", "expects"},
            {"error_func_arity_args", "argument(s)"},
            {"error_func_arity_but", "but"},
            {"error_func_arity_provided", "provided"},
            {"error_bitwise_not_postfix", "Bitwise NOT cannot be used after an operand"},
            {"error_log_base", "Logarithm base must be > 0 and not equal to 1"},
            {"error_log_negative", "Logarithm argument must be positive"},
            {"error_log_overflow", "Logarithm argument too large (overflow)"},
            {"error_bn_infinite_double", "Cannot construct BigNumber from infinite double"},
            {"error_bn_num_too_large", "Number too large to represent: "},
            {"error_bn_invalid_mantissa", "Invalid mantissa in: "},
            {"error_bn_div_zero", "Division by zero in BigNumber"},
            {"error_bn_zero_pow_zero", "0^0 is undefined"},
            {"error_bn_pow_large_exp", "BigNumber::pow: exponent too large or not integer (only integer exponents up to 1e18 are supported)"},
            {"error_bn_neg_tetration", "Negative tetration not supported"},
            {"error_bn_tetration_height", "Tetration height too large (max "},
            {"error_bn_tetration_fast", "Tetration grows too fast, operation aborted"},
            {"error_bn_neg_pentation", "Negative pentation not supported"},
            {"error_bn_pentation_height", "Pentation height too large (max "},
            {"error_bn_pentation_fast", "Pentation grows too fast, operation aborted"},
            {"error_bn_pentation_height_large", "Pentation height too large to compute"},
            {"error_value_too_large_double", "Value too large for double in this operation"},
            {"error_multiple_separators", "Multiple decimal separators in number"},
            {"egcd_prompt", "Enter two integers (a b or a,b): "},
            {"factor_prompt", "Enter an integer: "},
            {"crt_prompt", "Enter pairs (remainder modulus): "},
            {"invalid_input", "Invalid input."},
            {"factorization", "Factorization"},
            {"crt_at_least_two", "At least two pairs required."},
            {"crt_coprime", "Moduli must be coprime."},
            {"examples_label", "Examples: "},
            {"special_commands", "Special commands: "},
            {"egcd_expects_two", "egcd expects 2 integers (e.g., egcd 48 18)"},
            {"factor_not_function", "factor is a command, not a function. Use 'factor 120' without parentheses."},
            {"factor_expects_one", "factor expects one integer (e.g., factor 120)"},
            {"crt_expects_pairs", "crt expects pairs of integers (e.g., crt 2 3 3 5)"},
            {"precision_set", "Output precision set to"},
            {"precision_range", "Precision must be 0-15."},
            {"current_precision", "Current precision:"},
            {"precision_auto", "Precision: auto (default)"},
            {"invalid_precision", "Invalid precision value."},
            {"cmd_precision", "  precision  - set output decimal precision (0-15)"},
            {"lang_current", "Current language: English. Use 'lang ru' or 'lang en' to change."},
            {"rpn_on",  "RPN debug output ON"},
            {"rpn_off", "RPN debug output OFF"},
            {"gcd_label", "gcd = "},
            {"x_label", ", x = "},
            {"y_label", ", y = "},
            {"factor_eq", " = "},
            {"factor_mul", " * "},
            {"factor_pow", "^"},
            {"crt_x_prefix", "x = "},
            {"crt_mod_infix", " (mod "},
            {"crt_mod_suffix", ")"},
        }},
        {"ru", {
            {"welcome", "Супер Калькулятор версии " + VERSION + " от Назара Шева"},
            {"type_help", "Введите 'help' для команд, 'exit' для выхода."},
            {"goodbye", "До свидания!"},
            {"prompt", "> "},
            {"error_prefix", "Ошибка: "},
            {"help_header", "\n====== СУПЕР КАЛЬКУЛЯТОР — СПРАВКА ======\n\n"},
            {"help_categories", "Введите 'help <категория>' для подробностей:"},
            {"help_basic", "help basic   - базовый синтаксис и операторы"},
            {"help_func", "help func    - математические функции"},
            {"help_const", "help const   - встроенные константы"},
            {"help_oper", "help oper    - все операторы с приоритетами"},
            {"help_detail", "help <имя>   - детальная справка по функции/оператору"},
            {"help_examples", "Примеры: help sin, help ^^, help clamp, help ?:"},
            {"help_commands", "Команды: exit, quit, clear, cls, ans, lang en/ru, help, egcd, factor, crt, precision rpn on/off (Отладочный вывод Обратной Польской Нотации RPN)"},
            {"basic_header", "\n--- БАЗОВЫЙ СИНТАКСИС ---\n"},
            {"basic_numbers", "  Числа: 123, 3.14, .5, 1.2e-5, 2E3"},
            {"basic_parentheses", "  Скобки: ( ) для группировки (пробелы игнорируются)"},
            {"basic_operators", "  Операторы: + - * / ^ (степень) %% (остаток)"},
            {"basic_unary", "  Унарные: -5, +5, !5 (логическое НЕ)"},
            {"basic_postfix", "  Постфиксные: 5! (факториал), 5!!, 20%"},
            {"basic_precedence", "  Приоритет (от высокого к низкому): постфиксные, унарные, ^, * / %%, + -, сравнения, логические"},
            {"basic_implicit", "  Неявное умножение: 2(3+5), 2pi, (1+2)(3+4)"},
            {"basic_ans", "  Последний результат: ans (например, ans*2, 2ans)"},
            {"basic_comments", "  Комментарии: // комментарий (всё после // игнорируется)"},
            {"const_header", "\n--- КОНСТАНТЫ ---\n"},
            {"const_pi", "  pi  = 3.141592653589793  (отношение длины окружности к диаметру)"},
            {"const_e",  "  e   = 2.718281828459045  (основание натурального логарифма)"},
            {"const_phi","  phi = 1.618033988749895  (золотое сечение (1+√5)/2)"},
            {"const_ans","  ans = последний результат (используйте как ans*2, 2ans)"},
            {"const_examples", "\nПримеры:\n  sin(pi/2) = 1\n  ln(e) = 1\n  phi^2 = 2.618...\n  ans после 2+3: ans*2 = 10\n"},
            {"examples_header", "\n--- КРУТЫЕ ПРИМЕРЫ ---\n"},
            {"examples_list",
            " 1. 2(3+5)           → 16        (неявное умножение)\n"
            " 2. 2^^3             → 16        (тетрация)\n"
            " 3. 5! & 7           → 0         (побитовое И)\n"
            " 4. clamp(-2,0,10)   → 0         (ограничение)\n"
            " 5. fib(10)          → 55        (Фибоначчи)\n"
            " 6. 2>3 ? 10 : 20    → 20        (тернарный оператор)\n"
            " 7. deg2rad(90)==pi/2 → 1        (сравнение с погрешностью)\n"
            " 8. map(5,0,10,0,100)→ 50        (линейное отображение)\n"
            " 9. 2e^pi            → 46.28     (константы)\n"
            "10. !(5&3) && 1      → 1         (логика)\n"
            "11. hypot(3!,4!!)    → 10        (смесь функций)\n"
            "12. zeta(2)*beta(2,3)→ 0.137     (спецфункции)\n"},
            {"examples_prompt", "Введите номер для выполнения (0 для отмены): "},
            {"examples_cancelled", "Отменено.\n"},
            {"examples_invalid", "Неверный выбор.\n"},
            {"oper_header", "\n--- ОПЕРАТОРЫ (приоритет от высокого к низкому) ---\n"},
            {"oper_table",
            "  Опер (название)            | Приор | Ассоц | Пример\n"
            "  ---------------------------+-------+-------+------------\n"
            "  ! (постфиксный факториал)  |   7   | левая | 5! = 120\n"
            "  !! (двойной факториал)     |   7   | левая | 5!! = 15\n"
            "  % (постфиксный процент)    |   7   | левая | 20% = 0.2\n"
            "  ++ (постфиксный инкремент) |   7   | левая | 5++ = 6\n"
            "  -- (постфиксный декремент) |   7   | левая | 5-- = 4\n"
            "  ^^ (тетрация)              |   5   | правая| 2^^3 = 16\n"
            "  ^^^ (пентация)             |   6   | правая| 3^^^2 ≈ 7.6e12\n"
            "  ++у (префиксный инкремент) |   8   | правая| ++5 = 6\n"
            "  --у (префиксный декремент) |   8   | правая| --5 = 4\n"
            "  -у (унарный минус)         |   8   | правая| -5\n"
            "  +у (унарный плюс)          |   8   | правая| +5\n"
            "  ~ (побитовое НЕ)           |   8   | правая| ~0 = -1\n"
            "  !у (префиксное логич. НЕ)  |   8   | правая| !5 = 0\n"
            "  ^ (степень)                |   3   | правая| 2^3^2 = 512\n"
            "  *  /  %% (остаток)         |   2   | левая | 10%%3 = 1\n"
            "  +  -                       |   1   | левая | 2+3 = 5\n"
            "  << >> (побитовые сдвиги)   |   2   | левая | 1<<3 = 8\n"
            "  & (побитовое И)            |   1   | левая | 5&3 = 1\n"
            "  | (побитовое ИЛИ)          |   0   | левая | 5|3 = 7\n"
            "  == != < > <= >=            |   1   | левая | 2<3 = 1\n"
            "  && (логическое И)          |   0   | левая | 1&&0 = 0\n"
            "  || (логическое ИЛИ)        |  -1   | левая | 1||0 = 1\n"
            "  ?: (тернарный)             |  -2   | правая| 2>3?10:20 = 20\n"},
            {"oper_notes",
            "\nПримечания:\n"
            "  - Приоритет: чем выше число, тем раньше выполняется оператор.\n"
            "  - Постфиксный ! это факториал; префиксный ! — логическое НЕ.\n"
            "  - Постфиксный % делит на 100; бинарный %% — остаток от деления.\n"
            "  - ++ и -- могут быть префиксными (++5) или постфиксными (5++) — инкремент/декремент.\n"
            "  - Побитовые операторы требуют целых операндов.\n"
            "  - Тернарный ?: правоассоциативен и имеет самый низкий приоритет.\n"
            "  - Результаты меньше 1e-12 отображаются как 0.\n"},
            {"func_header", "\n--- ФУНКЦИИ ---\n"},
            {"func_trig_rad", "  Тригонометрические (радианы):\n    sin, cos, tan, cot, sec, csc"},
            {"func_trig_deg", "  Тригонометрические (градусы):\n    sind, cosd, tand, cotd, secd, cscd"},
            {"func_inv_rad", "  Обратные тригонометрические (радианы):\n    asin, acos, atan, acot, asec, acsc"},
            {"func_inv_deg", "  Обратные тригонометрические (градусы):\n    asind, acosd, atand, acotd, asecd, acscd"},
            {"func_hyp", "  Гиперболические:\n    sinh, cosh, tanh, coth, sech, csch"},
            {"func_inv_hyp", "  Обратные гиперболические:\n    asinh, acosh, atanh, acoth, asech, acsch"},
            {"func_exp_log_root", "  Степенные, логарифмические, корни:\n    exp, ln, log10, log(base,x), sqrt, cbrt, root(x,n)"},
            {"func_round_sign", "  Округления и знак:\n    round, floor, ceil, abs, sign"},
            {"func_comb", "  Комбинаторика и теория чисел:\n    fact, subfact, nCr, nPr, gcd, lcm, isPrime, fib, catalan, bell, stirling1, stirling2, nextPrime, prevPrime, eulerPhi, moebius, sigma, isPerfect, primitiveRoot, isArmstrong, isSuperPrime"},
            {"func_special", "  Специальные функции:\n    gamma, beta, erf, zeta"},
            {"func_hyper", "  Гипероператоры:\n    tet(a,b), pent(a,b)"},
            {"func_multi", "  Многоаргументные:\n    clamp(x,min,max), map(val,in_min,in_max,out_min,out_max), modpow(a,b,m)"},
            {"func_conv", "  Конвертеры:\n    deg2rad, rad2deg"},
            {"cmd_egcd", "  egcd       - расширенный алгоритм Евклида (НОД и коэффициенты Безу)"},
            {"cmd_factor", "  factor     - разложение целого числа на простые множители"},
            {"cmd_crt",   "  crt        - Китайская теорема об остатках (введите пары: r1 m1 r2 m2 ...)"},
            {"func_nextprime", "nextPrime(n): наименьшее простое >= n"},
            {"func_prevprime", "prevPrime(n): наибольшее простое <= n"},
            {"func_eulerphi", "eulerPhi(n): функция Эйлера φ(n)"},
            {"func_moebius", "moebius(n): функция Мёбиуса μ(n)"},
            {"func_more", "\nВведите 'help <функция>' для подробностей и примеров.\n"},
            {"no_detail", "Нет подробной справки для '"},
            {"error_tetration_int_exp", "Высота тетрации должна быть неотрицательным целым числом"},
            {"error_tetration_height_too_large", "Высота тетрации слишком велика для представления целым числом"},
            {"error_tetration_height_inf", "Высота тетрации должна быть конечным целым числом"},
            {"error_pentation_int_exp", "Высота пентации должна быть неотрицательным целым числом"},
            {"error_pentation_height_too_large", "Высота пентации слишком велика для представления целым числом"},
            {"error_pentation_height_inf", "Высота пентации должна быть конечным целым числом"},
            {"try_list", "Попробуйте 'help func' или 'help oper' для списков."},
            {"lang_changed", "Язык изменён на Русский."},
            {"search_header", "\n--- РЕЗУЛЬТАТЫ ПОИСКА ДЛЯ '"},
            {"search_none", "Ничего не найдено."},
            {"error_div_zero", "Деление на ноль"},
            {"error_mismatched_parens", "Несогласованные скобки"},
            {"error_misplaced_semicolon", "Точка с запятой не на месте"},
            {"error_factorial_neg", "Факториал требует неотрицательное целое"},
            {"error_double_factorial_neg", "Двойной факториал требует неотрицательное целое"},
            {"error_not_enough_operands", "Недостаточно операндов для "},
            {"error_unknown_function", "Неизвестная функция: "},
            {"error_invalid_number", "Неверный формат числа: "},
            {"error_invalid_expression", "Неверный формат выражения"},
            {"error_bitwise_and", "Побитовое И требует целые операнды"},
            {"error_bitwise_or",  "Побитовое ИЛИ требует целые операнды"},
            {"error_shift_int", "Сдвиг требует целые операнды"},
            {"error_unary_minus_missing", "Отсутствует операнд для унарного минуса"},
            {"error_unary_plus_missing", "Отсутствует операнд для унарного плюса"},
            {"error_bitwise_not_int", "Побитовое НЕ требует целый операнд"},
            {"error_tetration_neg_exp", "Тетрация требует неотрицательный целый показатель"},
            {"error_pentation_neg_exp", "Пентация требует неотрицательный целый показатель"},
            {"error_zeta_domain", "zeta(x) требует x > 1"},
            {"error_primitive_root_prime", "primitiveRoot требует простое > 2"},
            {"error_euler_phi_positive", "eulerPhi требует положительное целое"},
            {"error_moebius_positive", "moebius требует положительное целое"},
            {"error_sigma_positive", "sigma требует положительное целое"},
            {"error_subfact_neg", "Субфакториал требует неотрицательное целое"},
            {"error_catalan_neg", "Числа Каталана требуют неотрицательное целое"},
            {"error_bell_neg", "Числа Белла требуют неотрицательное целое"},
            {"error_fib_neg", "fib требует неотрицательное целое"},
            {"error_clamp_min_max", "clamp: min > max"},
            {"error_map_denom", "map: in_min == in_max"},
            {"error_modpow_mod_positive", "modpow: модуль должен быть положительным"},
            {"error_crt_even_args", "crt ожидает чётное число аргументов (остаток модуль ...)"},
            {"error_crt_coprime", "crt: модули должны быть попарно взаимно просты"},
            {"error_stack_underflow", "Опустошение стека"},
            {"error_invalid_arity", "Неверная арность функции "},
            {"error_not_enough_args", "Недостаточно аргументов для "},
            {"error_empty_arg", "Пустой аргумент в функции "},
            {"error_expected_lparen", "Ожидалась '(' после имени функции: "},
            {"error_unknown_character", "Неизвестный символ: "},
            {"error_multiple_dots", "Несколько точек в числе"},
            {"error_invalid_operator", "Неверный оператор: "},
            {"error_misplaced_colon", "':' без предшествующего '?'"},
            {"error_mod_zero", "Остаток от деления на ноль"},
            {"error_func_arity_expects", "ожидается"},
            {"error_func_arity_args", "аргументов"},
            {"error_func_arity_but", "но"},
            {"error_func_arity_provided", "передано"},
            {"error_bitwise_not_postfix", "Побитовое НЕ нельзя использовать после операнда"},
            {"error_log_base", "Основание логарифма должно быть > 0 и не равно 1"},
            {"error_log_negative", "Аргумент логарифма должен быть положительным"},
            {"error_log_overflow", "Аргумент логарифма слишком велик (переполнение)"},
            {"error_bn_infinite_double", "Невозможно создать BigNumber из бесконечного double"},
            {"error_bn_num_too_large", "Число слишком велико для представления: "},
            {"error_bn_invalid_mantissa", "Некорректная мантисса в: "},
            {"error_bn_div_zero", "Деление на ноль в BigNumber"},
            {"error_bn_zero_pow_zero", "0^0 не определён"},
            {"error_bn_pow_large_exp", "BigNumber::pow: показатель слишком велик или не целый (поддерживаются только целые показатели до 1e18)"},
            {"error_bn_neg_tetration", "Отрицательная тетрация не поддерживается"},
            {"error_bn_tetration_height", "Высота тетрации слишком велика (макс. "},
            {"error_bn_tetration_fast", "Тетрация растёт слишком быстро, операция прервана"},
            {"error_bn_neg_pentation", "Отрицательная пентация не поддерживается"},
            {"error_bn_pentation_height", "Высота пентации слишком велика (макс. "},
            {"error_bn_pentation_fast", "Пентация растёт слишком быстро, операция прервана"},
            {"error_bn_pentation_height_large", "Высота пентации слишком велика для вычисления"},
            {"error_value_too_large_double", "Значение слишком велико для double в этой операции"},
            {"error_multiple_separators", "Несколько десятичных разделителей в числе"},
            {"egcd_prompt", "Введите два целых числа (a b или a,b): "},
            {"factor_prompt", "Введите целое число: "},
            {"crt_prompt", "Введите пары (остаток модуль): "},
            {"invalid_input", "Неверный ввод."},
            {"factorization", "Разложение"},
            {"crt_at_least_two", "Требуется минимум две пары."},
            {"crt_coprime", "Модули должны быть взаимно просты."},
            {"examples_label", "Примеры: "},
            {"special_commands", "Специальные команды: "},
            {"egcd_expects_two", "egcd ожидает 2 целых числа (например, egcd 48 18)"},
            {"factor_not_function", "factor — это команда, а не функция. Используйте 'factor 120' без скобок."},
            {"factor_expects_one", "factor ожидает одно целое число (например, factor 120)"},
            {"crt_expects_pairs", "crt ожидает пары целых чисел (например, crt 2 3 3 5)"},
            {"precision_set", "Точность вывода установлена на"},
            {"precision_range", "Точность должна быть от 0 до 15."},
            {"current_precision", "Текущая точность:"},
            {"precision_auto", "Точность: авто (по умолчанию)"},
            {"invalid_precision", "Неверное значение точности."},
            {"cmd_precision", "  precision  - установить точность вывода (0-15 знаков)"},
            {"lang_current", "Текущий язык: Русский. Используйте 'lang ru' или 'lang en' для смены."},
            {"rpn_on",  "Отладочный вывод RPN включён"},
            {"rpn_off", "Отладочный вывод RPN выключен"},
            {"gcd_label", "НОД = "},
            {"x_label", ", x = "},
            {"y_label", ", y = "},
            {"factor_eq", " = "},
            {"factor_mul", " * "},
            {"factor_pow", "^"},
            {"crt_x_prefix", "x = "},
            {"crt_mod_infix", " (mod "},
            {"crt_mod_suffix", ")"},
        }}
    };
    return translations;
}

std::string tr(const std::string& key) {
    const auto& translations = getTranslations();
    auto langIt = translations.find(currentLang);
    if (langIt != translations.end()) {
        auto msgIt = langIt->second.find(key);
        if (msgIt != langIt->second.end())
            return msgIt->second;
    }
    return "[" + key + "]";
}