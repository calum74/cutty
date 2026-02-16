#pragma once

// #include <cstddef>
#include <ostream>

namespace cutty
{
/**
    A constexpr string for template arguments.
*/
template <std::size_t N> struct fixed_string
{
    using size_type = std::size_t;
    using value_type = char;

    value_type value[N];

    constexpr fixed_string(const char (&str)[N])
    {
        for (size_t i = 0; i < N; ++i)
            value[i] = str[i];
    }

    template<size_type M>
    constexpr fixed_string(fixed_string<M> str, size_type offset)
    {
        for (size_t i = 0; i < N - 1; ++i)
            value[i] = str[i+offset];
        value[N - 1] = 0;
    }

    constexpr char operator[](size_t i) const
    {
        return value[i];
    }

    constexpr const char *c_str() const
    {
        return value;
    }

    constexpr size_type size() const
    {
        return N - 1;
    }

    constexpr int index_of(char ch) const
    {
        for (int i = 0; i < N; ++i)
        {
            if (value[i] == ch)
                return i;
        }
        return -1;
    }

    template <size_type M> consteval int index_of(fixed_string<M> str, size_type start = 0) const
    {
        for (int i = start; i < N; ++i)
        {
            int j;
            for (j = 0; j < M - 1; ++j)
            {
                if (value[i + j] != str[j])
                    break;
            }
            if (j == M - 1)
                return i;
        }
        return -1;
    }

    template <size_type N2> constexpr bool operator==(fixed_string<N2> rhs) const
    {
        if (size() != rhs.size())
            return false;

        for (size_t i = 0; i < N; ++i)
            if (value[i] != rhs[i])
                return false;
        return true;
    }

    template<size_type M>
    constexpr bool operator==(const char (&rhs)[M]) const
    {
        return *this == fixed_string<M>(rhs);
    }
};

template <std::size_t N> std::ostream &operator<<(std::ostream &os, fixed_string<N> str)
{
    return os << str.c_str();
}

template <fixed_string Haystack, fixed_string Needle, size_t Index = 0> constexpr int fixed_string_index_of = Haystack.index_of(Needle, Index);

template <fixed_string Haystack, fixed_string Needle, size_t Offset>
constexpr bool fixed_string_contains = fixed_string_index_of<Haystack, Needle, Offset> >= 0;

template<fixed_string str, size_t from, size_t to> constexpr auto fixed_string_substr =
    fixed_string<to-from+1>{str, from};

template <fixed_string Haystack, fixed_string Needle, size_t Index = 0>
    requires(fixed_string_contains<Haystack, Needle, Index>)
constexpr auto fixed_string_before =
    fixed_string<Haystack.index_of(Needle, Index) + 1>(Haystack, 0);

template <fixed_string Haystack, fixed_string Needle, size_t Index = 0>
    requires(fixed_string_contains<Haystack, Needle, Index>)
constexpr fixed_string<Haystack.size() - Haystack.index_of(Needle, Index) + 1 - Needle.size()> fixed_string_after =
    fixed_string<Haystack.size() - Haystack.index_of(Needle, Index) + 1 - Needle.size()>(
        Haystack, Haystack.index_of(Needle, Index) + Needle.size());

namespace detail
{
template <fixed_string Haystack, fixed_string Needle, size_t Index> constexpr size_t count_matches()
{
    if constexpr (Index + Needle.size() > Haystack.size())
    {
        return 0;
    }
    else if constexpr (fixed_string_contains<Haystack, Needle, Index>)
    {
        return 1 + count_matches<Haystack, Needle, Haystack.index_of(Needle, Index) + 1>();
    }
    else
    {
        return 0;
    }
}
} // namespace detail

template <fixed_string Haystack, fixed_string Needle>
constexpr size_t fixed_string_count = detail::count_matches<Haystack, Needle, 0>();

// Split a fixed string

namespace detail
{
/*
    Gets the ith substring
 */
template <fixed_string Str, fixed_string Separator, size_t Item, size_t Index> constexpr auto fixed_split()
{
    if constexpr (Item == 0)
    {
        if constexpr (fixed_string_contains<Str, Separator, Index>)
        {
            return fixed_string_substr<Str, Index, fixed_string_index_of<Str, Separator, Index>>;
        }
        else
        {
            return fixed_string_substr<Str, Index, Str.size()>;
        }
    }
    else
    {
        return fixed_split<Str, Separator, Item - 1, Str.index_of(Separator, Index) + Separator.size()>();
    }
}
} // namespace detail

template <fixed_string Str, fixed_string Separator, size_t Item>
constexpr auto fixed_string_split = detail::fixed_split<Str, Separator, Item, 0>();
} // namespace cutty
