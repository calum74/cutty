# Dynamic

Samples: ...

Solves the problem of working with multiple types.

*Dynamic* adds the ability to store and work with data of different types. Philosophical arguments aside, sometimes it's easier to just store something in a dynamic variable. Unlike `std::any` or `std::variant`, you can work with the dynamic data, for example iterating elements or converting to a string. *Dynamic* could also be used for scenarios like JSON, interfacing with dynamic programming languages, or creating generic wrappers.

*Dynamic* behaves similarly to the C# `dynamic` keyword, and can convert any C++ type to dynamic.

# Tutorial

Firstly, must include the header files, and as usual we'll use the namespace `cy` for `cutty`.

```c++
#include <cutty/dynamic.hpp>

namespace cy = cutty;
```

This header file includes the main "client-side" library for working with dynamic types. If you need to define new dynamic types, you also need to include `<cutty/dynamic/instantiate.hpp>`. If you need to create generic function wrappers, include `<cutty/dynamic/function.hpp>`.

** This is an awkward name **

To create a dynamic value, pass a C++ value to `dynamic` constructor, or assign it.

```c++
cy::dynamic a;          // An empty dynamic
cy::dynamic b = 12;     // An integer dynamic
cy::dynamic c = "abc";  // A string dynamic
```

## Operations

`dynamic` values support all of the operators of their underlying types.

When operations are not supported by their underlying types (for example, `std::string`+`int`), then a runtime exception is thrown.

```c++
cy::dynamic("abc") + 12;   // Throws `dynamic::incompatible`
```

## Output

Output is supported using `operator<<` or `cy::print()`. The `str()` method returns a string representation of the dynamic, and this is always defined.

## Accessing the internal value

The stored type can be accessed using the `type()` method, and `type_str()` method returns a string representation of the stored type. The `as()` method gets a reference to the internal data, or throws `dynamic::incompatible` if the stored data does not match the requested type.

## Lists and containers

## References and pointers

## Maps and objects

## Functions

## User types



# Reference

## Header file and namespace

## Builtin types

## class `dynamic`

Constructors:
- `dynamic()` - creates an empty dynamic object, of type `empty`.

Mixins:

Methods:

Conversions:


Operators:

## Class `dynamic::empty`

Used to represent the empty value.

