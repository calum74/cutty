#include <cutty/ansi/raw.hpp>
#include <iostream>

namespace ancy = cutty::ansi;

int main()
{
    // The raw API sends ANSI commands directly to a terminal, via an ostream.

    // Reset the terminal
    ancy::reset(std::cout);

    // Colours

    // Set a colour
    ancy::fg_colour(ancy::red, std::cout);
    std::cout << "This is in red!\n";


    // Back to default colour
    ancy::fg_colour(ancy::terminal_default, std::cout);

    // Set a weight
    ancy::bold_on(std::cout);

    // Set a custom colour

    // Set underline

    // Styles
    // A style combines forground colour, background colour, weight and underline

    // Reset the terminal
    ancy::reset(std::cout);
}