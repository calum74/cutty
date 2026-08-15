#pragma once

#include <cutty/ansi/common.hpp>

#include <functional>

namespace cutty::ansi
{
enum class event_type
{
    key_press,
    mouse_move,
    mouse_click,
    mouse_release,
    mouse_scroll
};

struct event
{
    event_type type;
    int key, x, y;

    bool is_key_press() const;
    bool is_mouse() const;
    bool is_mouse_move() const;
    bool is_mouse_click() const;
    bool is_mouse_release() const;

    bool shift_key() const;
    bool ctrl_key() const;
    bool option_key() const;
};

struct mouse_flags
{
    bool valid : 1 = false;
    bool click : 1 = false;
    bool scroll : 1 = false;
    bool move : 1 = false;
    bool release : 1 = false;
};

class key_press
{
public:
    key_press(const event &e);
    explicit operator bool() const;
    int key() const;
private:
    int m_key;
};

class mouse_event
{
public:
    mouse_event(mouse_flags flags, position p);

    position pos() const;
    bool shift();
    bool ctrl() const;
    bool option() const;

    explicit operator bool() const;

    mouse_flags flags() const;

protected:
    mouse_flags m_flags;
    position m_position;
};

class mouse_click : public mouse_event
{
public:
    mouse_click(const event &e);
};

class mouse_release : public mouse_event
{
public:
    mouse_release(const event&e);
};

class mouse_move : public mouse_event
{
public:
    mouse_move(const event &e);
};

class mouse_scroll : public mouse_event
{
public:
    mouse_scroll(const event &e);
};


enum key
{
    TAB = '\t',
    // RET = 13,
    ENTER = '\r',
    ESC = 27,
    UP = 201,
    DOWN = 202,
    RIGHT = 203,
    LEFT = 204
};



enum event_return
{
    continue_loop,
    exit_loop
};

void run(const std::function<event_return(event)>&fn);

}
