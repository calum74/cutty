#include <cutty/ansi/raw.hpp>
#include <iostream>

namespace ancy = cutty::ansi;

int main()
{
    // The raw API sends ANSI commands directly to a terminal, via an ostream.
    auto &os = std::cout;

    // 1. Reset the terminal
    ancy::reset(os);

    // 2. Colours

    // 2a. Builtin normal colours

    auto show_colour = [](const char * name, ancy::colour c)
    {
        ancy::reset(std::cout);
        ancy::fg_colour(c, std::cout);
        std::cout << "Foreground colour " << name;
        ancy::reset(std::cout);
        std::cout << std::endl;
        ancy::reset(std::cout);
        ancy::bg_colour(c, std::cout);
        std::cout << "Background colour " << name;
        ancy::reset(std::cout);
        std::cout << std::endl;
    };

    show_colour("default", ancy::colour::terminal_default);
    show_colour("black", ancy::colour::black);
    show_colour("red", ancy::colour::red);
    show_colour("green", ancy::colour::green);
    show_colour("yellow", ancy::colour::yellow);
    show_colour("blue", ancy::colour::blue);
    show_colour("magenta", ancy::colour::magenta);
    show_colour("cyan", ancy::colour::cyan);
    show_colour("white", ancy::colour::white);

    // 2b. Bright colours

    // 2c. RGB colours

    // 2.d. Greyscale colours

    // 2d. Invert colours

    // 3. Weight

    // 4. Other styles

    // 4a. Underling
    // 4b. Strikethrought
    // 4c. Hidden
    // 4d. Blink

    // 5. Styles


    ancy::reset(std::cout);
}