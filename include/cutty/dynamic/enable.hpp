#pragma once

#ifdef CY_DYNAMIC_EXPLICIT
#error CY_DYNAMIC_EXPLICIT is already defined
#else
#define CY_DYNAMIC_EXPLICIT explicit
#endif

#include "../dynamic.hpp"
#include "empty.hpp"
#include "function.hpp"
#include "get_type.hpp"
#include "type.hpp"
#include "type_impl.hpp"

// TODO: This isn't a great name for this function and header file
template <typename T> void cutty::dynamic::enable()
{
    dynamic::get_type<T, dynamic::by_value_tag>();
    dynamic::get_type<const T, dynamic::by_value_tag>();
    dynamic::get_type<T &, dynamic::by_value_tag>();
    dynamic::get_type<const T &, dynamic::by_value_tag>();
    dynamic::get_type<T, dynamic::shared_tag>();
    dynamic::get_type<const T, dynamic::shared_tag>();
}
