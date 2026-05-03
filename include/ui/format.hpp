#ifndef UI_FORMAT_HPP
#define UI_FORMAT_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

static inline std::string formatNumber(double value, int precision, const std::string& lang) {
    // 1. Обнуляем шумы, близкие к нулю
    if (std::fabs(value) < 1e-12) {
        value = 0.0;
    }
    // 2. Если значение очень близко к целому — делаем целым
    else if (std::fabs(value - std::round(value)) < 1e-12) {
        value = std::round(value);
    }

    std::ostringstream oss;
    if (precision >= 0) {
        oss << std::fixed << std::setprecision(precision);
    } else {
    oss << std::setprecision(14);   // вместо 16
    }
    oss << value;
    std::string str = oss.str();

    // 3. Убираем trailing zeros и, возможно, точку
    //    Но только если нет фиксированной точности (precision < 0)
    //    Иначе пользователь сам хочет видеть заданное число знаков.
    if (precision < 0) {
        auto dotPos = str.find('.');
        if (dotPos != std::string::npos) {
            // Удаляем нули с конца
            while (str.size() > 1 && str.back() == '0') {
                str.pop_back();
            }
            // Если последний символ — точка, убираем и её
            if (str.back() == '.') {
                str.pop_back();
            }
        }
    }

    // 4. Замена точки на запятую для русского языка
    if (lang == "ru") {
        size_t dotPos = str.find('.');
        if (dotPos != std::string::npos) {
            str[dotPos] = ',';
        }
        // после замены тоже может остаться висячая запятая в конце строки
        if (str.back() == ',') {
            str.pop_back();
        }
    }

    return str;
}

#endif