#include <cutty/check.hpp>
#include <cutty/with.hpp>
#include <exception>
#include <sstream>

namespace cy = cutty;

void cy::check(detail::convertible_boolean cond, const char *msg, const std::source_location &src)
{
    detail::checkpoint(src);
    if (!cond.value)
    {
        std::stringstream ss;
        ss << msg << " at " << src.file_name() << ":" << src.line();
        throw check_failed(ss.str().c_str(), src);
    }
}

void cy::detail::check_expected_exception(const char *expected, const char *actual, const std::source_location &src)
{
    if (std::string_view(expected) != std::string_view(actual))
    {
        std::stringstream ss;
        ss << "Expected exception not thrown: expected '" << expected << "' but got '" << actual << "'";
        ss << " at " << src.file_name() << ":" << src.line();
        throw check_failed(ss.str().c_str(), src);
    }
}

cy::check_failed::check_failed(const char *msg, const std::source_location &src) : std::runtime_error(msg)
{
}

void cy::detail::checkpoint(const std::source_location &src)
{
    if (auto *cp = try_get<std::source_location, checkpoint_tag>())
    {
        *cp = src;
    }
}