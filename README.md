# super-engine-calculator
C++ calculator designed to go beyond standard limits. Current stable version handles precise classic math, while the experimental branch explores Googology—implementing Knuth's up-arrows and fast-growing hierarchies to reach numbers like Graham's. A student project focused on overcoming overflow issues and mastering complex logic. Work in progress.

# SuperCalc v2.8ref

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey)]()
[![License](https://img.shields.io/badge/License-MIT-green)]()

**SuperCalc** is a feature‑rich command‑line engineering calculator with support for high‑precision arithmetic, hyperoperations, combinatorics, number theory, and special functions. It evaluates mathematical expressions using the standard algebraic (infix) notation and displays results with a clean, colorful interface — available in both **English** and **Russian**.

---

## Table of Contents

- [Features](#features)
- [Installation & Quick Start](#installation--quick-start)
- [Usage](#usage)
  - [Basic Arithmetic](#basic-arithmetic)
  - [Constants & Implicit Multiplication](#constants--implicit-multiplication)
  - [Operators](#operators)
  - [Functions](#functions)
  - [Commands](#commands)
- [Project Structure](#project-structure)
- [Roadmap – Googology](#roadmap--googology)
- [Building from Source](#building-from-source)
- [Contributing](#contributing)
- [License](#license)

---

## Features

- **Full mathematical expression parser**  
  Infix notation with correct operator precedence, associativity, and implicit multiplication (`2(3+5)` = 16).

- **Comprehensive operator set**
  - Arithmetic: `+`, `-`, `*`, `/`, `^` (power), `%%` (modulo)
  - Postfix: `!` (factorial), `!!` (double factorial), `%` (percent)
  - Prefix/Postfix `++` / `--` (increment/decrement)
  - Bitwise: `~`, `&`, `|`, `<<`, `>>`
  - Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
  - Logical: `&&`, `||`, `!`
  - Ternary conditional: `? :`
  - Hyperoperators: `^^` (tetration), `^^^` (pentation)

- **Huge library of mathematical functions** (over 60 built‑ins)
  - Trigonometry (radians & degrees): `sin`, `cos`, `tan`, `cot`, `sec`, `csc` and their inverses
  - Hyperbolic functions and their inverses
  - Logarithms & exponentials: `exp`, `ln`, `log10`, `log(base, x)`
  - Roots: `sqrt`, `cbrt`, `root(x, n)`
  - Rounding & sign: `round`, `floor`, `ceil`, `abs`, `sign`
  - Combinatorics: `fact`, `subfact`, `nCr`, `nPr`, `catalan`, `bell`, `stirling1`, `stirling2`
  - Number theory: `gcd`, `lcm`, `isPrime`, `fib`, `nextPrime`, `prevPrime`, `eulerPhi`, `moebius`, `sigma`, `isPerfect`, `primitiveRoot`, `isArmstrong`, `isSuperPrime`, `modpow`
  - Special functions: `gamma`, `beta`, `erf`, `zeta`
  - Multi‑argument utilities: `clamp(x, min, max)`, `map(val, in_min, in_max, out_min, out_max)`, `crt`, `egcd`

- **Constants**  
  Built‑in `pi`, `e`, `phi` (golden ratio), and the `ans` variable that holds the last result.

- **Smart parsing**
  - Leading‑zero scaling: `0005` → `0.005`
  - Decimal separators: both `.` and `,` are accepted (localized output uses the system language).
  - Comments: everything after `//` is ignored.
  - Empty lines are silently skipped.

- **Commands & helpers**
  - `help` / `help <topic>` – detailed documentation of every function, operator, and command.
  - `factor <n>` – prime factorisation of an integer.
  - `egcd <a> <b>` – extended Euclidean algorithm (gcd + Bézout coefficients).
  - `crt <r1> <m1> <r2> <m2> ...` – Chinese Remainder Theorem solver.
  - `precision <n>` – set number of decimal places (0‑15).  
  - `rpn on/off` – toggle display of the internal Reverse Polish Notation (useful for debugging).
  - `lang en/ru` – switch interface language on the fly.

- **Cross‑platform**
  - Works on **Windows** (native console with UTF‑8) and **Linux/macOS**.
  - Color output is supported on all platforms.

- **Tested & stable**  
  The evaluation core uses `double` arithmetic and has been extensively tested (see the `examples` command). The codebase is modular, with a clear separation between parsing, evaluation, and UI.

---

## Installation & Quick Start

SuperCalc is distributed as a single executable. You can either [download a pre‑compiled binary](../../releases) (if available) or build it yourself.

**Prerequisites:**
- A C++23 capable compiler (GCC ≥ 13, Clang ≥ 17, or MSVC ≥ 19.38)
- Standard build tools (`make` or a terminal)

**Build (single command):**
```bash
g++ -std=c++23 -Iinclude src/*.cpp src/ui/*.cpp main.cpp -o SuperCalc

Then run:

bash
./SuperCalc       # Linux/macOS
SuperCalc.exe     # Windows
Usage
Basic Arithmetic
text
> 2 + 3 * 4
14
> (2 + 3) * 4
20
> 10 %% 3
1
Constants & Implicit Multiplication
text
> 2pi
6.283185307179586
> sin(pi/2)
1
> e^2
7.38905609893065
> 2ans  (after 3+4)
14
Operators
text
> 2^^3          # tetration
16
> 5! & 7        # bitwise AND after factorial
0
> 2>3 ? 10 : 20 # ternary
20
> !0            # logical NOT
1
Functions
text
> fib(10)
55
> zeta(2)
1.644934066848226
> clamp(-5, 0, 10)
0
> map(3, 0, 10, 0, 100)
30
> log(2, 8)
3
Commands
text
> help           # shows main help
> help sin       # detailed help for the sine function
> factor 120     # prime factorisation
120 = 2^3 * 3 * 5
> egcd 48 18     # extended Euclidean algorithm
gcd = 6, x = -1, y = 3
> crt 2 3 3 5    # Chinese Remainder Theorem
x = 8 (mod 15)
> precision 5    # set 5 decimal places
> lang ru        # switch to Russian


### PROJECT STRUCTURE
SuperCalc/
├── include/                  # Header files
│   ├── token.hpp             # Token definition (Number, Operator, Function)
│   ├── parser.hpp            # Infix → RPN parser
│   ├── evaluator.hpp         # RPN evaluator (double‑based)
│   ├── mathfunc.hpp          # All mathematical functions
│   ├── calculator.hpp        # High‑level calculator facade
│   ├── bignumber.hpp         # (stub) Future big‑number core
│   └── ui/                   # User interface headers
│       ├── console.hpp       # Color, language detection
│       ├── commands.hpp      # Session state & command processing
│       ├── format.hpp        # Number formatting helpers
│       ├── helpdata.hpp      # Detailed help entries
│       └── translations.hpp  # i18n (en/ru)
├── src/                      # Implementation files
│   ├── parser.cpp
│   ├── evaluator.cpp
│   ├── mathfunc.cpp
│   ├── calculator.cpp
│   ├── bignumber.cpp         # (stub)
│   └── ui/
│       ├── commands.cpp
│       ├── console.cpp
│       ├── helpdata.cpp
│       └── translations.cpp
├── main.cpp                  # Entry point, UI loop
└── README.md


Roadmap – Googology
One of the major goals of SuperCalc is googology – the ability to work with extremely large numbers that dwarf even double’s limits.

A dedicated BigNumber class has already been designed (bignumber.hpp/cpp) to support:

Plain (double) – standard 64‑bit floating point for everyday use.

Tower – scientific notation m × 10^e, where e itself can be a BigNumber, allowing numbers like 10^(10^100).

Level‑Index – a generalised exponential representation exp^level(index) for truly gigantic quantities.

All arithmetic, comparison, and mathematical functions (logs, roots, factorial via Stirling, hyperoperations) have been partially implemented for these representations. The next phase will:

Stabilise BigNumber precision (eliminate double‑noise in mantissa and exponent).

Integrate BigNumber into the parser/evaluator pipeline as an alternative evaluation mode.

Add full support for trigonometric and special functions on tower‑form arguments.

Stay tuned for v3.0 – the Googology Edition!

Contributing
Contributions, bug reports, and feature requests are very welcome!
Please open an issue or a pull request on the GitHub repository.

License
SuperCalc is distributed under the MIT License. See LICENSE for details.

“If you think you understand big numbers, you don’t understand big numbers.”
