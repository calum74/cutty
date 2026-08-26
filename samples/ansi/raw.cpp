#include <cutty/ansi/raw.hpp>
#include <cutty/test.hpp>
#include <iostream>

namespace ancy = cutty::ansi;

int main()
{
    // The raw API sends ANSI commands directly to a terminal, via an ostream.
    // We'll use `os` to avoid writing out `std::cout` all the time.
    auto &os = std::cout;

    // 1. Reset the terminal
    ancy::reset(os);

    // 2. Colours

    // 2a. Builtin normal colours

    auto show_colour = [](std::string name, ancy::colour c) {
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
    show_colour("bright default", ancy::colour::terminal_default.bright());
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

    // 2d. Greyscale colours
    for (int i = 0; i < 24; ++i)
    {
        show_colour(std::format("grey {}", i), ancy::colour::grey(i));
    }

    // 2. SGR text styles

    // 2a. Invert
    ancy::apply(ancy::sgr_style::reverse, os);
    std::cout << "This is inverted";
    ancy::reset(os);
    os << std::endl;

    // 2b. Weight
    ancy::apply(ancy::sgr_style::heavy_weight, os);
    std::cout << "This is heavy, ";
    ancy::apply(ancy::sgr_style::light_weight, os);
    std::cout << "this is light, ";
    ancy::apply(ancy::sgr_style::normal_weight, os);
    std::cout << "this is normal weight.";
    ancy::reset(os);
    os << std::endl;

    // 2c. Underline
    ancy::apply(ancy::sgr_style::underline, os);
    std::cout << "This is underlined, ";
    ancy::apply(ancy::sgr_style::double_underline, os);
    std::cout << "this is double underlined"; // Note: Some terminals don't support this
    ancy::apply(ancy::sgr_style::no_underline, os);
    std::cout << ", not underlined.\n";

    // 2c. Strikethrough - not always supported
    ancy::apply(ancy::sgr_style::strikethrough, os);
    std::cout << "This is strikethrough, ";
    ancy::apply(ancy::sgr_style::no_strikethrough, os);
    std::cout << "no strikethrough\n";

    // 2d. Box - not supported very well in terminals
    ancy::apply(ancy::sgr_style::frame, os);
    std::cout << "This is framed, ";
    ancy::apply(ancy::sgr_style::round_frame, os);
    std::cout << "round frame";
    ancy::apply(ancy::sgr_style::no_frame, os);
    std::cout << "no frame\n";

    // 2e. Blink - not supported very well
    ancy::apply(ancy::sgr_style::slow_blink, os);
    std::cout << "Slow blink, ";
    ancy::apply(ancy::sgr_style::rapid_blink, os);
    std::cout << "rapid blink, "; // Not supported very well
    ancy::apply(ancy::sgr_style::no_blink, os);
    std::cout << "no blink\n";

    // 2f. Hidden
    ancy::apply(ancy::sgr_style::conceal, os);
    std::cout << "Concealed, ";
    ancy::apply(ancy::sgr_style::no_conceal, os);
    std::cout << "not concealed\n";

    // 3. Styles
    // Styles offer a more encapsulated way to describe a 

    ancy::reset(os);

    // s1 is the default style (just terminal defaults), and s2 is the desired style
    ancy::style s1, s2{.fg = ancy::colour::red, .bg = ancy::colour::blue, .weight = ancy::weight::heavy};

    // Change style from s1 to s2
    // change_style outputs just the differences between the two styles
    ancy::change_style(s1, s2, os);
    std::cout << "Red on blue heavy weight";

    // change style back from s2 to s1.
    ancy::change_style(s2, s1, os);
    std::cout << std::endl;
    std::cout << "Back to default style\n";

    // 4. Cursor movement

    os << "a\nb\nc\nd\n";
    // Move up one line
    ancy::up(os);
    os << "D";
    // Move up 3 lines, left 1 line
    ancy::up(3, os);
    ancy::left(os);
    os << "A";

    // Move down 4 and left 1
    ancy::down(4, os);
    ancy::left(os);
    os << "E\n";

    // 5. Other
    try
    {
        auto ts = ancy::get_terminal_size();
        std::cout << "Your terminal is " << ts.w << " x " << ts.h << " characters\n";
    }
    catch(std::runtime_error&)
    {
        std::cout << "Could not get terminal size - probably not running in a terminal\n";
    }

    ancy::reset(std::cout);
}
