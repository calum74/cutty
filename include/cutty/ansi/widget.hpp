#pragma once

#include "writer.hpp"

namespace cutty::ansi
{

class widget
{
  public:
    widget();
    widget(widget &parent);
    virtual ~widget();
    virtual void draw(writer &vp);
    virtual void key_press(char32_t k);
    virtual void mouse_click(position p);
    virtual void mouse_release(position p);
    virtual void mouse_move(position p);

    virtual void add_child(widget &child);
    virtual void remove_child(widget &child);
    protected:
    widget * m_parent;
};



}