#pragma once

#include <source_location>
#include <exception>
#include <string_view>

namespace cutty
{
    namespace detail
    {
        // Helper to cast objects to boolean if they have an explicit bool operator
        struct convertible_boolean
        {
            convertible_boolean(auto&& b) { value = b ? true : false; }
            bool value;
        };
    }

    void check(detail::convertible_boolean cond, const char * msg = "Failed check", 
        const std::source_location & src = std::source_location::current());

    void check_throws(auto &&fn, std::string_view expected_text, 
        const std::source_location & src = std::source_location::current())
    {
        try
        {
            fn();
            check(false, "exception not thrown", src);
        }
        catch(const std::exception&ex)
        {
            check(expected_text == ex.what(), "exception text differs", src);
        }

    }

}
