# Tags

A *tag* is a lightweight wrapper around a datatype (for example a `string`, `double`, or `int`) which labels it in some way, to enable

- better documentation of raw numbers or strings
- prevent accidental or illogical operations
- easily implement allowable conversions 
- makes it simpler to implement wrapper types

It is a generalisation of the idea of *units* (for example mass, energy, time), but is extensible to user types.

## How to create a tag

A tag is just any data type, for example

```c++
struct cash_tag;
using cash = cy::tagged<cash_tag, double>;

cash x(1.20);
x = cash(3.00);
x = 1;  // Illegal
double value = *x;  // Get the value from the tag
x + cash(1);  // ok
x * 2;  // ok
x * x;  // not ok unless you define what this means
```



## cy::tagged<Tag, T>

- tag_type
- value_type
- operator *
- explicit tagged(const T&);
- operator = (const tagged&);
- Arithmetic operators:
    tagged<Tag, T> + tagged<Tag, T>
    tagged<Tag, T> - tagged<Tag, T>
    tagged * double
- operator <<

## cy::tag_traits<Tag, T>
    static constexpr bool 
    static 

## Complex numbers

## Times

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