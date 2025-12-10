#pragma once

#include "fraction.hpp"
#include "pretty_type.hpp"

#include <iostream>
#include <tuple>

namespace cutty
{
template <typename V, typename T> class tagged;

template <typename V, typename T> void initialise(tagged<V, T> &v)
{
    *v = V();
}

template <typename V1, typename V2, typename T>
void convert(const tagged<V1, T> &src, tagged<V2, T> &dest)
{
    *dest = *src;
}

///////////////////////////////////////////////////////////////////////////////
// class tagged

template <typename V, typename T> class tagged
{
  public:
    using value_type = V;
    using tag_type = T;

    tagged() // !! Concept requires has_default_value<Tag,T>
    {
        initialise(*this);
    }

    template <typename V2, typename T2> tagged(const tagged<V2, T2> &src)
    {
        convert(src, *this);
    }

    explicit constexpr tagged(const V &v) : value(v)
    {
    }

    const value_type &operator*() const
    {
        return value;
    }

    value_type &operator*()
    {
        return value;
    }

    template <typename V2, typename T2> tagged &operator=(const tagged<V2, T2> &src)
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

template <typename... Ts> struct product;

template <typename... Ts> struct sum;

template <typename T, fraction P> struct power;

template <fraction F> struct scalar;
} // namespace tags

///////////////////////////////////////////////////////////////////////////////
// Simplify

template<typename T>
struct simplify
{
    using type = T;
};

template <typename T> struct simplify<tags::power<T, 0>> : simplify<tags::unit>
{
};

template <typename T> struct simplify<tags::power<T, 1>> : simplify<T>
{
};

template <typename T> struct simplify<tags::sum<T>> : simplify<T>
{
};

template <typename T> struct simplify<tags::product<T>> : simplify<T>
{
};

template <> struct simplify<tags::product<tags::unit>> : simplify<tags::unit>
{
};


template<>
struct simplify<typename tags::product<>> : simplify<tags::unit>
{
};

template<typename... Ts1, typename...Ts2>
struct simplify<tags::product<tags::product<Ts1...>, Ts2...>> : simplify<tags::product<Ts1..., Ts2...>>
{
};

template<typename... Ts>
struct simplify<tags::product<tags::unit, Ts...>> : simplify<tags::product<Ts...>>
{
};

template<typename T>
struct simplify<tags::product<T, tags::unit>> : simplify<T>
{
};

template<>
struct simplify<tags::scalar<1>> : simplify<tags::unit>
{
};

template<typename T>
using simplify_t = typename simplify<T>::type;

///////////////////////////////////////////////////////////////////////////////
// Strip scalars

namespace detail
{

template <typename T> struct strip_scalars
{
    using type = simplify_t<T>;
    static constexpr fraction multiplier{1, 1};
};

template<typename T>
using strip_scalars_t = simplify_t<typename strip_scalars<simplify_t<T>>::type>;

template <fraction F, typename... Ts>
struct strip_scalars<tags::product<tags::scalar<F>, Ts...>> : strip_scalars<tags::product<Ts...>>
{
    static constexpr fraction multiplier = F * strip_scalars<tags::product<Ts...>>::multiplier;
};

template <typename T, typename... Ts>
struct strip_scalars<tags::product<T, Ts...>>
{
    using t1 = strip_scalars<T>;
    using t2 = strip_scalars<tags::product<Ts...>>;
    static constexpr fraction multiplier = t1::multiplier * t2::multiplier;
    using type = simplify_t<tags::product<typename t1::type, typename t2::type>>;
};


template <fraction P, typename T> struct strip_scalars<tags::power<T, P>>
{
    using t = strip_scalars<simplify_t<T>>;
    using type = tags::power<typename t::type, P>;
    static constexpr fraction multiplier = pow(t::multiplier, P);
};

}; // namespace detail

///////////////////////////////////////////////////////////////////////////////
// Tag traits

template <typename T> struct default_tag_traits
{
    using tag_type = T;
    using common_type = detail::strip_scalars_t<T>;

    static void write_tag(std::ostream &os)
    {
        if (auto s = tag_suffix<tag_type>)
        {
            os << s;
        }
        else
        {
            os << pretty_type<T>();
        }
    }
};

template <typename T> struct tag_traits : public default_tag_traits<T>
{
};

template <typename T, typename... Ts>
struct tag_traits<tags::product<T, Ts...>> : public default_tag_traits<tags::product<T, Ts...>>
{
    static void write_tag(std::ostream &os)
    {
        if (auto s = tag_suffix<tags::product<T, Ts...>>)
        {
            os << s;
        }
        else
        {
            tag_traits<T>::write_tag(os);
            if constexpr (sizeof...(Ts)>0)
            {
                tag_traits<tags::product<Ts...>>::write_tag(os);
            }
        }
    }
};

template <fraction P, typename T>
struct tag_traits<tags::power<T, P>> : public default_tag_traits<tags::power<T, P>>
{
    static void write_tag(std::ostream &os)
    {
        if constexpr (P == -1)
        {
            os << "/";
            tag_traits<T>::write_tag(os);  // TODO: We need a way to write the denominator better.
        }
        else
        {
            tag_traits<T>::write_tag(os);
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


template <typename T1, typename T2>
concept common_tags = std::same_as<typename tag_traits<T1>::common_type, typename tag_traits<T2>::common_type>;

template <typename T1, typename T2>
concept has_scalar_conversion =
    std::same_as<typename detail::strip_scalars<T1>::type, typename detail::strip_scalars<T2>::type>;

template <typename T1, typename T2>
concept has_scalar_common_conversion =
    common_tags<typename detail::strip_scalars<T1>::type, typename detail::strip_scalars<T2>::type>;

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
    template<typename T>
    concept needs_scalar_conversion = !std::same_as<T, detail::strip_scalars_t<T>>;

    template<typename T>
    concept needs_common_conversion = !std::same_as<T, typename tag_traits<T>::common_type>;
}

template <typename V1, typename T1, typename V2, typename T2>
void convert(const tagged<V1, T1> &from, tagged<V2, T2> &to)
    requires(detail::needs_scalar_conversion<T1> || detail::needs_scalar_conversion<T2>)
{
    using S1 = detail::strip_scalars_t<T1>;
    using S2 = detail::strip_scalars_t<T2>;
    static_assert(!detail::needs_scalar_conversion<S1>, "strip_scalars should not need further scalar conversion");
    static_assert(!detail::needs_scalar_conversion<S2>, "strip_scalars should not need further scalar conversion");
    tagged<V1, S1> scale1{*from};
    tagged<V2, S2> scale2;
    convert(scale1, scale2);
    *to = *scale2 * (detail::strip_scalars<T1>::multiplier / detail::strip_scalars<T2>::multiplier);
}


template <typename V1, typename T1, typename V2, typename T2>
void convert(const tagged<V1, T1> &from, tagged<V2, T2> &to)
    requires ((detail::needs_common_conversion<T1> || detail::needs_common_conversion<T2>) && !detail::needs_scalar_conversion<T1> && !detail::needs_scalar_conversion<T2>)
{
    tagged<V1, typename tag_traits<T1>::common_type> i;
    convert(from, i);
    convert(i, to);
}

///////////////////////////////////////////////////////////////////////////////
// Functions

template <typename V, typename T> std::ostream &operator<<(std::ostream &os, const tagged<V, T> &t)
{
    os << *t;
    tag_traits<T>::write_tag(os);
    return os;
}

template <typename T, typename V> tagged<V, T> tag(const V &v)
{
    return tagged<V, T>{v};
}

template <typename T> constexpr tagged<int, T> unit{1};

template <typename T, typename V, typename T2> tagged<V, T> tag(const tagged<V, T2> &src)
{
    return tagged<V, T>{src};
}

template <typename T, typename V, typename T2> tagged<V, T> delta(const tagged<V, T2> &src)
{
    return tagged<V, T>{*tag<T>(src) - *tag<T>(tag<T2>(0))};
}

///////////////////////////////////////////////////////////////////////////////
// Operators

template <typename V1, typename V2, typename T> bool operator==(const tagged<V1, T> &x, const tagged<V2, T> &y)
{
    return *x == *y;
}

template <typename V1, typename T1, typename V2, typename T2>
tagged<V1, T1> operator-(const tagged<V1, T1> &x, const tagged<V2, T2> &y)
{
    const tagged<V1, T1> y2 = y;
    return tagged<V1, T1>{*x - *y2};
}

template <typename V1, typename T1, typename V2, typename T2>
tagged<V1, T1> operator+(const tagged<V1, T1> &x, const tagged<V2, T2> &y)
{
    const tagged<V1, T1> y2 = y;
    return tagged<V1, T1>{*x + *y2};
}

template <typename V1, typename T1, typename V2, typename T2>
auto operator<=>(const tagged<V1, T1> &lhs, const tagged<V2, T2> &rhs)
{
    return *lhs <=> *tag<T1>(rhs);
}

template <typename V1, typename T1, typename V2, typename T2>
bool operator==(const tagged<V1, T1> &lhs, const tagged<V2, T2> &rhs)
{
    return *lhs == *tag<T1>(rhs);
}

///////////////////////////////////////////////////////////////////////////////
// Multiplication

template <typename V, typename T> tagged<V, T> operator*(const V &x, const tagged<V, T> &y)
{
    return tagged<V, T>{x * *y};
}

template <typename V, typename T> tagged<V, T> operator*(const tagged<V, T> &x, const V &y)
{
    return tagged<V, T>{*x * y};
}

namespace detail
{
template<typename T1, typename T2>
struct multiply_tags
{
    using type = tags::product<T1, T2>;
};
}

template<typename V1, typename T1, typename V2, typename T2>
auto operator*(const tagged<V1, T1> & lhs, const tagged<V2, T2> &rhs)
{
    return tagged<V1, typename detail::multiply_tags<T1, T2>::type> { *lhs * *rhs };
}

template<typename V1, typename T1, typename V2, typename T2>
auto operator/(const tagged<V1, T1> & lhs, const tagged<V2, T2> &rhs)
{
    return tagged<V1, typename detail::multiply_tags<T1, tags::power<T2, -1>>::type> { *lhs / *rhs };
}

namespace tags
{
    template<typename T1, typename T2>
    using multiply = typename detail::multiply_tags<T1, T2>::type;

    template<typename T1, typename T2>
    using divide = multiply<T1, power<T2, -1>>;
}

///////////////////////////////////////////////////////////////////////////////
// Unknown junk

namespace detail
{
template <typename From, typename To>
concept convertible = requires(const tagged<double, From> &from, tagged<double, To> &to) { convert(from, to); };
} // namespace detail


} // namespace cutty
