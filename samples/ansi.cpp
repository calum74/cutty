#include <cutty/ansi.hpp>

namespace cy=cutty;

int main()
{
    // The low level library write specific ANSI properties directly to std::cout
    cy::ansi::reset(std::cout);
    cy::ansi::bold_on(std::cout);
    std::cout << "Hello, world!\n";
    cy::ansi::fg_colour(cy::ansi::colour{0,128,255}, std::cout);
    std::cout << "Hello, in burple!\n";
    cy::ansi::reset(std::cout);

    cy::ansi::style red {.fg = cy::ansi::red};
    cy::ansi::style bright_green {.fg = cy::ansi::green, .bold=true};

    cy::ansi::change_style({}, bright_green, std::cout);
    std::cout << "Bright green";
    cy::ansi::reset(std::cout);
    std::cout << std::endl;

    // A writer tracks the console state and cursor position
    // By default, it outputs to std::cout
    cy::ansi::raw_writer writer;
    writer.text("Writing to a raw buffer");
    writer.endl();
    
    //  << "The console size is " << std::to_string(writer.console_size());

    // A window 
    cy::ansi::window w({40,20});
    w.text("Writing to a window");
    w.go_to({10, 5});
    w.text("The title");

    // Windows are buffered, so you must remember to flush
    w.flush();
}
