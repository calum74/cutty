/**
    Utilities to print values, even without an operator <<
 */
#pragma once

#include "pretty_type.hpp"

#include <iostream>

namespace cutty
{
template <typename T>
concept printable = requires(const T &t, std::ostream &os) { os << t; };

template <typename T> void print_stream(std::ostream &os, const T &t)
{
    if constexpr (printable<T>)
    {
        os << t;
    }
    else
    {
        os << pretty_type<T>() << "@" << &t;
    }
}

template <typename T, typename... Ts> void print_stream(std::ostream &os, const T &t, const Ts &...ts)
{
    print_stream(os, t);
    os << ' ';
    print_stream(os, ts...);
}

template <typename T> void print_stream(std::ostream &os)
{
    os << std::endl;
}

template <typename... Ts> void print(const Ts &...ts)
{
    print_stream(std::cout, ts..., '\n');
}

// print_str
} // namespace cutty
