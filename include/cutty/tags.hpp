#pragma once

#include "fraction.hpp"
#include "pretty_type.hpp"

#include <iostream>
#include <tuple>

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

///////////////////////////////////////////////////////////////////////////////
// class tagged

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

template <typename T> const char *tag_suffix = nullptr;

///////////////////////////////////////////////////////////////////////////////
// Tags

namespace tags
{
struct unit;

template <typename... Tags> struct product;

template <typename... Tags> struct sum;

template <typename Tag, fraction Power> struct power;

template <fraction F> struct scalar;
} // namespace tags

///////////////////////////////////////////////////////////////////////////////
// Simplify

template<typename Tag>
struct simplify
{
    using type = Tag;
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

template <> struct simplify<tags::product<tags::unit>> : simplify<tags::unit>
{
};


template<>
struct simplify<typename tags::product<>> : simplify<tags::unit>
{
};

template<typename... Tags1, typename...Tags2>
struct simplify<tags::product<tags::product<Tags1...>, Tags2...>> : simplify<tags::product<Tags1..., Tags2...>>
{
};

template<typename... Tags>
struct simplify<tags::product<tags::unit, Tags...>> : simplify<tags::product<Tags...>>
{
};

template<typename Tag>
struct simplify<tags::product<Tag, tags::unit>> : simplify<Tag>
{
};

template<>
struct simplify<tags::scalar<1>> : simplify<tags::unit>
{
};

template<typename Tag>
using simplify_t = typename simplify<Tag>::type;

///////////////////////////////////////////////////////////////////////////////
// Strip scalars

namespace detail
{

template <typename Tag> struct strip_scalars
{
    using type = simplify_t<Tag>;
    static constexpr fraction multiplier{1, 1};
};

template<typename Tag>
using strip_scalars_t = simplify_t<typename strip_scalars<simplify_t<Tag>>::type>;

template <fraction F, typename... Tags>
struct strip_scalars<tags::product<tags::scalar<F>, Tags...>> : strip_scalars<tags::product<Tags...>>
{
    static constexpr fraction multiplier = F * strip_scalars<tags::product<Tags...>>::multiplier;
};

template <typename Tag, typename... Tags>
struct strip_scalars<tags::product<Tag, Tags...>>
{
    using t1 = strip_scalars<Tag>;
    using t2 = strip_scalars<tags::product<Tags...>>;
    static constexpr fraction multiplier = t1::multiplier * t2::multiplier;
    using type = simplify_t<tags::product<typename t1::type, typename t2::type>>;
};


template <fraction P, typename Tag> struct strip_scalars<tags::power<Tag, P>>
{
    using t = strip_scalars<simplify_t<Tag>>;
    using type = tags::power<typename t::type, P>;
    static constexpr fraction multiplier = pow(t::multiplier, P);
};

}; // namespace detail

///////////////////////////////////////////////////////////////////////////////
// Tag traits

template <typename Tag> struct default_tag_traits
{
    using tag_type = Tag;
    using common_type = detail::strip_scalars_t<simplify_t<Tag>>;

    static void write_tag(std::ostream &os)
    {
        if (auto s = tag_suffix<tag_type>)
        {
            os << s;
        }
        else
        {
            os << pretty_type<Tag>();
        }
    }
};

template <typename Tag> struct tag_traits : public default_tag_traits<Tag>
{
};

template <typename Tag, typename... Tags>
struct tag_traits<tags::product<Tag, Tags...>> : public default_tag_traits<tags::product<Tag, Tags...>>
{
    static void write_tag(std::ostream &os)
    {
        if (auto s = tag_suffix<tags::product<Tag, Tags...>>)
        {
            os << s;
        }
        else
        {
            tag_traits<Tag>::write_tag(os);
            if constexpr (sizeof...(Tags)>0)
            {
                tag_traits<tags::product<Tags...>>::write_tag(os);
            }
        }
    }
};

template <fraction P, typename Tag>
struct tag_traits<tags::power<Tag, P>> : public default_tag_traits<tags::power<Tag, P>>
{
    static void write_tag(std::ostream &os)
    {
        if constexpr (P == -1)
        {
            os << "/";
            tag_traits<Tag>::write_tag(os);  // TODO: We need a way to write the denominator better.
        }
        else
        {
            tag_traits<Tag>::write_tag(os);
            os << "^" << P;
        }
    }
};

template <fraction P>
struct tag_traits<tags::scalar<P>> : public default_tag_traits<tags::scalar<P>>
{
    static void write_tag(std::ostream &os)
    {
        if(auto s = tag_suffix<tags::scalar<P>>)
        {
            os << s;
        }
        else
        {
            os << "*(" << P << ")";
        }
    }
};


///////////////////////////////////////////////////////////////////////////////
// Conversion helpers


template <typename Tag1, typename Tag2>
concept common_tags = std::same_as<typename tag_traits<Tag1>::common_type, typename tag_traits<Tag2>::common_type>;

template <typename Tag1, typename Tag2>
concept has_scalar_conversion =
    std::same_as<typename detail::strip_scalars<Tag1>::type, typename detail::strip_scalars<Tag2>::type>;

template <typename Tag1, typename Tag2>
concept has_scalar_common_conversion =
    common_tags<typename detail::strip_scalars<Tag1>::type, typename detail::strip_scalars<Tag2>::type>;

///////////////////////////////////////////////////////////////////////////////
// Conversion

/*
    Conversion proceeds as follows:
    1) Strip scalars
    2) Find a common type
    3) Convert both via common type
*/

namespace detail
{
    template<typename Tag>
    concept needs_scalar_conversion = !std::same_as<Tag, detail::strip_scalars_t<Tag>>;

    template<typename Tag>
    concept needs_common_conversion = !std::same_as<Tag, typename tag_traits<Tag>::common_type>;
}

template <typename T1, typename Tag1, typename T2, typename Tag2>
void convert(const tagged<T1, Tag1> &from, tagged<T2, Tag2> &to)
    requires(detail::needs_scalar_conversion<Tag1> || detail::needs_scalar_conversion<Tag2>)
{
    using S1 = detail::strip_scalars_t<Tag1>;
    using S2 = detail::strip_scalars_t<Tag2>;
    static_assert(!detail::needs_scalar_conversion<S1>, "strip_scalars should not need further scalar conversion");
    static_assert(!detail::needs_scalar_conversion<S2>, "strip_scalars should not need further scalar conversion");
    tagged<T1, S1> scale1{*from};
    tagged<T2, S2> scale2;
    convert(scale1, scale2);
    *to = *scale2 * (detail::strip_scalars<Tag1>::multiplier / detail::strip_scalars<Tag2>::multiplier);
}


template <typename T1, typename Tag1, typename T2, typename Tag2>
void convert(const tagged<T1, Tag1> &from, tagged<T2, Tag2> &to)
    requires ((detail::needs_common_conversion<Tag1> || detail::needs_common_conversion<Tag2>) && !detail::needs_scalar_conversion<Tag1> && !detail::needs_scalar_conversion<Tag2>)
{
    tagged<T1, typename tag_traits<Tag1>::common_type> i;
    convert(from, i);
    convert(i, to);
}

// template <typename T1, typename Tag1, typename T2, typename Tag2>
// void convert(const tagged<T1, Tag1> &from, tagged<T2, Tag2> &to)
//     requires (has_scalar_conversion<Tag1, Tag2> && !has_scalar_common_conversion<Tag1,Tag2>)
// {
//     // ?? A general functor library, for example
//     // functor::offset, functor::product, functor::inverse
//     // functor::apply
//     *to = *from * (detail::strip_scalars<Tag1>::multiplier / detail::strip_scalars<Tag2>::multiplier);
// }

// template<typename Tag>
// auto get_scale(const tagged<T, Tag> &src)
// {
//     using Tag2 = detail::strip_scalars<Tag>;
//     return std::pair { tagged<T, typename Tag2::type>(*src), 
// }

// template<typename Tag>
// concept needs_scalar_conversion = !std::same_as<Tag, detail::strip_scalars_t<Tag>>;

// template <typename T1, typename Tag1, typename T2, typename Tag2>
// void convert(const tagged<T1, Tag1> &from, tagged<T2, Tag2> &to)
//     requires has_scalar_common_conversion<Tag1, Tag2> && (
//         needs_scalar_conversion<Tag1> || needs_scalar_conversion<Tag2>)
// {
//     // ?? A general functor library, for example
//     // functor::offset, functor::product, functor::inverse
//     // functor::apply
//     *to = *from * (detail::strip_scalars<Tag1>::multiplier / detail::strip_scalars<Tag2>::multiplier);
// }


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


namespace detail
{


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

template <typename T1, typename Tag1, typename T2, typename Tag2>
auto operator<=>(const tagged<T1, Tag1> &lhs, const tagged<T2, Tag2> &rhs)
{
    return *lhs <=> *tag<Tag1>(rhs);
}

template <typename T1, typename Tag1, typename T2, typename Tag2>
bool operator==(const tagged<T1, Tag1> &lhs, const tagged<T2, Tag2> &rhs)
{
    return *lhs == *tag<Tag1>(rhs);
}

template <typename Tag, typename T, typename Tag2> tagged<T, Tag> delta(const tagged<T, Tag2> &src)
{
    return tagged<T, Tag>{*tag<Tag>(src) - *tag<Tag>(tag<Tag2>(0))};
}



template<typename Tag1, typename Tag2>
struct multiply_tags
{
    using type = tags::product<Tag1, Tag2>;
};

template<typename T1, typename Tag1, typename T2, typename Tag2>
auto operator*(const tagged<T1, Tag1> & lhs, const tagged<T2, Tag2> &rhs)
{
    return tagged<T1, typename multiply_tags<Tag1, Tag2>::type> { *lhs * *rhs };
}

template<typename T1, typename Tag1, typename T2, typename Tag2>
auto operator/(const tagged<T1, Tag1> & lhs, const tagged<T2, Tag2> &rhs)
{
    return tagged<T1, typename multiply_tags<Tag1, tags::power<Tag2, -1>>::type> { *lhs / *rhs };
}

namespace tags
{
    template<typename Tag1, typename Tag2>
    using multiply = typename multiply_tags<Tag1, Tag2>::type;

    template<typename Tag1, typename Tag2>
    using divide = multiply<Tag1, power<Tag2, -1>>;
}

} // namespace cutty
