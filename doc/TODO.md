# ANSI library

Mouse events in the raw loop.
- Anchor the mouse into the window

Tidy up alt mode
- ostream
- cursor positioning



Next:
- Text widget that prints the x and y cursor positions
- A button that can be clicked on
- A button that changes colour when hovered
- A button that has a shortcut key?

Next steps:
1. Full screen alt mode
2. Mouse and keyboard input
3. Raw terminal mode

Widgets:
- coutdown
- timer
- time of day



class alt_screen
{
};

class raw_mode
{
};

Split into:
ansi/raw.h/.cpp
ansi/writer.h/.cpp
ansi/widgets.h/.cpp
ansi/event_loop

- Working in a reduced colour space
- Working in different default style

- Widgets
    - Layout

- Full screen support, and alt-screen
- Screen width and height
- mouse and keyboard input support
  - event loop and timer

- Bitmap graphics using half chars
- How to render a bitmap?
  (a) with default background
  (b) using background colour as well

Bitmap drawing
1. Gather:
    - The proportion of the cell that a Braille pixel uses
    - The default background colour if using it
2. identify the pixel with the max luminosity
3. identify the pixel with the minimum luminosity
4. Classify all pixels as "on" or "off" depending on their proximity to
   the min and max.
5. Identify the average "off" colour. This is your background.
6. Identify the average "on" colour.
7. Set the fg colour to cancel out the bg colour as much as possible.

Render a mandelbrot set
Render images generally

What about a generic map 4x4 to a character? Then we could make use of a variety of different characters?

Widgets
    Charts...
        Progress bars
        Build progress
    Box drawing
    Lines
    Pixels
    Mapped viewport & regions
    Clipping

## Build progress

Progress:    ██████████▌        95%
Remaining:   12s
Done:        ⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹ ⣻⣽⣹⣻⣽⣹⣻⣽ ⣹⣻⣽
In progress: ⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹ ⣻⣽⣹⣻⣽⣹⣻⣽ ⣹⣻⣽
Ready:      ⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹ ⣻⣽⣹⣻⣽⣹⣻⣽ ⣹⣻⣽
Waiting umnet:

C++:       ⣹⣻⣽⣹⣻⣽⣹⣻⣽⣹
Linker:    ⣹⣻
CPU:       ⣼⣼
Graph:     checkmark foo.lib  ⣽⣹⣻
Drawn as layers. Each target is either green (done), grey (not done) or red (working)

Progress: Only a progress bar
Ultra-compact:  Drawn on one line with three groups in Braille
Compact: Drawn on three lines, still
Labelled layers: Grouped text on each layer. Use dots where too much text. Use first letter of target

Process-based. Shows list of all running targets, and a graph showing what they generate



Profile output:
- Which targets are the slowest




# Dynamic
- Mixins?
- i++ not implemented
- at not implemented
- Not all operators implemented:
    << >> |
- Operators maybe not in cutty namespace, but should be members of dynamic?
- reverse iterators and list containers fail due to operator + not working as advertised on.
- ?? Optional
- ?? Assignment from optional ??
- ?? Assignment from variant ??
- What about functions exported as C functions dynamic(*)(dynamic, ...) etc?
- Functions should not take const reference?

# Units
- Unify scalar and dscalar
- Bug converting $/mile int o £/km
- Literals prevent mixins

# Tensors
- Fixed-size vs variable-size tensor
- Fixed-rank vs variable-rank tensor

# Tags
- [ ] Get rid of fraction, and use the builtin `kilo`, etc.

# What about a "indexer" that can be assignable

```c++
template<typename Container, typename Key>
struct index
{

};

index_type operator[](const key_type&);
```



# Docs
- [ ] Separator output

# Print
- [ ] Make it configurable
print_opts
- beg = ""
- sep = " "
- end = "\n"

- Compare with std::string.
- Test suite, docs, samples

# Tests
- [ ] Test should allow command line options
- Run specific tests, selected by number
- Specify a format string?

- Should we have a "test case" local variable, and use satellite to specify it?

# Command line argument parser

# Sequence
Deprecate filters etc.
Maybe based on mixins
Rename pointer_sequence
Maybe typedef to template<typename T> using sequence = const sequence_base<T>&
Output sequence is a bit weird as it's not a sequence.
In/out sequence?
What about a movable sequence to ensure no reuse/reentry?

# Dynamic
Get map test passing
Finish stuff
Strings that are char*
Find a use case

# Projects to add
Entropy
Numbers and traits
Parsing

# Persist
- Rewrite persist so that the pointer is to map_file and not shared_record
- Think about allocators, for example we moved the file descriptor into the shared memory. Could we use satellite instead?
- Problem is growing the heap.
