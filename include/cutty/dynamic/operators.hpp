#pragma once
#include <cutty/pretty_type.hpp>

#include <sstream>
#include <string>

namespace cutty::dynamic_detail
{
    template<typename T>
    concept string = requires(T x) { std::string_view(x); };

    template<typename T>
    concept supports_add = requires(T x) { x+x; };

    template<typename T>
    concept supports_sub = requires(T x) { x-x; };

    template<typename T>
    concept supports_mul = requires(T x) { x*x; };

    template<typename T>
    concept supports_div = requires(T x) { x/x; };

    template<typename T>
    concept supports_mod = requires(T x) { x%x; };


    template <typename T> constexpr auto pretty_type()
    {
        std::string_view s(__PRETTY_FUNCTION__);
#if defined(__clang__)
        s.remove_prefix(40);
#elif defined (__GNUC__)
        s.remove_prefix(55);
#else
#error "Unhandled compiler (add it here)"
#endif
        
        s.remove_suffix(1);
        return s;
    }

    #define BINOP(Name, Op) \
        template<typename T>  \
        struct Name  \
        {  \
            using return_type = bool; \
            bool operator()(const T &x, const T&y) const requires requires { x Op y; } { return x Op y; }  \
        };

    BINOP(less, <)

    template<typename T, template<typename> typename Operator>
    struct operator_impl
    {
        using op = Operator<T>;
        static constexpr bool supported = requires(T t, op o) { { o(t, t) }; };

        typename op::return_type operator()(const T &x, const dynamic &y, const char * msg) const
        {
            if constexpr (supported)
            {
                if(const auto *p = y.try_get<T>())
                {
                    return op()(x, *p);
                }
                else
                {
                    std::stringstream ss;
                    ss << "Operator " << msg << " could not convert " << y << " to " << pretty_type<T>();
                    throw dynamic::unsupported(ss.str().c_str());
                }
            }
            else
            {
                std::stringstream ss;
                ss << "Operator " << msg << " is not supported on " << pretty_type<T>() << " and " << y;
                throw dynamic::unsupported(ss.str().c_str());
            }
        }
    };

    template<typename T>
    [[noreturn]]
    void throw_unsupported(const T &, const dynamic &y, const char *op)
    {
        std::stringstream ss;
        ss << "Operator " << op << " not supported on values of type " << pretty_type<T>() << " and " << y;
        throw dynamic::incompatible(ss.str().c_str());
    }

    template<typename T>
    [[noreturn]]
    void throw_incompatible(const T &, const dynamic &y, const char *op)
    {
        std::stringstream ss;
        ss << "Incompatible types on " << op << " for " << pretty_type<T>() << " and " << y;
        throw dynamic::incompatible(ss.str().c_str());
    }

}

template<typename T>
cutty::dynamic operator+(const T&x, const cutty::dynamic &y)
{
    if constexpr (std::floating_point<T> || std::integral<T>)
    {
        if(auto d = y.m_type->try_get_double(y))
        {
            return cutty::dynamic(x+*d);
        }
        else if(auto i = y.m_type->try_get_integral(y))
        {
            return cutty::dynamic(x + *i);
        }
    }
    else if constexpr (cutty::dynamic_detail::string<T>)
    {
        if(auto s = y.m_type->try_get_string(y))
        {
            return cutty::dynamic(std::string(x)+std::string(*s));
        }
    }
    else if constexpr (cutty::dynamic_detail::supports_add<T>)
    {
        if(auto p = y.try_get<T>())
        {
            return cutty::dynamic(x + *p);
        }
    }
    cutty::dynamic_detail::throw_unsupported(x, y, "+");
}

template<typename T>
cutty::dynamic operator-(const T&x, const cutty::dynamic &y)
{
    if constexpr (std::floating_point<T> || std::integral<T>)
    {
        if(auto d = y.m_type->try_get_double(y))
        {
            return cutty::dynamic(x-*d);
        }
        else if(auto i = y.m_type->try_get_integral(y))
        {
            return cutty::dynamic(x - *i);
        }
    }
    else if constexpr (cutty::dynamic_detail::supports_sub<T>)
    {
        if(auto p = y.try_get<T>())
        {
            return cutty::dynamic(x - *p);
        }
    }
    cutty::dynamic_detail::throw_unsupported(x, y, "-");
}


template<typename T>
cutty::dynamic operator*(const T&x, const cutty::dynamic &y)
{
    if constexpr (std::floating_point<T> || std::integral<T>)
    {
        if(auto d = y.m_type->try_get_double(y))
        {
            return cutty::dynamic(x* *d);
        }
        else if(auto i = y.m_type->try_get_integral(y))
        {
            return cutty::dynamic(x * *i);
        }
    }
    else if constexpr (cutty::dynamic_detail::supports_mul<T>)
    {
        if(auto p = y.try_get<T>())
        {
            return cutty::dynamic(x * *p);
        }
    }
    cutty::dynamic_detail::throw_unsupported(x, y, "*");
}

template<typename T>
cutty::dynamic operator/(const T&x, const cutty::dynamic &y)
{
    if constexpr (std::floating_point<T> || std::integral<T>)
    {
        if(auto d = y.m_type->try_get_double(y))
        {
            return cutty::dynamic(x* *d);
        }
        else if(auto i = y.m_type->try_get_integral(y))
        {
            return cutty::dynamic(x / *i);
        }
    }
    else if constexpr (cutty::dynamic_detail::supports_mul<T>)
    {
        if(auto p = y.try_get<T>())
        {
            return cutty::dynamic(x / *p);
        }
    }
    cutty::dynamic_detail::throw_unsupported(x, y, "/");
}


template<typename T>
cutty::dynamic operator%(const T&x, const cutty::dynamic &y)
{
    if constexpr (std::integral<T>)
    {
        if (auto i = y.m_type->try_get_integral(y))
        {
            return cutty::dynamic(x % *i);
        }
        cutty::dynamic_detail::throw_incompatible(x, y, "%");
    } else if constexpr (cutty::dynamic_detail::supports_mod<T>)
    {
        if(auto p = y.try_get<T>())
        {
            return cutty::dynamic(x % *p);
        }
    }

    cutty::dynamic_detail::throw_unsupported(x, y, "%");
}

template<typename T>
bool operator<(const T&x, const cutty::dynamic &y)
{
    if constexpr (std::integral<T> || std::floating_point<T>)
    {
        if (auto d = y.m_type->try_get_double(y))
        {
            return x < *d;
        }
        else
        {
            return true;
        }
    }
    else if constexpr (cutty::dynamic_detail::string<T>)
    {
        if (auto s = y.m_type->try_get_string(y))
        {
            return x < *s;
        }
        else if (y.m_type->try_get_double(y))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        // Look at the type-ids (TODO)
        return cutty::dynamic_detail::operator_impl<T, cutty::dynamic_detail::less>()(x, y, "<");
    }
}

template<typename T>
bool operator==(const T&x, const cutty::dynamic &y)
{
    if constexpr (cutty::dynamic_detail::string<T>)
    {
        if (auto r = y.m_type->try_get_string(y))
        {
            return x == *r;
        }
        return false;
    }

    if constexpr (std::integral<T>)
    {
        if (auto r = y.m_type->try_get_integral(y))
        {
            return x == *r;
        }
        return false;
    }

    if constexpr (std::floating_point<T>)
    {
        if (auto r = y.m_type->try_get_double(y))
        {
            return x == *r;
        }
        return false;
    }

    if constexpr (requires(T t) { t == t; })
    {
        if (auto *p = y.try_get<T>())
        {
            return x == *p;
        }
    }

    return false;
}
