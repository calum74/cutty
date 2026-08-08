#pragma once

#include "character.hpp"
#include "common.hpp"

#include <iostream>
#include <string_view>

namespace cutty::ansi
{
class writer
{
  public:
    virtual ~writer() = default;

    virtual void reset() = 0;

    // Writes the string to the output using the current style
    virtual void text(std::string_view str) = 0;

    // Writes the string to the current position using the given style
    virtual void text(const ansi::style &s, std::string_view str) = 0;

    // Changes the current style
    virtual void apply(const ansi::style &s) = 0;

    // Moves to a specific position relative to the initial position
    virtual void go_to(position p) = 0;

    // Puts a character at the current position
    virtual void put(const character &ch) = 0;

    // Puts a character at the given position
    virtual void put(const character &ch, position p) = 0;

    // Move current position to the next line
    virtual void endl() = 0;

    // Output all buffered changes
    virtual void flush() = 0;

    // Gets the size of the terminal, or the size of the viewport
    virtual size dimensions() const = 0;

    // Gets the current position of the cursor on the screen
    virtual position current_position() const = 0;

    // Hide the cursor
    virtual void hide_cursor() = 0;

    // Shows the cursor at the specified position
    virtual void show_cursor(position) = 0;

    virtual void show_cursor() = 0;
};

class raw_writer : public writer
{
  public:
    explicit raw_writer(std::ostream &os = std::cout);

    void reset() override;

    void text(std::string_view str) override;

    // Writes the string to the current position using the given style
    void text(const ansi::style &s, std::string_view str) override;

    // Changes the current style
    void apply(const ansi::style &s) override;

    // Moves to a specific position relative to the initial position
    void go_to(position p) override;

    // Puts a character at the current position
    void put(const character &ch) override;

    // Puts a character at the given position
    void put(const character &ch, position p) override;

    // Move current position to the next line
    void endl() override;

    // Output all buffered changes
    void flush() override;

    // Gets the size of the terminal, or the size of the viewport
    size dimensions() const override;

    position current_position() const override;

    void hide_cursor() override;

    void show_cursor(position p) override;

    void show_cursor() override;

  private:
    std::ostream &os;

    // The current state of the terminal
    ansi::style current_style;

    // The current x,y of the terminal relative to the initial position
    position m_position;
};

} // namespace cutty::ansi