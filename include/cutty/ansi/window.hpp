#pragma once

#include "events.hpp"
#include "widget.hpp"
#include "writer.hpp"

#include <memory>

namespace cutty::ansi
{

class window : public writer, public widget
{
  public:
    window(); // Alt-screen mode

    explicit window(size s, std::ostream &os = std::cout);

    ~window();

    void put(const character &ch, position p) override;

    void flush() override;

    void reset() override;

    void text(std::string_view) override;

    void text(const style &, std::string_view) override;

    void apply(const style &) override;

    void go_to(position) override;

    void endl() override;

    void put(const character &) override;

    size dimensions() const override;
    position current_position() const override;

    writer &get_writer() override;

    void run();

    void add_child(widget &w) override;

    void remove_child(widget &w) override;

    void key_press(char32_t ch) override;
    void mouse_click(position p, mouse_flags) override;
    void mouse_release(position p, mouse_flags) override;
    void mouse_move(position p, mouse_flags) override;
    void mouse_scroll(position p, mouse_flags) override;
    void hide_cursor() override;
    void show_cursor(position p) override;
    void show_cursor() override;

    void set_focus(widget &child) override;
    void next_focus() override;
    void prev_focus() override;

    // Signal to the event loop to exit
    void quit();

    bool has_quit() const;

    std::function<void()> quit_action();

  private:
    class impl;
    std::unique_ptr<impl> m_impl;

};
} // namespace cutty::ansi