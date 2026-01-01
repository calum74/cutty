#pragma once
#include "function.hpp"
#include "get_type.hpp"
#include "operators.hpp"
#include "reference.hpp"
#include "traits.hpp"
#include <atomic>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#define TRY_TO(CMD, OP)                                                                                                \
    if constexpr (requires { CMD; })                                                                                   \
    {                                                                                                                  \
        CMD;                                                                                                           \
    }                                                                                                                  \
    else                                                                                                               \
        throw_unsupported(OP, x);

namespace cutty::dynamic_detail
{

template <typename T> const T &try_convert(const dynamic &x, const char *op)
{
    auto p = x.try_get<T>();
    if (!p)
    {
        std::stringstream ss;
        ss << "Failed to convert " << x << " to type " << pretty_type<T>() << " for '" << op << "'";
        throw dynamic::incompatible(ss.str().c_str());
    }
    return *(const T *)p;
}

template <typename T> T &try_convert(dynamic &x, const char *op)
{
    auto p = x.try_get<T>();
    if (!p)
    {
        std::stringstream ss;
        ss << "Failed to convert " << x << " to type " << pretty_type<T>() << " for '" << op << "'";
        throw dynamic::incompatible(ss.str().c_str());
    }
    return *(T *)p;
}

template <> const dynamic &try_convert<dynamic>(const dynamic &x, const char *op);

template <typename Fn> struct fn_args;

template <typename R, typename C, typename... Args> struct fn_args<R (C::*)(Args...)>
{
    using type = std::tuple<Args...>;

    template <int N> static auto &get_arg(const char *method_name, dynamic &x)
    {
        auto *p = x.try_get<std::tuple_element_t<N, type>>(x);
        if (!p)
        {
            std::stringstream ss;
            ss << "Invalid object of type " << x.type_str() << " passed to method '" << method_name << "' taking type "
               << pretty_type<std::tuple_element_t<N, type>>();
            throw dynamic::incompatible(ss.str().c_str());
        };
        return *p;
    }
};

// Specialization for const member functions
template <typename R, typename C, typename... Args> struct fn_args<R (C::*)(Args...) const>
{
    using type = std::tuple<Args...>;
};

template <int N, typename Fn> using fn_arg = typename std::tuple_element_t<N, typename fn_args<Fn>::type>;

template <typename U, typename Mode> class type_impl : public dynamic::type
{
    using T = std::remove_reference_t<U>;
    using mutable_t = std::remove_const_t<T>;
    using traits_type = dynamic::traits<T>;

    static constexpr bool is_reference = std::is_reference_v<U>;
    static constexpr bool is_const = std::is_const_v<T>;
    static constexpr bool is_weak_shared = std::is_same_v<Mode, dynamic::weak_reference_tag>;
    static constexpr bool is_strong_shared = std::is_same_v<Mode, dynamic::shared_tag>;
    static constexpr bool is_small = sizeof(T) <= sizeof(void *);

    struct shared_impl
    {
        shared_impl(const T &src)
        {
            new (m_data) T(src);
        }
        shared_impl(T &&src)
        {
            new (m_data) T(std::move(src));
        }

        T &get()
        {
            return *(T *)m_data;
        }

        std::atomic<size_t> m_weak_count = 1, m_strong_count = 1;

        alignas(T) char m_data[sizeof(T)];
    };

    void construct(dynamic &x, const void *src) const override
    {
        x.m_type = this;
        if constexpr (is_strong_shared)
        {
            x.m_ptr = new shared_impl(*(const T *)src);
        }
        else if constexpr (is_reference)
        {
            x.m_ptr = (void *)src;
        }
        else if constexpr (is_small)
        {
            new (&x.m_ptr) T(*(const T *)src);
        }
        else
        {
            x.m_ptr = new mutable_t(*(const T *)src);
        }
    }

    void construct(dynamic &x, void *src) const override
    {
        x.m_type = this;
        if constexpr (is_strong_shared)
        {
            x.m_ptr = new shared_impl(std::move(*(mutable_t *)src));
        }
        else if constexpr (is_reference)
        {
            x.m_ptr = src;
        }
        else if constexpr (is_small)
        {
            new (&x.m_ptr) T(std::move(*(const T *)src));
        }
        else
        {
            x.m_ptr = new mutable_t(std::move(*(const T *)src));
        }
    }

    void construct_copy(const dynamic &src, dynamic &x) const override
    {
        x.m_type = this;
        if constexpr (is_strong_shared)
        {
            x.m_ptr = src.m_ptr;
            auto shared = (shared_impl *)x.m_ptr;
            ++shared->m_strong_count;
            ++shared->m_weak_count;
        }
        else if constexpr (is_reference)
        {
            x.m_ptr = (void *)&get(src);
        }
        else if constexpr (is_small)
        {
            new (&x.m_ptr) T(get(src));
        }
        else
        {
            x.m_ptr = new mutable_t(get(src));
        }
    }

    void construct_shared(const dynamic &src, dynamic &dest_unassigned) const override
    {
        dest_unassigned.m_type = &dynamic::instantiate<T>().shared;
        if constexpr (is_strong_shared)
        {
            auto *p = (shared_impl *)src.m_ptr;
            dest_unassigned.m_ptr = p;
            ++p->m_strong_count;
            ++p->m_weak_count;
        }
        else if constexpr (is_weak_shared)
        {
            auto *p = (shared_impl *)src.m_ptr;
            dest_unassigned.m_ptr = p;
            if (p->m_strong_count == 0)
                throw_expired();
            ++p->m_strong_count;
            ++p->m_weak_count;
            // Check not weak
        }
        else
        {
            dest_unassigned.m_ptr = new shared_impl(get(src));
        }
    }

    void construct_weak_ref(const dynamic &src, dynamic &dest_unassigned) const override
    {
        dest_unassigned.m_type = &dynamic_detail::get_type<T, dynamic::weak_reference_tag>::get();
        if constexpr (is_strong_shared || is_weak_shared)
        {
            auto *p = (shared_impl *)src.m_ptr;
            dest_unassigned.m_ptr = p;
            ++p->m_weak_count;
        }
        else
        {
            throw dynamic::unsupported("weak_ref() not supported on a value that has not been shared");
        }
    }

    void construct_const_copy(const dynamic &src, dynamic &x) const override
    {
        x.m_type = &dynamic_detail::get_type<const T, Mode>::get();
        if constexpr (is_strong_shared || is_weak_shared)
        {
            x.m_ptr = src.m_ptr;
            auto shared = (shared_impl *)x.m_ptr;
            if constexpr (is_strong_shared)
                ++shared->m_strong_count;
            ++shared->m_weak_count;
        }
        else if constexpr (is_reference)
        {
            x.m_ptr = (void *)&get(src);
        }
        else if constexpr (is_small)
        {
            new (&x.m_ptr) T(get(src));
        }
        else
        {
            x.m_ptr = new mutable_t(get(src));
        }
    }

    void destroy(dynamic &x) const override
    {
        if constexpr (is_strong_shared || is_weak_shared)
        {
            auto shared = (shared_impl *)x.m_ptr;
            if constexpr (is_strong_shared)
            {
                if (--shared->m_strong_count == 0)
                {
                    shared->get().~T();
                }
            }
            if (--shared->m_weak_count == 0)
            {
                delete shared;
            }
        }
        else if constexpr (is_reference)
        {
            // Skip
        }
        else if constexpr (is_small)
        {
            ((T *)(&x.m_ptr))->~T();
        }
        else
        {
            delete (T *)x.m_ptr;
        }
    }

    void construct_reference(const dynamic &src, dynamic &dest) const override
    {
        dynamic_detail::construct_reference_to_dynamic(src, dest);
    }

    void construct_reference(dynamic &src, dynamic &dest) const override
    {
        dynamic_detail::construct_reference_to_dynamic(src, dest);
    }

    void assign_from(dynamic &x, const dynamic &src) const override
    {
        if constexpr (is_reference)
        {
            TRY_TO(get(x) = try_convert<T>(src, "="), "=");
        }
        else
        {
            dynamic copy = src;
            x.swap(copy);
        }
    }

    void assign_to(const dynamic &src, dynamic &dest) const override
    {
        dest.m_type->assign_from(dest, src);
    }

    [[noreturn]]
    void throw_expired() const
    {
        throw std::runtime_error("Expired weak pointer");
    }

    const T &get(const dynamic &x) const
    {
        if constexpr (is_strong_shared)
        {
            auto *sp = (shared_impl *)x.m_ptr;
            if (sp->m_strong_count > 0)
                return sp->get();
            throw_expired();
        }
        else if constexpr (is_weak_shared)
        {
            throw dynamic::unsupported("Convert to a shared_ref() first");
        }
        else if constexpr (is_small && !is_reference)
        {
            return *(const T *)&x.m_ptr;
        }
        else
        {
            return *(const T *)x.m_ptr;
        }
    }

    std::remove_const_t<T> &get_mut(dynamic &r, const char *op) const
    {
        if constexpr (std::is_const_v<T>)
        {
            std::stringstream ss;
            ss << "Operation '" << op << "' is not supported on const reference";
            throw dynamic::unsupported(ss.str().c_str());
        }
        else
        {
            return get(r);
        }
    }

    T &get(dynamic &x) const
    {
        if constexpr (is_strong_shared)
        {
            auto *sp = (shared_impl *)x.m_ptr;
            if (sp->m_strong_count > 0)
                return sp->get();
            throw_expired(); // !! Actually this is a serious internal bug
        }
        else if constexpr (is_weak_shared)
        {
            throw dynamic::unsupported("Convert to a shared_ref() first");
        }
        else if constexpr (is_small && !is_reference)
        {
            return *(T *)&x.m_ptr;
        }
        else
        {
            return *(T *)x.m_ptr;
        }
    }

    void stream_to(const dynamic &x, std::ostream &os) const override
    {
        traits_type::stream_to(get(x), os);
    }

    constexpr static bool string_like = requires(T t) { std::string_view{t}; };

    bool op_eq(const dynamic &x, const dynamic &y) const override
    {
        return traits_type::op_eq(get(x), y);
    }

    std::optional<std::int64_t> try_get_integral(const dynamic &x) const override
    {
        return traits_type::try_get_integral(get(x));
    }

    std::optional<double> try_get_double(const dynamic &x) const override
    {
        return traits_type::try_get_double(get(x));
    }

    std::optional<std::string_view> try_get_string(const dynamic &x) const override
    {
        return traits_type::try_get_string(get(x));
    }

    void *try_get(dynamic &x, const std::type_info &t) const override
    {
        if constexpr (is_const)
        {
            return nullptr;
        }
        else
        {
            return typeid(T) == t ? &get(x) : nullptr;
        }
    }

    const void *try_get(const dynamic &x, const std::type_info &t) const override
    {
        return typeid(T) == t ? &get(x) : nullptr;
    }

    bool op_lt(const dynamic &x, const dynamic &y) const override
    {
        return traits_type::op_lt(get(x), y);
    }

    [[noreturn]]
    void throw_incompatible(const char *op, const dynamic &x, const dynamic &y) const
    {
        std::stringstream ss;
        ss << "Incompatible types for '" << op << "' on " << x << " and " << y;
        throw dynamic::incompatible(ss.str().c_str());
    }

    [[noreturn]]
    void throw_unsupported(const char *op, const dynamic &x) const
    {
        std::stringstream ss;
        ss << "Unsupported operation '" << op << "' on " << x;
        throw dynamic::unsupported(ss.str().c_str());
    }

    dynamic op_add(const dynamic &x, const dynamic &y) const override
    {
        return traits_type::op_add(get(x), y);
    }

    dynamic op_sub(const dynamic &x, const dynamic &y) const override
    {
        return traits_type::op_sub(get(x), y);
    }

    dynamic op_mul(const dynamic &x, const dynamic &y) const override
    {
        return traits_type::op_mul(get(x), y);
    }

    dynamic op_div(const dynamic &x, const dynamic &y) const override
    {
        return traits_type::op_div(get(x), y);
    }

    dynamic op_mod(const dynamic &x, const dynamic &y) const override
    {
        return traits_type::op_mod(get(x), y);
    }

    std::size_t hash(const dynamic &x) const override
    {
        return traits_type::hash(get(x));
    }

    void push_back(dynamic &x, const dynamic &y) const override
    {
        traits_type::push_back(get_mut(x, "push_back()"), y);
    }

    void pop_back(dynamic &x) const override
    {
        traits_type::pop_back(get_mut(x, "pop()"));
    }

    void push_front(dynamic &x, const dynamic &y) const override
    {
        traits_type::push_front(get_mut(x, "push_front()"), y);
    }

    dynamic front(dynamic &x) const override
    {
        return traits_type::front(get(x));
    }

    dynamic front(const dynamic &x) const override
    {
        return traits_type::front(get(x));
    }

    dynamic back(dynamic &x) const override
    {
        return traits_type::back(get(x));
    }

    dynamic back(const dynamic &x) const override
    {
        return traits_type::back(get(x));
    }

    void pop_front(dynamic &x) const override
    {
        traits_type::pop_front(get_mut(x, "pop_front()"));
    }

    void op_inc(dynamic &x) const override
    {
        traits_type::op_inc(get_mut(x, "++"));
    }

    void op_dec(dynamic &x) const override
    {
        traits_type::op_dec(get_mut(x, "--"));
    }

    dynamic op_star(const dynamic &x) const override
    {
        return traits_type::op_star(get(x));
    }

    dynamic op_minus(const dynamic &x) const override
    {
        return traits_type::op_minus(get(x));
    }

    dynamic op_star(dynamic &x) const override
    {
        return traits_type::op_star(get(x));
    }

#define WRAP_METHOD0(Name, Type, Const)                                                                                \
    Type Name(Const dynamic &x) const override                                                                         \
    {                                                                                                                  \
        return traits_type::Name(get(x));                                                                              \
    }

    WRAP_METHOD0(begin, dynamic, )
    WRAP_METHOD0(begin, dynamic, const)
    WRAP_METHOD0(end, dynamic, )
    WRAP_METHOD0(end, dynamic, const)
    WRAP_METHOD0(size, std::size_t, const)

    const std::string &type_str(const dynamic &) const override
    {
        return traits_type::type_str();
    }

    bool as_bool(const dynamic &x) const override
    {
        return traits_type::as_bool(get(x));
    }

    int as_int(const dynamic &x) const override
    {
        return traits_type::as_int(get(x));
    }

    double as_double(const dynamic &x) const override
    {
        return traits_type::as_double(get(x));
    }

    dynamic call(const dynamic &self, std::size_t n_args, const dynamic *args) const override
    {
        return traits_type::call(get(self), n_args, args);
    }

    dynamic op_index(const dynamic &x, dynamic::int_type i) const override
    {
        return traits_type::op_index(get(x), i);
    }

    dynamic op_index(dynamic &x, dynamic::int_type i) const override
    {
        return traits_type::op_index(get(x), i);
    }

    dynamic op_index(const dynamic &x, const char *i) const override
    {
        return traits_type::op_index(get(x), i);
    }

    dynamic op_index(dynamic &x, const char *i) const override
    {
        return traits_type::op_index(get(x), i);
    }

    dynamic op_index(const dynamic &x, const dynamic &i) const override
    {
        return traits_type::op_index(get(x), i);
    }

    dynamic op_index(dynamic &x, const dynamic &i) const override
    {
        return traits_type::op_index(get(x), i);
    }

    void insert(dynamic &x, const dynamic &value) const override
    {
        traits_type::insert(get_mut(x, "insert()"), value);
    }

    void insert(dynamic &x, const dynamic &k, const dynamic &v) const override
    {
        traits_type::insert(get_mut(x, "insert()"), k, v);
    }

    dynamic first(dynamic &x) const override
    {
        return traits_type::first(get(x));
    }

    dynamic first(const dynamic &x) const override
    {
        return traits_type::first(get(x));
    }

    dynamic second(dynamic &x) const override
    {
        return traits_type::second(get(x));
    }

    dynamic second(const dynamic &x) const override
    {
        return traits_type::second(get(x));
    }
};

template <typename T, typename Mode> struct get_type
{
    static dynamic::type &get()
    {
        static type_impl<T, Mode> type;
        return type;
    }
};
} // namespace cutty::dynamic_detail
