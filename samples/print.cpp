#include <cutty/print.hpp>
#include <cutty/check.hpp>

#include <sstream>

struct Foo {};

namespace cy = cutty;

int main()
{
    cy::print("The value is", 5);
    cy::print("Foo is", Foo());

    std::stringstream ss;
    cy::print_stream(ss, "The value is", 5);
    cy::check_equal(ss.str(), "The value is 5");
}
