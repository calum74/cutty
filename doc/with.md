# With

Sample: [with.cpp](../samples/with.cpp)

Solves the problem of sharing state across function boundaries, without the drawbacks of global variables and singletons.

*With* allows you to pass arguments to nested functions without adding them to function signatures.
It gives the convenience of global variables without the drawbacks.

```c++
#include <cutty/with.hpp>
namespace cy = cutty;

int main()
{
  Logger logger;
  cy::with<Logger> set_logger{logger};
  f();
}

void f()
{
    cy::get<Logger>().info("f called");
}
```

## Why not just use global variables?

*With* is much more constrained than global variables, because you can locally define them, and they are
thread-local. This means that they can be used safely in unit tests, used in multiple threads,
and they can be stubbed, which is something that regular singletons, globals or singleton functions cannot do.

# Reference

## Header file and namespace

```c++
#include <cutty/with.hpp>

namespace cy = cutty;
```

## Function `get()`

```c++
template<typename T, typename Tag=T>
T &get();
```

Gets the value assigned to `with<T,Tag>` in an outer scope on the same thread. Throws `without` if the variable has not been set. Threadsafe.

## Function `try_get()`

```c++
template<typename T, typename Tag=T>
T *try_get();
```

Gets the value assigned to `with<T,Tag>`, or returns `nullptr` if the variable has not been set. Threadsafe, does not throw.

## Class `with`

```c++
template<typename T, typename Tag=T>
class with;
```

Assigns the value of the variable for the lifetime of the object. Threadsafe, does not throw.

Constructors:

- `with(T &object)`: Sets the current value for the scope of `with`. This is thread-local and will have no impact on other threads. `object` can be a derived class of `T`, and should have a lifetime of at least the scope of `with`.
- `with(T&&) = delete`
- `with(const with&) = delete`

This class has no other methods. To access the stored value, use `get()` or `try_get()`.

`with` objects can be nested, where the innermost `with` is the current value used. When `with` goes out of scope, then the previous value is restored.

## Class `without`

Exception that is thrown when `get()` is called outside of a `with`.