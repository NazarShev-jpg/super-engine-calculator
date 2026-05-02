#ifndef UI_FORMAT_HPP
#define UI_FORMAT_HPP

#include <string>
#include <sstream>
#include <iomanip>

static inline std::string formatNumber(double value, int precision, const std::string& lang) {
    std::ostringstream oss;
    if (precision >= 0) {
        oss << std::fixed << std::setprecision(precision);
    } else {
        oss << std::setprecision(16);
    }
    oss << value;
    std::string str = oss.str();
    if (lang == "ru") {
        size_t dotPos = str.find('.');
        if (dotPos != std::string::npos) {
            str[dotPos] = ',';
        }
    }
    return str;
}

#endif