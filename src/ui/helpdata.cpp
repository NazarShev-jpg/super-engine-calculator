#include "ui/helpdata.hpp"

const std::map<std::string, HelpEntry> &getDetailedHelp()
{
    static const std::map<std::string, HelpEntry> detailedHelp = {
        // ========== КОНСТАНТЫ ==========
        {"pi", {"π ≈ 3.14159. Ratio of a circle's circumference to its diameter.",
                "π ≈ 3.14159. Отношение длины окружности к диаметру.",
                "pi = 3.141592653589793\nsin(pi/2) = 1\n2*pi = 6.283185",
                "pi = 3.141592653589793\nsin(pi/2) = 1\n2*pi = 6.283185"}},
        {"e", {"e ≈ 2.71828. Base of natural logarithm.",
                "e ≈ 2.71828. Основание натурального логарифма.",
                "e = 2.718281828459045\nln(e) = 1\ne^2 = 7.389056",
                "e = 2.718281828459045\nln(e) = 1\ne^2 = 7.389056"}},
        {"phi", {"φ ≈ 1.61803. The golden ratio (1+√5)/2.",
                 "φ ≈ 1.61803. Золотое сечение (1+√5)/2.",
                 "phi = 1.618033988749895\nphi^2 = 2.618034",
                 "phi = 1.618033988749895\nphi^2 = 2.618034"}},
        {"ans", {"Stores the last computed result. Allows chaining calculations.",
                 "Хранит последний вычисленный результат. Позволяет строить цепочки вычислений.",
                 "2+3 = 5\nans*2 = 10\n2ans = 10",
                 "2+3 = 5\nans*2 = 10\n2ans = 10"}},

        // ========== АРИФМЕТИЧЕСКИЕ ОПЕРАТОРЫ ==========
        {"+", {"Addition operator.", "Оператор сложения.", "2+3=5\n-2+5=3", "2+3=5\n-2+5=3"}},
        {"-", {"Subtraction operator.", "Оператор вычитания.", "5-2=3\n10-15=-5", "5-2=3\n10-15=-5"}},
        {"*", {"Multiplication operator.", "Оператор умножения.", "4*3=12\n-2*5=-10", "4*3=12\n-2*5=-10"}},
        {"/", {"Division operator. Division by zero causes error.", "Оператор деления. Деление на ноль вызывает ошибку.", "10/4=2.5\n1/2=0.5", "10/4=2.5\n1/2=0.5"}},
        {"^", {"Power operator (right-associative).", "Возведение в степень (правоассоциативно).", "2^3=8\n2^3^2=512", "2^3=8\n2^3^2=512"}},
        {"%%", {"Binary modulo operator (remainder). Use double %% to distinguish from postfix %.",
                 "Бинарный оператор остатка от деления. Используйте двойной %% чтобы отличить от постфиксного %.",
                 "10%%3 = 1\n20%%2 = 0\n5.5%%2.2 = 1.1\n10%%0 = error",
                 "10%%3 = 1\n20%%2 = 0\n5.5%%2.2 = 1.1\n10%%0 = ошибка"}},

        // ========== УНАРНЫЕ И ПОСТФИКСНЫЕ ==========
        {"u", {"Unary minus (internal). Negates operand.", "Унарный минус (внутренний). Изменяет знак операнда.", "-5 = -5\n-(2+3) = -5", "-5 = -5\n-(2+3) = -5"}},
        {"u+", {"Unary plus (internal). Does nothing.", "Унарный плюс (внутренний). Ничего не делает.", "+5 = 5", "+5 = 5"}},
        {"!", {"Postfix factorial or prefix logical NOT.", "Постфиксный факториал или префиксное логическое НЕ.", "5! = 120\n!5 = 0\n!0 = 1", "5! = 120\n!5 = 0\n!0 = 1"}},
        {"!!", {"Double factorial: n!! = n*(n-2)*...", "Двойной факториал: n!! = n*(n-2)*...", "5!!=15\n6!!=48", "5!!=15\n6!!=48"}},
        {"%", {"Postfix percent operator: divides by 100. Can be chained: 20%% = 0.002.",
                 "Постфиксный оператор процента: деление на 100. Можно объединять в цепочку: 20%% = 0.002.",
                 "20% = 0.2\n20%% = 0.002\n20%%% = 0.00002\n50% + 30% = 0.8",
                 "20% = 0.2\n20%% = 0.002\n20%%% = 0.00002\n50% + 30% = 0.8"}},

        // ========== СРАВНЕНИЯ ==========
        {"==", {"Equal to (with epsilon 1e-9).", "Равно (с точностью 1e-9).", "2+2==4 → 1\n5==3 → 0", "2+2==4 → 1\n5==3 → 0"}},
        {"!=", {"Not equal to.", "Не равно.", "5!=3 → 1", "5!=3 → 1"}},
        {"<", {"Less than.", "Меньше.", "2<3 → 1", "2<3 → 1"}},
        {">", {"Greater than.", "Больше.", "5>10 → 0", "5>10 → 0"}},
        {"<=", {"Less than or equal to.", "Меньше или равно.", "2+3<=5 → 1", "2+3<=5 → 1"}},
        {">=", {"Greater than or equal to.", "Больше или равно.", "10>=5 → 1", "10>=5 → 1"}},

        // ========== ЛОГИЧЕСКИЕ ==========
        {"&&", {"Logical AND.", "Логическое И.", "1&&1=1\n1&&0=0", "1&&1=1\n1&&0=0"}},
        {"||", {"Logical OR.", "Логическое ИЛИ.", "1||0=1\n0||0=0", "1||0=1\n0||0=0"}},
        {"?:", {"Ternary conditional: cond ? true : false.", "Тернарный условный оператор: условие ? истина : ложь.", "2>3 ? 10 : 20 → 20\n1 ? 2 : 3 → 2", "2>3 ? 10 : 20 → 20\n1 ? 2 : 3 → 2"}},

        // ========== ПОБИТОВЫЕ ==========
        {"<<", {"Bitwise left shift. Requires integers.", "Побитовый сдвиг влево. Требует целые операнды.", "1<<3 = 8", "1<<3 = 8"}},
        {">>", {"Bitwise right shift. Requires integers.", "Побитовый сдвиг вправо. Требует целые операнды.", "8>>2 = 2", "8>>2 = 2"}},
        {"&", {"Bitwise AND. Requires integers.", "Побитовое И. Требует целые операнды.", "5&3 = 1", "5&3 = 1"}},
        {"|", {"Bitwise OR. Requires integers.", "Побитовое ИЛИ. Требует целые операнды.", "5|3 = 7", "5|3 = 7"}},
        {"~", {"Bitwise NOT (unary). ~n = -n-1. Requires integer operand. Cannot be used after an operand.",
                "Побитовое НЕ (унарное). ~n = -n-1. Требует целый операнд. Не может использоваться после операнда.",
                "~0 = -1\n~5 = -6\n5~ = error", "~0 = -1\n~5 = -6\n5~ = ошибка"}},

        // ========== ГИПЕРОПЕРАТОРЫ (ИНФИКСНЫЕ) ==========
        {"^^", {"Tetration operator: a ^^ b = a^(a^(...^a)).", "Оператор тетрации: a ^^ b = a^(a^(...^a)).", "2^^3 = 16\n3^^2 = 27", "2^^3 = 16\n3^^2 = 27"}},
        {"^^^", {"Pentation operator: next hyperoperation.", "Оператор пентации: следующая гипероперация.", "3^^^2 = 7.6e12", "3^^^2 = 7.6e12"}},

        // ========== ТРИГОНОМЕТРИЧЕСКИЕ (РАДИАНЫ) ==========
        {"sin", {"Sine (radians).", "Синус (радианы).", "sin(pi/2)=1", "sin(pi/2)=1"}},
        {"cos", {"Cosine (radians).", "Косинус (радианы).", "cos(0)=1", "cos(0)=1"}},
        {"tan", {"Tangent (radians).", "Тангенс (радианы).", "tan(pi/4)=1", "tan(pi/4)=1"}},
        {"cot", {"Cotangent (radians).", "Котангенс (радианы).", "cot(pi/4)=1", "cot(pi/4)=1"}},
        {"sec", {"Secant (radians).", "Секанс (радианы).", "sec(0)=1", "sec(0)=1"}},
        {"csc", {"Cosecant (radians).", "Косеканс (радианы).", "csc(pi/2)=1", "csc(pi/2)=1"}},

        // ========== ТРИГОНОМЕТРИЧЕСКИЕ (ГРАДУСЫ) ==========
        {"sind", {"Sine of angle in degrees.", "Синус угла в градусах.", "sind(30)=0.5", "sind(30)=0.5"}},
        {"cosd", {"Cosine in degrees.", "Косинус в градусах.", "cosd(60)=0.5", "cosd(60)=0.5"}},
        {"tand", {"Tangent in degrees.", "Тангенс в градусах.", "tand(45)=1", "tand(45)=1"}},
        {"cotd", {"Cotangent in degrees.", "Котангенс в градусах.", "cotd(45)=1", "cotd(45)=1"}},
        {"secd", {"Secant in degrees.", "Секанс в градусах.", "secd(60)=2", "secd(60)=2"}},
        {"cscd", {"Cosecant in degrees.", "Косеканс в градусах.", "cscd(30)=2", "cscd(30)=2"}},

        // ========== ОБРАТНЫЕ ТРИГОНОМЕТРИЧЕСКИЕ (РАДИАНЫ) ==========
        {"asin", {"Arcsine (radians).", "Арксинус (радианы).", "asin(1)=1.5708", "asin(1)=1.5708"}},
        {"acos", {"Arccosine (radians).", "Арккосинус (радианы).", "acos(0)=1.5708", "acos(0)=1.5708"}},
        {"atan", {"Arctangent (radians).", "Арктангенс (радианы).", "atan(1)=0.7854", "atan(1)=0.7854"}},
        {"acot", {"Arccotangent (radians).", "Арккотангенс (радианы).", "acot(1)=0.7854", "acot(1)=0.7854"}},
        {"asec", {"Arcsecant (radians).", "Арксеканс (радианы).", "asec(1)=0", "asec(1)=0"}},
        {"acsc", {"Arccosecant (radians).", "Арккосеканс (радианы).", "acsc(1)=1.5708", "acsc(1)=1.5708"}},

        // ========== ОБРАТНЫЕ ТРИГОНОМЕТРИЧЕСКИЕ (ГРАДУСЫ) ==========
        {"asind", {"Arcsine in degrees.", "Арксинус в градусах.", "asind(0.5)=30", "asind(0.5)=30"}},
        {"acosd", {"Arccosine in degrees.", "Арккосинус в градусах.", "acosd(0.5)=60", "acosd(0.5)=60"}},
        {"atand", {"Arctangent in degrees.", "Арктангенс в градусах.", "atand(1)=45", "atand(1)=45"}},
        {"acotd", {"Arccotangent in degrees.", "Арккотангенс в градусах.", "acotd(1)=45", "acotd(1)=45"}},
        {"asecd", {"Arcsecant in degrees.", "Арксеканс в градусах.", "asecd(2)=60", "asecd(2)=60"}},
        {"acscd", {"Arccosecant in degrees.", "Арккосеканс в градусах.", "acscd(2)=30", "acscd(2)=30"}},

        // ========== ГИПЕРБОЛИЧЕСКИЕ ==========
        {"sinh", {"Hyperbolic sine.", "Гиперболический синус.", "sinh(0)=0", "sinh(0)=0"}},
        {"cosh", {"Hyperbolic cosine.", "Гиперболический косинус.", "cosh(0)=1", "cosh(0)=1"}},
        {"tanh", {"Hyperbolic tangent.", "Гиперболический тангенс.", "tanh(0)=0", "tanh(0)=0"}},
        {"coth", {"Hyperbolic cotangent.", "Гиперболический котангенс.", "coth(1)=1.313", "coth(1)=1.313"}},
        {"sech", {"Hyperbolic secant.", "Гиперболический секанс.", "sech(0)=1", "sech(0)=1"}},
        {"csch", {"Hyperbolic cosecant.", "Гиперболический косеканс.", "csch(1)=0.8509", "csch(1)=0.8509"}},

        // ========== ОБРАТНЫЕ ГИПЕРБОЛИЧЕСКИЕ ==========
        {"asinh", {"Inverse hyperbolic sine.", "Обратный гиперболический синус.", "asinh(0)=0", "asinh(0)=0"}},
        {"acosh", {"Inverse hyperbolic cosine (x≥1).", "Обратный гиперболический косинус (x≥1).", "acosh(1)=0", "acosh(1)=0"}},
        {"atanh", {"Inverse hyperbolic tangent (|x|<1).", "Обратный гиперболический тангенс (|x|<1).", "atanh(0)=0", "atanh(0)=0"}},
        {"acoth", {"Inverse hyperbolic cotangent (|x|>1).", "Обратный гиперболический котангенс (|x|>1).", "acoth(2)=0.5493", "acoth(2)=0.5493"}},
        {"asech", {"Inverse hyperbolic secant (0<x≤1).", "Обратный гиперболический секанс (0<x≤1).", "asech(0.5)=1.317", "asech(0.5)=1.317"}},
        {"acsch", {"Inverse hyperbolic cosecant (x≠0).", "Обратный гиперболический косеканс (x≠0).", "acsch(1)=0.8814", "acsch(1)=0.8814"}},

        // ========== ЭКСПОНЕНТА, ЛОГАРИФМЫ, КОРНИ ==========
        {"exp", {"Exponential function e^x.", "Экспонента e^x.", "exp(1)=2.71828", "exp(1)=2.71828"}},
        {"ln", {"Natural logarithm (base e).", "Натуральный логарифм (основание e).", "ln(e)=1", "ln(e)=1"}},
        {"log10", {"Base-10 logarithm.", "Десятичный логарифм.", "log10(100)=2", "log10(100)=2"}},
        {"log", {"log(base, x) – logarithm of x to any base.", "log(основание, x) – логарифм x по произвольному основанию.", "log(2,8)=3", "log(2,8)=3"}},
        {"sqrt", {"Square root.", "Квадратный корень.", "sqrt(16)=4", "sqrt(16)=4"}},
        {"cbrt", {"Cube root. Works for negative numbers.", "Кубический корень. Работает для отрицательных чисел.", "cbrt(27)=3", "cbrt(27)=3"}},
        {"root", {"root(x, n) – n-th root of x.", "root(x, n) – корень n-й степени из x.", "root(16,4)=2", "root(16,4)=2"}},

        // ========== ОКРУГЛЕНИЯ И ЗНАК ==========
        {"round", {"Rounds to nearest integer.", "Округление до ближайшего целого.", "round(3.7)=4", "round(3.7)=4"}},
        {"floor", {"Greatest integer ≤ x (round down).", "Наибольшее целое ≤ x (округление вниз).", "floor(3.7)=3", "floor(3.7)=3"}},
        {"ceil", {"Smallest integer ≥ x (round up).", "Наименьшее целое ≥ x (округление вверх).", "ceil(3.2)=4", "ceil(3.2)=4"}},
        {"abs", {"Absolute value.", "Модуль числа.", "abs(-5)=5", "abs(-5)=5"}},
        {"sign", {"Signum: -1, 0, 1.", "Знак числа: -1, 0, 1.", "sign(-5)=-1", "sign(-5)=-1"}},

        // ========== КОМБИНАТОРИКА И ТЕОРИЯ ЧИСЕЛ ==========
        {"fact", {"Factorial n!.", "Факториал n!.", "fact(5)=120", "fact(5)=120"}},
        {"ncr", {"Combinations C(n,k).", "Сочетания C(n,k).", "nCr(5,2)=10", "nCr(5,2)=10"}},
        {"npr", {"Permutations P(n,k).", "Размещения P(n,k).", "nPr(5,2)=20", "nPr(5,2)=20"}},
        {"gcd", {"Greatest Common Divisor.", "НОД – наибольший общий делитель.", "gcd(48,18)=6", "gcd(48,18)=6"}},
        {"lcm", {"Least Common Multiple.", "НОК – наименьшее общее кратное.", "lcm(4,6)=12", "lcm(4,6)=12"}},
        {"isprime", {"Returns 1 if n is prime.", "Возвращает 1, если n простое.", "isPrime(17)=1", "isPrime(17)=1"}},
        {"fib", {"Fibonacci numbers.", "Числа Фибоначчи.", "fib(10)=55", "fib(10)=55"}},
        {"subfact", {"Subfactorial (derangements).", "Субфакториал (беспорядки).", "subfact(5)=44", "subfact(5)=44"}},
        {"catalan", {"Catalan numbers.", "Числа Каталана.", "catalan(5)=42", "catalan(5)=42"}},
        {"bell", {"Bell numbers.", "Числа Белла.", "bell(5)=52", "bell(5)=52"}},
        {"stirling1", {"Stirling numbers of the first kind.", "Числа Стирлинга первого рода.", "stirling1(5,2)=50", "stirling1(5,2)=50"}},
        {"stirling2", {"Stirling numbers of the second kind.", "Числа Стирлинга второго рода.", "stirling2(5,2)=15", "stirling2(5,2)=15"}},

        // ========== СПЕЦИАЛЬНЫЕ ФУНКЦИИ ==========
        {"gamma", {"Gamma function.", "Гамма-функция.", "gamma(5)=24", "gamma(5)=24"}},
        {"beta", {"Beta function.", "Бета-функция.", "beta(2,3)=0.03333", "beta(2,3)=0.03333"}},
        {"erf", {"Error function.", "Функция ошибок.", "erf(1)=0.8427", "erf(1)=0.8427"}},
        {"zeta", {"Riemann zeta function (s>1).", "Дзета-функция Римана (s>1).", "zeta(2)≈1.64493", "zeta(2)≈1.64493"}},

        // ========== ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ ТЕОРИИ ЧИСЕЛ ==========
        {"nextprime", {"Smallest prime >= n.", "Наименьшее простое >= n.", "nextPrime(10)=11", "nextPrime(10)=11"}},
        {"prevprime", {"Largest prime <= n.", "Наибольшее простое <= n.", "prevPrime(10)=7", "prevPrime(10)=7"}},
        {"eulerphi", {"Euler's totient function φ(n).", "Функция Эйлера φ(n).", "eulerPhi(10)=4", "eulerPhi(10)=4"}},
        {"moebius", {"Möbius function μ(n).", "Функция Мёбиуса μ(n).", "moebius(6)=1", "moebius(6)=1"}},
        {"sigma", {"Sum of all positive divisors of n.", "Сумма всех положительных делителей n.", "sigma(6)=12", "sigma(6)=12"}},
        {"isperfect", {"Returns 1 if n is perfect.", "Возвращает 1, если n совершенное.", "isPerfect(6)=1", "isPerfect(6)=1"}},
        {"primitiveroot", {"Primitive root modulo prime p.", "Первообразный корень по модулю простого p.", "primitiveRoot(7)=3", "primitiveRoot(7)=3"}},
        {"modpow", {"(a^b) mod m efficiently.", "Эффективное вычисление (a^b) mod m.", "modpow(2,10,1000)=24", "modpow(2,10,1000)=24"}},
        {"isarmstrong", {"Armstrong (narcissistic) number.", "Число Армстронга (нарциссическое).", "isArmstrong(153)=1", "isArmstrong(153)=1"}},
        {"issuperprime", {"Super-prime: prime with prime index.", "Суперпростое число: простое с простым номером.", "isSuperPrime(11)=1", "isSuperPrime(11)=1"}},

        // ========== ГИПЕРОПЕРАТОРЫ (ФУНКЦИИ) ==========
        {"tet", {"Tetration tet(a,b) = a ^^ b.", "Тетрация tet(a,b) = a ^^ b.", "tet(2,3)=16", "tet(2,3)=16"}},
        {"pent", {"Pentation pent(a,b) = a ^^^ b.", "Пентация pent(a,b) = a ^^^ b.", "pent(2,2)=4", "pent(2,2)=4"}},

        // ========== МНОГОАРГУМЕНТНЫЕ ==========
        {"clamp", {"clamp(x, min, max) constrains x.", "clamp(x, min, max) ограничивает x.", "clamp(5,0,10)=5", "clamp(5,0,10)=5"}},
        {"map", {"map(val, in_min, in_max, out_min, out_max) linearly maps value.",
                 "map(val, in_min, in_max, out_min, out_max) линейно отображает значение.",
                 "map(5,0,10,0,100)=50", "map(5,0,10,0,100)=50"}},
        {"hypot", {"hypot(a, b) – Euclidean distance: sqrt(a²+b²). Numerically stable for large values.",
                   "hypot(a, b) – Евклидово расстояние: sqrt(a²+b²). Численно устойчив при больших значениях.",
                   "hypot(3,4)=5\nhypot(3!,4!!)=10",
                   "hypot(3,4)=5\nhypot(3!,4!!)=10"}},
        {"atan2", {"atan2(y, x) – angle (radians) of point (x,y) from positive x-axis. Range: (-π, π].",
                   "atan2(y, x) – угол (радианы) точки (x,y) относительно положительной оси x. Диапазон: (-π, π].",
                   "atan2(1,1)=0.7854\natan2(0,-1)=3.14159\natan2(1,0)=1.5708",
                   "atan2(1,1)=0.7854\natan2(0,-1)=3.14159\natan2(1,0)=1.5708"}},

        // ========== КОНВЕРТЕРЫ ==========
        {"deg2rad", {"Convert degrees to radians.", "Переводит градусы в радианы.", "deg2rad(180)=3.14159", "deg2rad(180)=3.14159"}},
        {"rad2deg", {"Convert radians to degrees.", "Переводит радианы в градусы.", "rad2deg(pi)=180", "rad2deg(pi)=180"}},

        // ========== СПЕЦИАЛЬНЫЕ КОМАНДЫ ==========
        {"egcd", {"egcd a b: computes gcd(a,b) and Bezout coefficients x,y such that a*x + b*y = gcd.\nAlso available as expression: egcd(a;b) returns just gcd(a,b).",
                  "egcd a b: вычисляет НОД(a,b) и коэффициенты Безу x,y такие что a*x + b*y = НОД.\nТакже доступно как выражение: egcd(a;b) возвращает только НОД(a,b).",
                  "egcd 48 18 -> gcd=6, x=-1, y=3\negcd(48;18) = 6",
                  "egcd 48 18 -> НОД=6, x=-1, y=3\negcd(48;18) = 6"}},
        {"factor", {"factor n: prime factorization of integer n.", "factor n: разложение целого числа n на простые множители.", "factor 120 -> 2^3 * 3 * 5", "фактор/разложить 120 -> 2^3 * 3 * 5"}},
        {"crt", {"crt: solves system x ≡ a1 (mod m1), x ≡ a2 (mod m2), ...", "crt: решает систему сравнений x ≡ a1 (mod m1), x ≡ a2 (mod m2), ...", "crt 2 3 3 5 -> x=8", "кто/китайская 2 3 3 5 -> x=8"}},
        {"++", {"Increment operator. ++x or x++ adds 1 to the number.", "Оператор инкремента. ++x или x++ увеличивает число на 1.", "++5 = 6\n5++ = 6\n2+5++ = 8", "++5 = 6\n5++ = 6\n2+5++ = 8"}},
        {"--", {"Decrement operator. --x or x-- subtracts 1 from the number.", "Оператор декремента. --x или x-- уменьшает число на 1.", "--5 = 4\n5-- = 4\n10-5-- = 6", "--5 = 4\n5-- = 4\n10-5-- = 6"}},
        {"precision", {"Sets the number of decimal places for output (0-15). 'precision' without argument shows current setting.",
                       "Устанавливает количество знаков после запятой (0-15). Просто 'precision' показывает текущую точность.",
                       "precision 5   -> sets 5 decimal places\nprecision 0   -> integers only\nprecision     -> shows current precision",
                       "precision 5   -> устанавливает 5 знаков\nprecision 0   -> только целые\nprecision     -> показывает текущую точность"}},
        {"rpn", {
        "RPN stands for Reverse Polish Notation. It is a postfix notation where operators follow their operands. "
        "For example, the expression '2 + 3' is written as '2 3 +' in RPN. "
        "SuperCalc uses RPN internally to evaluate expressions after parsing. "
        "You can toggle the display of RPN notation for debugging purposes using the commands:\n"
        "  rpn on   - enable RPN output\n"
        "  rpn off  - disable RPN output\n"
        "  rpn      - toggle current state.",
        "RPN (Обратная польская нотация) — постфиксная запись, в которой операторы идут после операндов. "
        "Например, выражение '2 + 3' записывается как '2 3 +'. "
        "SuperCalc использует RPN внутри после парсинга для вычислений. "
        "Вы можете включить/выключить отображение RPN для отладки с помощью команд:\n"
        "  rpn on   - включить вывод RPN\n"
        "  rpn off  - выключить вывод RPN\n"
        "  rpn      - переключить состояние.",
        "rpn on   -> enables RPN debug output\nrpn off  -> disables RPN output\nrpn      -> toggles",
        "rpn on   -> включает отладочный вывод RPN\nrpn off  -> выключает вывод RPN\nrpn      -> переключает"
        }},
        {"hardmode", {
        "hardmode on/off – switches between standard double mode and googology mode (BigNumber internal representation). "
        "In hardmode, numbers can exceed 1e308 and use tower representation or level-index, allowing computation with extremely large values.",
        "hardmode on/off – переключает между обычным режимом (double) и режимом гугологии (внутреннее представление BigNumber). "
        "В hardmode числа могут превышать 1e308 и использовать башенное или level‑index представление, открывая вычисления с экстремально большими значениями.",
        "hardmode on   -> switch to googology mode\nhardmode off  -> back to double mode",
        "hardmode on   -> переключиться в режим гугологии\nhardmode off  -> вернуться в double-режим"
        }},
    };
    return detailedHelp;
}