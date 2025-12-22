# Tags

Sample: [tags.cpp](../tests/tags.cpp)

A *tag* is a lightweight wrapper around a datatype (for example a `string`, `double`, or `int`) which labels it in some way, to enable

- better documentation of raw numbers or strings
- prevent accidental or invalid operations
- implement allowable conversions 
- makes it simpler to implement wrapper types

It is a generalisation of the idea of *units* (for example distance, mass, time), but its main purpose is for user types.

## Tutorial

A tag is just any data type, for example `struct bytes_t` or `struct hostname_t`. You can then  define bespoke datatypes representing particular types of data, for example:

```c++
using ChunkSize = cy::tagged<size_t, cy::byte>;
using CompressionLevel = cy::tagged<int, struct comp_level_t>;
using Blocking = cy::tagged<bool, struct blocking_t>;
using Hostname = cy::tagged<std::string, struct hostname_t>;
using Uri = cy::tagged<std::string, struct uri_t>;
using Port = cy::tagged<int, struct port_t>;
using Ssl = cy::tagged<bool, struct force_ssl_t>;
```

Function signatures can be changed to take more specific tagged types, for example

```c++
#include <cutty/tags.hpp>
namespace cy = cutty;

void wait(cy::tagged<std::size_t, cy::millisecond> timeout, Blocking block);

void send(Hostname hostname, Port port, Uri uri, ChunkSize chunksize, Ssl force_ssl, CompressionLevel compression);
```

To create a tag, pass the value to the `tagged` constructor, or use the `cy::tag<T>()` function. For example

```c++
    Ssl using_ssl{true};
    wait(5_second, Blocking{true});
```

Note that the tag takes care of units, for example converting seconds to milliseconds.
To access the value of a tag, use the `*` operator

```c++
void wait(cy::tagged<std::size_t, cy::milliseconds> timeout, Blocking block)
{
    if(*block)
    {
        Sleep(*timeout);
    }
}
```

Writing a tag to a stream will output the appropriate unit if available.

```c++
std::cout << cy::tag<cy::second>(10);  // Outputs "10s"
```

The [units](units.md) library provide a large number of built-in tags covering common metric and imperial units, distance, mass, time, temperature etc.

See [tags.cpp](../test/tags.cpp) for more examples and advanced usage.

# Reference

## Header and namespace

```c++
#include <cutty/tags.hpp>
namespace cy = cutty;
```

## Types

### `class tagged<V, T>`

```c++
template<typename V, typename T> class tagged;
```

Mixins:
- `tagged_methods<T>`

Member types:
- `using value_type = V;`
- `using tag_type = T;`

Constructors:
- `tagged()`: Initialise with a default value as defined by `cy::initialize()`.
- `explicit tagged(const V&)`: Initialise with a given value
- `explicit tagged(V&&)`: Initialise with a given rvalue
- `tagged(const tagged<T2, V2>&)`: Initialise from another compatible tag, performing conversion as necessary.

Operators:
- `operator=(const tagged<V2,T2>&other)`: Assign from another tag.
- `operator*()`: Access the underlying value, for reading or writing. 
- `bool operator ==(const tagged<V2,T2>&) const`: Compare with another tagged value, performing conversions as required. `!=` is synthesised.
- `auto operator <=>(const tagged<V2,T2>&) const`: Compare with another tagged value, performing conversions as required. `<`, `<=`, `>` and `>=` are synthesised.

Tag-preserving arithmetic:

- `tagged<V1,T1> operator+(const tagged<V1,T1>&lhs, const tagged<V2, T2>&rhs)`
- `tagged<V1,T1> operator-(const tagged<V1,T1>&lhs, const tagged<V2, T2>&rhs)`
- `tagged<V,T> operator*(const V&lhs, const tagged<V, T>&rhs)`: Perform a scalar multiplication, resulting in a tagged value of the same type.
- `tagged<V,T> operator*(const tagged<V, T>&lhs, const V&rhs)`:  Perform a scalar multiplication, resulting in a tagged value of the same type.
- `tagged<V,T> operator/(const tagged<V, T>&lhs, const V&rhs)`:  Perform a scalar division, resulting in a tagged value of the same type.

Tag-modifying arithmetic:

- `operator*` and `operator/`: always compile, generate a new tag with the unit being the product or quotient of the original units.

### `struct tag_traits<T>` 

Provides a customisation point for tags. This type can be specialised. (See source code for details.)

## Concepts

### `concept convertible_to<T1, T2>`

Holds if the tag `T1` is convertible to `T2`. It means that the `convert()` function will succeed.

### `concept common_type<T1, T2>`

Holds if `T1` and `T2` share a common type. A slightly weaker form of `convertible_to` which is sometimes needed for template deduction.

## Functions

### `tag<T>()`

```c++
template<typename T, typename Tag>
cy::tagged<T, Tag>cy::tag(const T &)
```

Creates a tag of a given type, for example `cy::tag<cy::Kelvin>(0.0001)`.

```c++
template<typename T1, typename V, convertible_to<T1> T2>
cy::tagged<V, T1> cy::tag(const tagged<V, T2> &)
```

Converts one tag into another, performing conversion operations as needed.

### `delta<T>()`

```c++
template<typename T1, typename V, convertible_to<T1> T2>
cy::tagged<V, T1> cy::delta(const tagged<V, T2> &)
```

Converts one tag to another type, but treats the value as a "delta". This is for conversions like

```c++
t1 + cy::delta<cy::Celcius>(t2)
```

where a naive conversion of `t2` from `Farenheit` to `Celcius` would do the wrong thing.

### `convert<T1, T2>()`

Converts one tagged value to another. This can be specialised to implement custom conversions.

```c++
template<typename V1, typename T1, typename V2, typename T2>
void convert(const tagged<V1,T1> &src, tagged<V2,T2> &dest)
```

By default, conversions are applied multi-stage:

1. Remove scalar differences
2. Convert src to the common type
3. Convert dest to the common type.

For example the common type of `cy::Celcius` and `cy::Farenheit` is `cy::Kelvin`.

### `initialize()`

```c++
template<typename V, typename T>
void initialize(tagged<V,T>&);
```

Initialises a tag with a default value. This can be specialised to implement custom initialisation.

## Constants

### `tag_text`

```
template<typename T> const char * tag_text;
```

Specialise this to provide a custom name for your tag when printing it. This will insert a space and automatically append `s` for plurals. If this is not desired, use `tag_symbol` instead.

### `tag_symbol`

```
template<typename T> const char * tag_symbol;
```

Specialise this to provide a custom symbol for your tag when printing it. 

## Configuration points

The available specialisations are: `struct tag_traits<T>`, `const char *tag_text<T>`, `const char *tag_symbol<T>`, `initialize<T>()` and `convert()`.
