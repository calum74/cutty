#pragma once

#include <exception>
#include <source_location>
#include <string_view>

/** Namespace for all cutty functionality */
namespace cutty
{
namespace detail
{
// Helper to cast objects to boolean if they have an explicit bool operator
struct convertible_boolean
{
    convertible_boolean(auto &&b)
    {
        value = b ? true : false;
    }
    bool value;
};
} // namespace detail

/**
    Checks if a condition holds or throws an exception.
 */
void check(detail::convertible_boolean cond, const char *msg = "Failed check",
           const std::source_location &src = std::source_location::current());

/**
    Checks that specific exception text is thrown.
    @p fn               The function/lambda to execute
    @p expected_text    The expected error messages


 */
void check_throws(auto &&fn, std::string_view expected_text,
                  const std::source_location &src = std::source_location::current())
{
    try
    {
        fn();
        check(false, "exception not thrown", src);
    }
    catch (const std::exception &ex)
    {
        check(expected_text == ex.what(), "exception text differs", src);
    }
}

// TODO
void check_equal(auto &&lhs, auto &&rhs, const std::source_location &src = std::source_location::current());
} // namespace cutty
