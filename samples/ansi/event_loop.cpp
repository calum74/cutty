#include <cutty/ansi/window.hpp>

#include <cutty/ansi/widgets.hpp>

#include <thread>

namespace ancy = cutty::ansi;

int main()
{
    // Create a 20x4 output area
    // By default, the event loop runs in parallel
    ancy::window window({40,4}, std::cout);

    int progress = 0;

    // Text widget:
    ancy::theme theme;
    ancy::text_box info(window, {3,0}, {25,1}, theme.text, theme, "Hit escape twice to exit");

    ancy::text_box text_number(window, {3,2}, {25,1}, theme.text, theme, "");

    ancy::timer t(window, [&] { 
        text_number.set_text(std::format("Processed {} items", ++progress));
        if(progress < 1000)
        {
            t.set_timer(std::chrono::milliseconds(1));
        }
        else
        {
            window.quit();
        }
    });

    t.set_timer(std::chrono::milliseconds(1));

    window.run();
}
