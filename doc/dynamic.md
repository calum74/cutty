# Dynamic

Samples: [short_tutorial.cpp](../samples/short_tutorial.cpp), [tutorial.cpp](../samples/tutorial.cpp)

Solves the problem of working with multiple types.

*Dynamic* adds the ability to store and work with data of different types. Philosophical arguments aside, sometimes it's easier to just store something in a dynamic variable. Unlike `std::any` or `std::variant`, you can work with dynamic objects, for example iterating elements or converting to a string. *Dynamic* could also be used for scenarios like JSON, interfacing with dynamic programming languages, or creating generic wrappers.

*Dynamic* behaves similarly to the C# `dynamic` keyword, and can wrap any C++ type as `dynamic`.

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

The default `dynamic` object is "empty", and has the value `dynamic::empty_type`. There is no "null" in the C++ sense.

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

Sample: TODO

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

Sample: TODO

Copying a `dynamic` normally results in the underlying object being copied. If this is not desired, a `ref()` can be used, but this does not keep the underlying object alive. To solve this problem, `dynamic` implements shared pointers, which have similar semantics to `std::shared_ptr`. Client code does not need to know it is dealing with a shared object.

Creating a shared value is done using the `shared()` method. Existing dynamic objects can be copied to a shared object using the `shared_ref()` method.

```c++
cy::dynamic i = cy::dynamic::shared(42);
cy::dynamic j = i;
++i;
cy::print(j);  // 43
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

Sample: TODO

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

Holds a C++ value or reference.

Constructors:
- `dynamic()` - creates an empty dynamic object, of type `empty_type`.
- `dynamic(const T&)` - copies and stores its argument, creating a dynamic of type `T`.
- `dynamic(T&&)` - moves and stores its argument, creating a dynamic of type `T`.
- `dynamic(const dynamic&)` - copies a dynamic value. For references and shared values, the wrapped value is not copied.
- `dynamic(dynamic&&)` - moves a dynamic
- `dynamic(const char*)`, `dynamic(std::string_view)` - creates a string
- `dynamic(std::initializer_list<dynamic>)` - creates a list

Types:
- `const_iterator` - a `dynamic` used for containers
- `default_traits<T>` - default implementation of dynamic traits. Inherit from this class to implement traits.
- `difference_type` - a `dynamic` for containers
- `empty_type` - the value of a default-initialised `dynamic`
- `incompatible` exception thrown when a wrong or incompatible type is requested
- `int_type` - how its are stored internally
- `iterator` - `dynamic` used for containers
- `size_type` - `std::size_t`
- `traits<T>` used to configure the behaviour of `instantiate()`
- `unsupported` exception thrown when an operation is not supported
- `value_type` - a `dynamic` used for containers

Methods:
- `T &as<T>()` - gets the internal value (or referenced value), or throws `incompatible` if `T` is not held.
- `const T &as<T>() const` - gets the internal value, const version.
- `int as_int()` - converts the type to an `int` if possible, otherwise `unsupported` is thrown.
- `double as_double()` - converts the value to a `double` if possible, otherwise `unsupported` is thrown
- `dynamic const_ref()` - returns a const ref to this dynamic. The result behaves like a `const dynamic&`, and the referee must be in scope for the reference to be valid.
- `dynamic first()` - gets the `first` field if available (particularly for wrapping `std::pair`). Throws `unsupported` if unsupported.
- `dynamic first() const` - const version
- `size_type hash() const` - calculates a hash value (using `traits::hash()`). Throws `unsupported` if unavailable.
- `dynamic make_shared()` - reassigns this dynamic to a shared pointer. (TODO: Existing pointers??)
- `dynamic ref()` - returns a ref to this dynamic. The result behaves like a `dynamic&` and can be used to update referee. (TODO: shared pointers??)
- `dynamic ref() const` - returns a const ref to this dynamic. See `const_ref()`.
- `dynamic second()` - gets the `second` field if available (particularly for wrapping `std::pair`)
- `dynamic second() const` - const version
- `dynamic shared_ref()` - TODO
- `dynamic shared_ref() const` TODO
- `std::string str() const` - gets a string representation of this `dynamic`, using `traits::str()`.
- `T * try_get<T>()` - returns a pointer or `nullptr` if the `dynamic` does not hold exactly a `T`.
- `const T * try_get<T>() const` - const version
- `const std::type_info &type_info() const` - gets the `type_info` of the held object
- `void swap(dynamic&)` - swaps this dynamic with another
- `const std::string &type_str() const` - gets a pretty type string of this dynamic. The string is not portable.
- `dynamic weak_ref()` - gets a weak reference to a shared `dynamic`. Throws `unsupported` if the object is not shared.

Static methods:
- `static dynamic const_reference(const T &t)` - returns a `dynamic` holding a reference to `t`
- `static dynamic dict()` - returns a `dynamic` holding a dictionary (`std::unordered_map<dynamic,dynamic>`)
- `static dynamic dict(std::initializer_list<std::pair<dynamic, dynamic>>)` - returns a `dynamic` holding a dictionary initialized with the given data
- `static dynamic function(auto f)` - returns a `dynamic` that is callable with the given function `f`. Requires `#include <cutty/dynamic/function.hpp>`
- `template <typename T> static const types &instantiate()` - internal function that needs to be instantiated to add new wrapped types
- `static dynamic list()` - returns a `dynamic` holding an empty list (`std::vector<dynamic>`)
- `static dynamic list(std::initializer_list<dynamic>)` - returns a `dynamic` holding a list initialized with the given data
- `static dynamic map()` - returns a `dynamic` holding an empty map
- `static dynamic map(std::initializer_list<std::pair<dynamic, dynamic>>)` - returns a `dynamic` holding a map initialised with the given data
- `static dynamic queue()` - returns a `dynamic` holding a queue (`std::deque<dynamic>`)
- `static dynamic queue(std::initializer_list<dynamic>)`
- `static dynamic reference(T&)`
- `static dynamic set()`
- `static dynamic set(std::initializer_list<dynamic>)`
- `static dynamic shared(const T&)`
- `static dynamic shared(T&&)`

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
- `void push_front()`
- `void push_back()`
- `void pop_back()`
- `void pop_front()`
- `void erase(const dynamic&)`
- `void erase(const dynamic&, const dynamic&)`

Conversions:
- `explicit operator bool`
- `explicit operator int`
- `explicit operator size_type`
- `explicit operator double`

Wrapped operators:
- `dynamic operator()(auto&&...)`
- `dynamic & operator++()`
- `dynamic operator++(int)`
- `dynamic & operator--()`
- `dynamic operator--(int)`
- `dynamic operator[](dynamic)` (comes in multiple versions)
- `dynamic &operator=(dynamic&&)`, 
- `dynamic &operator=(const dynamic&)`, 
- `dynamic &operator+=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=`, 
- `bool operator==()`
- `std::weak_ordering operator<=>()`
- Binary operators: `+` `-` `*` `/` `%` `|` `&` `^`, '>>', `>>`
- Unary operators: `+`, `-`, `~`, `*`
- `std::ostream &<<(std::ostream&, const dynamic&)`

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

This class is used to configure the behaviour of `dynamic` when wrapping a type. Clients can specialise this type.

```c++
template<typename T> struct dynamic::traits;
```

Types:
- `const_reference`
- `reference`

Methods:
- `static void stream_to(const_reference self, std::ostream &os)`
- `static const std::string &type_str()`
- `static bool as_bool(const_reference self)`
- `static int as_int(const_reference self)`
- `static double as_double(const_reference self)`
- `static std::optional<dynamic::int_type> try_get_integral(const_reference self)`
- `static std::optional<double> try_get_double(const_reference self)`
- `static std::optional<std::string_view> try_get_string(const_reference self)`

- `static bool op_eq(const_reference x, const dynamic &y)`
- `static bool op_lt(const_reference x, const dynamic &y)`
- `static dynamic op_add(const_reference x, const dynamic &y)`
- `static dynamic op_sub(const_reference x, const dynamic &y)`
- `static dynamic op_mul(const_reference x, const dynamic &y)`
- `static dynamic op_div(const_reference x, const dynamic &y)`
- `static dynamic op_mod(const_reference x, const dynamic &y)`
- `static dynamic op_star(reference self)`
- `static dynamic op_star(const_reference self)`
- `static void op_inc(reference self)`
- `static void op_dec(reference self)`
- `static dynamic op_minus(const_reference self)`
- `static dynamic call(const_reference self, std::size_t n_args, const dynamic *args)`
- `static dynamic op_index(reference self, dynamic::int_type i)`
- `static dynamic op_index(const_reference self, dynamic::int_type i)`
- `static dynamic op_index(reference self, const char *i)`
- `static dynamic op_index(const_reference self, const char *i)`
- `static dynamic op_index(const_reference self, const dynamic &i)`
- `static dynamic op_index(reference self, const dynamic &i)`
- `static std::size_t hash(const_reference self)`
- `static void push_back(reference self, const dynamic &y)`
- `static void pop_back(reference self)`
- `static void push_front(reference self, const dynamic &value)`
- `static void pop_front(reference self)`
- `static std::size_t size(const_reference self)`
- `static dynamic begin(const_reference self)`
- `static dynamic begin(reference self)`
- `static dynamic rbegin(const_reference self)`
- `static dynamic rbegin(reference self)`
- `static dynamic end(const_reference self)`
- `static dynamic end(reference self)`
- `static void erase(reference self, const dynamic&)`
- `static void erase(reference self, const dynamic&, const dynamic&)`
- `static dynamic rend(const_reference self)`
- `static dynamic rend(reference self)`
- `static dynamic front(const_reference self)`
- `static dynamic front(reference self)`
- `static dynamic back(const_reference self)`
- `static dynamic back(reference self)`
- `static void insert(reference self, const dynamic &value)`
- `static void insert(reference self, const dynamic &k, const dynamic &v)`
- `static dynamic first(const_reference self)`
- `static dynamic first(reference self)`
- `static dynamic second(const_reference &self)`
- `static dynamic second(reference self)`

## Class `dynamic::empty_type`

Used to represent the empty value.

```c++
cy::dynamic d;
cy::dynamic::empty_type &e = d.as<cy::dynamic::empty_type>();
```
