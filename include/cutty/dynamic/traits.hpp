#pragma once
#include "function.hpp"
#include "operators.hpp"

#include <cutty/print.hpp>

namespace cutty
{
std::ostream &operator<<(std::ostream &os, dynamic &x);
}

namespace cutty::dynamic_detail
{
template <typename T> const T &try_convert(const dynamic &x, const char *op);

template <std::size_t... Args, typename Fn> dynamic invoke(Fn &&fn, const dynamic *args, std::index_sequence<Args...>)
{
    return fn(args[Args]...);
}

} // namespace cutty::dynamic_detail

#define TRY_TO_RETURN(CMD, OP)                                                                                         \
    if constexpr (requires { CMD; })                                                                                   \
        return CMD;                                                                                                    \
    else                                                                                                               \
        throw_unsupported(OP, self);

template <typename Container>
concept indexed_container = requires(Container &c) {
    c[0];
    c.at(0);
};

template <typename Container>
concept keyed_container = requires(const Container &c) {
    typename Container::key_type;
    c.at(std::declval<typename Container::key_type>());
};

// Customize this class to implement bespoke behaviour for a type T.
template <typename T> class cutty::dynamic::default_traits
{
  public:
    using reference = std::remove_const_t<T> &;
    using const_reference = const T &;

    static std::string_view type_name()
    {
        return pretty_type<T>();
    }

    static void stream_to(const_reference self, std::ostream &os)
    {
        print_stream(os, self);
    }

    static const std::string &type_str()
    {
        static std::string str{type_name()};
        return str;
    }

    [[noreturn]]
    static void throw_unsupported(const char *op, const_reference self)
    {
        std::stringstream ss;
        ss << "Unsupported operation '" << op << "' on value of type " << pretty_type<T>();
        throw dynamic::unsupported(ss.str().c_str());
    }

    static bool as_bool(const_reference self)
    {
        TRY_TO_RETURN((bool)self, "bool");
    }

    static dynamic::int_type as_int(const_reference self)
    {
        TRY_TO_RETURN((dynamic::int_type)self, "as_int");
    }

    static double as_double(const_reference self)
    {
        TRY_TO_RETURN((double)self, "double");
    }

    static bool op_eq(const_reference x, const dynamic &y)
    {
        return x == y;
    }

    static std::optional<dynamic::int_type> try_get_integral(const_reference self)
    {
        if constexpr (std::integral<T> || std::floating_point<T>)
        {
            return {self};
        }
        return {};
    }

    static std::optional<double> try_get_double(const_reference self)
    {
        if constexpr (std::floating_point<T> || std::integral<T>)
        {
            return {self};
        }
        return {};
    }

    static std::optional<std::string_view> try_get_string(const_reference self)
    {
        if constexpr (dynamic_detail::string<T>)
        {
            return std::string_view(self);
        }
        return {};
    }

    static std::partial_ordering op_cmp(const_reference x, const dynamic &y)
    {
        return x <=> y;
    }

    static dynamic op_add(const_reference x, const dynamic &y)
    {
        return x + y;
    }

    static dynamic op_sub(const_reference x, const dynamic &y)
    {
        return x - y;
    }

    static dynamic op_mul(const_reference x, const dynamic &y)
    {
        return x * y;
    }

    static dynamic op_div(const_reference x, const dynamic &y)
    {
        return x / y;
    }
    static dynamic op_mod(const_reference x, const dynamic &y)
    {
        return x % y;
    }

    static std::size_t hash(const_reference self)
    {
        TRY_TO_RETURN(std::hash<T>()(self), "hash()");
    }

    static void push_back(reference self, const dynamic &y)
    {
        TRY_TO_RETURN(self.push_back(dynamic_detail::try_convert<typename T::value_type>(y, "push_back()")),
                      "push_back()");
    }

    static void pop_back(reference self)
    {
        TRY_TO_RETURN(self.pop_back(), "pop_back()");
    }

    static void push_front(reference self, const dynamic &value)
    {
        TRY_TO_RETURN(self.push_front(dynamic_detail::try_convert<typename T::value_type>(value, "push_front()")),
                      "push_front()");
    }

    static void pop_front(reference self)
    {
        TRY_TO_RETURN(self.pop_front(), "pop_front()");
    }

    static std::size_t size(const_reference self)
    {
        TRY_TO_RETURN(self.size(), "size()");
    }

    static bool empty(const_reference self)
    {
        TRY_TO_RETURN(self.empty(), "empty()");
    }

    static bool has_value(const_reference self)
    {
        return !std::is_same_v<T, dynamic::empty_type>;
    }

    static dynamic begin(const_reference self)
    {
        TRY_TO_RETURN(dynamic(self.begin()), "begin()");
    }

    static dynamic begin(reference self)
    {
        TRY_TO_RETURN(dynamic(self.begin()), "begin()");
    }

    static dynamic rbegin(const_reference self)
    {
        TRY_TO_RETURN(dynamic(self.rbegin()), "rbegin()");
    }

    static dynamic rbegin(reference self)
    {
        TRY_TO_RETURN(dynamic(self.rbegin()), "rbegin()");
    }

    static dynamic end(const_reference self)
    {
        TRY_TO_RETURN(dynamic(self.end()), "end()");
    }

    static dynamic end(reference self)
    {
        TRY_TO_RETURN(dynamic(self.end()), "end()");
    }

    static dynamic rend(const_reference self)
    {
        TRY_TO_RETURN(dynamic(self.rend()), "rend()");
    }

    static dynamic rend(reference self)
    {
        TRY_TO_RETURN(dynamic(self.rend()), "rend()");
    }

    static dynamic front(const_reference self)
    {
        TRY_TO_RETURN(dynamic(self.front(), dynamic::by_reference_tag()), "front()");
    }

    static dynamic front(reference self)
    {
        TRY_TO_RETURN(dynamic(self.front(), dynamic::by_reference_tag()), "front()");
    }

    static dynamic back(const_reference self)
    {
        TRY_TO_RETURN(dynamic(self.back(), dynamic::by_reference_tag()), "back()");
    }

    static dynamic back(reference self)
    {
        TRY_TO_RETURN(dynamic(self.back(), dynamic::by_reference_tag()), "back()");
    }

    static void insert(reference self, const dynamic &value)
    {
        TRY_TO_RETURN((void)self.insert(value), "insert()");
    }

    static void insert(reference self, const dynamic &k, const dynamic &v)
    {
        TRY_TO_RETURN((void)self.insert(std::make_pair(k, v)), "insert()");
    }

    static void erase(reference self, const dynamic &i)
    {
        TRY_TO_RETURN((void)self.erase(i.as<typename T::iterator>()), "erase()");
    }

    static void erase(reference self, const dynamic &i, const dynamic &j)
    {
        TRY_TO_RETURN((void)self.erase(i.as<typename T::iterator>(), j.as<typename T::iterator>()), "erase()");
    }

    static dynamic first(const_reference self)
    {
        TRY_TO_RETURN(dynamic(self.first, dynamic::by_reference_tag()), "first()");
    }

    static dynamic first(reference self)
    {
        TRY_TO_RETURN(dynamic(self.first, dynamic::by_reference_tag()), "first()");
    }

    static dynamic second(const_reference &self)
    {
        TRY_TO_RETURN(dynamic(self.second, dynamic::by_reference_tag()), "second()");
    }

    static dynamic second(reference self)
    {
        TRY_TO_RETURN(dynamic(self.second, dynamic::by_reference_tag()), "second()");
    }

    static dynamic op_star(reference self)
    {
        TRY_TO_RETURN(dynamic(*self, dynamic::by_reference_tag()), "*");
    }

    static dynamic op_star(const_reference self)
    {
        TRY_TO_RETURN(dynamic(*self, dynamic::by_reference_tag()), "*");
    }

    static void op_inc(reference self)
    {
        TRY_TO_RETURN((void)++self, "++");
    }

    static void op_dec(reference self)
    {
        TRY_TO_RETURN((void)--self, "--");
    }

    static dynamic op_minus(const_reference self)
    {
        TRY_TO_RETURN(dynamic(-self), "-");
    }

    static dynamic op_plus(const_reference self)
    {
        TRY_TO_RETURN(dynamic(+self), "+");
    }

    static dynamic call(const_reference self, std::size_t n_args, const dynamic *args)
    {
        if constexpr (requires { typename dynamic_detail::function_traits<T>::args_type; })
        {
            constexpr std::size_t args_size = std::tuple_size_v<typename dynamic_detail::function_traits<T>::args_type>;
            if (n_args == args_size)
            {
                return dynamic_detail::invoke(self, args, std::make_index_sequence<args_size>());
            }
            else
            {
                std::stringstream ss;
                ss << "Function called with wrong number of arguments, expected " << args_size << " but got " << n_args;
                throw dynamic::unsupported(ss.str().c_str());
            }
        }
        else
        {
            throw_unsupported("()", self);
        }
    }

    static dynamic op_index(reference self, dynamic::int_type i)
    {
        if constexpr (indexed_container<reference>)
        {
            if constexpr (requires { dynamic(self.at(i)); })
            {
                TRY_TO_RETURN(dynamic(self.at(i), dynamic::by_reference_tag{}), "[]");
            }
        }

        // Safe version first
        if constexpr (requires { dynamic(self.at(i)); })
        {
            TRY_TO_RETURN(dynamic(self.at(i), dynamic::by_reference_tag{}), "[]");
        }
        // Problem is that the dynamic constructor is explicit in this context, so we'll need
        // to convert it to dynamic first
        TRY_TO_RETURN(dynamic(self[dynamic(i)], dynamic::by_reference_tag{}), "[]");
    }

    static dynamic op_index(const_reference self, dynamic::int_type i)
    {
        // Use the safe version if possible
        if constexpr (requires { dynamic(self.at(i)); })
        {
            TRY_TO_RETURN(dynamic(self.at(i), dynamic::by_reference_tag{}), "[]");
        }
        if constexpr (requires { dynamic(self.at(dynamic(i))); })
        {
            TRY_TO_RETURN(dynamic(self.at(dynamic(i)), dynamic::by_reference_tag{}), "[]");
        }
        TRY_TO_RETURN(dynamic(self[(long)i], dynamic::by_reference_tag{}), "[]");
    }

    static dynamic op_index(reference self, const char *i)
    {
        if constexpr (requires { dynamic(self[i]); })
        {
            TRY_TO_RETURN(dynamic(self[i], dynamic::by_reference_tag{}), "[]");
        }
        TRY_TO_RETURN(dynamic(self[dynamic(i)], dynamic::by_reference_tag{}), "[]");
    }

    static dynamic op_index(const_reference self, const char *i)
    {
        if constexpr (requires { dynamic(self.at(i)); })
        {
            TRY_TO_RETURN(dynamic(self.at(i), dynamic::by_reference_tag{}), "[]");
        }
        TRY_TO_RETURN(dynamic(self.at[dynamic(i)], dynamic::by_reference_tag{}), "[]");
    }

    static dynamic op_index(const_reference self, const dynamic &i)
    {
        if constexpr (requires { self.at(i); })
        {
            TRY_TO_RETURN(dynamic(self.at(i), dynamic::by_reference_tag{}), "[]");
        }
        if constexpr (requires { self[i]; })
        {
            TRY_TO_RETURN(dynamic(self[i], dynamic::by_reference_tag{}), "[]");
        }
        if constexpr (keyed_container<std::remove_reference_t<reference>>)
        {
            auto key = dynamic_detail::try_convert<typename std::remove_reference_t<reference>::key_type>(i, "[]");
            return dynamic(self.at(key), dynamic::by_reference_tag{});
        }
        TRY_TO_RETURN(dynamic(self[dynamic(i)], dynamic::by_reference_tag{}), "[]");
    }

    static dynamic op_index(reference self, const dynamic &i)
    {
        if constexpr (indexed_container<reference>)
        {
            TRY_TO_RETURN(dynamic(self.at(i.as_int()), dynamic::by_reference_tag{}), "[]");
        }
        if constexpr (requires { self[i]; })
        {
            TRY_TO_RETURN(dynamic(self[i], dynamic::by_reference_tag{}), "[]");
        }
        if constexpr (keyed_container<std::remove_reference_t<reference>>)
        {
            // !! Currently not working
            auto key = dynamic_detail::try_convert<typename std::remove_reference_t<reference>::key_type>(i, "[]");
            return dynamic(self[key], dynamic::by_reference_tag{});
        }
        TRY_TO_RETURN(dynamic(self[dynamic(i)], dynamic::by_reference_tag{}), "[]");
    }
};

// Customize this class to implement bespoke behaviour for a type T.
template <typename T> class cutty::dynamic::traits : public default_traits<T>
{
};

#undef TRY_TO_RETURN
