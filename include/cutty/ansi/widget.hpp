#pragma once

#include "events.hpp"
#include "theme.hpp"
#include "writer.hpp"

namespace cutty::ansi
{

class widget
{
  public:
    widget();
    widget(const widget &) = delete;
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
    bool mouse_hit(position) const;

    // Window methods
    virtual void set_focus(widget &child);
    virtual void next_focus();
    virtual void prev_focus();

    // Input methods
    virtual bool can_take_focus();
    virtual void grant_focus(bool has_focus);

  protected:
    widget *m_parent;
    position m_position;
    size m_size;
};

} // namespace cutty::ansi