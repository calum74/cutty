// Illustrates use of scope_hooks() to hook scope entry, exit and exceptions.

#include <cutty/scope_hooks.hpp>
#include <cutty/print.hpp>

namespace cy = cutty;

int main()
{
    // Define our entry and exit hooks using scope_hooks().
    // This would normally be defined in a header file, or be passed in some kind of context.
    constexpr auto event_logger1 =
        cy::scope_hooks([] { cy::print("Entering function"); }, [] { cy::print("Exiting function"); });

    {
        // We can then call operator() on our scope_hooks object to construct a scope guard.
        // The scope guard calls the hooks on scope entry and exit.
        auto guard = event_logger1();
        cy::print("Running example 1");

        // Output:
        // Entering function
        // Running example 1
        // Exiting function
    }

    // We can add arbitrary parameters to each function
    constexpr auto event_logger2 =
        cy::scope_hooks([](const char *name, int id) { cy::print("Entering", name, id); },
                        [](const char *name, int id) { cy::print("Exiting", name, id); });

    {
        // If the functions require parameters, then they are passed in when we construct the scope guard.
        // The parameters are passed to both the entry and exit functions.
        auto guard = event_logger2("example", 2);
        cy::print("Running example 2");

        // Outputs:
        // Entering example 2
        // Running example 2
        // Exiting example 2
    }

    {
        // Events logging can be nested, even within the same scope:
        auto guard1 = event_logger2("example (outer)", 3);
        auto guard2 = event_logger2("example (inner)", 3);
        cy::print("Running example 3");

        // Outputs:
        // Entering example (outer) 3
        // Entering example (inner) 3
        // Running example 3
        // Exiting example (inner) 3
        // Exiting example (outer) 3
    }

    // Functors can be ... arguments, for example
    auto varargs_logger = cy::scope_hooks(
        [](auto &&...args) {
            cy::print("Entered", args...);
        },
        [](auto &&...args) {
            cy::print("Exited", args...);
        });

    {
        auto guard = varargs_logger("example 4:", 1, 2.0, "3");
        cy::print("Running example 4");

        // Outputs:
        // Entered example 4: 1 2 3
        // Running example 4
        // Exited example 4: 1 2 3
    }

// You can define macros to make scope_hooks easier to use
#define BINLOG(ARGS, ...) auto binguard = varargs_logger(__FUNCTION__, ":", __LINE__, ":", __VA_ARGS__)

    {
        BINLOG("Example ", 5);
        cy::print("Running example 5");
        // Outputs:
        // Entered main:83: Example 5
        // Running example 5
        // Exited main:83: Example 5
    }

    // Exceptions are logged as scope exits
    try
    {
        auto guard = event_logger2("example ", 6);
        cy::print("Running example");
        throw std::exception();
    }
    catch (std::exception &)
    {
    }
    // Outputs:
    // Entering example  6
    // Running example 6
    // Exiting example  6

    // You can specify a separate function to hook exceptional scope exits:
    constexpr auto event_logger3 =
        cy::scope_hooks([](const char *name, int id) { cy::print("Entering", name, id); },
                        [](const char *name, int id) { cy::print("Exiting", name, id); },
                        [](const char *name, int id) { cy::print("Exception", name, id); });

    try
    {
        auto guard = event_logger3("example ", 7);
        cy::print("Running example 7");
        throw std::exception();
    }
    catch (std::exception &)
    {
    }
    // Outputs:
    // Entering example  7
    // Running example 7
    // Exception example  7
}
