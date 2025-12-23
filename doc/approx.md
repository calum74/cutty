# Approx

Sample: [approx.cpp](../samples/approx.cpp)

Solves the problem of comparing `double` values. An `approx` represents approximate values, particularly for `double` comparisons.

```c++
cy::check_equal(std::numbers::pi, cy::approx(3.14159265));
cy::check_equal(3.14159, cy::approx(3.1415, 1e-3));
```

# Reference

## Header file and namespace

```c++
#include <cutty/approx.hpp>
namespace cy = cutty;
```

## Class `approx`

Represents an approximate `double` value.

Constructors:
- `approx(double value, double tolerance = 1e-6)`

Operators
- `bool operator ==()` - Approximate equality operator
- `std::weak_ordering operator <=>()` - Approximate inequality operator
- Operators `!=`, `<`, `<=`, `>`, `>=` are generated automatically.
- `std::ostream &operator<<(std::ostream &os, const approx &x)` - Output to stream
