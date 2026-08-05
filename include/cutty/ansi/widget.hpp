#pragma once

#include "writer.hpp"
#include "events.hpp"

namespace cutty::ansi
{

class widget
{
  public:
    widget();
    widget(const widget&) = delete;
    widget(widget *parent);
    widget(widget &parent, position p, size s);
    virtual ~widget();
    virtual void draw(writer &vp);
    virtual void key_press(char32_t k);
    virtual void mouse_click(position p, mouse_flags);
    virtual void mouse_release(position p, mouse_flags);
    virtual void mouse_move(position p, mouse_flags);
    virtual void mouse_scroll(position p, mouse_flags);

    virtual void add_child(widget &child);
    virtual void remove_child(widget &child);
    virtual writer &get_writer();
    protected:
    widget * m_parent;
    position m_position;
    size m_size;
};

}