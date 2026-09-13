#include <cutty/ansi/window.hpp>

#include <cutty/ansi/widgets.hpp>

namespace ancy = cutty::ansi;

int main()
{
    // Create a 40x4 output area
    ancy::window window({40,4}, std::cout);

    int progress = 0;

    // Text widget:
    auto theme = ancy::default_theme();

    ancy::text_box info(window, {4,0}, {20,1}, theme.text, theme, "Timer demonstration. ^C or select Quit to exit");

    ancy::text_box text_number(window, {4,1}, {20,1}, theme.text, theme, "");

    ancy::button reset(window, {6,3}, {7,1}, 'r', "Reset", theme.button_normal, theme.button_focus, [&] {
        progress = 0;
    });

    ancy::progress_bar progress_bar({.parent=window, .position={2,2}, .size={24,1}, .style=ancy::style{.fg=ancy::colour::yellow, .bg=ancy::colour::green}, .value=0, .max_value=1000});

    ancy::button quit(window, {16,3}, {6,1}, 'q', "Quit", theme.button_normal, theme.button_focus, [&] {
        window.quit();
    });

    ancy::timer t(window, [&] { 
        text_number.set_text(std::format("Processed {} items", ++progress));
        progress_bar.set_progress(progress, 1000);
        if(progress < 100000)
        {
            t.set_timer(std::chrono::milliseconds(100));
        }
        else
        {
            window.quit();
        }
    });

    t.set_timer(std::chrono::milliseconds(100));

    window.run();
}
