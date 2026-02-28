# Separator

Sample: [separator.cpp](../samples/separator.cpp)

Solves the annoyance of writing comma-separated lists.

```c++
// Outputs 1; 2; 3
sep = cy::separator("; ");
for (auto i : {1, 2, 3})
{
    std::cout << sep << i;
}
```

# Reference

## Header file and namespace

```c++
#include <cutty/separator.hpp>

namespace cy = cutty;
```

## Class `separator`

Represents a printable separator, and stores whether it's been printed or not.

Constructors:

- `separator(const char *str = ", ")` create a new separator with the specified separator text.

To reset the separator, assign it a new value.

## Function `operator<<()`

```c++
std::ostream &operator<<(std::ostream &os, separator &sep);
```

Prints the separator to the output stream. Note that the parameter is non-const and the separator is suppressed on the first call. The state of the separator `sep` is updated to store whether it has been previously output or not.
