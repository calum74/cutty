#include <cutty/check.hpp>
#include <cutty/print.hpp>

#include <map>
#include <vector>

struct Foo
{
};

namespace cy = cutty;

int main()
{
    // Perform a simple print. Spaces are inserted, like in Python.
    cy::print("The value is", 5);

    // Prints the object's type and address.
    cy::print("Foo is", Foo());

    // Print to a stream
    std::stringstream ss;
    cy::print_stream(ss, "The value is", 5);
    cy::check_equal(ss.str(), "The value is 5");

    // Print to a string
    cy::check_equal(cy::print_str("Hello,", "world!"), "Hello, world!");

    // Print a range
    std::vector<int> values = {1, 2, 3};
    cy::print("The values are", values);

    // Print a tuple (or a pair)
    cy::print(std::tuple{"123", 456});

    // Print a range of tuples
    std::map<int, std::vector<std::string>> map;
    map[1] = {"a", "b", "c"};
    map[2] = {};
    cy::print(map);

    // Print with a different separator
    cy::print({.start = "(", .sep = ",", .end = ")"}, 1, 2, 3);
    cy::print();

    /*
    Output:
        The value is 5
        Foo is Foo@0x16ee0ac67
        The values are {1, 2, 3}
        (123, 456)
        {(1, {a, b, c}), (2, {})}
        (1,2,3)
    */
}
