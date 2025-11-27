#pragma once

#include "fraction.hpp"

// ?? Forward only
#include <iostream>

namespace cutty
{
template <typename T, typename Tag> class tagged;

template <typename... Tags> struct product_tag;

template <typename Tag, fraction power> struct power_tag;

template <typename T, typename Tag> void initialise(tagged<T, Tag> &v)
{
    *v = T();
}

template <typename T1, typename T2, typename Tag> void convert(const tagged<T1, Tag> &src, tagged<T2, Tag> &dest)
{
    *dest = *src;
}

template <typename T1, typename T2, typename Tag> auto add(const tagged<T1, Tag> &lhs, const tagged<T2, Tag> &rhs)
{
    return tag<Tag>(*lhs * *rhs);
}

// Naive and wrong
template <typename T1, typename T2, typename Tag1, typename Tag2>
auto add(const tagged<T1, Tag1> &lhs, const tagged<T2, Tag2> &rhs)
{
    return tag<std::tuple<Tag1, Tag2>>(std::tuple{*lhs, *rhs});
}

template <typename T>
concept multiplicative = requires(T t) { t * t; };

template <typename T>
concept additive = requires(T t) { t + t; };

template <typename T1, multiplicative T2, typename Tag> auto mul(const tagged<T1, Tag> &lhs, const T2 &rhs)
{
    return tag<Tag>(*lhs * rhs);
}

template <multiplicative T1, typename T2, typename Tag> auto mul(const T1 &lhs, const tagged<T2, Tag> &rhs)
{
    return tag<Tag>(lhs * *rhs);
}

template <typename T, typename Tag> class tagged
{
  public:
    using value_type = T;
    using tag_type = Tag;

    tagged() // !! Concept requires has_default_value<Tag,T>
    {
        initialise(*this);
    }

    template <typename T2, typename Tag2> tagged(const tagged<T2, Tag2> &src)
    {
        convert(src, *this);
    }

    explicit constexpr tagged(const T &v) : value(v)
    {
    }

    // explicit tagged(value_type v) : value(std::move(v)) {}

    const value_type &operator*() const
    {
        return value;
    }

    value_type &operator*()
    {
        return value;
    }

    template <typename T2, typename Tag2> tagged &operator=(const tagged<T2, Tag2> &src)
    {
        convert(src, *this);
        return *this;
    }

  private:
    value_type value;
};

template <typename T, typename Tag> const char *tag_suffix(tagged<T, Tag>)
{
    return "";
}

template <typename T, typename Tag> const char *tag_prefix(tagged<T, Tag>)
{
    return "";
}

template <typename T, typename Tag> std::ostream &operator<<(std::ostream &os, const tagged<T, Tag> &t)
{
    return os << tag_prefix(t) << *t << tag_suffix(t);
}

template <typename Tag, typename T> tagged<T, Tag> tag(const T &v)
{
    return tagged<T, Tag>{v};
}

template <typename Tag> constexpr tagged<int, Tag> unit{1};

template <typename Tag, typename T, typename Tag2> tagged<T, Tag> tag(const tagged<T, Tag2> &src)
{
    return tagged<T, Tag>{*src};
}
} // namespace cutty
