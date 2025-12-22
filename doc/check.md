# Check

Sample: [check.cpp](../samples/check.cpp)

Macro-free runtime checks.

```c++
cy::check(a < b);
cy::check_equal(a, b);
cy::check_throw(f, " Null pointer");
```

# Reference

## Header file and namespace

```c++
#include <cutty/check.hpp>
namespace cy = cutty;
```

## Function `check()`

```c++
void check(bool_like cond, const char *msg = "Failed check",
           const std::source_location &src = std::source_location::current());
```

Checks if the condition evaluates to `true`, and throws `check_failed` if not.

## Function `check_equal()`

```c++
void check_equal(auto &&lhs, auto &&rhs, const std::source_location &src = std::source_location::current())
```

Checks that two expressions are equal, and throws `check_failed` if they are not.

## Function `check_throws()`

```c++

void check_throws(auto &&fn, const char *expected_text,
                  const std::source_location &src = std::source_location::current());
```

Checks that the given function throws an exception that matches exactly the same `what()` as the expected text. This is a bit different to other libraries which use a macro to catch a specific type.

## Class `check_failed`

Exception thrown when a check has failed.