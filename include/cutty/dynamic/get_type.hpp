#pragma once

namespace dynamic_detail
{
    template<typename T, typename Mode>
    struct get_type;
}

template<typename T, typename Mode> const dynamic::type * dynamic::get_type()
{
    return &dynamic_detail::get_type<T,Mode>::get();
}
