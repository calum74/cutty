# Test

Sample: [test.cpp](../samples/test.cpp)

A very simple, macro-free unit test framework.

```c++
#include <cutty/test.hpp>
namespace cy = cutty;

void test_1_equals_1()
{
    cy::check_equal(1, 1);
}

int main()
{
    return cy::test({test_1_equals_1});
}
```

# Reference

## Header file and include

```c++
#include <cutty/test.hpp>
namespace cy = cutty;
```

This also includes `cutty/approx.hpp` and `cutty/check.hpp`

## struct `test_fixture`

Holder for a test fixture to pass to `test()`.

```c++
struct test_fixture;
```

Types:
- `using test_function = void(*)();`

Constructors:
- `test_fixture(test_function)`
- `test_fixture(const char *name, test_function`

## Function `test()`

```c++
int test(std::initialiser_list<test_fixture>);
```

Runs the given test fixtures and returns `0` on success, or non-0 otherwise.