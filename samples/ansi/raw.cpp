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

    auto show_colour = [](std::string name, ancy::colour c)
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

    show_colour("bright default", ancy::colour::terminal_default);
    show_colour("bright black", ancy::colour::black.bright());
    show_colour("bright red", ancy::colour::red.bright());
    show_colour("bright green", ancy::colour::green.bright());
    show_colour("bright yellow", ancy::colour::yellow.bright());
    show_colour("bright blue", ancy::colour::blue.bright());
    show_colour("bright magenta", ancy::colour::magenta.bright());
    show_colour("bright cyan", ancy::colour::cyan.bright());
    show_colour("bright white", ancy::colour::white.bright());

    // 2c. RGB colours
    show_colour("rgb", ancy::colour::rgb(232, 114, 41));

    // 2.d. Greyscale colours
    for(int i=0; i<24; ++i)
    {
        show_colour(std::format("grey {}", i), ancy::colour::grey(i));
    }

    // 2d. Invert colours
    ancy::invert(os);
    std::cout << "This is inverted";
    ancy::reset(os);
    os << std::endl;

    // 3. Weight

    // 4. Other styles

    // 4a. Underling
    // 4b. Strikethrought
    // 4c. Hidden
    // 4d. Blink (slow/fast)

    // 5. Styles
    // Styles offer a more encapsulated way to output text.

    // 6. Terminal control
    // Current terminal size
    // ?? terminal_size
    auto ts = ancy::get_terminal_size();
    std::cout << "Your terminal is " << ts.w << " x " << ts.h << " characters\n";

    // Raw mode? terminal size

    // 7. Positioning

    ancy::reset(std::cout);
}