# Cutty

C++ utility library. A collection of fun, interesting and useful C++ gadgets.

## List of libraries

Types:
* [Mixins](samples/mixins.cpp) - general framework to add extension methods to existing types
* [Tags](doc/tags.md) - a lightweight data wrapper
* [Satellite](include/cutty/satellite.hpp) - taming global variables

Testing:
* [Approx](include/cutty/approx.hpp) - perform approximate comparisons
* [Check](include/cutty/check.hpp) - runtime assertions without macros
* [Scope hooks](include/cutty/scope_hooks.hpp) - log scope entry and exits
* [Test](include/cutty/test.hpp) - simple unit test framework

Utilities:
* [Fraction](include/cutty/fraction.hpp) - similar to `std::ratio`
* [Pretty type](include/cutty/pretty_type.hpp) - get a pretty string for any type
* [Print](include/cutty/print.hpp) - print any value

## List of libraries coming soon

* Dynamic - full `dynamic` type available in C++
* Linq - Emulate C# Linq
* Units - 
* Property - Emulating C# property wrappers in C++
* [Sequence](doc/sequence.md) - Universal sequences
* Shared memory - cross platform memory mapped files
* Persist - a storage manager for memory-mapped files

## Building

Prerequisites: C++23, builds on Linux, MacOS and Windows using CMake.
