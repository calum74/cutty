# Cutty

C++ utility library. A collection of fun, interesting and useful C++ gadgets, with an emphasis on simplicity.

## List of libraries

Types:
* [Dynamic](docs/dynamic.md) - full `dynamic` type available in C++
* [Mixins](doc/mixins.md) - general framework to add extension methods to existing types
* [Sequence](doc/sequence.md) - universal sequences
* [Tags](doc/tags.md) - lightweight wrapped types
* [Units](doc/units.md) - units library built on tags
* [With](doc/with.md) - a more principled approach to singletons and global variables

Testing:
* [Approx](doc/approx.md) - perform approximate comparisons
* [Check](doc/check.md) - macro-free runtime assertions
* [Scope hooks](doc/scope_hooks.md) - log scope entry and exits
* [Test](doc/test.md) - simple macro-free unit test framework

Utilities:
* [Pretty type](doc/pretty_type.md) - get a pretty string for any type
* [Print](doc/print.md) - print any value
* [Separator](doc/separator.md) - makes printing lists easier

## Experimental libraries

* Property - Emulating C# property wrappers in C++
* Shared memory - cross platform memory mapped files
* Persist - a storage manager for memory-mapped files

## Building

Prerequisites: C++23, builds on Linux, MacOS and Windows using CMake.

[![CMake on multiple platforms](https://github.com/calum74/cutty/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/calum74/cutty/actions/workflows/cmake-multi-platform.yml)
