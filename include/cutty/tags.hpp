#pragma once

#include "fraction.hpp"

#include <iosfwd>

namespace cutty
{
template <typename T, typename Tag> class tagged;

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

template <typename T> const char *tag_suffix = "";

template <typename Tag> struct default_tag_traits
{
    using tag_type = Tag;
    using common_type = Tag;

    static void write_tag(std::ostream &os)
    {
        if (tag_suffix<tag_type>)
        {
            os << tag_suffix<tag_type>;
        }
    }
};

template <typename Tag> struct tag_traits : public default_tag_traits<Tag>
{
};

template <typename Tag1, typename Tag2>
concept common_tags = std::same_as<typename tag_traits<Tag1>::common_type, typename tag_traits<Tag2>::common_type>;

template <typename T1, typename Tag1, typename T2, typename Tag2>
void convert(const tagged<T1, Tag1> &from, tagged<T2, Tag2> &to)
    requires common_tags<Tag1, Tag2>
{
    tagged<T1, typename tag_traits<Tag1>::common_type> i;
    convert(from, i);
    convert(i, to);
}

template <typename T, typename Tag> std::ostream &operator<<(std::ostream &os, const tagged<T, Tag> &t)
{
    os << *t;
    tag_traits<Tag>::write_tag(os);
    return os;
}

template <typename Tag, typename T> tagged<T, Tag> tag(const T &v)
{
    return tagged<T, Tag>{v};
}

template <typename Tag> constexpr tagged<int, Tag> unit{1};

template <typename Tag, typename T, typename Tag2> tagged<T, Tag> tag(const tagged<T, Tag2> &src)
{
    return tagged<T, Tag>{src};
}

/// A list of builtin tags
namespace tags
{
struct unit;

template <typename... Tags> struct product;

template <typename... Tags> struct sum;

template <typename Tag, fraction Power> struct power;

template <fraction F> struct scalar;
} // namespace tags

namespace detail
{
template <typename Tag> struct simplify
{
    using type = Tag; // No simplification possible
};

template <typename Tag> struct simplify<tags::power<Tag, 0>> : simplify<tags::unit>
{
};

template <typename Tag> struct simplify<tags::power<Tag, 1>> : simplify<Tag>
{
};

template <typename Tag> struct simplify<tags::sum<Tag>> : simplify<Tag>
{
};

template <typename Tag> struct simplify<tags::product<Tag>> : simplify<Tag>
{
};

// template<typename Tag, typename...List>

// Indicates that a value in one tag should be converted to another tag
// possibly with a calculation.
template <typename From, typename To> struct converts : public std::false_type
{
};

// A tag can always be converted to itself
template <typename Tag> struct converts<Tag, Tag> : public std::true_type
{
};

// template<typename From, typename To>
// struct converts<

// !! requires no conversion
template <typename Tag1, typename Tag2> struct add_tags
{
    using type = tags::sum<Tag1, Tag2>;
};

template <typename From, typename To>
concept convertible = requires(const tagged<double, From> &from, tagged<double, To> &to) { convert(from, to); };

} // namespace detail

template <typename T, typename Tag> tagged<T, Tag> operator*(const tagged<T, Tag> &x, const T &y)
{
    return tagged<T, Tag>{*x * y};
}

template <typename T1, typename T2, typename Tag> bool operator==(const tagged<T1, Tag> &x, const tagged<T2, Tag> &y)
{
    return *x == *y;
}

template <typename T1, typename Tag1, typename T2, typename Tag2>
bool operator==(const tagged<T1, Tag1> &x, const tagged<T2, Tag2> &y)
{
    const tagged<T1, Tag1> y2 = y;
    return *x == *y2;
}

template <typename T1, typename Tag1, typename T2, typename Tag2>
tagged<T1, Tag1> operator-(const tagged<T1, Tag1> &x, const tagged<T2, Tag2> &y)
{
    const tagged<T1, Tag1> y2 = y;
    return tagged<T1, Tag1>{*x - *y2};
}

template <typename T1, typename Tag1, typename T2, typename Tag2>
tagged<T1, Tag1> operator+(const tagged<T1, Tag1> &x, const tagged<T2, Tag2> &y)
{
    const tagged<T1, Tag1> y2 = y;
    return tagged<T1, Tag1>{*x + *y2};
}

template <typename T, typename Tag> auto operator<=>(const tagged<T, Tag> &x, const tagged<T, Tag> &y)
{
    return *x <=> *y;
}

template <typename T, typename Tag> tagged<T, Tag> operator*(const T &x, const tagged<T, Tag> &y)
{
    return tagged<T, Tag>{x * *y};
}

template<typename Tag, typename T, typename Tag2>
tagged<T, Tag> delta(const tagged<T, Tag2> &src)
{
    return tagged<T, Tag> {*tag<Tag>(src) - *tag<Tag>(tag<Tag2>(0))};   
}

} // namespace cutty
