#include "cutty/satellite.hpp"
#include <cassert>

struct tag1;
struct tag2;

int main()
{
    {
        // Getting and setting the same satellite
        satellite<int, tag1> s1;
        assert(!s1);
        int x = 42;
        s1 = x;
        assert(s1);
        assert(*s1 == 42);
    }

    {
        // Nested getters and setters
        satellite<int, tag1> s1;
        satellite<int, tag2> s2;
        assert(!s1);
        assert(!s2);
        int v = 10;
        satellite<int, tag1> s3(v);
        satellite<int, tag1> s4;
        assert(s4);
        assert(*s4 == 10);
        satellite<int, tag2> s5;
        assert(!s5);
    }

    // Scoped assignment
    {
        int v1 = 10;
        satellite<int, tag1> s1 = v1;

        {
            int v2 = 20;
            satellite<int, tag1> s2;
            assert(*s2 == 10);
            s2 = v2;
            assert(*s2 == v2);
        }

        // The assignment to s2 was undone
        assert(*s1 == 10);
    }
}
