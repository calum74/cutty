# ANSI

Sample: [../samples/ansi.cpp]

Create attractive terminal-based apps, UIs, and simplify common tasks.

```c++
int main()
{
    cy::ansi::window w{{40,5}};
}
```

# Tutorial

The ANSI library is used to output colours and create attractive and easy to use text interfaces. Whilst outputting ANSI is not difficult per se, it is still useful to package this functionality.

The library is divided into:

1. The raw low level API. This is to simply send ANSI commands directly to the console.
2. The window API. This manages full screen or text output areas.
3. ANSI widgets.
4. TUI applications.

## Header file and namespace

```c++
#include <cutty/ansi.hpp>

namespace cy = cutty;
```

## Low level API

The low level API outputs ANSI sequences directly to a `std::ostream`. It does not track the current state of the terminal.

Resetting the terminal:

```c++
    cy::ansi::reset(std::cout);
```

Change colour and weight:

```c++
    cy::ansi::bold_on(std::cout);
    cy::ansi::fg_colour(cy::ansi::colour{255,128,0});
```

Colour spaces:

A colour space is used to specify how colours should be rendered, because not all terminals support colour, and colours can be turned off.

- `cs_ascii` - colour and weight information is not used
- `cs_ansi` - basic ansi colouring only
- `cs_666` - 6x6x6 colour cube is used
- `cs_rgb` - full 256 level RGB colours are used

### Styles

Styles:

```c++
    cy::ansi::change_style({}, s2, {}, std::cout);
```


The class `cy::ansi::style` describes how a character should be rendered:

- `fg` - the foreground colour
- `bg` - the background colour
- `bold` - if the text is heavy
- `faint` - if the text is light
- `underline` - if the text is underlined
- `strikethrough` if the text is strikethrough

## Unbuffered API

The class `cy::ansi::raw_writer` tracks basic information such as the cursor position and the current style. It writes directly to a `std::ostream` but you may still need to call `flush()` to ensure that changes are written to the console.

`raw_writer` does not manage a text area, but the cursor can be moved relative to its initial position which is assumed to be at the start of a new line.

```c++
// Create a raw writer, defaulting to std::cout
cy::ansi::raw_writer w;
```

### Characters

The `cy::ansi::character` represents a unicode character with a style. The `ch` field represents the code point, and the `style` field stores its style.

```c++
// Put a single character at position 10,10
w.put({.ch='x'});
w.put({.ch='y'}, {10,10});

// Set output style
w.style(s1);

// Write text in current style
w.text("Hello");

// Write text in a specific style
w.text("Hej", s2);

// Move to a given position
w.go_to({0,0});
w.text("      ");

// Remember to flush
w.flush();
```

## Window API

The window API allows you to create a text area on the screen for writing characters. The `cy::ansi::window` class is buffered and only outputs when the `flush()` methods is called, and only outputs the differences since the previous `flush()`.

Characters drawn outside of the window are are clipped.


