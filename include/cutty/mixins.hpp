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

template <typename T, typename Tag, int Index = 0, typename = void>
struct count_mixins : count_mixins<T, Tag, Index + 1>
{
};

template <typename T, typename Tag, int Index>
struct count_mixins<T, Tag, Index, void_t<typename mixin<T, Tag, Index>::last_mixin>>
{
    static const int value = Index;
};

// Utility to identify the next available sequence number
template <typename Tag, typename T> constexpr int next = detail::count_mixins<T, Tag>::value;

} // namespace detail

// Extend this class to inject your mixins into your class T.
template <typename T, typename... Tags> struct implements : detail::implement_mixin<T, Tags>...
{
};

template <typename T, typename Tag>
concept has_mixin = std::derived_from<T, mixin<T, Tag>>;
} // namespace mixins

// This is a subtle hack. DECLARE_MIXIN create a new partial template specialisation, but the problem
// is, what sequence number do we use? The hack is that ::cutty::detail::next counts the existing
// specializations, and then we use the result to add a new specialization. By passing a new parameter
// Impl<Tag>, we ensure that this gets recomputed each time.
#define DECLARE_MIXIN(Tag, Impl)                                                                                       \
    template <typename T> struct ::cutty::mixin<T, Tag, ::cutty::detail::next<Tag, Impl<Tag>>> : Impl<T>             \
    {                                                                                                                  \
    };

#define EXTEND_MIXIN(Tag, Base)                                                                                        \
    template <typename T>                                                                                              \
    struct ::cutty::mixin<T, Tag, ::cutty::detail::next<Tag, Base>> : ::cutty::implements<T, Base>                    \
    {                                                                                                                  \
    };

#define FUNCTION_MIXIN(Tag, Name)                                                                                      \
    template <typename T> struct ::cutty::mixin<T, Tag, ::cutty::detail::next<Tag, struct Name##_mixin>>             \
    {                                                                                                                  \
        template <typename... Args> decltype(auto) Name(Args &&...args)                                                \
        {                                                                                                              \
            return ::Name(*static_cast<T *>(this), std::forward<Args>(args)...);                                       \
        }                                                                                                              \
        template <typename... Args> decltype(auto) Name(Args &&...args) const                                          \
        {                                                                                                              \
            return ::Name(*static_cast<const T *>(this), std::forward<Args>(args)...);                                 \
        }                                                                                                              \
    };
