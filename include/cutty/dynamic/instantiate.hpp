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

template<typename T>
const cutty::dynamic::types &cutty::dynamic::instantiate()
{
    static types t = {
        dynamic::get_type<T, cutty::dynamic::by_value_tag>(),
        dynamic::get_type<T&, dynamic::by_value_tag>(),
        dynamic::get_type<T, dynamic::shared_tag>()
    };
    return t;
}
