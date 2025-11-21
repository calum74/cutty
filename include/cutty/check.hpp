#pragma once

#include <source_location>

namespace cutty
{
    namespace detail
    {
        struct convertible_boolean
        {
            bool value;
            convertible_boolean(auto&& b) { value = b ? true : false; }
        };
    }

    void check(detail::convertible_boolean cond, const char * msg = "Failed check", 
        const std::source_location & src = std::source_location::current());
}
