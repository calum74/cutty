#include <cutty/check.hpp>
#include <exception>
#include <sstream>

void cutty::check(detail::convertible_boolean cond, const char * msg, 
    const std::source_location & src)
{
    if(!cond.value)
    {
        std::stringstream ss;
        ss << msg << " at " << src.file_name() << ":" << src.line();
        throw std::runtime_error(ss.str());
    }
}
