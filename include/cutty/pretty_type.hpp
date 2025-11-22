#pragma once

#include <string_view>

namespace cutty
{
    template <typename T> constexpr auto pretty_type()
    {
#if defined(__clang__)
        std::string_view s(__PRETTY_FUNCTION__);
        s.remove_prefix(31);
        s.remove_suffix(1);
#elif defined (__GNUC__)
        std::string_view s(__PRETTY_FUNCTION__);
        // +15
        s.remove_prefix(46);
        s.remove_suffix(1);
#elif _MSC_VER >= 1920
        std::string_view s(__FUNCSIG__);
        s.remove_prefix(32);
        s.remove_suffix(7);
#else
#error "Unhandled compiler (add it here)"
#endif        
        return s;
    }

}
