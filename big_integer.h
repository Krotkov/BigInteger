#pragma once

#include <iostream>
#include "my_vector.h"
//#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

struct big_integer
{
    int sign;
    my_vector digits;
    big_integer();
    big_integer(big_integer const& other);

    big_integer(int a);

    big_integer(std::string const& str);

    void removeExtraZero();

    unsigned int div_long_short(const unsigned int& a);
    void mul_long_short(const unsigned int& a);
    void add_long_short(const unsigned int& a);

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();

    big_integer& operator--();

    std::string to_string() const;

    void divide(big_integer &res, const big_integer &a, const big_integer &b);
};

void makeSameSize(big_integer &a, big_integer &b);
void reverseNum(big_integer &a);
big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

int compare(big_integer const& a, big_integer const& b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

my_vector add(big_integer const& a, big_integer const& b);
my_vector sub(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);