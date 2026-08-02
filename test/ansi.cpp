#include <cutty/ansi.hpp>
#include <cutty/test.hpp>

#include <thread>
#include <chrono>

namespace cy = cutty;

void test_direct()
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
    s1.fg = cy::ansi::red;
    s1.bg = cy::ansi::blue1;

    cy::ansi::change_style({}, s1, {}, std::cout);
    std::cout << "Bold in red";
    cy::ansi::reset(std::cout);
    std::cout << std::endl;

}

void test_unbuffered()
{
    std::cout << std::endl;
    cy::ansi::raw_writer w(std::cout);

    cy::ansi::style s1, s2;
    s1.bold = true;
    s1.fg = cy::ansi::red;
    s1.bg = cy::ansi::blue1;

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

void test_buffered()
{
    // Writing outside of the region clips the output


    std::cout << std::endl;
    cy::ansi::window vp({40, 5});

    vp.put({.ch='x'}, {3,0});
    vp.put({.ch='y'}, {4,1});
    vp.flush();

    cy::ansi::style s1, s2;
    s1.bold = true;
    s1.fg = cy::ansi::red;
    s1.bg = cy::ansi::blue1;

    vp.put({s1, 'X'}, {.x=0, .y=0});
    vp.put({s2, ' '}, {0, 0});
    vp.put({.ch='+'}, {39, 0});

    s2.fg = cy::ansi::green;

    draw_box(vp, s2, cy::ansi::line_style::ascii, 10, 2, 10, 3);
}

void test_progress_bar()
{
    std::cout << std::endl;
    cy::ansi::window vp({40, 10});

    cy::ansi::style s1;
    s1.fg = cy::ansi::red;
    s1.bg = cy::ansi::white;
    draw_progress(vp, s1, 10, 0, 20, 0, 100);
    draw_progress(vp, s1, 10, 1, 20, 50, 100);
    draw_progress(vp, s1, 10, 2, 20, 205, 400);
    draw_progress(vp, s1, 10, 3, 20, 208, 400);
    draw_progress(vp, s1, 10, 5, 20, 210, 400);

    draw_progress(vp, s1, 10, 9, 20, 100, 100);
    vp.flush();

    cy::ansi::style s2;

    for(int i=0; i<=1000; ++i)
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

    b1[{0,0}] = cy::ansi::colour{128, 255, 0};
    b1[{0,1}] = cy::ansi::colour{255,0,0};
    b1[{1,0}] = cy::ansi::colour{0, 127, 255};
    b1[{1,1}] = cy::ansi::colour{255,0,0};

    b1[{0,2}] = cy::ansi::colour{128, 255, 0};
    b1[{0,3}] = cy::ansi::colour{255,0,0};
    b1[{1,2}] = cy::ansi::colour{0, 127, 255};
    b1[{1,3}] = cy::ansi::colour{255,0,0};

    b1[{4,4}] = cy::ansi::colour{255,0,0};

    cy::ansi::draw_bitmap(vp, {1,1}, b1, cy::ansi::terminal_default, cy::ansi::c_1x2);

    cy::ansi::draw_bitmap(vp, {10,1}, b1, cy::ansi::terminal_default, cy::ansi::c_2x4);

    cy::ansi::draw_bitmap(vp, {16,0}, b1, cy::ansi::terminal_default, cy::ansi::c_1x1);

    cy::ansi::draw_bitmap(vp, {22,0}, b1, cy::ansi::terminal_default, cy::ansi::c_w2x1);
}

void test_event_loop()
{
    std::cout << std::endl;
    cy::ansi::window vp({40, 10});
    cy::ansi::style s1 {.bg = cy::ansi::blue1 };
    cy::ansi::fill_rect(vp, {s1, ' '}, {0,0}, {40,10});

    vp.flush();

    cy::ansi::run([&](const cy::ansi::event &event)
    {
        if(cy::ansi::key_press e{event})
        {
            std::cout << "Key press: " << e.key() << "\r\n" << std::flush;
        }
        else if(cy::ansi::mouse_click e{event})
        {
            std::cout << "Click: " << e.pos().x << "," << e.pos().y << "\r\n" << std::flush;
        }
        return cy::ansi::event_return::continue_loop;
    }); 
}

int main()
{
    return cy::test({test_direct, test_unbuffered, test_buffered, test_progress_bar, test_graphics, test_event_loop});
}
