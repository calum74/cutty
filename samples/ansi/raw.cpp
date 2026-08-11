#include <cutty/ansi/raw.hpp>

namespace ancy = cutty::ansi;


int main()
{
    // The raw API sends ANSI commands directly to a terminal, via an ostream.

    // Reset the terminal
    ansy::reset(std::cout);

    // Set a colour
    ancy::fg(ancy::red);
    std::cout << "This is in red!\n";


    // Back to default colour
    ancy::fg(ancy::terminal_default);

    // Set a weight
    ancy::set_weight(ansi::weight::bold);

    // Set a custom colour

    // Set underline

    // Reset the terminal
    ansy::reset(std::cout);
}