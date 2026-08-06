#include <cutty/ansi/events.hpp>
#include <cutty/ansi/raw.hpp>

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

namespace ancy = cutty::ansi;

namespace
{
class RawEventsSetup
{
  public:
    RawEventsSetup(std::ostream &os) : m_os(os)
    {
        tcgetattr(STDIN_FILENO, &m_old);

        termios raw = m_old;
        cfmakeraw(&raw);

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

        m_os << "\x1b[?1003h"
             << "\x1b[?1006h" << std::flush;
    }

    ~RawEventsSetup()
    {
        m_os << "\x1b[?1003l"
             << "\x1b[?1006l" << std::flush;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_old);
    }

    std::ostream &m_os;
    termios m_old;
};
} // namespace

void ancy::run(const std::function<event_return(event)> &fn)
{
    RawEventsSetup setup(std::cout);

    unsigned char c;

    int state = 0;
    /*
        States:
            0:
            1: 27
            2: 27 [
            3: 27 [ <
    */

    // first things first - Send a device status report
    // ESC [ 6 n
    // Terminal replies with
    // ESC [ row ; column R
    // This lets us know where the cursor is so we can calculate mouse moves relative to
    // our window.

    auto initial_position = read_position(std::cout, std::cin);
    // std::cout << "Cursor X=" << initial_position.x << " Y=" << initial_position.y << "\r\n" << std::flush;

    int n;
    int num_params = 0;
    int params[3];

    event e;

    auto send_event = [&] { return fn(e) == event_return::exit_loop; };

    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        switch (state)
        {
        case 0: // (Start of line)
            if (c == 27)
            {
                state = 1;
            }
            else
            {
                e.type = event_type::key_press;
                e.key = c;
                if (send_event())
                    return;
                // Early exit if you press 'q':
                // if (c == 'q')
                //    return;
                if (c == 3)  // Ctrl+C
                    return;
                state = 0;
            }
            break;
        case 1: // 27
            if (c == '[')
            {
                state = 2;
            }
            else if( c == 27)
            {
                // Force exit double escape
                return;
            }
            else
            {
                state = 0;
            }
            break;
        case 2: // 27 [
            if (c == '<')
            {
                state = 3;
                n = 0;
                num_params = 0;
            }
            else
            {
                e.type = event_type::key_press;
                switch (c)
                {
                case 'A':
                    e.key = key::UP;
                    break;
                case 'B':
                    e.key = key::DOWN;
                    break;
                case 'C':
                    e.key = key::RIGHT;
                    break;
                case 'D':
                    e.key = key::LEFT;
                    break;
                default:
                    break;
                }
                state = 0;
                if (send_event())
                    return;
            }
            break;
        case 3:
            // 27 [ <
            // We have a sequence of semi-separated numbers followed by M
            if (c == ';')
            {
                if (num_params < 3)
                {
                    params[num_params] = n;
                    n = 0;
                    ++num_params;
                }
                else
                {
                    state = 0;
                }
            }
            else if (isdigit(c))
            {
                n = n * 10 + c - '0';
            }
            else
            {
                // Dump
                // for (int i = 0; i < num_params; ++i)
                // {
                //     std::cout << params[i] << ';';
                // }
                // std::cout << n << char(c) << "\r\n" << std::flush;

                if (c == 'M')
                {
                    // Mouse move or click
                    // param[0] indicates which buttons are pressed
                    // param[1] is the X
                    // param[2] is the Y

                    // 0 means just a click (no movement)
                    // +32 = mouse movement
                    // +3 = not buttons pressed (cursor movement)

                    // +4 = shift key
                    // +8 = option key
                    // +16 = CTRL key
                    // +64 = scroll up
                    // +65 = scroll down
                    e.key = params[0];
                    e.type = params[0] & 32 ? event_type::mouse_move : event_type::mouse_click;
                    e.x = params[1] - initial_position.x;
                    e.y = n - initial_position.y;
                    if (send_event())
                        return;
                }
                else if (c == 'm')
                {
                    // Mouse release
                    e.type = event_type::mouse_release;
                    e.key = params[0];
                    e.x = params[1] - initial_position.x;
                    e.y = n - initial_position.y;
                    if (send_event())
                        return;
                }
                else
                {
                    // std::cout << "Unknown: ";
                    // for (int i = 0; i < num_params; ++i)
                    // {
                    //     std::cout << params[i] << ';';
                    // }
                    // std::cout << n << char(c) << "\r\n" << std::flush;
                    // Unknown: drop
                }
                state = 0;
            }
            break;

        default:
            return;
        }
    }
}

ancy::key_press::key_press(const event &e) : m_key(e.type == event_type::key_press ? e.key : 0)
{
}

ancy::key_press::operator bool() const
{
    return m_key;
}

int ancy::key_press::key() const
{
    return m_key;
}


ancy::mouse_click::mouse_click(const event &e) : mouse_event(e.type == event_type::mouse_click ? mouse_flags{.valid=true, .click=true} : mouse_flags{.valid=false}, {e.x, e.y})
{
}

ancy::mouse_scroll::mouse_scroll(const event &e) : mouse_event(e.type == event_type::mouse_scroll ? mouse_flags{.valid=true, .scroll=true} : mouse_flags{.valid=false}, {e.x, e.y})
{
}


ancy::mouse_event::operator bool() const
{
    return m_flags.valid;
}

ancy::position ancy::mouse_event::pos() const
{
    return m_position;
}

ancy::mouse_move::mouse_move(const event &e) : mouse_event(e.type == event_type::mouse_move ? mouse_flags{.valid=true, .move=true} : mouse_flags{.valid=false}, {e.x, e.y})
{
}

ancy::mouse_release::mouse_release(const event &e) : mouse_event(e.type == event_type::mouse_release ? mouse_flags{.valid=true, .release=true} : mouse_flags{.valid=false}, {e.x, e.y})
{
}


ancy::mouse_event::mouse_event(mouse_flags f, position p) : m_flags(f), m_position(p)
{
}

ancy::mouse_flags ancy::mouse_event::flags() const
{
    return m_flags;
}
