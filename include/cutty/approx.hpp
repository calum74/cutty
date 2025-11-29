#pragma once

#include <compare>
#include <concepts>
#include <iostream>

namespace cutty
{
class approx
{
  public:
    using value_type = double;
    explicit approx(const value_type &value, const value_type &tolerance = 1e-6) : value(value), tolerance(tolerance)
    {
    }

    value_type max() const
    {
        return value + tolerance;
    }

    value_type min() const
    {
        return value - tolerance;
    }

    friend std::weak_ordering operator<=>(const value_type &lhs, const approx &rhs)
    {
        // Technically does not work with NaN
        return lhs < rhs.min()   ? std::weak_ordering::less
               : lhs > rhs.max() ? std::weak_ordering::greater
                                 : std::weak_ordering::equivalent;
    }

    friend std::weak_ordering operator<=>(const approx &lhs, const value_type &rhs)
    {
        auto r = rhs <=> lhs;
        return r == std::weak_ordering::less      ? std::weak_ordering::greater
               : r == std::weak_ordering::greater ? std::weak_ordering::less
                                                  : std::weak_ordering::equivalent;
    }

    friend bool operator==(const value_type &lhs, const approx &rhs)
    {
        return (lhs <=> rhs) == std::weak_ordering::equivalent;
    }

    friend std::ostream &operator<<(std::ostream &os, const approx &x)
    {
        return os << x.value << "±" << x.tolerance;
    }

  private:
    value_type value, tolerance;
};

} // namespace cutty