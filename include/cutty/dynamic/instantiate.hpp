#pragma once

#ifdef CY_DYNAMIC_EXPLICIT
#error CY_DYNAMIC_EXPLICIT is already defined
#else
#define CY_DYNAMIC_EXPLICIT explicit
#endif

#include <cutty/dynamic.hpp>
#include "empty.hpp"
#include "function.hpp"
#include "get_type.hpp"
#include "type.hpp"
#include "type_impl.hpp"

struct cutty::dynamic::types
{
    const dynamic::type &by_value;
    const dynamic::type &by_ref;
    const dynamic::type &shared;
};

template<typename T>
const cutty::dynamic::types &cutty::dynamic::instantiate()
{
    static types t = {
        dynamic_detail::get_type<T, cutty::dynamic::by_value_tag>::get(),
        dynamic_detail::get_type<T&, dynamic::by_value_tag>::get(),
        dynamic_detail::get_type<T, dynamic::shared_tag>::get()
    };
    return t;
}

#define CY_INSTANTIATE(T) template const cy::dynamic::types& cy::dynamic::instantiate<T>();
