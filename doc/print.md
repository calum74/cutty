# Print

Sample: [print.cpp](../samples/print.cpp)

Print any value, including ranges and values that lack stream and formatting support. Automatically prints the contents of ranges and tuples, or displays the type name and address for objects without iostream support.

```c++
std::vector<int> x;
cy::print("The values are", x);
```

# Reference

## Function `print()`

```c++
template<typename...Ts>
void print(const Ts&...);
```

Prints the values to `std::cout`, and appends a newline.

## Function `print_stream()`

```c++
template<typename...Ts>
void print_stream(std::ostream &os, const Ts&...);
```

Prints the values to the specified stream. A space is inserted between each value.
A best effort attempt is made to display each value:

1. If a type supports `operator<<`, use that.
2. If a type is a `std::ranges::range`, display the contents
3. If a type is a tuple (supports `std::tuple_size`), display the elements
4. Otherwise, prints the type name and its memory address.

## Function `print_str()`

```c++
template<typename...Ts>
std::string print_str(const Ts&...);
```

Prints the values to a string. A newline is not appended.
