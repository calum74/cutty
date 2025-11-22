// Illustrates use of scope_hooks() to hook scope entry, exit and exceptions.

#include <cutty/scope_hooks.hpp>

#include <iostream>

namespace cy = cutty;

int main()
{
    // Define our entry and exit hooks using scope_hooks().
    // This would normally be defined in a header file, or be passed in some kind of context.
    constexpr auto event_logger1 =
        cy::scope_hooks([] { std::cout << "Entering function\n"; }, [] { std::cout << "Exiting function\n"; });

    {
        // We can then call operator() on our scope_hooks object to construct a scope guard.
        // The scope guard calls the hooks on scope entry and exit.
        auto guard = event_logger1();
        std::cout << "Running example 1\n";

        // Output:
        // Entering function
        // Running example 1
        // Exiting function
    }

    // We can add arbitrary parameters to each function
    constexpr auto event_logger2 =
        cy::scope_hooks([](const char *name, int id) { std::cout << "Entering " << name << " " << id << "\n"; },
                        [](const char *name, int id) { std::cout << "Exiting " << name << " " << id << "\n"; });

    {
        // If the functions require parameters, then they are passed in when we construct the scope guard.
        // The parameters are passed to both the entry and exit functions.
        auto guard = event_logger2("example", 2);
        std::cout << "Running example 2\n";

        // Outputs:
        // Entering example 2
        // Running example 2
        // Exiting example 2
    }

    {
        // Events logging can be nested, even within the same scope:
        auto guard1 = event_logger2("example (outer)", 3);
        auto guard2 = event_logger2("example (inner)", 3);
        std::cout << "Running example 3\n";

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
            std::cout << "Entered ";
            (std::cout << ... << args) << std::endl;
        },
        [](auto &&...args) {
            std::cout << "Exited ";
            (std::cout << ... << args) << std::endl;
        });

    {
        auto guard = varargs_logger("example 4: ", 1, ' ', 2.0, " 3");
        std::cout << "Running example 4\n";

        // Outputs:
        // Entered example 4: 1 2 3
        // Running example 4
        // Exited example 4: 1 2 3
    }

// You can define macros to make scope_hooks easier to use
#define BINLOG(ARGS...) auto binguard = varargs_logger(__FUNCTION__, ":", __LINE__, ": ", ARGS)

    {
        BINLOG("Example ", 5);
        std::cout << "Running example 5\n";
        // Outputs:
        // Entered main:83: Example 5
        // Running example 5
        // Exited main:83: Example 5
    }

    // Exceptions are logged as scope exits
    try
    {
        auto guard = event_logger2("example ", 6);
        std::cout << "Running example 6\n";
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
        cy::scope_hooks([](const char *name, int id) { std::cout << "Entering " << name << " " << id << "\n"; },
                        [](const char *name, int id) { std::cout << "Exiting " << name << " " << id << "\n"; },
                        [](const char *name, int id) { std::cout << "Exception " << name << " " << id << "\n"; });

    try
    {
        auto guard = event_logger3("example ", 7);
        std::cout << "Running example 7\n";
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
