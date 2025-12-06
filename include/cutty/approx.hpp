#pragma once

#include <compare>
#include <iosfwd>

namespace cutty
{
class approx
{
  public:
    using value_type = double;
    explicit approx(const value_type &value, const value_type &tolerance = 1e-6);
    value_type max() const;
    value_type min() const;

    friend std::weak_ordering operator<=>(const value_type &lhs, const approx &rhs);
    friend std::weak_ordering operator<=>(const approx &lhs, const value_type &rhs);
    friend bool operator==(const value_type &lhs, const approx &rhs);
    friend std::ostream &operator<<(std::ostream &os, const approx &x);

  private:
    value_type value, tolerance;
};

bool operator==(const double &lhs, const approx &rhs);
std::ostream &operator<<(std::ostream &os, const approx &x);
std::weak_ordering operator<=>(const double &lhs, const approx &rhs);
std::weak_ordering operator<=>(const approx &lhs, const double &rhs);

} // namespace cutty