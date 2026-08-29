#include <cutty/ansi/window.hpp>

#include <cutty/ansi/widgets.hpp>

#include <thread>

namespace ancy = cutty::ansi;

int main()
{
    // Create a 20x4 output area
    // By default, the event loop runs in parallel
    ancy::window window({20,4}, std::cout);

    int progress = 0;

    // Text widget:
    ancy::theme theme;
    ancy::text_box info(window, {3,0}, {15,1}, theme.text, theme, "Hit escape twice to exit");

    ancy::text_box text_number(window, {3,2}, {15,1}, theme.text, theme, "");


    // Button widget:

    // Progress bar widget

    for(progress = 0; progress < 1000; ++progress)
    {
        // std::chrono::sleep_for(1ms);
        // Update the

        text_number.set_text(std::format("Processed {} items", progress));

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        window.flush();
        // ?? Flush ??
    }

    // window.run();
}
