# With

*With* allows you to pass arguments to nested functions without adding them to function signatures.
It gives the convenience of global variables without the drawbacks.

```c++
#include <cutty/with.hpp>
using cy = cutty;

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

With is much more constrained than global variables, because you can locally define them, and they are
thread-local. This means that they can be used safely in unit tests, used in multiple threads,
and they can be stubbed, which is something that regular singletons, globals or singleton functions cannot do.

# Reference

## T &get<T,Tag=T>()

Gets the current value, or throws `without` if the variable has not been set. Threadsafe.

## T *try_get<T,Tag=T>()

Gets the current value, or returns `nullptr` if the variable has not been set. Threadsafe, does not throw.

## class with<T,Tag=T>

Assigns the value of the variable for the lifetime of the object. Threadsafe, does not throw.

Constructors:
- `with(T &src)`
- `with(T&&) = delete`

