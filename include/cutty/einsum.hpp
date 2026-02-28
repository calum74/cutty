#pragma once

#include "fixed_string.hpp"
#include "tensor.hpp"

namespace cutty::einsum
{
using axis_label = char;

// Basic einsum parsing
template <fixed_string E> constexpr auto lhs = fixed_string_split<E, "->", 0>;

template <fixed_string E> constexpr auto rhs = fixed_string_split<E, "->", 1>;

// A valid einsum:
// Has each axis mentioned exactly twice
// Contains an arrow
// rhs is unique

namespace detail
{
template <fixed_string Haystack> constexpr bool contains_duplicates()
{
    for (size_t i = 0; i < Haystack.size(); ++i)
    {
        for (size_t j = i + 1; j < Haystack.size(); ++j)
        {
            if (Haystack[i] == Haystack[j])
                return true;
        }
    }
    return false;
}
} // namespace detail

template <fixed_string E> constexpr bool contains_duplicates = detail::contains_duplicates<E>();

namespace cost_estimates
{
template <fixed_string E, axis_label X> constexpr bool unique = fixed_string_count_ch<E, X> == 1;
template <fixed_string E, axis_label X> constexpr bool duplicated = fixed_string_count_ch<E, X> == 2;

template <fixed_string E, axis_label X, tensor T, labelled_tensor<E, T>>
    requires duplicated<E, X>
constexpr size_t reduce_costs = 0;

} // namespace cost_estimates

} // namespace cutty::einsum