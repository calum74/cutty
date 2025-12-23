#include <cutty/print.hpp>
#include <cutty/test.hpp>
#include <vector>
#include <map>

namespace cy = cutty;

void empty()
{
    cy::check_equal(cy::print_str(), "");
}

void options()
{
    cy::check_equal(cy::print_str({.start = "(", .sep = ",", .end = ")"}, 1, 2, 3), "(1,2,3)");
}

struct Foo {};

void objects()
{
    cy::check(cy::print_str(Foo()) > "Foo@");
    cy::check(cy::print_str(Foo()) < "Goo@");
}

void ranges()
{
    cy::check_equal(cy::print_str(std::vector {1,2,3}), "{1, 2, 3}");
}

void tuples()
{
    cy::check_equal(cy::print_str(std::tuple {1,2,3}), "(1, 2, 3)");
}

void maps()
{
    std::map<int, int> m1 = {{1,2}, {3,4}};
    cy::check_equal(cy::print_str(m1), "{(1, 2), (3, 4)}");
}

int main()
{
    return cy::test({empty, options, objects, ranges, tuples, maps});
}
