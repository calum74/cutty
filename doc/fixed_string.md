# Fixed strings

Solve the problem of using strings as template parameters.

In C++, normal strings like `std::string` or `char*` can't be used as template parameters. `fixed_string` is useful for template metaprogramming libraries, and binding strings to types.

```c++
#include <cutty/fixed_string.hpp>
namespace cy = cutty;

template<cy::fixed_string Prefix> struct Logger
{
    static_assert(Prefix.front() == '[', "prefix must start with '['");
    static_assert(Prefix.back() == ']', "prefix must end with ']'");
    static constexpr bool is_warning = Prefix.contains(cy::fixed_string("WARNING"));
    ...
};

using WarningLogger = Logger<"[WARNING]">;
```

# Reference

## Header file and namespace

```c++
#include <cutty/fixed_string.hpp>

namespace cy = cutty;
```

## Class `fixed_string`

A constant compile-time string.

```c++
template <std::size_t N> struct fixed_string;
```

A `fixed_string` is a `std::ranges::random_access_range`.

### Constructors

- `constexpr fixed_string(const char (&str)[N])` - construct from a string literal.
- `template<size_type M> constexpr fixed_string(fixed_string<M> str, size_type offset)` - construct from a substring.

### Types

- `using value_type = char`
- `using size_type = std::size_t`
- `using iterator = char*`
- `using const_iterator = const char*`

### Methods

- `constexpr const char *c_str() const` - Gets the null-terminated C-string of this fixed string.
- `constexpr size_type size() const` - Gets the number of characters (excluding the final `\0`).
- `template <size_type M> consteval int index_of(fixed_string<M> str, size_type start = 0) const` - Finds the index of a substring `str`, or returns `-1` if not found.
- `template <size_type M> consteval bool contains(fixed_string<M> str, size_type start = 0) const` - Returns true if `str` is found at an index >= `start`.
- `constexpr iterator begin()`, `constexpr const_iterator begin() const`, `constexpr const_iterator cbegin() const` - return an iterator to the beginning of the string
- `constexpr iterator end()`, `constexpr const_iterator end() const`, `constexpr const_iterator cend() const` - return an iterator to the end of the string

### Operators

- `constexpr char operator[](size_t i) const` - index operator
- `template <std::size_t N> std::ostream &operator<<(std::ostream &os, fixed_string<N> str)` - output string
- `template <size_type M> constexpr bool operator==(fixed_string<M> rhs) const` - compare strings for equality

## Utilities

- `template <fixed_string Haystack, fixed_string Needle> constexpr size_t fixed_string_count` - Counts the number of occurrences of `Needle` in `Haystack`.
- `template <fixed_string Haystack, char Needle> constexpr size_t fixed_string_count_ch` - Counts the number of occurrences of `Needle` in `Haystack`.
- `template <fixed_string Str, fixed_string Separator, size_t Item> constexpr auto fixed_string_split` - An item in a string split into parts by `Separator`.

