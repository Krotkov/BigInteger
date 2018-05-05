#include <string>
#include <vector>
#include "big_integer.h"

const unsigned long long MAX_UINT32 = 4294967296u;

big_integer::big_integer() : sign(1) {
    digits.push_back(0);
}

big_integer::big_integer(int a) {
    if (a < 0) {
        sign = -1;
    } else {
        sign = 1;
    }
    digits.push_back((unsigned int) std::abs((long long) a));
}

big_integer::big_integer(const big_integer &other) = default;

big_integer::big_integer(std::string const &str) {
    size_t uk = 0;
    if (str[0] == '-') {
        sign = -1;
        uk = 1;
    } else {
        sign = 1;
    }
    for (size_t i = uk; i < str.size(); i++) {
        mul_long_short(10);
        add_long_short((unsigned int) (str[i] - '0'));
    }
    removeExtraZero();
}

std::vector<unsigned int> add(const big_integer &first, const big_integer &second) {
    unsigned int carry = 0;
    std::vector<unsigned int> answer;
    int siz = std::max((int) first.digits.size(), (int) second.digits.size());
    for (int i = 0; i < siz; i++) {
        long long sum = carry;
        if (i < int(first.digits.size())) sum += (long long) first.digits[i];
        if (i < int(second.digits.size())) sum += (long long) second.digits[i];
        answer.push_back((unsigned int) sum);
        carry = (unsigned int) (sum >> 32);
    }
    if (carry != 0) answer.push_back(carry);
    return answer;
}

std::vector<unsigned int> sub(const big_integer &first, const big_integer &second) {
    unsigned int carry = 0;
    std::vector<unsigned int> answer;
    for (size_t i = 0; i < first.digits.size(); i++) {
        long long subtract = ((long long) first.digits[i] - (long long) carry);
        if (i < second.digits.size()) subtract -= (long long) second.digits[i];
        if (subtract < 0) {
            subtract += (1LL << 32);
            carry = 1;
        } else {
            carry = 0;
        }
        answer.push_back((unsigned int) (subtract));
    }
    return answer;
}

void big_integer::removeExtraZero() {
    while ((int) digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.size() == 1 && digits[0] == 0) sign = 1;
}

void makeSameSize(big_integer &a, big_integer &b) {
    while (a.digits.size() < b.digits.size()) {
        a.digits.push_back(0);
    }
    while (a.digits.size() > b.digits.size()) {
        b.digits.push_back(0);
    }
}

void reverseNum(big_integer &a) {
    for (unsigned int &uk: a.digits) {
        uk = ~uk;
    }
}

big_integer &big_integer::operator=(big_integer const &other)= default;

big_integer &big_integer::operator+=(big_integer const &other) {
    if (sign == other.sign) {
        digits = add(*this, other);
    } else {
        sign = -sign;
        if ((sign == 1 && *this >= other) || (sign == -1 && *this <= other)) {
            digits = sub(*this, other);
            sign = -sign;
        } else {
            digits = sub(other, *this);
        }
    }
    removeExtraZero();
    return *this;
}

big_integer &big_integer::operator-=(big_integer const &other) {
    if (sign != other.sign) {
        digits = add(*this, other);
    } else {
        big_integer copy(other);
        copy.sign = -copy.sign;
        *this += copy;
    }
    removeExtraZero();
    return *this;
}

big_integer &big_integer::operator*=(big_integer const &other) {
    sign = sign * other.sign;
    std::vector<unsigned int> newDigits;
    newDigits.resize(this->digits.size() * other.digits.size(), 0);
    unsigned int pointer = 0;
    for (unsigned int a: digits) {
        unsigned int carry = 0;
        unsigned long long sum = 0;
        unsigned int new_pointer = pointer;
        for (unsigned int b: other.digits) {
            sum = (unsigned long long) a * (unsigned long long) b + (unsigned long long) newDigits[new_pointer] +
                  (unsigned long long) carry;
            newDigits[new_pointer] = (unsigned int) sum;
            carry = (unsigned int) (sum >> 32);
            new_pointer++;
        }
        if (newDigits.size() <= new_pointer) {
            newDigits.push_back(0);
        }
        newDigits[new_pointer] += carry;
        pointer++;
    }
    digits = newDigits;
    removeExtraZero();
    return *this;
}

big_integer &big_integer::operator&=(big_integer const &other) {
    big_integer copy(other);
    makeSameSize(*this, copy);
    digits.push_back(0);
    copy.digits.push_back(0);
    if (sign < 0) {
        reverseNum(*this);
        *this -= 1;
    }
    if (copy.sign < 0) {
        reverseNum(copy);
        copy -= 1;
    }
    for (size_t i = 0; i < digits.size(); ++i) {
        if (i < copy.digits.size()) {
            digits[i] &= copy.digits[i];
        } else {
            break;
        }
    }
    if ((digits.back() & (unsigned int) (1LL << 31)) != 0) {
        sign = -1;
        *this += 1;
        reverseNum(*this);
    } else {
        sign = 1;
    }
    removeExtraZero();
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &other) {
    big_integer copy(other);
    makeSameSize(*this, copy);
    this->digits.push_back(0);
    copy.digits.push_back(0);
    if (sign < 0) {
        reverseNum(*this);
        *this -= 1;
    }
    if (copy.sign < 0) {
        reverseNum(copy);
        copy -= 1;
    }
    for (size_t i = 0; i < digits.size(); ++i) {
        if (i < copy.digits.size()) {
            digits[i] |= copy.digits[i];
        } else {
            break;
        }
    }
    if ((digits.back() & (unsigned int) (1LL << 31)) != 0) {
        sign = -1;
        *this += 1;
        reverseNum(*this);
    } else {
        sign = 1;
    }
    removeExtraZero();
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &other) {
    big_integer copy(other);
    makeSameSize(*this, copy);
    this->digits.push_back(0);
    copy.digits.push_back(0);
    if (sign < 0) {
        reverseNum(*this);
        *this -= 1;
    }
    if (copy.sign < 0) {
        reverseNum(copy);
        copy -= 1;
    }
    for (size_t i = 0; i < digits.size(); ++i) {
        if (i < copy.digits.size()) {
            digits[i] ^= copy.digits[i];
        } else {
            break;
        }
    }
    if ((digits.back() & (unsigned int) (1LL << 31)) != 0) {
        sign = -1;
        *this += 1;
        reverseNum(*this);
    } else {
        sign = 1;
    }
    removeExtraZero();
    return *this;
}

big_integer &big_integer::operator<<=(int shift) {
    if (sign < 0) {
        *this = ~*this;
        *this <<= shift;
        *this += 1;
        sign = -1;
        return *this;
    }
    std::vector<unsigned int> newDigits;
    if (shift / 32 != 0) newDigits.resize(shift / 32ull, 0);
    shift %= 32;
    unsigned int carry = 0;
    for (unsigned int &digit : digits) {
        unsigned int newCarry = (digit >> (32 - shift));
        digit <<= shift;
        digit += carry;
        carry = newCarry;
        newDigits.push_back(digit);
    }
    if (carry != 0) newDigits.push_back(carry);
    digits = newDigits;
    return *this;
}

big_integer &big_integer::operator>>=(int shift) {
    if (sign < 0) {
        *this = ~*this;
        *this >>= shift;
        *this += 1;
        sign = -1;
        return *this;
    }
    std::vector<unsigned int> newDigits;
    for (int i = shift / 32; i < (int) digits.size(); i++) {
        newDigits.push_back(digits[i]);
    }
    shift %= 32;
    unsigned int carry = 0;
    for (size_t i = 0; i < newDigits.size(); i++) {
        carry = newDigits[i] & (unsigned int) ((1LL << (shift + 1)) - 1);
        if (i != 0) newDigits[i - 1] += (carry << (32 - shift));
        newDigits[i] >>= shift;
    }
    digits = newDigits;
    removeExtraZero();
    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    return 0 - *this;
}

big_integer big_integer::operator~() const {
    return -*this - 1;
}

big_integer &big_integer::operator++() {
    return *this += 1;
}

big_integer &big_integer::operator--() {
    return *this -= 1;
}

big_integer &big_integer::operator/=(big_integer const &other) {
    *this = *this / other;
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &other) {
    big_integer copy(*this);
    copy /= other;
    copy *= other;
    *this -= copy;
    return *this;
}

unsigned int big_integer::div_long_short(const unsigned int &a) {
    unsigned long long carry = 0;
    for (int i = (int) (digits.size()) - 1; i >= 0; i--) {
        unsigned long long result = (long long) digits[i] + (carry << 32);
        digits[i] = (unsigned int) (result / a);
        carry = (result % a);
    }
    removeExtraZero();
    return (unsigned int) carry;
}

void big_integer::mul_long_short(const unsigned int &a) {
    unsigned int carry = 0;
    for (unsigned int &digit : digits) {
        unsigned long long result =
                (unsigned long long) digit * (unsigned long long) (a) + (unsigned long long) carry;
        digit = (unsigned int) result;
        carry = (unsigned int) (result >> 32);
    }
    if (carry != 0) digits.push_back(carry);
}

void big_integer::add_long_short(const unsigned int &a) {
    unsigned int carry = a;
    for (unsigned int &digit : digits) {
        unsigned long long result = (unsigned long long) digit + (unsigned long long) (carry);
        digit = (unsigned int) result;
        carry = (unsigned int) (result >> 32);
        if (carry == 0) {
            break;
        }
    }
    if (carry != 0) {
        digits.push_back(carry);
    }
}

std::string big_integer::to_string() const {
    if (digits.empty() || (*this == big_integer(0))) {
        return "0";
    }
    big_integer copy(*this);
    std::string answer;
    while ((copy != big_integer(0))) {
        unsigned int number = copy.div_long_short(10);
        answer += std::to_string(number);
    }
    if (sign == -1) {
        answer += '-';
    }
    std::reverse(answer.begin(), answer.end());
    return answer;
}

big_integer operator+(big_integer first, const big_integer &second) {
    first += second;
    return first;
}

big_integer operator-(big_integer first, const big_integer &second) {
    return first -= second;
}

big_integer operator*(big_integer first, big_integer const &second) {
    return first *= second;
}

big_integer operator&(big_integer first, big_integer const &second) {
    return first &= second;
}

big_integer operator|(big_integer first, big_integer const &second) {
    return first |= second;
}

big_integer operator^(big_integer first, big_integer const &second) {
    return first ^= second;
}

big_integer operator<<(big_integer first, int second) {
    return first <<= second;
}

big_integer operator>>(big_integer first, int second) {
    return first >>= second;
}

int compare(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign > b.sign ? 1 : -1;
    }
    if (a.digits.size() != b.digits.size()) {
        if (a.sign == 1) return a.digits.size() > b.digits.size() ? 1 : -1;
        else return a.digits.size() > b.digits.size() ? -1 : 1;
    }
    for (int i = (int) a.digits.size() - 1; i >= 0; i--) {
        if (a.digits[i] != b.digits[i]) {
            if (a.sign == 1) return a.digits[i] > b.digits[i] ? 1 : -1;
            else return a.digits[i] > b.digits[i] ? -1 : 1;
        }
    }
    return 0;
}

bool operator==(big_integer const &first, big_integer const &second) {
    int ans = compare(first, second);
    return ans == 0;
}

bool operator!=(big_integer const &first, big_integer const &second) {
    int ans = compare(first, second);
    return ans != 0;
}

bool operator<(big_integer const &first, big_integer const &second) {
    int ans = compare(first, second);
    return ans == -1;
}

bool operator>(big_integer const &first, big_integer const &second) {
    int ans = compare(first, second);
    return ans == 1;
}

bool operator<=(big_integer const &first, big_integer const &second) {
    int ans = compare(first, second);
    return ans != 1;
}

bool operator>=(big_integer const &first, big_integer const &second) {
    int ans = compare(first, second);
    return ans != -1;
}

big_integer operator/(big_integer Dividend, big_integer const &other) {
    if (other.digits.size() == 1) {
        Dividend.div_long_short(other.digits[0]);
        Dividend.sign *= other.sign;
        return Dividend;
    }
    big_integer ans;
    int new_sign = Dividend.sign * other.sign;
    Dividend.sign = 1;
    big_integer copy(other);
    copy.sign = 1;
    ans.divide(ans, Dividend, copy);
    ans.sign = new_sign;
    return ans;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}

std::string to_string(big_integer const &a) {
    return a.to_string();
}

void big_integer::divide(big_integer &res, big_integer const &a, big_integer const &b) {
    int cursign = compare(a, b);
    res.digits.clear();

    if (cursign < 0) {
        res.digits.push_back(0);
        return;
    }
    if (cursign == 0) {
        res.digits.push_back(1);
        return;
    }
    if (b.digits.size() == 1) {
        res = a;
        res.div_long_short(b.digits[0]);
        return;
    }

    auto d = (unsigned int) (MAX_UINT32 / (b.digits[b.digits.size() - 1] + 1));
    big_integer u(a), v(b);
    u.mul_long_short(d);
    v.mul_long_short(d);
    u.removeExtraZero();
    v.removeExtraZero();
    size_t n = u.digits.size(), m = v.digits.size(), len = n - m + 1;
    res.digits.resize(len);
    big_integer dividend(0), divider(0);
    dividend.digits.resize(m + 1);
    divider.digits.resize(m + 1);

    for (size_t i = 0; i < m; i++) {
        dividend.digits[i] = u.digits[n + i - m];
    }

    dividend.digits[m] = n < u.digits.size() ? u.digits[n] : 0;
    for (size_t i = 0; i < len; i++) {
        dividend.digits[0] = u.digits[n - m - i];
        size_t cur_pos = len - 1 - i;

        unsigned int tmp = (unsigned int) std::min(
                ((unsigned long long) (m < dividend.digits.size() ? dividend.digits[m] : 0) * MAX_UINT32 +
                 (m - 1 < dividend.digits.size() ? dividend.digits[m - 1] : 0)) / v.digits.back(),
                MAX_UINT32 - 1);
        divider = v;
        divider.mul_long_short(tmp);
        while (compare(dividend, divider) == -1) {
            divider = v;
            divider.mul_long_short(--tmp);
        }
        dividend.digits = sub(dividend, divider);
        for (size_t j = m; j > 0; j--) {
            dividend.digits[j] = dividend.digits[j - 1];
        }
        res.digits[cur_pos] = tmp;
    }
    res.removeExtraZero();
}