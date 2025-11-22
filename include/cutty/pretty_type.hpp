#pragma once

#include <string_view>

namespace cutty
{
    template <typename T> constexpr auto pretty_type()
    {
        std::string_view s(__PRETTY_FUNCTION__);
#if defined(__clang__)
        s.remove_prefix(31);
#elif defined (__GNUC__)
        // +15
        s.remove_prefix(46);
#else
#error "Unhandled compiler (add it here)"
#endif
        
        s.remove_suffix(1);
        return s;
    }

}
