# Print

Sample: [print.cpp](../samples/print.cpp)

Print any value, including ranges and values that lack stream and formatting support. Automatically prints the contents of ranges and tuples, or displays the type name and address for objects without ostream support. Start, end and separator text can be configured.

```c++
std::vector<int> x;
cy::print("The values are", x);
cy::print({.start = "(", .sep = ",", .end = ")"}, 1, 2, 3);
```

# Reference

## Function `print()`

```c++
template<typename...Ts>
void print(const Ts&...);

template<typename...Ts>
void print(const print_opts&, const Ts&...);
```

Prints the values to `std::cout`, and appends a newline unless otherwise specified.

## Function `print_stream()`

```c++
template<typename...Ts>
void print_stream(std::ostream &os, const Ts&...);

template<typename...Ts>
void print_stream(std::ostream &os, const print_opts &, const Ts&...);
```

Prints the values to the specified stream.  A newline is not appended by default.
A best effort attempt is made to display each value:

1. If a type supports `operator<<`, use that
2. If a type is a `std::ranges::range`, display the contents
3. If a type is a tuple (supports `std::tuple_size`), display the elements
4. Otherwise, prints the type name and its memory address.

## Function `print_str()`

```c++
template<typename...Ts>
std::string print_str(const Ts&...);
```

```c++
template<typename...Ts>
std::string print_str(const print_str_opts &, const Ts&...);
```

Prints the values to a string. A newline is not appended by default.

## Class `print_opts`

Configures the extra text that is printed.

Members:
- `const char *start="";` - string to print at the start
- `const char *sep=" ";` - string to print in between items
- `const char *end="\n";` - string to print at the end

## Class `print_string_opts`

Configures the extra text that is printed with `print_str()`. This class has different defaults to `print_opts`.

Members:
- `const char *start="";` - string to print at the start
- `const char *sep=" ";` - string to print in between items
- `const char *end="";` - string to print at the end
