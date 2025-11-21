#pragma once

#ifdef DYNAMIC_EXPLICIT
#error DYNAMIC_EXPLICIT is already defined
#else
#define DYNAMIC_EXPLICIT explicit
#endif

#include "../dynamic.hpp"
#include "empty.hpp"
#include "function.hpp"
#include "get_type.hpp"
#include "type.hpp"
#include "type_impl.hpp"

template <typename T> void dynamic::enable()
{
    dynamic::get_type<T, dynamic::by_value_tag>();
    dynamic::get_type<const T, dynamic::by_value_tag>();
    dynamic::get_type<T &, dynamic::by_value_tag>();
    dynamic::get_type<const T &, dynamic::by_value_tag>();
    dynamic::get_type<T, dynamic::shared_tag>();
    dynamic::get_type<const T, dynamic::shared_tag>();
}
