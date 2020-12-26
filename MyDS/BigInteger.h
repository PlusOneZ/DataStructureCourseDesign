//
// Created by Zhengyi on 2020/11/17.
//

#ifndef DATASTRUCTURE_BIGINTEGER_H
#define DATASTRUCTURE_BIGINTEGER_H

#include <string>
#include <exception>
#include <iostream>

namespace bigInteger {

    using std::string;

    class ParseError : public std::exception {
        const char *what() const _NOEXCEPT override {
            return "Cannot parse invalid integer.";
        }
    };

    class DivideByZero : public std::exception {
        const char *what() const _NOEXCEPT override {
            return "Cannot divide by zero";
        }
    };

    class BigInteger {
    public:

        BigInteger() : value("0"), negative(false) {}

        explicit BigInteger(int i);

        explicit BigInteger(const string &s);

        BigInteger(const BigInteger &bi);

        ~BigInteger() = default;

        // Unary operator
        BigInteger operator-() const;

        BigInteger operator+() const;

        BigInteger absolute() const;

        std::ostream &show(std::ostream &os) const;

        int compare(const BigInteger &bi) const;

        BigInteger &operator=(const BigInteger &bi);

        BigInteger operator+(const BigInteger &bi) const;

        BigInteger operator-(const BigInteger &bi) const;

        BigInteger operator*(const BigInteger &bi) const;

        BigInteger operator/(const BigInteger &bi) const;

        BigInteger operator%(const BigInteger &bi) const;

        BigInteger operator^(const BigInteger &bi) const;

        bool operator==(const BigInteger &bi) const;


    protected:
        string value;
        bool negative = false;

        int compareAbs(const BigInteger &bi) const;
    };

    BigInteger::BigInteger(int i) {
        if (i < 0) {
            value = std::to_string(-i);
            negative = true;
        } else {
            value = std::to_string(i);
            negative = false;
        }
    }

    BigInteger::BigInteger(const string &s) {
        negative = false;
        unsigned long begPos = 0;
        if (s.empty()) {
            value = "0";
            negative = false;
            return;
        }
        if (s[0] == '-') {
            negative = true;
            begPos = 1;
        }
        for (auto &c : s.substr(begPos)) {
            if (c > '9' || c < '0') {
                throw ParseError();
            }
        }
        value = s.substr(begPos);
    }

    BigInteger::BigInteger(const BigInteger &bi) {
        negative = bi.negative;
        value = bi.value;
    }

    BigInteger &BigInteger::operator=(const BigInteger &bi) {
        if (&bi == this)
            return *this;
        negative = bi.negative;
        value = bi.value;
        return *this;
    }

    int BigInteger::compare(const BigInteger &bi) const {
        if (negative && !bi.negative)
            return -1;
        if (bi.negative && !negative)
            return 1;

        int result = 0;
        if (value.length() < bi.value.length()) {
            result = -1;
        } else if (value.length() > bi.value.length()) {
            result = 1;
        } else {
            result = value.compare(bi.value);
        }
        if (negative)
            result = -result;
        return result;
    }

    BigInteger BigInteger::absolute() const {
        if (negative)
            return -*this;
        return *this;
    }

    int BigInteger::compareAbs(const BigInteger &bi) const {
        return this->absolute().compare(bi.absolute());
    }

    BigInteger BigInteger::operator+(const BigInteger &bi) const {
        // TODO 大数加法
        using std::reverse;
        BigInteger result;
        string lhs = value, rhs = bi.value;
        reverse(lhs.begin(), lhs.end());
        reverse(rhs.begin(), rhs.end());
        if (this->negative == bi.negative) {
            // Adding
            result.value = "";
            unsigned long length = std::min(lhs.length(), rhs.length());
            int i = 0;
            for (; i < length; i++) {
                // Roughly adding each digit, not caring carrying.
                result.value.push_back(lhs[i] - '0' + rhs[i]);
            }

            if (length < lhs.length()) {
                result.value += lhs.substr(i);
            } else if (length < rhs.length()) {
                result.value += rhs.substr(i);
            }

            int carry = 0;
            for (auto &c : result.value) {
                int temp = c + carry - '0';
                carry = temp / 10;
                temp %= 10;
                c = temp + '0';
            }
            if (carry != 0) {
                result.value.push_back('1');
            }
            result.negative = negative;
        } else {
            // Subtracting

            int flag = compareAbs(bi);
            if (flag == -1) {
                return bi + *this;
            } else if (!flag) {
                return BigInteger("0");
            } else {
                result.value = "";
                unsigned long length = std::min(lhs.length(), rhs.length());
                int i = 0;
                for (; i < length; ++i) {
                    // Roughly subtracting each digit, not caring carrying.
                    result.value.push_back(lhs[i] - (rhs[i] - '0'));
                }

                if (i < lhs.length()) {
                    result.value += lhs.substr(i);
                }
//            std::cout << result.value << std::endl;
                int carry = 0;
                for (auto &c : result.value) {
                    int temp = c - carry - '0';
                    carry = (temp < 0);
                    temp = carry * 10 + temp;
                    c = temp + '0';
                }
                result.negative = negative;
            }
        }
        reverse(result.value.begin(), result.value.end());
        auto valid = result.value.find_first_not_of('0');
        if (valid == string::npos) {
            return BigInteger("0");
        }
        result.value = result.value.substr(valid);
        return result;
    }

    std::ostream &BigInteger::show(std::ostream &os) const {
        if (negative)
            os << '-';
        os << value;
        return os;
    }

    BigInteger BigInteger::operator-(const BigInteger &bi) const {
        return (*this + (-bi));
    }

    BigInteger BigInteger::operator-() const {
        auto ret = BigInteger(*this);
        ret.negative = !negative;
        return ret;
    }

    BigInteger BigInteger::operator+() const {
        return *this;
    }

    BigInteger BigInteger::operator*(const BigInteger &bi) const {
        using std::reverse;
        if (this->value == "0" || bi.value == "0") {
            return BigInteger("0");
        }
        BigInteger result = BigInteger();
        if (this->negative != bi.negative) {
            result.negative = true;
        }
        string lhs = value, rhs = bi.value;
        reverse(lhs.begin(), lhs.end());
        reverse(rhs.begin(), rhs.end());
        result.value.resize(lhs.length() + rhs.length(), '0');

        // Loop for multiplying
        for (int i = 0; i < lhs.length(); i++) {
            for (int j = 0; j < rhs.length(); j++) {
                int temp = (result.value[i + j] - '0') +
                           (lhs[i] - '0') * (rhs[j] - '0');
                int carry = temp / 10;
                result.value[i + j] = temp % 10 + '0';
                // Adding carrying to every digit
                for (int higherDigits = i + j + 1;
                     carry != 0 && higherDigits < result.value.length();
                     higherDigits++) {
                    int renewal = result.value[higherDigits] - '0' + carry;
                    carry = renewal / 10;
                    result.value[higherDigits] = renewal % 10 + '0';
                }
            }
        }

        reverse(result.value.begin(), result.value.end());
        result.value = result.value.substr(result.value.find_first_not_of('0'));
        return result;
    }

// Throws DivideByZero exception
    BigInteger BigInteger::operator/(const BigInteger &bi) const {
        if (bi.value == "0") {
            throw DivideByZero();
        }

        // Integer division, return 0 if |*this| < |bi|
        int cmp = this->compareAbs(bi);
        if (cmp == -1) {
            return BigInteger("0");
        }

        BigInteger result("0");
        if (cmp == 0) {
            result = BigInteger("1");
        } else {
            BigInteger absLhs(this->absolute()), absRhs(bi.absolute());
            const BigInteger kTen("10");
            for (int loopCount = int(value.length() - bi.value.length());
                 loopCount >= 0;
                 --loopCount) {
                BigInteger temp(absRhs);
                for (int i = 0; i < loopCount; ++i) {
                    temp = temp * kTen;
                }

                int divider = 0;
                while (absLhs.compareAbs(temp) >= 0) {
                    absLhs = absLhs - temp;
                    ++divider;
                }
                result = result * kTen + BigInteger(divider);
            }
        }
        result.negative = negative ^ bi.negative;
        return result;
    }

// Throws DivideByZero exception
    BigInteger BigInteger::operator%(const BigInteger &bi) const {
        BigInteger divider = *this / bi;
        return (*this - divider * bi);
    }

    BigInteger BigInteger::operator^(const BigInteger &bi) const {
        BigInteger kZero("0");
        int powZero = bi.compare(kZero);
        int thisIsZero = compare(kZero);
        if (thisIsZero == 0) {
            if (powZero <= 0) {
                throw DivideByZero();
            } else {
                return BigInteger("0");
            }
        } else if (powZero == 0) {
            return BigInteger("1");
        } else if (powZero < 0) {
            std::cerr
                    << "Warning: Integer to negative power always evaluate to zero\n";
            return BigInteger("0");
        }

        BigInteger result("1");
        BigInteger lhs = *this, rhs = bi;
        while (rhs.compare(kZero) > 0) {
            bool isOdd = (unsigned) (rhs.value.back() - '0') & 1u;
            if (isOdd) {
                result = result * lhs;
            }
            lhs = lhs * lhs;
            rhs = rhs / BigInteger("2");
        }
        return result;
    }

    bool BigInteger::operator==(const BigInteger &bi) const {
        return this->compare(bi) == 0;
    }

    std::ostream &operator<<(std::ostream &os, const BigInteger &bi) {
        bi.show(os);
        return os;
    }
}

#endif //DATASTRUCTURE_BIGINTEGER_H
