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

struct print_opts
{
    const char *start = "";
    const char *sep = " ";
    const char *end = "\n";
};

struct print_str_opts
{
    const char *start = "";
    const char *sep = " ";
    const char *end = "";
};

namespace detail
{
template <typename T>
concept printable = requires(const T &t, std::ostream &os) { os << t; };

template <typename T>
concept print_as_tuple = std::tuple_size<std::remove_cvref_t<T>>::value >= 0;

template <typename T> void print_item(std::ostream &os, const T &t)
{
    if constexpr (printable<T>)
    {
        os << t;
    }
    else if constexpr (std::ranges::range<T>)
    {
        separator sep;
        os << "{";
        for (auto &&i : t)
        {
            os << sep;
            print_item(os, i);
        }
        os << "}";
    }
    else if constexpr (print_as_tuple<T>)
    {
        os << "(";
        separator sep;
        [&]<size_t... Index>(std::index_sequence<Index...>) {
            if constexpr (sizeof...(Index) > 0)
            {
                ((os << sep, print_item(os, std::get<Index>(t))), ...);
            }
        }(std::make_index_sequence<std::tuple_size_v<T>>());
        os << ")";
    }
    else
    {
        os << pretty_type<T>() << "@" << &t;
    }
}

template <typename T> void print_loop(std::ostream &os, const print_opts &opts, const T &t)
{
    detail::print_item(os, t);
    os << opts.end;
}

template <typename T, typename... Ts>
void print_loop(std::ostream &os, const print_opts &opts, const T &t, const Ts &...ts)
{
    detail::print_item(os, t);
    os << opts.sep;
    print_loop(os, opts, ts...);
}

inline void print_loop(std::ostream &os, const print_opts &opts)
{
    os << opts.end;
}

} // namespace detail

template <typename... Ts> void print_stream(std::ostream &os, const print_opts &opts, const Ts &...ts)
{
    os << opts.start;
    detail::print_loop(os, opts, ts...);
}

template <typename... Ts> void print_stream(std::ostream &os, const Ts &...ts)
{
    print_stream(os, print_opts{.end = ""}, ts...);
}

template <typename... Ts> void print(const print_opts &opts, const Ts &...ts)
{
    print_stream(std::cout, opts, ts...);
}

template <typename... Ts> void print(const print_opts &opts)
{
    print_stream(std::cout, opts);
}

template <typename... Ts> void print(const Ts &...ts)
{
    print(print_opts{}, ts...);
}

template <typename... Ts> std::string print_str(const print_str_opts &opts, const Ts &...ts)
{
    std::stringstream ss;
    print_stream(ss, print_opts{.start=opts.start, .sep = opts.sep, .end=opts.end}, ts...);
    return ss.str();
}

template <typename... Ts> std::string print_str(const Ts &...ts)
{
    return print_str(print_str_opts{}, ts...);
}

} // namespace cutty
