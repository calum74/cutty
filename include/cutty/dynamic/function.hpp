#pragma once
#include <functional>

namespace dynamic_detail
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

template <typename T> const T &try_convert(const dynamic &src)
{
    if constexpr (std::is_same_v<std::decay_t<T>, dynamic>)
    {
        return src;
    }
    else if (auto *p = src.try_get<T>())
        return *p;
    else
    {
        // Better diagnostic ??
        throw dynamic::incompatible("Wrong argument type to function");
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
} // namespace dynamic_detail

template <typename Fn> dynamic dynamic::function(Fn fn)
{
    static_assert(dynamic_detail::fully_typed_function<Fn>, "Function must have fully typed arguments");
    return dynamic(dynamic_detail::make_dynamic_function(fn), dynamic::shared_tag());
}
