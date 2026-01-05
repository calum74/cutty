# Dynamic

Samples: [short_tutorial.cpp](../samples/short_tutorial.cpp), [tutorial.cpp](../samples/tutorial.cpp)

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
cy::dynamic x = 12;
int &i = x.as<int>();
x.type_info();
int *p = x.try_get<int>();
cy::print(x.type_str());  // "int"
```

## Null objects

The default `dynamic` object is "empty", and has the value `dynamic::empty`. There is no "null" in the C++ sense.

## Conversions

C++ values can be turned into `dynamic` by passing them to the `dynamic` constructor. Then a copy of the value is stored in the `dynamic`. Copying the `dynamic` creates a copy of the value.

Every `dynamic` has a string representation, accessed by the `dynamic::str()` methods. The methods `as_int()` and `as_double()`

There are also explicit conversion operators for `bool`, `int` and `double`.

## Lists and containers

Dynamic objects can wrap C++ containers, and the methods `list()`, `dict()`, `map()`, `object()` and `queue()` create these objects.

| Container | Internal type |
| --------- | ------------- |
| `dynamic::list()` | `std::vector<dynamic>` |
| `dynamic::dict()` | `std::unordered_map<dynamic, dynamic>` |
| `dynamic::map()` | `std::map<dynamic, dynamic>` |
| `dynamic::set()` | `std::set<dynamic, dynamic>` |
| `dynamic::object()` | `std::unordered_map<std::string, dynamic>` |
| `dynamic::queue()` | `std::deque<dynamic>` |

These objects can be initialised with values, for example

```c++
cy::dynamic list = { 1, 2.1, "joe" };
auto map = cy::dynamic::dict({{"joe", 12}, {"sam", {1,2,3}}});
```

Lists behave like vectors, for example

```c++
list.size();
list.push_back(67);
list[0] = 3;
list.front() = "";
```

As usual, if an object does not support an operation at compile time, it will result in an exception thrown at runtime.

```c++
list.push_front(0);  // Exception thrown
```

Containers can be iterated, and pairs bound:

```c++
for(auto [k,v] : map)
{
    cy::print(k, "is", v);
}
```

`dynamic` types are fully compatible with C++ ranges.

## References

`dynamic` values can in fact be references to C++ values. It is the caller's responsibility to ensure that the underlying C++ object remains valid for the duration of the `dynamic` reference. To create a reference, use the `reference()` method, or the `const_reference` method.

```c++
int value = 42;
auto r = cy::dynamic::reference(value);
value = 67;
cy::print(r);  // 67
```

References can refer to other dynamic objects, which are created using the `ref()` method. For example the `[]` operator can return a reference, and assigning to a reference overwrites the refered-to object. 

```c++
cy::dynamic i = 42;
cy::dynamic j = i.ref();
j = 41;
cy::print(i);  // 41
```

## Shared objects

Copying a `dynamic` normally results in the underlying object being copied. If this is not desired, a `ref()` can be used, but this does not keep the underlying object alive. To solve this problem, `dynamic` implements shared pointers, which have similar semantics to `std::shared_ptr`. Client code does not need to know it is dealing with a shared object.

Creating a shared value is done using the `shared()` method. Existing dynamic objects can be copied to a shared object using the `shared_ref()` method.

```c++
cy::dynamic i = cy::dynamic::shared(42);
cy::dynamic j = i;
++i;
cy::print(j);  // 43;
```

## Associative containers

Dictionaries, maps and objects are different associative containers, created by `dict()`, `map()` and `object()` respectively. Dictionaries are unsorted, maps are sorted, and objects have string keys, but otherwise the behave very similarly.

The containers can be created using an initializer list, and accessed using `[]`. Other container methods (such as `size()`, `front()`, `back()`, `begin()`, `end()` etc. also work). Attempting to modify the contents of a const container will result in an `unsupported` exception.

```c++
auto o = cy::dynamic::object();
o["joe"] = 12;
auto o2 = o.const_ref();

cy::print(o2["joe"]);   // Ok
cy::print(o2["fred"]);  // throws std::out_of_range
```

## Functions

Functions are created using the `dynamic::function()` method, and requires the header file `cutty/dynamic/function.hpp` just to cut down on header files.

```c++
#include <cutty/dynamic/function.hpp>
```

Any C++ function can be wrapped, provided that it can be converted to a `std::function<>` with dynamic arguments. C++ arguments are automatically wrapped if needed.

```c++
auto fn = cy::dynamic::function([] { return cy::dynamic(42); });
cy::print(fn());  // 42

fn = cy::dynamic::function([&] (int x) { return x+1; });
cy::print(fn(10)); // 11
```

Calling a function with the wrong number of arguments results in an `unsupported` exception thrown.

## User types

In order to keep header files small, the standard header files do not support adding user-defined types. This can be done by including `cutty/dynamic/instantiate.hpp` in a .cpp file which defines the type to wrap, and specialising `cy::dynamic::instantiate<T>()` with the type.

```c++
#include "MyStruct.hpp"

#include <cutty/dynamic/instantiate.hpp>

template const cy::dynamic::types & cy::dynamic::instantiate<MyStruct>();
```

If this is done in one .cpp file, then any other source file can wrap `MyStruct` as needed.

```c++
auto x = cy::dynamic(MyStruct{});
```

The type `cy::dynamic::traits<>` can be specialised to configure the wrapping.

# Reference

## Header file and namespace

```c++
#include <cutty/dynamic.hpp>
#include <cutty/dynamic/function.hpp>
#include <cutty/dynamic/instantiate.hpp>
```

`class dynamic` is in the `cutty` namespace. Literals are defined in the `cutty::literals` namespace.

## Builtin types

## Class `dynamic`

Constructors:
- `dynamic()` - creates an empty dynamic object, of type `empty`.
- `dynamic(const T&)`
- `dynamic(T&&)`
- `dynamic(const dynamic&)`
- `dynamic(dynamic&&)`
- `dynamic(const char*)`, `dynamic(std::string_view)`
- `dynamic(std::initializer_list<dynamic>)`

Wrapped methods:
- `dynamic begin()`
- `dynamic begin() const`
- `dynamic cbegin() const`
- `dynamic cend() const`
- `dynamic end()`
- `dynamic end() const`
- `dynamic rbegin()`
- `dynamic rbegin() const`
- `dynamic crbegin() const`
- `dynamic rend()`
- `dynamic rend() const`
- `dynamic crend() const`
- `size_type size() const`

Methods:
- `int as_int()`
- `double as_double()`
- `dynamic const_ref()`
- `dynamic first()`
- `dynamic first() const`
- `size_type hash() const`
- `dynamic make_shared()`
- `dynamic ref()`
- `dynamic ref() const`
- `dynamic second()`
- `dynamic second() const`
- `dynamic shared_ref()`
- `dynamic shared_ref() const`
- `std::string str() const`
- `void swap(dynamic&)`
- `const std::string &type_str() const`
- `dynamic weak_ref()`

Static methods:
- `static dynamic const_reference(const T&)`
- `static dynamic dict()`
- `static dynamic dict(std::initializer_list<std::pair<dynamic, dynamic>>)`
- `static dynamic function(auto)`
- `static dynamic list()`
- `static dynamic list(std::initializer_list<dynamic>)`
- `static dynamic map()`
- `static dynamic map(std::initializer_list<std::pair<dynamic, dynamic>>)`
- `static dynamic queue()`
- `static dynamic queue(std::initializer_list<dynamic>)`
- `static dynamic reference(T&)`
- `static dynamic set()`
- `static dynamic set(std::initializer_list<dynamic>)`
- `static dynamic shared(const T&)`
- `static dynamic shared(T&&)`


Types:
- `const_iterator`
- `default_traits<T>`
- `difference_type`
- `empty`
- `incompatible`
- `int_type`
- `iterator`
- `size_type`
- `traits<T>`
- `unsupported`
- `value_type`

Conversions:
- `explicit operator bool`
- `explicit operator int`
- `explicit operator size_type`
- `explicit operator double`

Wrapped operators:
- `dynamic operator()`
- `dynamic operator*()`
- `dynamic & operator++()`
- `dynamic operator++(int)`
- `dynamic & operator--()`
- `dynamic operator--(int)`
- `dynamic operator[]`
- `dynamic &operator=(dynamic&&)`, 
- `dynamic &operator=(const dynamic&)`, 
- `dynamic &operator+=, -= *= /= %= &=|= ^=, <<= >>=`, 
- `bool operator==()`
- `std::weak_ordering operator<=>`
- + - * / %
- << >>

Literals:
- `dynamic operator""_d(const char *, std::size_t)`
- `dynamic operator""_d(unsigned long long)`
- `dynamic operator""_d(long double)`
- `dynamic operator""_d(char)`

## Class `dynamic::default:traits`

```c++
template<typename T> struct dynamic::default_traits;
```

Implements the default traits for a dynamic type.

## Class `dynamic::traits`

```c++
template<typename T> struct dynamic::traits;
```

Members

## Class `dynamic::empty`

Used to represent the empty value.

