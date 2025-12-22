#include <cutty/check.hpp>
#include <cutty/with.hpp>

namespace cy = cutty;

struct tag1;
struct tag2;

int main()
{
    {
        // Getting and setting the same value
        cy::check(!cy::try_get<int>());
        int x = 42;
        cy::with<int> s = x;
        cy::check(cy::try_get<int>());
        cy::check(cy::get<int>() == 42);
    }

    {
        // Nested getters and setters, multiple tags
        cy::check(!cy::try_get<int, tag1>());
        cy::check(!cy::try_get<int, tag2>());
        int v1 = 10, v2 = 12;
        cy::with<int, tag1> s1(v1);
        cy::with<int, tag2> s2(v2);
        cy::check(cy::try_get<int, tag1>());
        cy::check(cy::get<int, tag1>() == 10);
        cy::check(cy::get<int, tag2>() == 12);
    }

    // Scoped assignment
    {
        int v1 = 10;
        cy::with<int> s1 = v1;

        {
            int v2 = 20;
            cy::with<int> s2 = v2;
            cy::check(cy::get<int>() == v2);
        }

        // The assignment to s2 was undone
        cy::check(cy::get<int>() == 10);
    }
}
