# Dynamic

Samples: [short_tutorial.cpp](../samples/dynamic/short_tutorial.cpp), [tutorial.cpp](../samples/dynamic/tutorial.cpp)

Solves the problem of working with multiple types.

Finally, what you never asked for, a fully-featured `dynamic` type in C++!  *Dynamic* adds the ability to wrap and work with any C++ type, with errors thrown at runtime instead of compile time.  Unlike `std::any` or `std::variant`, `dynamic` allows you can work with dynamic objects, for example accessing elements, performing arithmetic or converting to a string. Possible use cases include working with structured data, interfacing with dynamic programming languages, spreadsheets, or creating generic wrappers.

*Dynamic* behaves similarly to the C# `dynamic` keyword, and can wrap any C++ type as `dynamic`.

# Tutorial

The header file for the library is `cutty/dynamic.hpp`, and we'll use the namespace `cy` for `cutty`.

```c++
#include <cutty/dynamic.hpp>

namespace cy = cutty;
```

This header file includes the main user-facing library for working with dynamic types. If you need to define new dynamic types, you also need to include `<cutty/dynamic/instantiate.hpp>`. If you need function wrappers, include `<cutty/dynamic/function.hpp>`.

To create a dynamic value, pass a C++ value to the `dynamic` constructor.

```c++
cy::dynamic a;          // An empty dynamic
cy::dynamic b = 12;     // An integer dynamic
cy::dynamic c = "abc";  // A string dynamic
```

## Output

Output is supported using `operator<<` or `cy::print()`. The `str()` method returns a string representation of the dynamic, and this is always implemented.

```c++
cy::print(a);   // Output: (empty)  
std::cout << b << std::endl;
std::cout << c.str() << std::endl;
```

## Operations

`dynamic` values support all of the operators of their underlying types. The other operand will be converted to `dynamic` if needed.

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

All other operators are wrapped, including all arithmetic and comparison operators, as well as call `()` and index `[]`. Multi-parameter indexers are not implemented at this time. The `<=>` operator has type `std::partial_ordering`.

## Accessing the wrapped value

The "category" of stored object can be obtained using `dynamic::category()`, which returns one of the following:

- `dynamic::value_category::empty`
- `dynamic::value_category::integer`
- `dynamic::value_category::floating_point`
- `dynamic::value_category::string`
- `dynamic::value_category::list`
- `dynamic::value_category::dictionary`
- `dynamic::value_category::other`

The methods `has_value()`, `has_integer()`, `has_floating_point()`, `has_number()` and `has_string()` return whether the given value category is stored (`has_number()` being true for both integers and doubles), and the methods `as_int()`, `as_double()`, `as_string()` and `as_string_view()` return the value *if available*, otherwise throw an exception.

```c++
cy::dynamic x = 12;

if(x.has_number())
{
    std::cout << "The number is " << x.as_double() << std::endl;
}
```

The precise type can be accessed using the `type_info()` method, and the `type_str()` method returns a string representation of the stored type. The `as()` method gets a reference to the wrapped value, or throws `dynamic::incompatible` if the stored data does not match the requested type. `try_get<T>()` returns a pointer to `T` if successful, or `nullptr` if the stored type is not `T`.

```c++
cy::dynamic x = 12;
int &i = x.as<int>();  // Prefer: as_int()
x.type_info();
int *p = x.try_get<int>();
cy::print(x.type_str());  // "int"
```

It is generally safer and easier to use the `has_/as_` accessors instead of relying on the stored type.

## Null objects

Sample: [empty.cpp](../samples/dynamic/empty.cpp)

The default `dynamic` object has a value of type `dynamic::empty_type`. There is no "null" in the C++ sense. To test if an object is null, compare it with another empty dynamic, or use the `has_value()` method as follows:

```c++
if(x.has_value())
{
    // x is not empty
}
```

Don't use `dynamic::empty()` as this checks if a container is empty.

## Conversions

C++ values can be turned into `dynamic` by passing them to the `dynamic` constructor. This takes a copy of the value and is stored in the `dynamic`. Copying the `dynamic` creates a copy of the value.

Every `dynamic` has a string representation, accessed by the `dynamic::str()` method. The methods `as_int()` and `as_double()` return a numerical representation of the value if available, or throws `dynamic::unsupported` otherwise.

There are explicit conversion operators for `bool`, `int_type` and `double`.

## Lists and containers

Sample: [lists.cpp](../samples/dynamic/lists.cpp)

Dynamic objects can wrap C++ containers, and the methods `list()`, `dict()`, `map()`, `object()` and `queue()` create these objects.

| Construction | Wrapped type |
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

As usual, if an object does not support an operation at compile time, it will result in an exception (`dynamic::unsupported`) thrown at runtime.

```c++
not_a_list.push_front(0);  // Exception thrown
```

Containers can be iterated, and its elements bound:

```c++
for(auto [k,v] : map)
{
    cy::print(k, "is", v);
}
```

`dynamic` types are fully compatible with C++ ranges.

## References

`dynamic` values can be references to C++ values. It is the caller's responsibility to ensure that the underlying C++ object remains valid for the duration of the `dynamic` reference. To create a reference, use the `reference()` method, or the `const_reference` method.

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

## Shared references

Copying a `dynamic` normally results in the underlying object being copied. If this is not desired, a `ref()` can be used, but this does not keep the underlying object alive. To solve this problem, `dynamic` implements shared references, which have similar semantics to `std::shared_ptr`. Client code does not need to know it is dealing with a shared object.

Creating a shared value is done using the `dynamic::shared()` static method. Existing dynamic objects can be copied to a shared object using the `shared_ref()` method.

```c++
cy::dynamic i = cy::dynamic::shared(42);
cy::dynamic j = i;
++i;
cy::print(j);  // 43
```

Reference counted pointers could form cyclical data structures, so a solution to this is *weak pointers* which can be used to break cycles by implementing non-owning back-edges in the data structure. The method `dynamic::weak_ref()` create a *weak reference*, which does not keeps its value alive, and cane be upgraded to a `shared_ref()` as follows:

```c++
cy::dynamic i = cy::dynamic::shared(42);
cy::dynamic j = i.weak_ref();
cy::print(j.shared_ref());  // 42
i = 40;
// No more refs to 42
cy::print(j.shared_ref());  // (empty)
```

You must always call `shared_ref()` before attempting to use a weak reference.

## Associative containers

Sample: [maps.cpp](../samples/dynamic/maps.cpp)

Dictionaries, maps and objects are different associative containers, created by `dict()`, `map()` and `object()` respectively. Dictionaries are unsorted, maps are sorted, and objects have string keys, but otherwise the behave very similarly.

The containers can be created using an initializer list, and accessed using `[]`. Other container methods (such as `size()`, `front()`, `back()`, `begin()`, `end()`, `push_back` etc. also work). Attempting to modify the contents of a const container will result in an `unsupported` exception.

```c++
auto o = cy::dynamic::object();
o["joe"] = 12;
auto o2 = o.const_ref();

cy::print(o2["joe"]);   // Ok
cy::print(o2["fred"]);  // throws std::out_of_range
```

## Functions

Sample: [functions.cpp](../samples/dynamic/functions.cpp)

Functions are created by passing a callable object (a lambda or a function) to the `dynamic` constructor. It requires (for now) the header file `cutty/dynamic/function.hpp` just to avoid unnecessary includes.

```c++
#include <cutty/dynamic/function.hpp>
```

Any C++ function can be wrapped, provided that it can be converted to a `std::function<>` with dynamic arguments. C++ arguments are automatically wrapped if needed.

```c++
cy::dynamic fn = [] { return cy::dynamic(42); };
cy::print(fn());  // 42

fn = [] (int x) { return x+1; };
cy::print(fn(10)); // 11
```

Calling a function with the wrong number of arguments results in an `unsupported` exception thrown.

## User types

Samples: [enable_user_type.cpp](../samples/dynamic/enable_user_type.cpp), [use_user_type.cpp](../samples/dynamic/use_user_type.cpp), [user_type.hpp](../samples/dynamic/user_type.hpp)

In order to keep header files small, the `dynamic.hpp` header files do not support user-defined types. Instead, clients should create a separate `.cpp` file which includes `cutty/dynamic/instantiate.hpp`, and specialise `cy::dynamic::instantiate<T>()` with the new type.

This scheme ensures that the header files are relatively small and faster to compile, and that the user-defined type only gets compiled once.

```c++
#include "MyStruct.hpp"

#include <cutty/dynamic/instantiate.hpp>

template const cy::dynamic::types &cy::dynamic::instantiate<MyStruct>();
```

Any other source file can wrap `MyStruct` as needed.

```c++
auto x = cy::dynamic(MyStruct{});
```

The type `cy::dynamic::traits<>` can be specialised to configure the wrapping.

## Const correctness

Mutating methods will fail `unsupported` if attempts are made to modify a `const` value. Const values can be obtained by the regular `const` keyword in C++

```c++
cy::dynamic d;
const cy::dynamic c1;  // const
const cy::dynamic &c2 = c1;  // const
int i;
const int j;
cy::dynamic c3 = cy::dynamic::const_reference(i);  // const
cy::dynamic c4 = cy::dynamic::reference(j);  // const
cy::dynamic c5 = d.as_const();  // const
cy::dynamic c6 = d.const_ref();  // const
cy::dynamic c7 = c1.ref();  // const
cy::dynamic c8 = c1.shared_ref();  // const
```

Users of a `dynamic` will be unaware that their object is immutable simply by looking at the type.

## Object lifetimes

`dynamic` can pass objects by value, reference or by shared reference, and the user cannot usually notice the difference. Assigning to a `dynamic` will generally create a copy, unless the copied object is a shared reference.

The destructor of `dynamic` will also destroy the wrapped object and free any additional resources.

## Threading

`dynamic` types have the same thread safety as their wrapped type. Concurrent assignments to a `dynamic` are not threadsafe. Shared references can be acquired and released safely from multiple threads.

## Exceptions

In addition to exceptions from the wrapped object, creating or assigning a `dynamic` can throw `std::bad_alloc` if additional memory resources could not be acquired. The destructor of the wrapped object must not throw exceptions.

# Implementation

`dynamic` is implemented as a class with 2 pointers. The first pointer is the *type* and the second pointer is the *value*. For small values (for example, integers, doubles, iterators, references etc) *value* stores the wrapped object inline and there is no memory allocation. *type* points to a pure const static class (of type `dynamic::type`) with `virtual` methods to implement all functionality for the wrapped type.

For larger values (strings, lists etc.), *value* points to some heap object managed through `type`. The `type` class is responsible for allocating, copying and freeing any heap-allocated memory.

This scheme is flexible enough for `type` to also implement references, const references and shared pointers.

Whilst clients can implement a `dynamic::type` class manually, the default `type` implementation is configurable via the `dynamic::traits` class.

# Reference

## Header file and namespace

```c++
#include <cutty/dynamic.hpp>
#include <cutty/dynamic/function.hpp>
#include <cutty/dynamic/instantiate.hpp>
```

`class dynamic` is in the `cutty` namespace. Literals are defined in the `cutty::literals` namespace.

## Built in types

The following types are wrapped by default:

- `dynamic::empty_type`
- `std::string`, `std::string_view`, `char*`
- `bool`
- `signed char`, `unsigned char`, `char`
- `short`, `unsigned short`
- `int`, `unsigned int`
- `long`, `unsigned long`
- `long long`, `unsigned long long`
- `float`, `double`
- `std::function<dynamic(const dynamic&, ...)>`

Containers:
- `std::vector<dynamic>`
- `std::deque<dynamic>`
- `std::unordered_map<dynamic, dynamic>`
- `std::unordered_set<dynamic>`
- `std::unordered_map<std::string, dynamic>`
- `std::map<dynamic, dynamic>`
- `std::set<dynamic>`

Associated types (iterators etc) are also wrapped.

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

Constructors can throw `std::bad_alloc` if it is unable to allocate memory.

Types:
- `const_iterator` - a `dynamic` used for container support
- `default_traits<T>` - default implementation of dynamic traits. Inherit from this class to implement traits.
- `difference_type` - a `dynamic` used for container support
- `empty_type` - the value of a default-initialised `dynamic`
- `exception` - base class of `dynamic` exceptions
- `incompatible` exception thrown when a wrong or incompatible type is requested
- `int_type` - how ints are stored internally
- `iterator` - a `dynamic` used with for container support
- `size_type` - `std::size_t`
- `traits<T>` used to configure the behaviour of `instantiate()`
- `unsupported` exception thrown when an operation is not supported
- `value_category` used to indicate the semantic contents
- `value_type` - a `dynamic` used with containers

Methods:
- `T &as<T>()` - gets the wrapped value (or referenced value), or throws `incompatible` if `T` is not held.
- `const T &as<T>() const` - gets the wrapped value, const version.
- `int as_int() const` - converts the type to an `int` if possible, otherwise `unsupported` is thrown.
- `double as_double() const` - converts the value to a `double` if possible, otherwise `unsupported` is thrown
- `std::string as_string() const` - converts the value to a string if possible, otherwise `unsupported` is thrown. Unlike `str()`, this function can fail if the value is not a string.
- `std::string_view as_string_view() const` - returns a `std::string_view` of the stored value if possible, otherwise `unsupported` is thrown. Unlike `str()`, this function can fail if a string_view representation is unavailable, even for string-like objects.
- `dynamic const_ref()` - returns a const ref to this dynamic. The result behaves like a `const dynamic&`, and the referee must be in scope for the reference to be valid.
- `dynamic first()` - gets the `first` field if available (particularly for wrapping `std::pair`). Throws `unsupported` if unsupported.
- `dynamic first() const` - const version
- `size_type hash() const` - calculates a hash value (using `traits::hash()`). Throws `unsupported` if unavailable.
- `bool has_floating_point() const` - returns true if the value is semantically a floating point value, and `as_double()` will succeed.
- `bool has_integer() const` - returns true if the value is semantically an integer, and `as_int()` will succeed. Should not throw.
- `bool has_number() const` - returns true if this value is numeric (integer or floating point). Should not throw.
- `bool has_value() const` - returns true for all values except empty. Should not throw.
- `bool has_string() const` - returns true if this value is a string. `as_string()` will succeed. Should not throw.
- `dynamic make_shared()` - reassigns this dynamic to a shared pointer.
- `dynamic ref()` - returns a ref to this dynamic. The result behaves like a `dynamic&` and can be used to update referee.
- `dynamic ref() const` - returns a const ref to this dynamic. See `const_ref()`.
- `dynamic second()` - gets the `second` field if available (particularly for wrapping `std::pair`)
- `dynamic second() const` - const version
- `dynamic shared_ref()` - return a shared reference to this object, by copying it if necessary
- `dynamic shared_ref() const` - return a const shared reference to this object
- `std::string str() const` - gets a string representation of this `dynamic`, using `traits::str()`.
- `T * try_get<T>()` - returns a pointer or `nullptr` if the `dynamic` does not hold exactly a `T`.
- `const T * try_get<T>() const` - const version
- `const std::type_info &type_info() const` - gets the `type_info` of the held object
- `void swap(dynamic&)` - swaps this dynamic with another
- `const std::string &type_str() const` - gets a pretty type string of this dynamic. The string is not portable.
- `dynamic weak_ref()` - gets a weak reference to a shared `dynamic`. Throws `unsupported` if the object is not shared.

Static methods:
- `static dynamic const_reference(const T &t)` - returns a `dynamic` holding a const reference to `t`
- `static dynamic dict()` - returns a `dynamic` holding a dictionary (`std::unordered_map<dynamic,dynamic>`)
- `static dynamic dict(std::initializer_list<std::pair<dynamic, dynamic>>)` - returns a `dynamic` holding a dictionary initialized with the given data
- `static dynamic function(auto f)` - returns a `dynamic` that is callable with the given function `f`. Requires `#include <cutty/dynamic/function.hpp>`
- `template <typename T> static const types &instantiate()` - function that needs to be instantiated to add new wrapped types
- `static dynamic list()` - returns a `dynamic` holding an empty list (`std::vector<dynamic>`)
- `static dynamic list(std::initializer_list<dynamic>)` - returns a `dynamic` holding a list initialized with the given data
- `static dynamic map()` - returns a `dynamic` holding an empty map
- `static dynamic map(std::initializer_list<std::pair<dynamic, dynamic>>)` - returns a `dynamic` holding a map initialised with the given data
- `static dynamic queue()` - returns a `dynamic` holding a queue (`std::deque<dynamic>`)
- `static dynamic queue(std::initializer_list<dynamic>)` - returns a `dynamic` holding a queue, initialised with the given data
- `static dynamic reference(T&)` - returns a `dynamic` holding a reference to `T`. If `T` is const, then the reference is const. 
- `static dynamic set()` - returns a `dynamic` holding a set (`std::unordered_set<dynamic>`)
- `static dynamic set(std::initializer_list<dynamic>)` - returns a `dynamic` holding an unordered set, initialised with data
- `static dynamic shared(const T&)`, `static dynamic shared(T&&)` - returns a `dynamic` containing a shared reference-counted reference to a `T`.

Wrapped methods:
- `dynamic begin()`
- `dynamic begin() const`
- `dynamic cbegin() const`
- `dynamic cend() const`
- `dynamic crbegin() const`
- `dynamic crend() const`
- `dynamic end()`
- `dynamic end() const`
- `void erase(const dynamic&)`
- `void erase(const dynamic&, const dynamic&)`
- `void pop_back()`
- `void pop_front()`
- `void push_front(const dynamic&)`
- `void push_back(const dynamic&)`
- `dynamic rbegin()`
- `dynamic rbegin() const`
- `dynamic rend()`
- `dynamic rend() const`
- `size_type size() const`

Wrapped conversions:
- `explicit operator bool`
- `explicit operator int_type`
- `explicit operator double`

Wrapped operators:
- `dynamic operator()(auto&&...)`
- `dynamic & operator++()`
- `dynamic operator++(int)`
- `dynamic & operator--()`
- `dynamic operator--(int)`
- `dynamic operator[](...)`
- `dynamic &operator=(dynamic&&)`, 
- `dynamic &operator=(const dynamic&)`, 
- `dynamic &operator+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`
- `bool operator==()`
- `std::partial_ordering operator<=>()`
- Binary operaors: `+` `-` `*` `/` `%` `|` `&` `^`, `>>`, `>>`
- Unary operators: `+`, `-`, `~`, `*`
- `std::ostream &<<(std::ostream&, const dynamic&)` - outputs the contents to a stream, using `cy::print_str()`.

Literals (in the namespace `cutty::literals`):
- `dynamic operator""_d(const char *, std::size_t)` - creates a dynamic wrapping `std::string`
- `dynamic operator""_d(unsigned long long)` - creates a dynamic wrapping `dynamic::int_type`
- `dynamic operator""_d(long double)` - creates a dynamic wrapping `double`
- `dynamic operator""_d(char)` - creates a dynamic wrapping `char`


## Class `dynamic::default_traits`

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
- `static bool as_bool(const_reference self)`
- `static double as_double(const_reference self)`
- `static int as_int(const_reference self)`
- `static std::string as_string(const_reference self)`
- `static std::string_view as_string_view(const_reference self)`
- `static dynamic back(const_reference self)`
- `static dynamic back(reference self)`
- `static dynamic begin(const_reference self)`
- `static dynamic begin(reference self)`
- `static dynamic call(const_reference self, std::size_t n_args, const dynamic *args)`
- `static dynamic::value_category category(const_reference self)`
- `static dynamic end(const_reference self)`
- `static dynamic end(reference self)`
- `static void erase(reference self, const dynamic&)`
- `static void erase(reference self, const dynamic&, const dynamic&)`
- `static dynamic first(const_reference self)`
- `static dynamic first(reference self)`
- `static dynamic front(const_reference self)`
- `static dynamic front(reference self)`
- `static std::size_t hash(const_reference self)`
- `static bool has_value(const_reference self)`
- `static void insert(reference self, const dynamic &value)`
- `static void insert(reference self, const dynamic &k, const dynamic &v)`
- `static dynamic op_add(const_reference x, const dynamic &y)`
- `static std::partial_ordering op_cmp(const_reference x, const dynamic &y)`
- `static void op_dec(reference self)`
- `static dynamic op_div(const_reference x, const dynamic &y)`
- `static bool op_eq(const_reference x, const dynamic &y)`
- `static void op_inc(reference self)`
- `static dynamic op_index(reference self, dynamic::int_type i)`
- `static dynamic op_index(const_reference self, dynamic::int_type i)`
- `static dynamic op_index(reference self, const char *i)`
- `static dynamic op_index(const_reference self, const char *i)`
- `static dynamic op_index(const_reference self, const dynamic &i)`
- `static dynamic op_index(reference self, const dynamic &i)`
- `static std::partial_ordering op_cmp(const_reference x, const dynamic &y)`
- `static dynamic op_minus(const_reference self)`
- `static dynamic op_mod(const_reference x, const dynamic &y)`
- `static dynamic op_mul(const_reference x, const dynamic &y)`
- `static dynamic op_plus(const_reference self)`
- `static dynamic op_star(reference self)`
- `static dynamic op_star(const_reference self)`
- `static dynamic op_sub(const_reference x, const dynamic &y)`
- `static void stream_to(const_reference self, std::ostream &os)`
- `static void pop_back(reference self)`
- `static void pop_front(reference self)`
- `static void push_back(reference self, const dynamic &y)`
- `static void push_front(reference self, const dynamic &value)`
- `static dynamic rbegin(const_reference self)`
- `static dynamic rbegin(reference self)`
- `static dynamic rend(const_reference self)`
- `static dynamic rend(reference self)`
- `static dynamic second(const_reference &self)`
- `static dynamic second(reference self)`
- `static std::size_t size(const_reference self)`
- `static const std::string &type_str()`
- `static std::optional<double> try_get_double(const_reference self)`
- `static std::optional<dynamic::int_type> try_get_integral(const_reference self)`
- `static std::optional<std::string_view> try_get_string(const_reference self)`

## Class `dynamic::empty_type`

Used to represent the empty value. This class has no members.

```c++
cy::dynamic d;
cy::dynamic::empty_type &e = d.as<cy::dynamic::empty_type>();
```

## Enum `dynamic::value_category`

The enum has the following members

- `empty` - `has_value()` is false
- `integer` - `has_integer()` and `has_number()` are true, and `as_int()` and `as_double()` return values
- `floating_point` - `has_float()` and `has_number()` are true, and `as_int() and `as_double()` return values.
- `string` - `has_string()` is true, and `as_string()` returns a value. `as_string_view()` *may* return a value.
- `list`
- `dictionary`
- `other` - anything else that isn't empty. `has_value()` is true.
