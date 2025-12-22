# Cutty

C++ utility library. A collection of fun, interesting and useful C++ gadgets, with an emphasis on simplicity.

## List of libraries

Types:
* [Mixins](doc/mixins.md) - general framework to add extension methods to existing types
* [Sequence](doc/sequence.md) - universal sequences
* [Tags](doc/tags.md) - create lightweight wrapped types
* [Units](doc/units.md) - SI units library built on tags
* [With](doc/with.md) - taming global variables

Testing:
* [Approx](include/cutty/approx.hpp) - perform approximate comparisons
* [Check](include/cutty/check.hpp) - macro-free runtime assertions
* [Scope hooks](include/cutty/scope_hooks.hpp) - log scope entry and exits
* [Test](include/cutty/test.hpp) - simple macro-free unit test framework

Utilities:
* [Fraction](include/cutty/fraction.hpp) - similar to `std::ratio`
* [Pretty type](include/cutty/pretty_type.hpp) - get a pretty string for any type
* [Print](include/cutty/print.hpp) - print any value

## List of libraries coming soon

* Dynamic - full `dynamic` type available in C++
* Property - Emulating C# property wrappers in C++
* Shared memory - cross platform memory mapped files
* Persist - a storage manager for memory-mapped files

## Building

Prerequisites: C++23, builds on Linux, MacOS and Windows using CMake.
