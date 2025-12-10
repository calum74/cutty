# Tags

A *tag* is a lightweight wrapper around a datatype (for example a `string`, `double`, or `int`) which labels it in some way, to enable

- better documentation of raw numbers or strings
- prevent accidental or illogical operations
- easily implement allowable conversions 
- makes it simpler to implement wrapper types

It is a generalisation of the idea of *units* (for example distance, mass, time), but is extensible to user types.

## Tutorial

A tag is just any data type, for example

```c++
struct bytes;
struct compression_level;
struct blocking;
struct uri;
struct port;
struct force_ssl;
```

You can then define functions that take particular argument types, for example

```c++
void wait(cy::tagged<std::size_t, cy::milliseconds> time, cy::tagged<bool, blocking> block);

void send(
    cy::tagged<std::string, host> host_name, 
    cy::tagged<int, port> port_number,
    cy::tagged<std::string, uri> uri,
    cy::tagged<size_t, bytes> max_size,
    cy::tagged<bool, force_ssl> ssl,
    cy::tagged<int, compression_level> compression);
```

To create a tag, use the `cy::tag<T>()` function, for example

```c++
cy::tagged<bool, force_ssl> using_ssl{true};

wait(cy::tag<cy::seconds>(5), cy::tag<blocking>(false));
```

To access the value of a tag, use the `*` operator

```c++
void wait(cy::tagged<std::size_t, cy::milliseconds> timeout, cy::tagged<bool, blocking> block)
{
    if(*block)
    {
        ::Sleep(*timeout);
    }
}
```

# Advanced tag usage

## Tag IO

## Metric and other units

The [units](units.md) library provide a large number of built-in tags covering common metric and imperial units, distance, mass, time, temperature etc.

## Tuples



# Reference

## Types

### `cy::tagged<T, Tag>`

Types:
- `using value_type = T;`
- `using tag_type = Tag;`

Constructors

Operators
- `template<typename T1, Tag2, typename T2 ,Tag2> operator+(const tagged<T1,Tag1>&lhs, const tagged<T2, Tag2>&rhs);`
- operator *
- ...

Comparators

### `enum cy::text_type`

- all
- singular (defaults to all)
- plural (defaults to all)
- denominator (defaults to all)

## Functions

### `template<typename T, typename Tag> cy::tagged<T, Tag> cy::tag(const T &)`

Creates a tag of a given type.

```c++
auto temp = cy::tag<cy::Kelvin>(0.0001);
```

### `template<typename T, typename Tag, typename Tag2> cy::tagged<T, Tag> cy::tag(const tagged<T, Tag2> &)`

Converts one tag into another, performing conversion operations as needed.

### `template<typename T, typename Tag, typename Tag2> cy::tagged<T, Tag> cy::delta(const tagged<T, Tag2> &)`

### `template<typename V1, typename T1, typename V2, typename T2> convert(const tagged<V1,T1> &src, tagged<V2,T2> &dest)`

## Constants

### `template<typename Tag, text_type> const char * tag_text`

### `template<typename Tag, text_type> const char * tag_symbol`





## cy::composite<>
Allows tagged tuples, for example

```
struct imaginary_tag;

template<typename T>
struct cy::tag_traits<imaginary_tag, T>
{
    // Special multiplication rules
};

template<typename T>
T operator*(cy::tagged<imaginary_tag, T> x, cy::tagged<imaginary_tag, T> y)
{
    return -x*y;
}

```