# Dynamic

Samples: ...

Solves the problem of working with multiple types.

*Dynamic* adds the ability to store and work with data of different types. Philosophical arguments aside, sometimes it's easier to just store something in a dynamic variable. Unlike `std::any` or `std::variant`, you can work with dynamic objects, for example iterating elements or converting to a string. *Dynamic* could also be used for scenarios like JSON, interfacing with dynamic programming languages, or creating generic wrappers.

*Dynamic* behaves similarly to the C# `dynamic` keyword, and can convert any C++ type to dynamic.

# Tutorial

The header file for the library is `cutty/dynamic.hpp`, and as usual we'll use the namespace `cy` for `cutty`.

```c++
#include <cutty/dynamic.hpp>

namespace cy = cutty;
```

This header file includes the main "client-side" library for working with dynamic types. If you need to define new dynamic types, you also need to include `<cutty/dynamic/instantiate.hpp>`. If you need to create generic function wrappers, include `<cutty/dynamic/function.hpp>`.

To create a dynamic value, pass a C++ value to `dynamic` constructor, or assign it.

```c++
cy::dynamic a;          // An empty dynamic
cy::dynamic b = 12;     // An integer dynamic
cy::dynamic c = "abc";  // A string dynamic
```

## Output

Output is supported using `operator<<` or `cy::print()`. The `str()` method returns a string representation of the dynamic, and this is always defined.

```c++
    cy::print(a);   // Output: (empty)  
    std::cout << b << std::endl;
    std::cout << c.str() << std::endl;
```

## Operations

`dynamic` values support all of the operators of their underlying types. In addition, the other operand will be converted to `dynamic` if needed.

```c++
cy::dynamic(1) + cy::dynamic(2);  // 3
cy::dynamic(1) + 2;  // 3
1 + cy::dynamic(2);  // 3

cy::print(cy::dynamic("abc") + cy::dynamic("def"));   // abcdef
```

When operations are not supported by their underlying types (for example, `std::string`+`int`), then a runtime exception is thrown.

```c++
cy::dynamic("abc") + 12;   // Throws `dynamic::incompatible`
```

## Accessing the internal value

The stored type can be accessed using the `type()` method, and `type_str()` method returns a string representation of the stored type. The `as()` method gets a reference to the internal data, or throws `dynamic::incompatible` if the stored data does not match the requested type. `try_get<T>()` returns a pointer to `T` if successful, or `nullptr` if the stored type is not `T`.

```c++
dynamic x = 12;
int &i = x.as<int>();
x.type_info();
int *p = x.try_get<int>();
cy::print(x.type_str());  // "int"
```

## Null objects

The default `dynamic` object is "empty", and has the value `dynamic::empty`. There is no "null"

## Conversions




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

