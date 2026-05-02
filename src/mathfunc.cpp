#include "mathfunc.hpp"
#include <cmath>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <functional>
#include <numeric>
#include "ui/translations.hpp"

namespace mathfunc {

    const double PI = std::acos(-1.0);
    const double E  = std::exp(1.0);
    const double PHI = (1.0 + std::sqrt(5.0)) / 2.0;

    // ------------------ вспомогательные утилиты ------------------
    static int getIntArg(double x, const std::string& errKey) {
        int n = static_cast<int>(x);
        if (n != x) throw std::runtime_error(tr(errKey));
        return n;
    }

    static int getNonNegIntArg(double x, const std::string& errKey) {
        int n = getIntArg(x, errKey);
        if (n < 0) throw std::runtime_error(tr(errKey));
        return n;
    }

    static bool isPrime(int n) {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        for (int i = 3; i * i <= n; i += 2)
            if (n % i == 0) return false;
        return true;
    }

    // ------------------ числа Стирлинга (мемоизация) ------------------
    static double stirling1(int n, int k) {
        if (k == 0) return (n == 0) ? 1.0 : 0.0;
        if (k == n) return 1.0;
        static std::map<std::pair<int,int>, double> cache;
        auto key = std::make_pair(n, k);
        auto it = cache.find(key);
        if (it != cache.end()) return it->second;
        double res = stirling1(n - 1, k - 1) + (n - 1) * stirling1(n - 1, k);
        cache[key] = res;
        return res;
    }

    static double stirling2(int n, int k) {
        if (k == 0) return (n == 0) ? 1.0 : 0.0;
        if (k == n) return 1.0;
        static std::map<std::pair<int,int>, double> cache;
        auto key = std::make_pair(n, k);
        auto it = cache.find(key);
        if (it != cache.end()) return it->second;
        double res = k * stirling2(n - 1, k) + stirling2(n - 1, k - 1);
        cache[key] = res;
        return res;
    }

    // ------------------ реализации функций (evalFunction1) ------------------
    static double factFunc(double x) {
        int n = getNonNegIntArg(x, "error_factorial_neg");
        double res = 1.0;
        for (int i = 2; i <= n; ++i) res *= i;
        return res;
    }

    static double subfactFunc(double x) {
        int n = getNonNegIntArg(x, "error_subfact_neg");
        if (n == 0) return 1;
        if (n == 1) return 0;
        double fact = 1.0;
        for (int i = 2; i <= n; ++i) fact *= i;
        return std::round(fact / E);
    }

    static double catalanFunc(double x) {
        int n = getNonNegIntArg(x, "error_catalan_neg");
        if (n == 0) return 1;
        double res = 1.0;
        for (int i = 0; i < n; ++i)
            res = res * (4 * i + 2) / (i + 2);
        return res;
    }

    static double bellFunc(double x) {
        int n = getNonNegIntArg(x, "error_bell_neg");
        double sum = 0.0;
        for (int k = 0; k <= n; ++k)
            sum += stirling2(n, k);
        return sum;
    }

    static double fibFunc(double x) {
        int n = getNonNegIntArg(x, "error_fib_neg");
        if (n == 0) return 0;
        if (n == 1) return 1;
        double a = 0, b = 1, c;
        for (int i = 2; i <= n; ++i) {
            c = a + b;
            a = b;
            b = c;
        }
        return b;
    }

    static double isPrimeFunc(double x) {
        int n = static_cast<int>(x);
        if (n < 2 || x != n) return 0.0;
        return isPrime(n) ? 1.0 : 0.0;
    }

    static double signFunc(double x) {
        return (x > 0) ? 1.0 : ((x < 0) ? -1.0 : 0.0);
    }

    static double zetaFunc(double x) {
        if (x <= 1.0) throw std::runtime_error(tr("error_zeta_domain"));
        double sum = 0.0;
        for (int k = 1; k <= 10000; ++k)
            sum += std::pow(k, -x);
        return sum;
    }

    static double nextPrimeFunc(double x) {
        int n;
        if (x < 2 || std::floor(x) != x) n = 2;
        else n = static_cast<int>(x) + 1;
        while (!isPrime(n)) ++n;
        return n;
    }

    static double prevPrimeFunc(double x) {
        if (x <= 2 || std::floor(x) != x) return 2;
        int n = static_cast<int>(x) - 1;
        while (n >= 2 && !isPrime(n)) --n;
        return (n >= 2) ? n : 2;
    }

    static double eulerPhiFunc(double x) {
        int n = getIntArg(x, "error_euler_phi_positive");
        if (n <= 0) throw std::runtime_error(tr("error_euler_phi_positive"));
        int result = n;
        int temp = n;
        for (int p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                while (temp % p == 0) temp /= p;
                result -= result / p;
            }
        }
        if (temp > 1) result -= result / temp;
        return result;
    }

    static double moebiusFunc(double x) {
        int n = getIntArg(x, "error_moebius_positive");
        if (n <= 0) throw std::runtime_error(tr("error_moebius_positive"));
        if (n == 1) return 1;
        int primeCount = 0;
        int temp = n;
        for (int p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                int count = 0;
                while (temp % p == 0) { temp /= p; ++count; }
                if (count > 1) return 0;
                ++primeCount;
            }
        }
        if (temp > 1) ++primeCount;
        return (primeCount % 2 == 0) ? 1 : -1;
    }

    static double sigmaFunc(double x) {
        int n = getIntArg(x, "error_sigma_positive");
        if (n <= 0) throw std::runtime_error(tr("error_sigma_positive"));
        int sum = 0;
        for (int i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                sum += i;
                if (i != n / i) sum += n / i;
            }
        }
        return sum;
    }

    static double isPerfectFunc(double x) {
        int n = static_cast<int>(x);
        if (n <= 0 || x != n) return 0.0;
        int sum = 1;
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                sum += i;
                if (i != n / i) sum += n / i;
            }
        }
        return (sum == n && n != 1) ? 1.0 : 0.0;
    }

    static double primitiveRootFunc(double x) {
        int p = static_cast<int>(x);
        if (p <= 2 || x != p || !isPrime(p))
            throw std::runtime_error(tr("error_primitive_root_prime"));
        int phi = p - 1;
        std::vector<int> factors;
        int temp = phi;
        for (int i = 2; i * i <= temp; ++i) {
            if (temp % i == 0) {
                factors.push_back(i);
                while (temp % i == 0) temp /= i;
            }
        }
        if (temp > 1) factors.push_back(temp);
        for (int g = 2; g < p; ++g) {
            bool ok = true;
            for (int q : factors) {
                long long res = 1, base = g, exp = phi / q;
                while (exp) {
                    if (exp & 1) res = (res * base) % p;
                    base = (base * base) % p;
                    exp >>= 1;
                }
                if (res == 1) { ok = false; break; }
            }
            if (ok) return g;
        }
        throw std::runtime_error(tr("error_primitive_root_prime"));
    }

    static double isArmstrongFunc(double x) {
        int n = static_cast<int>(x);
        if (n < 0 || x != n) return 0.0;
        int original = n, sum = 0, digits = 0;
        for (int t = n; t; t /= 10) ++digits;
        for (int t = n; t; t /= 10) {
            int d = t % 10;
            sum += static_cast<int>(std::pow(d, digits));
        }
        return (sum == original) ? 1.0 : 0.0;
    }

    static double isSuperPrimeFunc(double x) {
        int n = static_cast<int>(x);
        if (n < 2 || x != n) return 0.0;
        if (!isPrime(n)) return 0.0;
        int idx = 0;
        for (int p = 2; p <= n; ++p)
            if (isPrime(p)) ++idx;
        return isPrime(idx) ? 1.0 : 0.0;
    }

    static double factorFunc(double x) {
        int n = static_cast<int>(x);
        if (n < 2 || x != n) {
            std::cout << "factor(" << x << ") = " << x << "\n";
            return x;
        }
        int temp = n;
        std::map<int, int> factors;
        for (int p = 2; p * p <= temp; ++p)
            while (temp % p == 0) { factors[p]++; temp /= p; }
        if (temp > 1) factors[temp]++;
        std::ostringstream oss;
        oss << n << " = ";
        bool first = true;
        for (auto &kv : factors) {
            if (!first) oss << " * ";
            oss << kv.first;
            if (kv.second > 1) oss << "^" << kv.second;
            first = false;
        }
        std::cout << oss.str() << "\n";
        return x;
    }

    // ------------------ публичные функции ------------------
    double logBase(double base, double x) {
        if (std::isinf(base) || std::isinf(x))
            throw std::runtime_error(tr("error_log_overflow"));
        if (base <= 0.0 || base == 1.0)
            throw std::runtime_error(tr("error_log_base"));
        if (x <= 0.0)
            throw std::runtime_error(tr("error_log_negative"));
        return std::log(x) / std::log(base);
    }

    double tetration(double a, int b) {
        if (b == 0) return 1.0;
        if (b > 0) {
            double result = a;
            for (int i = 2; i <= b; ++i)
                result = std::pow(a, result);
            return result;
        }
        double result = 1.0;
        for (int i = 0; i > b; --i)
            result = std::log(result) / std::log(a);
        return result;
    }

    double pentation(double a, int b) {
        if (b < 0) throw std::runtime_error(tr("error_pentation_neg_exp"));
        if (b == 0) return 1.0;
        double result = a;
        for (int i = 2; i <= b; ++i)
            result = tetration(a, static_cast<int>(result));
        return result;
    }

    // ------------------ таблицы функций ------------------
    double evalFunction1(const std::string &name, double arg) {
        using Func1 = double(*)(double);
        static const std::map<std::string, Func1> funcs = {
            {"sign",    signFunc},
            {"sqrt",    std::sqrt},
            {"cbrt",    std::cbrt},
            {"abs",     std::fabs},
            {"round",   std::round},
            {"floor",   std::floor},
            {"ceil",    std::ceil},
            {"exp",     std::exp},
            {"sin",     std::sin},
            {"cos",     std::cos},
            {"tan",     std::tan},
            {"cot",     [](double x) { return 1.0/std::tan(x); }},
            {"sec",     [](double x) { return 1.0/std::cos(x); }},
            {"csc",     [](double x) { return 1.0/std::sin(x); }},
            {"asin",    std::asin},
            {"acos",    std::acos},
            {"atan",    std::atan},
            {"acot",    [](double x) { return std::atan(1.0/x); }},
            {"asec",    [](double x) { return std::acos(1.0/x); }},
            {"acsc",    [](double x) { return std::asin(1.0/x); }},
            {"sinh",    std::sinh},
            {"cosh",    std::cosh},
            {"tanh",    std::tanh},
            {"coth",    [](double x) { return std::cosh(x)/std::sinh(x); }},
            {"sech",    [](double x) { return 1.0/std::cosh(x); }},
            {"csch",    [](double x) { return 1.0/std::sinh(x); }},
            {"asinh",   std::asinh},
            {"acosh",   std::acosh},
            {"atanh",   std::atanh},
            {"acoth",   [](double x) { return std::atanh(1.0/x); }},
            {"asech",   [](double x) { return std::acosh(1.0/x); }},
            {"acsch",   [](double x) { return std::asinh(1.0/x); }},
            {"sind",    [](double x) { return std::sin(x * PI / 180.0); }},
            {"cosd",    [](double x) { return std::cos(x * PI / 180.0); }},
            {"tand",    [](double x) { return std::tan(x * PI / 180.0); }},
            {"cotd",    [](double x) { return 1.0 / std::tan(x * PI / 180.0); }},
            {"secd",    [](double x) { return 1.0 / std::cos(x * PI / 180.0); }},
            {"cscd",    [](double x) { return 1.0 / std::sin(x * PI / 180.0); }},
            {"asind",   [](double x) { return std::asin(x) * 180.0 / PI; }},
            {"acosd",   [](double x) { return std::acos(x) * 180.0 / PI; }},
            {"atand",   [](double x) { return std::atan(x) * 180.0 / PI; }},
            {"acotd",   [](double x) { return std::atan(1.0/x) * 180.0 / PI; }},
            {"asecd",   [](double x) { return std::acos(1.0/x) * 180.0 / PI; }},
            {"acscd",   [](double x) { return std::asin(1.0/x) * 180.0 / PI; }},
            {"deg2rad", [](double x) { return x * PI / 180.0; }},
            {"rad2deg", [](double x) { return x * 180.0 / PI; }},
            {"ln",      std::log},
            {"log10",   std::log10},
            {"gamma",   std::tgamma},
            {"fact",    factFunc},
            {"isprime", isPrimeFunc},
            {"fib",     fibFunc},
            {"erf",     std::erf},
            {"zeta",    zetaFunc},
            {"subfact", subfactFunc},
            {"catalan", catalanFunc},
            {"bell",    bellFunc},
            {"nextprime", nextPrimeFunc},
            {"prevprime", prevPrimeFunc},
            {"eulerphi",  eulerPhiFunc},
            {"moebius",   moebiusFunc},
            {"sigma",     sigmaFunc},
            {"isperfect", isPerfectFunc},
            {"primitiveroot", primitiveRootFunc},
            {"isarmstrong",   isArmstrongFunc},
            {"issuperprime",  isSuperPrimeFunc},
            {"factor",        factorFunc}
        };
        auto it = funcs.find(name);
        if (it != funcs.end()) return it->second(arg);
        throw std::runtime_error(tr("error_unknown_function") + name);
    }

    double evalFunction2(const std::string &name, double a, double b) {
        using Func2 = double(*)(double,double);
        static const std::map<std::string, Func2> funcs = {
            {"log",       [](double x,double y){ return logBase(x,y); }},
            {"tet",       [](double x,double y){ return tetration(x,static_cast<int>(y)); }},
            {"pent",      [](double x,double y){ return pentation(x,static_cast<int>(y)); }},
            {"hypot",     std::hypot},
            {"atan2",     std::atan2},
            {"beta",      [](double a,double b){ return std::tgamma(a)*std::tgamma(b)/std::tgamma(a+b); }},
            {"gcd",       [](double a,double b){ int x=static_cast<int>(a),y=static_cast<int>(b); while(y){ int t=y; y=x%y; x=t; } return static_cast<double>(x); }},
            {"lcm",       [](double a,double b){ int x=static_cast<int>(a),y=static_cast<int>(b); int g=x,yy=y; while(yy){ int t=yy; yy=g%yy; g=t; } return static_cast<double>(x/g*y); }},
            {"ncr",       [](double n,double k){ int ni=static_cast<int>(n),ki=static_cast<int>(k); if(ki<0||ki>ni) return 0.0; double res=1.0; for(int i=1;i<=ki;++i) res=res*(ni-ki+i)/i; return res; }},
            {"npr",       [](double n,double k){ int ni=static_cast<int>(n),ki=static_cast<int>(k); if(ki<0||ki>ni) return 0.0; double res=1.0; for(int i=0;i<ki;++i) res*=(ni-i); return res; }},
            {"root",      [](double x,double n){ return std::pow(x,1.0/n); }},
            {"stirling1", [](double n,double k){ int ni=static_cast<int>(n),ki=static_cast<int>(k); if(ni<0||ki<0||ki>ni) return 0.0; return stirling1(ni,ki); }},
            {"stirling2", [](double n,double k){ int ni=static_cast<int>(n),ki=static_cast<int>(k); if(ni<0||ki<0||ki>ni) return 0.0; return stirling2(ni,ki); }},
            {"egcd",      [](double a,double b){ long long x=static_cast<long long>(std::round(a)),y=static_cast<long long>(std::round(b)); while(y){ long long t=y; y=x%y; x=t; } return static_cast<double>(x); }}
        };
        auto it = funcs.find(name);
        if (it != funcs.end()) return it->second(a,b);
        throw std::runtime_error(tr("error_unknown_function") + name);
    }

    double evalFunctionN(const std::string &name, const std::vector<double> &args) {
        using FuncN = std::function<double(const std::vector<double>&)>;
        static const std::map<std::string, FuncN> funcs = {
            {"clamp", [](const std::vector<double>& a){
                if (a.size()!=3) throw std::runtime_error(tr("error_not_enough_args")+"clamp");
                double x=a[0],minVal=a[1],maxVal=a[2];
                if (minVal>maxVal) throw std::runtime_error(tr("error_clamp_min_max"));
                return (x<minVal)?minVal:(x>maxVal)?maxVal:x;
            }},
            {"map",   [](const std::vector<double>& a){
                if (a.size()!=5) throw std::runtime_error(tr("error_not_enough_args")+"map");
                double val=a[0],in_min=a[1],in_max=a[2],out_min=a[3],out_max=a[4];
                if (in_min==in_max) throw std::runtime_error(tr("error_map_denom"));
                return out_min + (val-in_min)*(out_max-out_min)/(in_max-in_min);
            }},
            {"modpow",[](const std::vector<double>& a){
                if (a.size()!=3) throw std::runtime_error(tr("error_not_enough_args")+"modpow");
                long long base=static_cast<long long>(a[0]),exp=static_cast<long long>(a[1]),mod=static_cast<long long>(a[2]);
                if (mod<=0) throw std::runtime_error(tr("error_modpow_mod_positive"));
                long long res=1%mod; base%=mod;
                while (exp>0) { if (exp&1) res=(res*base)%mod; base=(base*base)%mod; exp>>=1; }
                return static_cast<double>(res);
            }},
            {"crt",   [](const std::vector<double>& a){
                if (a.size()<2 || a.size()%2!=0) throw std::runtime_error(tr("error_crt_even_args"));
                size_t pairs=a.size()/2;
                std::vector<long long> rem,mod;
                for (size_t i=0;i<pairs;++i){ rem.push_back(static_cast<long long>(std::round(a[2*i]))); mod.push_back(static_cast<long long>(std::round(a[2*i+1]))); }
                for (size_t i=0;i<mod.size();++i) for (size_t j=i+1;j<mod.size();++j) if (std::gcd(mod[i],mod[j])!=1) throw std::runtime_error(tr("error_crt_coprime"));
                long long prod=1; for (long long m:mod) prod*=m;
                long long result=0;
                for (size_t i=0;i<rem.size();++i){
                    long long pp=prod/mod[i],a=pp,b=mod[i],x1=1,y1=0,x2=0,y2=1;
                    while (b){ long long q=a/b,t=b; b=a%b; a=t; t=x2; x2=x1-q*x2; x1=t; t=y2; y2=y1-q*y2; y1=t; }
                    long long inv=(x1%mod[i]+mod[i])%mod[i];
                    result=(result+rem[i]*pp%prod*inv)%prod;
                }
                return static_cast<double>(result);
            }}
        };
        auto it = funcs.find(name);
        if (it != funcs.end()) return it->second(args);
        throw std::runtime_error(tr("error_unknown_function") + name);
    }

} // namespace mathfunc