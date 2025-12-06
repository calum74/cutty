#pragma once

#include <iosfwd>

namespace cutty
{
struct fraction
{
    int numerator;
    int denominator;

    constexpr int hcf(int a, int b)
    {
        while (b)
        {
            int r = a % b;
            a = b;
            b = r;
        }
        return a < 0 ? -a : a;
    }

    constexpr fraction(int n, int d = 1)
    {
        auto h = hcf(n, d);
        if (d < 0)
            h = -h;
        numerator = n / h;
        denominator = d / h;
    }

    constexpr bool operator==(const fraction &) const = default;
};

constexpr fraction operator*(const fraction &lhs, const fraction &rhs)
{
    return fraction{lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator};
}

// !! Concept here
template <typename T> constexpr T operator*(const T &lhs, const fraction &rhs)
{
    return lhs * rhs.numerator / rhs.denominator;
}

constexpr fraction operator/(const fraction &lhs, const fraction &rhs)
{
    return fraction{lhs.numerator * rhs.denominator, lhs.denominator * rhs.numerator};
}

std::ostream &operator<<(std::ostream &os, fraction p);
} // namespace cutty