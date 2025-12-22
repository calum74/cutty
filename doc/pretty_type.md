# Pretty type

Sample: [pretty_type.cpp](../samples/pretty_type.cpp)

A portable way to obtain a nice string representing a C++ type.

# Reference

## Header and namespace

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