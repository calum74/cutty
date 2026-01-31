#pragma once
#include <functional>

#ifndef CY_DYNAMIC_EXPLICIT
#include <cutty/dynamic.hpp>
#endif

#include <cutty/pretty_type.hpp>

namespace cutty::dynamic_detail
{
template <typename Fn> struct lambda_traits;

template <typename Fn> struct function_traits;

template <typename Fn>
    requires requires { &Fn::operator(); }
struct function_traits<Fn> : public lambda_traits<decltype(&Fn::operator())>
{
};

template <typename T, typename R, typename... Args> struct lambda_traits<R (T::*)(Args...)>
{
    using return_type = R;
    using args_type = std::tuple<Args...>;
};

template <typename T, typename R, typename... Args> struct lambda_traits<R (T::*)(Args...) const>
{
    using return_type = R;
    using args_type = std::tuple<Args...>;
};

template <typename R, typename... Args> struct function_traits<R (*)(Args...)>
{
    using return_type = R;
    using args_type = std::tuple<Args...>;
};

[[noreturn]] void throw_wrong_arg_type(std::string_view given, std::string_view expected);

template <typename T> const T &try_convert(const dynamic &src)
{
    if constexpr (std::is_same_v<std::decay_t<T>, dynamic>)
    {
        return src;
    }
    else if (auto *p = src.try_get<T>())
    {
        return *p;
    }
    else
    {
        throw_wrong_arg_type(src.type_str(), pretty_type<T>());
    }
}

template <typename T, size_t> struct drop_index
{
    using type = T;
};

template <typename Fn, std::size_t... Args> auto make_dynamic_function(Fn fn, std::index_sequence<Args...>)
{
    using traits = function_traits<Fn>;
    using args_type = typename traits::args_type;
    using function_type = std::function<dynamic(typename drop_index<const dynamic &, Args>::type...)>;

    if constexpr (std::same_as<typename traits::return_type, void>)
    {
        // Returns void
        return function_type([fn]<typename... Args2>(Args2 &&...args) {
            std::tuple<Args2 &...> argtuple(args...);
            fn(try_convert<std::tuple_element_t<Args, args_type>>(std::get<Args>(argtuple))...);
            return dynamic();
        });
    }
    else
    {
        // Need to wrap the return value
        return function_type([fn]<typename... Args2>(Args2 &...args) {
            std::tuple<Args2 &...> argtuple(args...);

            return dynamic(fn(try_convert<std::tuple_element_t<Args, args_type>>(std::get<Args>(argtuple))...));
        });
    }

    return function_type();
}

template <typename Fn> auto make_dynamic_function(Fn fn)
{
    return make_dynamic_function(
        std::move(fn), std::make_index_sequence<std::tuple_size_v<typename function_traits<Fn>::args_type>>());
}

template <typename Fn>
concept fully_typed_function = requires { typename function_traits<Fn>::args_type; };
} // namespace cutty::dynamic_detail

template <typename Fn> cutty::dynamic cutty::dynamic::function(Fn fn)
{
    static_assert(dynamic_detail::fully_typed_function<Fn>, "Function must have fully typed arguments");
    return dynamic(dynamic_detail::make_dynamic_function(fn), dynamic::shared_tag());
}

cutty::dynamic::dynamic(dynamic_detail::probably_a_function auto&& fn) : dynamic(dynamic_detail::make_dynamic_function(fn), dynamic::shared_tag())
{
    // static_assert(dynamic_detail::fully_typed_function<decltype(fn)>, "Function must have fully typed arguments");
}
