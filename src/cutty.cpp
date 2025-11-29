#include <cutty/approx.hpp>
#include <cutty/fraction.hpp>

#include <iostream>

namespace cy = cutty;

std::ostream &cy::operator<<(std::ostream &os, fraction p)
{
    os << p.numerator;
    if (p.denominator != 1)
        os << "/" << p.denominator;
    return os;
}

cy::approx::approx(const value_type &value, const value_type &tolerance) : value(value), tolerance(tolerance)
{
}

cy::approx::value_type cy::approx::max() const
{
    return value + tolerance;
}

cy::approx::value_type cy::approx::min() const
{
    return value - tolerance;
}

std::weak_ordering operator<=>(const cy::approx::value_type &lhs, const cy::approx &rhs)
{
    // Technically does not work with NaN
    return lhs < rhs.min()   ? std::weak_ordering::less
           : lhs > rhs.max() ? std::weak_ordering::greater
                             : std::weak_ordering::equivalent;
}

std::weak_ordering cy::operator<=>(const cy::approx &lhs, const cy::approx::value_type &rhs)
{
    auto r = rhs <=> lhs;
    return r == std::weak_ordering::less      ? std::weak_ordering::greater
           : r == std::weak_ordering::greater ? std::weak_ordering::less
                                              : std::weak_ordering::equivalent;
}

bool cy::operator==(const cy::approx::value_type &lhs, const cy::approx &rhs)
{
    return (lhs <=> rhs) == std::weak_ordering::equivalent;
}

std::ostream &cy::operator<<(std::ostream &os, const cy::approx &x)
{
    return os << x.value << "±" << x.tolerance;
}
