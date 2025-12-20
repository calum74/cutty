#pragma once

#include <iostream>

namespace cutty
{
template<typename T>
struct fraction_t
{
    T numerator;
    T denominator;

    constexpr T hcf(int a, int b)
    {
        while (b)
        {
            T r = a % b;
            a = b;
            b = r;
        }
        return a < 0 ? -a : a;
    }

    constexpr fraction_t(T n, T d = 1)
    {
        auto h = hcf(n, d);
        if (d < 0)
            h = -h;
        numerator = n / h;
        denominator = d / h;
    }

    constexpr bool operator==(const fraction_t &) const = default;
};

template<typename T>
constexpr fraction_t<T> operator*(const fraction_t<T> &lhs, const fraction_t<T> &rhs)
{
    return fraction_t<T>{lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator};
}

template<typename T>
constexpr fraction_t<T> operator+(const fraction_t<T> &lhs, const fraction_t<T> &rhs)
{
    return {lhs.numerator*rhs.denominator +  rhs.numerator*lhs.denominator, rhs.denominator * rhs.denominator};
}


// !! Concept here
template <typename U, typename T> constexpr U operator*(const U &lhs, const fraction_t<T> &rhs)
{
    return (lhs * rhs.numerator) / rhs.denominator;
}

template<typename T>
constexpr fraction_t<T> operator/(const fraction_t<T> &lhs, const fraction_t<T> &rhs)
{
    return {lhs.numerator * rhs.denominator, lhs.denominator * rhs.numerator};
}

template<typename T>
constexpr fraction_t<T> pow(const fraction_t<T> &lhs, const fraction_t<T> &rhs)
{
    // static_assert(rhs.denominator == 1, "sorry, roots are not supported");
    if (rhs.numerator == 0)
    {
        return 1;
    }
    else if (rhs.numerator < 0)
    {
        return pow(fraction_t<T>{lhs.denominator, lhs.numerator}, fraction_t<T>{-rhs.numerator, rhs.denominator});
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


template<typename T>
std::ostream &operator<<(std::ostream &os, fraction_t<T> p)
{
    os << p.numerator;
    if (p.denominator != 1)
        os << "/" << p.denominator;
    return os;
}

using fraction = fraction_t<long long>;
} // namespace cutty