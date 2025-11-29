#include <cutty/check.hpp>
#include <exception>
#include <sstream>

void cutty::check(detail::convertible_boolean cond, const char *msg, const std::source_location &src)
{
    if (!cond.value)
    {
        std::stringstream ss;
        ss << msg << " at " << src.file_name() << ":" << src.line();
        throw std::runtime_error(ss.str());
    }
}

void cutty::detail::check_expected_exception(const char * expected, const char *actual, const std::source_location &src)
{
    if (std::string_view (expected) != std::string_view(actual))
    {
        std::stringstream ss;
        ss << "Expected exception not thrown: expected '" << expected << "' but got '" << actual << "'";
        ss << " at " << src.file_name() << ":" << src.line();
        throw std::runtime_error(ss.str());
    }
}
