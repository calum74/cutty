# Pretty type

Sample: [pretty_type.cpp](../samples/pretty_type.cpp)

Solves the problem of getting a nice string representing a type in a portable way.

# Reference

## Header file and namespace

```c++
#include <cutty/pretty_type.hpp>
namespace cy = cutty;
```

## Function `pretty_type()`

```c++
template<typename T>
constexpr std::string_view pretty_type();
```

Returns a `string_view` of the type name `T`.
