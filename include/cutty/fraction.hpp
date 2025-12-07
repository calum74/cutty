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

constexpr fraction operator+(const fraction &lhs, const fraction &rhs)
{
    return {lhs.numerator*rhs.denominator +  rhs.numerator*lhs.denominator, rhs.denominator * rhs.denominator};
}


// !! Concept here
template <typename T> constexpr T operator*(const T &lhs, const fraction &rhs)
{
    return lhs * rhs.numerator / rhs.denominator;
}

constexpr fraction operator/(const fraction &lhs, const fraction &rhs)
{
    return {lhs.numerator * rhs.denominator, lhs.denominator * rhs.numerator};
}

constexpr fraction pow(const fraction &lhs, const fraction &rhs)
{
    // static_assert(rhs.denominator == 1, "sorry, roots are not supported");
    if (rhs.numerator == 0)
    {
        return 1;
    }
    else if (rhs.numerator < 0)
    {
        return pow({lhs.denominator, lhs.numerator}, {-rhs.numerator, rhs.denominator});
    }
    else if (rhs.numerator == 1)
    {
        // Redundant case
        return lhs;
    }
    else
    {
        // !! There are more efficient power algorithms available
        return lhs * pow(lhs, {rhs.numerator-rhs.denominator, rhs.denominator});
    }
}


std::ostream &operator<<(std::ostream &os, fraction p);
} // namespace cutty