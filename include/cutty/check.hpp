#pragma once

#include "print.hpp"

#include <exception>
#include <source_location>
#include <sstream>
#include <string_view>
#include <iomanip>

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

void check_expected_exception(const char *expected_text, const char *actual_text, const std::source_location &src);
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
void check_throws(auto &&fn, const char *expected_text,
                  const std::source_location &src = std::source_location::current())
{
    try
    {
        fn();
        check(false, "exception not thrown", src);
    }
    catch (const std::exception &ex)
    {
        detail::check_expected_exception(expected_text, ex.what(), src);
    }
}

class check_failed : public std::runtime_error
{
  public:
    check_failed(const char *msg, const std::source_location &src);
};

// TODO
void check_equal(auto &&lhs, auto &&rhs, const std::source_location &src = std::source_location::current())
{
    if (lhs != rhs)
    {
        std::stringstream ss;
        ss << "'";
        ss << std::setprecision(10);
        print_stream(ss, lhs);
        ss << "' != '";
        print_stream(ss, rhs);
        ss << "' at " << src.file_name() << ":" << src.line();
        throw check_failed(ss.str().c_str(), src);
    }
}

} // namespace cutty
