/**
    Utilities to print values, even without an operator <<
 */
#pragma once

#include "pretty_type.hpp"
#include "separator.hpp"

#include <iostream>
#include <sstream>

namespace cutty
{
template <typename T>
concept printable = requires(const T &t, std::ostream &os) { os << t; };

template <typename T>
concept print_as_tuple = std::tuple_size<std::remove_cvref_t<T>>::value >=0;

template <typename T> void print_stream(std::ostream &os, const T &t)
{
    if constexpr (printable<T>)
    {
        os << t;
    }
    else if constexpr (std::ranges::range<T>)
    {
        separator sep;
        os << "{";
        for(auto &i : t)
        {
            os << sep;
            print_stream(os, i);
        }
        os << "}";
    }
    else if constexpr (print_as_tuple<T>)
    {
        os << "(";
        separator sep;
        [&]<size_t...Index> (std::index_sequence<Index...>)
        {
            if constexpr (sizeof...(Index)>0)
            {
                ((os << sep, print_stream(os, std::get<Index>(t))), ...);
            }
        } (std::make_index_sequence<std::tuple_size_v<T>>());
        os << ")";
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

template <typename... Ts> std::string print_str(const Ts &...ts)
{
    std::stringstream ss;
    print_stream(ss, ts...);
    return ss.str();
}
} // namespace cutty
