// Main header file for sequences, included in .cpp files.
// This contains the full implementation of sequences.

#pragma once

// We don't use this much so avoid including this by default

#include <array>
#include <cstring>
#include <iterator>
#include <ranges>
#include <stdexcept>
#include <type_traits>

#include "sequence_fwd.hpp"
#include "sequences/fwd.hpp"
#include "sequences/helpers.hpp"
#include "sequences/int_iterator.hpp"

#include "sequences/base_sequence.hpp"
#include "sequences/concat_sequence.hpp"
#include "sequences/empty_sequence.hpp"
#include "sequences/generated_sequence.hpp"
#include "sequences/iterator_sequence.hpp"
#include "sequences/merge_sequence.hpp"
#include "sequences/output_sequence.hpp"
#include "sequences/pointer_sequence.hpp"
#include "sequences/repeat_sequence.hpp"
#include "sequences/select_sequence.hpp"
#include "sequences/sequence.hpp"
#include "sequences/sequence_ref.hpp"
#include "sequences/singleton_sequence.hpp"
#include "sequences/skip_sequence.hpp"
#include "sequences/skip_until_sequence.hpp"
#include "sequences/split_sequence.hpp"
#include "sequences/stored_sequence.hpp"
#include "sequences/take_sequence.hpp"
#include "sequences/take_while_sequence.hpp"
#include "sequences/virtual_sequence.hpp"
#include "sequences/where_sequence.hpp"

namespace cutty
{
// Constructs a sequence from a container
template <std::ranges::input_range Container> auto seq(const Container &c)
{
    return sequences::iterator_sequence{std::ranges::begin(c), std::ranges::end(c)};
}

// Constructs a sequence from a container
template <std::ranges::input_range Container> auto seq(Container &c)
{
    return sequences::iterator_sequence{std::begin(c), std::end(c)};
}

// Constructs a sequence from a fixed-length array
template <typename T, int Size> pointer_sequence<T> seq(const T (&items)[Size])
{
    return {items, items + Size};
}

// Constructs a sequence from a list
template <typename T, typename... Ts> sequences::stored_sequence<std::array<T, 1 + sizeof...(Ts)>> list(T t, Ts... ts)
{
    return {std::array<T, 1 + sizeof...(Ts)>({t, ts...})};
}

// Constructs a sequence from a single element
template <typename T> sequences::singleton_sequence<T> single(const T &t)
{
    return {t};
}

// Constructs an empty sequence
template <typename T> sequences::empty_sequence<T> seq()
{
    return {};
}

// Constructs an empty sequence (alternative)
template <typename T> sequences::empty_sequence<T> list()
{
    return {};
}

// Constructs an integer range sequence
inline auto seq(int a, int b)
{
    return sequences::iterator_sequence{sequences::int_iterator(a), sequences::int_iterator(b + 1)};
}

// Constructs a sequence from a C string
inline pointer_sequence<char> seq(const char *cstr)
{
    return {cstr, cstr + std::strlen(cstr)};
}

// Constructs a sequence from a pointer
template <typename T> pointer_sequence<T> seq(const T *ptr, int size)
{
    return {ptr, ptr + size};
}

// Constructs a sequence of characters from a std::basic_string
template <typename Ch> pointer_sequence<Ch> seq(const std::basic_string<Ch> &str)
{
    return {str.data(), str.data() + str.size()};
}

// Constructs a pointer_sequence from a vector
template <std::ranges::contiguous_range T> pointer_sequence<std::ranges::range_value_t<T>> seq(T &vec)
{
    return {std::ranges::data(vec), std::ranges::data(vec) + std::ranges::size(vec)};
}

// Constructs a sequence that stores the container
// This is potentially quite slow so be careful.
template <std::ranges::input_range T> sequences::stored_sequence<T> seq(T &&src)
{
    return {std::move(src)};
}

// Constructs a sequence from a pair of pointers
template <typename T> pointer_sequence<T> seq(const T *a, const T *b)
{
    return {a, b};
}

// Constructs a sequence from a pair of iterators
template <std::input_iterator It, std::input_iterator EndIt> auto seq(It a, EndIt b)
{
    return sequences::iterator_sequence{a, b};
}

// Constructs a sequence from a stream
template <typename T>
sequences::cached_iterator_sequence<std::istreambuf_iterator<T>, std::istreambuf_iterator<T>> seq(
    std::basic_istream<T> &is)
{
    return {{is}, {}};
}

// Constructs an output sequence from a function
template <typename Fn, typename T> sequences::function_inserter<T, Fn> receiver(Fn fn)
{
    return {fn};
}

// Constructs an output sequence from a function, deducing the element type
template <typename Fn, typename = decltype(&Fn::operator()),
          typename = typename sequences::helpers::deduce_param<Fn>::type>
sequences::function_inserter<typename sequences::helpers::deduce_param<Fn>::type, Fn> receiver(Fn fn)
{
    return {fn};
}

// Constructs an output sequence to insert data into a container
template <typename Container>
sequences::function_inserter<typename Container::value_type, sequences::detail::appender<Container>> writer(
    Container &c)
{
    return {sequences::detail::appender<Container>{c}};
}
} // namespace cutty