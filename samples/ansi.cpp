#include <cutty/ansi.hpp>

namespace cy=cutty;

int main()
{
    // The low level library write specific ANSI properties directly to std::cout
    cy::ansi::reset(std::cout);
    cy::ansi::bold_on(std::cout);
    std::cout << "Hello, world!\n";
    cy::ansi::fg_colour(cy::ansi::colour{0,128,255}, cy::ansi::colour_space::cs_256, std::cout);
    cy::ansi::reset(std::cout);

    // A writer tracks the console state
    cy::ansi::unbuffered_writer writer{std::cout};

    writer.text("Hello, world");
    
    //  << "The console size is " << std::to_string(writer.console_size());

    // A buffered_writer 
}
