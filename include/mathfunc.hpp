#ifndef MATHFUNC_HPP
#define MATHFUNC_HPP

#include <string>
#include <vector>

namespace mathfunc {
    extern const double PI;
    extern const double E;
    extern const double PHI;

    double logBase(double base, double x);
    double tetration(double a, int b);
    double pentation(double a, int b);
    double evalFunction1(const std::string& name, double arg);
    double evalFunction2(const std::string& name, double a, double b);
    double evalFunctionN(const std::string& name, const std::vector<double>& args);
}

#endif