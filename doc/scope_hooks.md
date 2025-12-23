# Scope hooks

Sample: [scope_hooks.cpp](../samples/scope_hooks.cpp)

Solves the problem of logging function or scope entries and exits. Logs scope entry, exit and exceptions.

```c++
// Create hooks requiring arguments
constexpr auto event_logger =
    cy::scope_hooks([](const char *name, int id) { cy::print("Entering", name, id); },
                    [](const char *name, int id) { cy::print("Exiting", name, id); });

{
    // If the functions require parameters, then they are passed in when we construct the scope guard.
    // The parameters are passed to both the entry and exit functions.
    auto guard = event_logger("example", 2);
    cy::print("Running example 2");

    // Outputs:
    // Entering example 2
    // Running example 2
    // Exiting example 2
}
```

# Reference

## Header file and namespace

```c++
#include <cutty/scope_hooks.hpp>
namespace cy = cutty;
```

## Function `scope_hooks()`

```c++
template <typename EntryFn, typename ExitFn, typename ExceptFn>
constexpr auto scope_hooks(EntryFn &&entry, ExitFn &&exit, ExceptFn &&except);

template <typename EntryFn, typename ExitFn>
constexpr auto scope_hooks(EntryFn &&entry, ExitFn &&exit);

template <typename ExitFn>
constexpr auto scope_hooks(ExitFn &&exit);
```

Creates a scope hooks functor. The resulting functor must be called with (possibly empty) arguments to create a scope guard to capture scope entry and exit.

## Class `scope_hooks`

This is an internal class returned by `scope_hooks()`. This class is a functor and has operator `()` to generate scope guards.
