#include <cutty/ansi.hpp>
#include <cutty/test.hpp>

#include <chrono>
#include <thread>

namespace cy = cutty;
namespace ancy = cy::ansi;

void test_raw()
{
    // Tests the low level ANSI commands

    cy::ansi::reset(std::cout);
    std::cout << std::endl;
    cy::ansi::bold_on(std::cout);
    std::cout << "Bold\n";
    cy::ansi::bold_off(std::cout);
    std::cout << "Not bold\n";

    cy::ansi::style s1, s2;
    s1.bold = true;
    s1.fg = cy::ansi::colour::red;
    s1.bg = cy::ansi::colour::blue;

    cy::ansi::change_style({}, s1, std::cout);
    std::cout << "Bold in red";
    cy::ansi::reset(std::cout);
    std::cout << std::endl;
}

void test_raw_writer()
{
    std::cout << std::endl;
    cy::ansi::raw_writer w(std::cout);

    cy::ansi::style s1, s2;
    s1.bold = true;
    s1.fg = cy::ansi::colour::red;
    s1.bg = cy::ansi::colour::blue;

    w.apply(s1);
    w.text("In red");
    w.reset();
    w.endl();

    w.go_to({10, 5});
    w.text("X");
    w.endl();
    w.endl();
    w.go_to({9, 4});
    w.text("W");
    w.go_to({0, 7});
    w.text("Y");
}

void test_window_writer()
{
    // Writing outside of the region clips the output

    std::cout << std::endl;
    cy::ansi::window vp({40, 5});

    vp.put({.ch = 'x'}, {3, 0});
    vp.put({.ch = 'y'}, {4, 1});
    vp.flush();

    cy::ansi::style s1, s2;
    s1.bold = true;
    s1.fg = cy::ansi::colour::red;
    s1.bg = cy::ansi::colour::blue;

    vp.put({s1, 'X'}, {.x = 0, .y = 0});
    vp.put({s2, ' '}, {0, 0});
    vp.put({.ch = '+'}, {39, 0});

    s2.fg = cy::ansi::colour::green;

    draw_box(vp, s2, cy::ansi::line_style::ascii, 10, 2, 10, 3);
}

void test_progress_bar()
{
    std::cout << std::endl;
    cy::ansi::window vp({40, 10});

    cy::ansi::style s1;
    s1.fg = cy::ansi::colour::red;
    s1.bg = cy::ansi::colour::white;
    draw_progress(vp, s1, 10, 0, 20, 0, 100);
    draw_progress(vp, s1, 10, 1, 20, 50, 100);
    draw_progress(vp, s1, 10, 2, 20, 205, 400);
    draw_progress(vp, s1, 10, 3, 20, 208, 400);
    draw_progress(vp, s1, 10, 5, 20, 210, 400);

    draw_progress(vp, s1, 10, 9, 20, 100, 100);
    vp.flush();

    cy::ansi::style s2;

    for (int i = 0; i <= 1000; ++i)
    {
        draw_progress(vp, s1, 0, 9, 40, i, 1000);
        draw_progress(vp, s2, 15, 7, 20, i, 1000);
        vp.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void test_graphics()
{
    std::cout << std::endl;
    cy::ansi::window vp({40, 10});
    cy::ansi::bitmap b1({5, 5});

    b1[{0, 0}] = cy::ansi::colour::rgb(128, 255, 0);
    b1[{0, 1}] = cy::ansi::colour::rgb(255, 0, 0);
    b1[{1, 0}] = cy::ansi::colour::rgb(0, 127, 255);
    b1[{1, 1}] = cy::ansi::colour::rgb(255, 0, 0);

    b1[{0, 2}] = cy::ansi::colour::rgb(128, 255, 0);
    b1[{0, 3}] = cy::ansi::colour::rgb(255, 0, 0);
    b1[{1, 2}] = cy::ansi::colour::rgb(0, 127, 255);
    b1[{1, 3}] = cy::ansi::colour::rgb(255, 0, 0);

    b1[{4, 4}] = cy::ansi::colour::rgb(255, 0, 0);

    cy::ansi::draw_bitmap(vp, {1, 1}, b1, cy::ansi::colour::terminal_default, cy::ansi::c_1x2);

    cy::ansi::draw_bitmap(vp, {10, 1}, b1, cy::ansi::colour::terminal_default, cy::ansi::c_2x4);

    cy::ansi::draw_bitmap(vp, {16, 0}, b1, cy::ansi::colour::terminal_default, cy::ansi::c_1x1);

    cy::ansi::draw_bitmap(vp, {22, 0}, b1, cy::ansi::colour::terminal_default, cy::ansi::c_w2x1);
}

void test_event_loop()
{
    std::cout << std::endl;
    cy::ansi::window vp({40, 10});
    cy::ansi::style s1{.bg = cy::ansi::colour::blue};
    cy::ansi::fill_rect(vp, {s1, ' '}, {0, 0}, {40, 10});

    vp.flush();

    cy::ansi::run([&](const cy::ansi::event &event) {
        if (cy::ansi::key_press e{event})
        {
            if (e.key() == 'x')
            {
                return cy::ansi::event_return::exit_loop;
            }
        }
        else if (cy::ansi::mouse_click e{event})
        {
            vp.go_to({3, 6});
            vp.text("Mouse click " + std::to_string(e.pos().x) + "," + std::to_string(e.pos().y) + "  ");
            vp.flush();
        }
        else if (cy::ansi::mouse_move e{event})
        {
            vp.go_to({3, 5});
            vp.text("Mouse moved " + std::to_string(e.pos().x) + "," + std::to_string(e.pos().y) + "  ");
            vp.flush();
        }
        return cy::ansi::event_return::continue_loop;
    });
}

void test_widgets()
{
    std::cout << std::endl;

    ancy::window vp({40, 10});
    ancy::style s1{.bg = cy::ansi::colour::blue};
    ancy::fill_rect(vp, {s1, ' '}, {0, 0}, {40, 10});

    int count = 0;
    ancy::theme th;
    ancy::text_box t1(vp, {10, 1}, {10, 1}, s1, th, "Welcome to widgets");
    ancy::text_box t2(vp, {10, 3}, {10, 1}, s1, th, "Press SPACE to count");
    ancy::text_box t2a(vp, {10, 4}, {10, 1}, s1, th, "Q to quit");
    ancy::text_box t2b(vp, {10, 5}, {10, 1}, s1, th, "R to reset");
    ancy::text_box t3(vp, {2, 7}, {10, 1}, s1, th, "");

    auto redraw = [&] { t3.set_text(std::format("You have pressed {} times ", count)); };

    ancy::key_command cmd1(vp, ' ', [&] {
        ++count;
        redraw();
    });
    ancy::key_command cmd1a(vp, 'r', [&] {
        count = 0;
        redraw();
    });
    ancy::key_command cmd2(vp, 'q', vp.quit_action());

    vp.run();
}

void test_alt_screen()
{
    ancy::window vp;
    ancy::style s1{.bg = cy::ansi::colour::blue};
    // ancy::fill_rect(vp, {s1, ' '}, {0,0}, vp.dimensions());
    ancy::fill_rect(vp, {s1, ' '}, {0, 0}, vp.dimensions());

    ancy::theme th;
    ancy::text_box t(vp, {5, 2}, {10, 1}, th.text, th, "Testing alt screen");
    ancy::text_box t2(vp, {5, 7}, {10, 1}, th.text, th, "Press x");

    ancy::key_command cmd1(vp, 'x', [&] { t2.set_text("You pressed x"); });
    ancy::move_command m(vp, [&](ancy::position p, ancy::mouse_flags f) {
        t2.set_text(std::format("Mouse moved to {},{}  ", p.x, p.y));
    });

    vp.run();
}

void test_mouse_coords()
{
    ancy::window vp({40, 5});
    ancy::style s1{.bg = cy::ansi::colour::blue, .bold = true};
    // ancy::fill_rect(vp, {s1, ' '}, {0,0}, vp.dimensions());
    ancy::fill_rect(vp, {s1, ' '}, {0, 0}, vp.dimensions());

    ancy::theme th;
    ancy::text_box t(vp, {5, 1}, {25, 1}, th.text, th, "Testing mouse_coords");
    ancy::text_box t2(vp, {5, 3}, {25, 1}, th.text, th, "Press x");

    ancy::key_command cmd1(vp, 'x', [&] { t2.set_text("You pressed x"); });
    ancy::move_command m(vp, [&](ancy::position p, ancy::mouse_flags f) {
        t2.set_text(std::format("Mouse moved to {},{}  ", p.x, p.y));
    });

    vp.run();
}

void test_button()
{
    ancy::theme th;
    ancy::window vp({40, 5});
    ancy::style s1{.bg = cy::ansi::colour::blue, .bold = true};
    // ancy::fill_rect(vp, {s1, ' '}, {0,0}, vp.dimensions());
    ancy::fill_rect(vp, {s1, ' '}, {0, 0}, vp.dimensions());
    ancy::style button_normal{.fg = ancy::colour::red, .bg = ancy::colour::white};
    ancy::style button_highlight{.fg = ancy::colour::red, .bg = ancy::colour::yellow};

    ancy::style x_normal{.fg = ancy::colour::white, .bg = ancy::colour::red};
    ancy::style x_highlight{.fg = ancy::colour::white, .bg = ancy::colour::red, .bold = true};

    ancy::style text_normal{.fg = ancy::colour::yellow};

    ancy::button b2(vp, {39, 0}, {1, 1}, 'x', "X", x_normal, x_highlight, vp.quit_action());

    ancy::text_box t1(vp, {20, 1}, {10, 1}, th.data_text, th, "The sly fox jumps over the lazy dog");
    ancy::text_box t2(vp, {20, 3}, {10, 1}, th.data_text, th, "The sly fox jumps over the lazy dog");
    std::string value = "";
    char c = 'a';
    ancy::button b1(vp, {3, 1}, {10, 1}, 'c', "Clear", button_normal, button_highlight, [&] {
        value = "";
        t1.set_text(value);
    });
    ancy::button b3(vp, {3, 3}, {10, 1}, 'u', "Update", button_normal, button_highlight, [&] {
        value += c;
        c++;
        if (c > 'z')
            c = 'a';
        t1.set_text(value);
    });
    vp.run();
}

void test_input_box()
{
    const auto &th = ancy::default_theme();
    ancy::window w({40, 5});
    ancy::fill_rect(w, {th.text}, {0, 0}, w.dimensions());

    ancy::text_box t1(w, {1,3}, {20,1}, th.text, th, "");
    ancy::text_input i1(w, {1, 1}, {11, 1}, th.text_input, th.button_normal, th.button_focus, th.disabled_text, "",
                        "First name", 
                        [&] { }, [&] { t1.set_text("Your name is " + i1.get_text()); });

    ancy::text_input i2(w, {15, 1}, {11, 1}, th.text_input, th.button_normal, th.button_focus, th.disabled_text, "",
                        "Surname", 
                        [&] { t1.set_text("Your surname is " + i2.get_text()); }, [&] { });

    auto update = [&]
    {
        t1.set_text(std::format("Name: {} {}", i1.get_text(), i2.get_text()));
    };

    auto check = [&]
    {
        t1.set_text("Check passed");
    };

    i1.on_changed = update;
    i2.on_changed = update;
    i1.on_enter = [&] { w.set_focus(i2); };

    ancy::button check_button(w, {30, 1}, {7,1}, 'C', "Check", th.x_normal, th.x_highlight, check);
    ancy::button ok(w, {30, 3}, {7,1}, 'O', "Ok", th.x_normal, th.x_highlight, w.quit_action());

    i2.on_enter = [&] { w.set_focus(ok); };

    w.run();
}

int main(int argc, const char *argv[])
{
    return cy::test(argc, argv,
                    {test_raw, test_raw_writer, test_window_writer, test_progress_bar, test_graphics, test_event_loop,
                     test_widgets, test_alt_screen, test_mouse_coords, test_button, test_input_box});
}
