#include <cutty/check.hpp>
#include <cutty/dynamic.hpp>

#include <cassert>
#include <iostream>

namespace cy = cutty;

int main()
{
    // Empty
    cy::dynamic empty;
    std::cout << empty << std::endl;
    assert(empty == cy::dynamic());
    std::cout << empty.type_str() << std::endl;
    assert(empty.type_str() == "dynamic::empty");

    // Integers
    std::cout << 2_d << std::endl;
    assert(1_d != 2);
    assert(2_d == 2);

    // Float
    std::cout << 3.14_d << std::endl;
    assert(3.14_d != 5);

    // Addition
    std::cout << (1_d + 2_d) << std::endl;
    assert(1_d + 2_d == 3_d);
    assert(0.5_d + 1.5_d == 2);
    assert(0.5_d + cy::dynamic(1) == 1.5);
    assert(1_d + 0.5_d == 1.5_d);

    // Strings
    assert("a"_d + "b"_d == "ab"_d);
    assert(empty == empty);

    // List
    {
        auto l = cy::dynamic::list({1, 2.1, "3"});
        assert(l.str() == "[1,2.1,3]");
        assert(l.size() == 3);

        for (auto x : l)
        {
            std::cout << "L:" << x << std::endl;
        }

        std::cout << "A list? " << l << std::endl;
    }

    // Reference
    {
        cy::dynamic d1 = 13;
        auto r1 = d1.ref();
        assert(r1 == 13);
        r1 = 12;
        assert(d1 == 12);
        assert(r1 == 12);

        auto r2 = d1.const_ref();
        try
        {
            r2 = 1; // Cannot write to a const reference
        }
        catch (cy::dynamic::unsupported &)
        {
        }

        // Reference to a reference?
        auto r3 = d1.ref();
        auto r4 = r3.ref();
        r4 = 42;
        assert(d1 == 42);
    }

    // Let's write to a list
    {
        auto l = cy::dynamic::list({1, 2, 3});
        for (auto x : l)
        {
            x = 0;
        }
        assert((l == cy::dynamic::list({0, 0, 0})));
    }

    // Can we modify a string??
    {
        cy::dynamic string = "abcde";
        auto string2 = string;
        for (auto x : string)
        {
            x = 'x';
        }
        assert(string2 == "abcde");
        assert(string == "xxxxx");
    }

    // Consts
    {
        cy::dynamic i = cy::dynamic::list();
        cy::dynamic j{i}; // , dynamic::const_value()};

        // j.push_back(1); // Throws (TODO: Test)
        std::cout << cy::dynamic::list({1, 2, 3}) << j << std::endl;
        j = 3; //??
    }

    // References
    {
        int x = 42;
        cy::dynamic i(x, cy::dynamic::by_reference_tag());
        assert(i == 42);
        x = 43;
        assert(i == 43);

        i = 42; // !! How is this going to work
        std::cout << i << std::endl;

        assert(i.type_str() == "int");
    }

    // Shared pointers
    {
        cy::dynamic shared(43, cy::dynamic::shared_tag());
        auto shared_list = cy::dynamic::list({3, 2, 1}).shared_ref();
        auto shared_list2 = shared_list;
        shared_list.push_back(4);

        std::cout << "Shared 43: " << shared << std::endl;

        std::cout << shared_list2 << std::endl;

        auto shared_string = "abcde"_d.shared_ref();
        auto string2 = shared_string;
        shared_string.push_back('x');
        string2.push_back('y');
        assert(string2 == "abcdexy");
        std::cout << string2 << std::endl;
    }

    // Weak pointers
    {
        auto ss = "hello"_d.shared_ref();
        auto wp = ss.weak_ref();
        auto ss2 = ss;
        ss = {}; // Bye bye
        std::cout << wp.shared_ref() << std::endl;
    }

    // Lists and references
    {
        cy::dynamic l3 = {1, 2, 3};
        auto old_value = l3[1];
        auto copy = old_value;
        copy = "new_value";
        assert(old_value == 2);
        assert(l3[1] == 2);
        assert(l3.str() == "[1,2,3]");
        old_value = "new_value";
        assert(old_value == "new_value");
        assert(l3[1] == "new_value");
        assert(l3.str() == "[1,new_value,3]");

        //
        assert(l3.front() == 1);
        l3.front() = 11;
        assert(l3.front() == 11);

        auto l3_ref = l3.ref();
        assert(l3_ref == l3);
    }

    // Comparisons
    {
        // The == operator always succeeds
        // This is the only way to test for "empty"
        assert(cy::dynamic() == cy::dynamic());
        assert(cy::dynamic() != 1);
        assert("abc"_d == "abc");
        assert("abc" == "abc"_d);

        // Integers vs integers
        assert(1_d < 2);
        assert(2_d > 1);
        assert(3_d <= 3);

        // Integer vs double
        assert(1_d < 1.1);
        assert(1_d <= 1.0);
        assert(0.5_d < 1_d);
        assert(-2.1_d < 2_d);

        // Integer vs string
        assert(1_d < "");

        // Integer vs object
        assert(1_d < cy::dynamic());

        // Double vs double
        assert(cy::dynamic(2.1) == cy::dynamic(2.1));
        assert(cy::dynamic(2.1) < cy::dynamic(2.2));

        // Double vs string
        assert(3.14_d < "3.14");

        // Double vs object
        assert(3.14_d < cy::dynamic());

        // String vs string

        assert("abc"_d == "abc");
        assert("ab"_d < "abc");

        // String vs object
        assert(""_d < cy::dynamic());

        // Object vs object
        assert(cy::dynamic() <= cy::dynamic());
    }

    // Literals
    {
        assert(12_d == 12);
        assert(3.14_d < 4);
        assert("abc"_d < "bcd"_d);
        assert("abc"_d[1] == 'b'_d);
    }

    // Const
    {
        auto s = "abc"_d;
        // auto const_str = s.as_const(); (TODO)
    }

    // Conversions
    {
        auto i = 123_d;
        // assert((int)i == 123); (TODO)
        // assert(i.as_int() == 123); (TODO)
    }

    // Ranges
    {
        const auto x = "abc"_d;
        static_assert(std::input_or_output_iterator<cy::dynamic>);
        //std::ranges::begin(x);
        //x.begin();
        //static_assert(std::ranges::range<cy::dynamic>);
        // x.begin();
    }
}
