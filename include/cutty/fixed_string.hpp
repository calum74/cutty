#pragma once

namespace cutty
{
    /**
        A constexpr string that is mainly for template arguments.
    */
template <size_t N> struct fixed_string
{
    char value[N];

    constexpr fixed_string(const char (&str)[N])
    {
        for (size_t i = 0; i < N; ++i)
            value[i] = str[i];
    }

    constexpr char operator[](size_t i) const
    {
        return value[i];
    }

    constexpr size_t size() const
    {
        return N - 1;
    }
};
} // namespace cutty
