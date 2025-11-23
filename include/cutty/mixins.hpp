/*
    Mixins allow you to declare CRTP hooks on any classes. Each hook is identified
    by an ID, and allows you to flexibly inject new methods into any class
    a bit like C# extension methods.
*/

#pragma once
#include <concepts>

namespace cutty
{
// The default (empty) implementation of all mixins
// Specialize this class to add more mixins.
template <typename T, typename Tag, int Sequence = 0> struct mixin
{
    struct last_mixin; // Sentinel to not look for further mixins
};

namespace detail
{
// std::void_t, but no need to #include <type_traits>
template <typename... T> using void_t = void;

// The implementation of each mixin
template <typename T, typename Tag, int Sequence = 0, typename = void>
struct implement_mixin : mixin<T, Tag, Sequence>, implement_mixin<T, Tag, Sequence + 1>
{
};

// If there's a stop member, don't recurse
template <typename T, typename Tag, int Sequence>
struct implement_mixin<T, Tag, Sequence, void_t<typename mixin<T, Tag, Sequence>::last_mixin>>
{
};

struct first_sequence
{
    static constexpr int sequence_number = -1;
};
} // namespace detail

// Extend this class to inject your mixins into your class T.
template <typename T, typename... Tags> struct implements : detail::implement_mixin<T, Tags>...
{
};

template <typename T, typename Tag>
concept has_mixin = std::derived_from<T, mixin<T, Tag>>;

template <typename Tag, typename Previous = detail::first_sequence> struct defines_mixin
{
    using tag = Tag;
    static constexpr int sequence_number = Previous::sequence_number + 1;
};

#define CY_REGISTER_MIXIN(C)                                                                                           \
    template <typename T> struct cutty::mixin<T, typename C<void>::tag, C<void>::sequence_number> : C<T>               \
    {                                                                                                                  \
    };

} // namespace cutty
