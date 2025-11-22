#pragma once

namespace cutty::dynamic_detail
{
    template<typename T, typename Mode>
    struct get_type;
}

template<typename T, typename Mode> const cutty::dynamic::type * cutty::dynamic::get_type()
{
    return &cutty::dynamic_detail::get_type<T,Mode>::get();
}
