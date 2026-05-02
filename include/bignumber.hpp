#ifndef SUPERCALC_BIGNUMBER_HPP
#define SUPERCALC_BIGNUMBER_HPP

#include <string>
#include <memory>
#include <cstdint>
#include <vector>

class BigNumber {
public:
    enum class StorageType { PLAIN, TOWER, LEVEL_INDEX };

    // Конструкторы
    BigNumber();                                 // ноль
    explicit BigNumber(double value);            // из double
    BigNumber(double mantissa, BigNumber exp);   // башня  mantissa*10^exp
    explicit BigNumber(std::int64_t level, double index); // level-index
    explicit BigNumber(const std::string& str);  // парсинг строки

    ~BigNumber();
    BigNumber(const BigNumber& other);
    BigNumber(BigNumber&& other) noexcept;
    BigNumber& operator=(const BigNumber& other);
    BigNumber& operator=(BigNumber&& other) noexcept;

    // Инспекция
    bool isZero() const;
    int sign() const;           // 0, +1, -1
    StorageType storageType() const;

    // Преобразование
    double toDouble() const;    // если влезает в double, иначе INFINITY
    std::string toString(int precision = 6) const;

    // Сравнения
    bool operator==(const BigNumber& other) const;
    bool operator!=(const BigNumber& other) const;
    bool operator<(const BigNumber& other) const;
    bool operator<=(const BigNumber& other) const;
    bool operator>(const BigNumber& other) const;
    bool operator>=(const BigNumber& other) const;

    // Арифметика
    BigNumber operator+(const BigNumber& other) const;
    BigNumber operator-(const BigNumber& other) const;
    BigNumber operator*(const BigNumber& other) const;
    BigNumber operator/(const BigNumber& other) const;
    BigNumber operator-() const; // унарный минус

    // Элементарные функции (позже добавим статические методы)

private:
    StorageType m_type;

    union {
        double plain;
        struct {
            double mantissa;
            std::unique_ptr<BigNumber> exponent; // для TOWER
        } tower;
        struct {
            std::int64_t level;
            double index; // [0, 1)
        } levelIndex;
    };

    void normalize();
    static BigNumber fromDouble(double x);
};

#endif