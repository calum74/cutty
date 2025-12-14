#include <cutty/check.hpp>
#include <cutty/satellite.hpp>

namespace cy = cutty;

struct tag1;
struct tag2;

int main()
{
    {
        // Getting and setting the same satellite
        cy::satellite<int> s1;
        cy::check(!s1);
        int x = 42;
        s1 = x;
        cy::check(s1);
        cy::check(*s1 == 42);
   }

    {
        // Nested getters and setters, multiple tags
        cy::satellite<int, tag1> s1;
        cy::satellite<int, tag2> s2;
        cy::check(!s1);
        cy::check(!s2);
        int v = 10;
        cy::satellite<int, tag1> s3(v);
        cy::satellite<int, tag1> s4;
        cy::check(s4);
        cy::check(*s4 == 10);
        cy::satellite<int, tag2> s5;
        cy::check(!s5);
    }

    // Scoped assignment
    {
        int v1 = 10;
        cy::satellite<int> s1 = v1;

        {
            int v2 = 20;
            cy::satellite<int> s2;
            cy::check(*s2 == 10);
            s2 = v2;
            cy::check(*s2 == v2);
            cy::check(*cy::satellite<int>() == v2);
        }

        // The assignment to s2 was undone
        cy::check(*s1 == 10);
    }
}
