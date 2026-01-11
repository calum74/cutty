#include <cutty/dynamic.hpp>
#include <cutty/test.hpp>

namespace cy = cutty;
using namespace cy::literals;

void empty()
{
    // Empty
    cy::dynamic empty;
    cy::check_equal(empty.str(), "(empty)");
    cy::check_equal(empty, cy::dynamic());

    // Empty is equal to itself
    cy::check_equal(empty, empty);

    // Empty is not equal to other values
    cy::check(empty != 1_d);

    auto empty_ref = empty.ref();
    cy::check_equal(empty, empty_ref);

    cy::check_equal(empty.hash(), empty.hash());

    cy::check_equal(cy::dynamic{}, cy::dynamic());
    cy::dynamic empty2 = {};
    cy::check_equal(empty2, cy::dynamic());
}

void bools()
{
    // Conversion to bool
    const cy::dynamic True = true, False = false;

    cy::check(True);
    cy::check(!False);

    // Equality
    cy::check_equal(True, True);
    cy::check_equal(False, False);
    cy::check(True != False);

    // Inequality
    cy::check(False < True);

    // Strings - unfortunately following C++ convention
    cy::check_equal(True.str(), "1");
    cy::check_equal(False.str(), "0");

    // Hash
    True.hash();
    False.hash();
}

void integers()
{
    // Integers
    cy::check_equal((2_d).str(), "2");
    cy::check(1_d != 2);
    cy::check(2_d == 2);

    cy::dynamic i = 42;
    cy::check_equal(i.hash(), i.hash());
    cy::dynamic i_ref = i.ref();
    i_ref = 40;
    cy::check_equal(i, 40);
}

void floats()
{
    // Float
    cy::check_equal((3.14_d).str(), "3.14");
    cy::check(3.14_d != 5);

    // Addition
    cy::check_equal(1_d + 2_d, 3_d);
    cy::check_equal(0.5_d + 1.5_d, 2);
    cy::check_equal(0.5_d + 1_d, 1.5);
    cy::check_equal(1_d + 0.5_d, 1.5_d);
}

void strings()
{
    // Strings
    cy::check_equal("a"_d + "b"_d, "ab"_d);

    cy::check_equal((""_d).size(), 0);
    cy::check_equal(("abc"_d).size(), 3);

    // Can we modify a string??
    cy::dynamic string = "abcde";
    auto string2 = string;
    for (auto x : string)
    {
        x = 'x';
    }
    cy::check_equal(string2, "abcde");
    cy::check_equal(string, "xxxxx");
}

void references()
{
    cy::dynamic d1 = 13;
    auto r1 = d1.ref();
    cy::check_equal(r1, 13);
    r1 = 12;
    cy::check_equal(d1, 12);
    cy::check_equal(r1, 12);

    auto r2 = d1.const_ref();
    // Cannot write to a const reference
    cy::check_throws<cy::dynamic::unsupported>([&] { r2 = 1; });

    // Reference to a reference?
    auto r3 = d1.ref();
    auto r4 = r3.ref();
    r4 = 42;
    cy::check(d1 == 42);

    // References
    {
        int x = 42;
        auto i = cy::dynamic::reference(x);
        cy::check_equal(i, 42);
        x = 43;
        cy::check_equal(i, 43);

        i = 42; // !! How is this going to work
        cy::check_equal(i, 42);
        cy::check(i.type_str() == "int");
    }
}

void consts()
{
    // Consts
    {
        cy::dynamic i = cy::dynamic::list({1, 2, 3});

        cy::dynamic ro = i.as_const();

        cy::check_throws<cy::dynamic::unsupported>([&] { ro.push_back(1); });

        cy::check_equal(ro, i);

        cy::check_throws<cy::dynamic::unsupported>([&] { ro[1] = 1; });

        cy::check_equal(ro[1], 2);

        // Just regular reassignment
        ro = 3;
    }

    {
        auto s = "abc"_d;
        auto const_str = s.as_const();
        cy::check_throws<cy::dynamic::unsupported>([&] { const_str[0] = 'A'; });
        s[0] = 'a'; // Ok
    }
}

void lists()
{
    auto l = cy::dynamic::list({1, 2.1, "3"});
    cy::check_equal(l.str(), "{1, 2.1, 3}");
    cy::check_equal(l.size(), 3);
    auto l2 = cy::dynamic::list();

    cy::check(cy::dynamic::list().empty());
    cy::check(!cy::dynamic::list({1, 2, 3}).empty());

    for (auto x : l)
    {
        l2.push_back(x);
    }

    cy::check_equal(l, l2);
    cy::check_equal(l[0], 1);

    // Let's write to a list
    for (auto x : l)
    {
        x = 0;
    }
    cy::check_equal(l, cy::dynamic::list({0, 0, 0}));

    l2.push_back(10);
    cy::check(l != l2);

    // Lists and references
    {
        cy::dynamic l3 = {1, 2, 3};
        auto old_value = l3[1];
        auto copy = old_value;
        copy = "new_value";
        cy::check(old_value == 2);
        cy::check(l3[1] == 2);
        cy::check_equal(l3.str(), "{1, 2, 3}");
        old_value = "new_value";
        cy::check(old_value == "new_value");
        cy::check(l3[1] == "new_value");
        cy::check_equal(l3.str(), "{1, new_value, 3}");

        //
        cy::check(l3.front() == 1);
        l3.front() = 11;
        cy::check(l3.front() == 11);

        auto l3_ref = l3.ref();
        cy::check(l3_ref == l3);
    }

    // Lists by dynamic index
    {
        cy::dynamic l = {1, 2, 3};

        cy::check_equal(l[0_d], 1);
        cy::check_throws<std::out_of_range>([&] { l[3]; });
        cy::check_throws<std::out_of_range>([&] { l[3_d]; });
    }

    // Push & pop
    {
        cy::dynamic l = cy::dynamic::list();
        l.push_back(1);
        cy::check_equal(1, l.back());
        l.back() = 2;
        cy::check_equal(2, l.back());
        l.pop_back();
        // cy::check(l.empty());
        cy::check_equal(l.size(), 0);
    }

    // Erase
    {
        cy::dynamic l = {1, 2, 3, 4};
        l.erase(l.begin());
        cy::check_equal(l, cy::dynamic({2, 3, 4}));
        l.erase(l.begin() + 1, l.end() - 1);
        cy::check_equal(l, cy::dynamic({2, 4}));
        l.erase(l.begin(), l.end());
        cy::check(l.empty());

        cy::check_throws<cy::dynamic::incompatible>([&] { l.erase({}); });
    }
}

void queues()
{
    {
        cy::dynamic l = cy::dynamic::queue();
        l.push_back(1);
        cy::check_equal(1, l.back());
        l.back() = 2;
        cy::check_equal(2, l.back());
        l.pop_back();

        l.push_front(2);
        cy::check_equal(2, l.front());
        l.front() = 3;
        cy::check_equal(3, l.front());
        l.pop_front();

        cy::check_equal(l.size(), 0);
    }
}

void maps()
{
    // Empty map
    auto m1 = cy::dynamic::map();
    cy::check_equal(m1.size(), 0);
    cy::check_equal(m1.str(), "{}");

    cy::check_equal(m1, m1);
    auto m1a = m1; // Copy
    m1a[1_d] = 1_d;

    cy::check_equal(m1a[1], 1);
    cy::check(m1 != m1a);

    {
        const auto &m1b = m1;
        cy::check_throws<std::out_of_range>([&] { m1b[1]; });
    }

    // Initialized map
    auto m2 = cy::dynamic::map({{1, "x"}, {2, "y"}});
    cy::check_equal(m2.size(), 2);
    cy::check_equal(m2.str(), "{(1, x), (2, y)}");

    // Iterating a map
    for (auto item : m2)
    {
        auto &[k, v] = item;
        cy::check_throws<cy::dynamic::unsupported>([&] { k = 0; });
        v = "z";
    }

    // Iterating a map backwards - fails due to a compilation error in the STL
    {
        cy::check_throws<cy::dynamic::unsupported>([&] {
            for (auto i = m2.rbegin(); i != m2.rend(); ++i)
            {
            }
        });
    }

    // Heterogenous key types (for example, a map)
    cy::check_throws<cy::dynamic::unsupported>([] { cy::check(cy::dynamic() < 1); });

    // But it works with a dict:
    cy::dynamic::dict({{1, 1}, {cy::dynamic(), {}}});
}

void dicts()
{
    {
        auto d = cy::dynamic::dict({{1, 1}, {cy::dynamic(), {}}});
        cy::check_equal(d.size(), 2);
        cy::check_equal(d[1], 1);
        cy::check_equal(d[1_d], 1);
        cy::check_equal(d[cy::dynamic()], cy::dynamic());
    }

    {
        auto d = cy::dynamic::dict({{1, 1}, {cy::dynamic(), {}}});
        auto c1 = d.as_const();
        const auto c2 = d;
        const auto &c3 = d;

        cy::check_equal(c1[1], 1);
        cy::check_equal(c1[1_d], 1);
        cy::check_equal(c1[cy::dynamic()], cy::dynamic());

        cy::check_equal(c2[1], 1);
        cy::check_equal(c2[1_d], 1);
        cy::check_equal(c2[cy::dynamic()], cy::dynamic());

        cy::check_equal(c3[1], 1);
        cy::check_equal(c3[1_d], 1);
        cy::check_equal(c3[cy::dynamic()], cy::dynamic());
    }
}

void objects()
{
    {
        auto o = cy::dynamic::object();
        o["hello"] = 123;
        cy::check_equal(o["hello"], 123);
        cy::check_equal(o["hello"_d], 123);
        auto c = o.as_const();
        cy::check_equal(c["hello"], 123);
        cy::check_equal(c["hello"_d], 123);
        cy::check_throws<std::out_of_range>([&] { c["x"]; });

        // Numerical indexes aren't supported
        cy::check_throws<cy::dynamic::unsupported>([&] { o[123]; });
    }
}

void shared_pointers()
{
    // Shared pointers
    {
        cy::dynamic shared = cy::dynamic::shared(43);
        auto shared_list = cy::dynamic::list({3, 2, 1}).shared_ref();
        auto shared_list2 = shared_list;
        shared_list.push_back(4);

        cy::check_equal(shared, 43);
        cy::check_equal(shared_list, shared_list2);
        cy::check_equal(shared_list, cy::dynamic::list({3, 2, 1, 4}));

        auto shared_string = "abcde"_d.shared_ref();
        auto string2 = shared_string;
        shared_string.push_back('x');
        string2.push_back('y');
        cy::check(string2 == "abcdexy");
    }

    // Weak pointers
    {
        auto ss = "hello"_d.shared_ref();
        auto wp = ss.weak_ref();
        auto ss2 = ss;
        ss = {}; // Bye bye
        cy::check_equal(ss, cy::dynamic());
        cy::check_equal(wp.shared_ref(), "hello");
    }
}

void comparisons()
{
    // The == operator always succeeds
    // This is the only way to test for "empty"
    cy::check(cy::dynamic() == cy::dynamic());
    cy::check(cy::dynamic() != 1);
    cy::check("abc"_d == "abc");
    cy::check("abc" == "abc"_d);

    // Integers vs integers
    cy::check(1_d < 2);
    cy::check(2_d > 1);
    cy::check(3_d <= 3);

    // Integer vs double
    cy::check(1_d < 1.1);
    cy::check(1_d <= 1.0);
    cy::check(0.5_d < 1_d);
    cy::check(-2.1_d < 2_d);

    // Integer vs string
    cy::check(1_d < "");

    // Integer vs object
    cy::check(1_d < cy::dynamic());

    // Double vs double
    cy::check(cy::dynamic(2.1) == cy::dynamic(2.1));
    cy::check(cy::dynamic(2.1) < cy::dynamic(2.2));

    // Double vs string
    cy::check(3.14_d < "3.14");

    // Double vs object
    cy::check(3.14_d < cy::dynamic());

    // String vs string
    cy::check("abc"_d == "abc");
    cy::check("ab"_d < "abc");

    // String vs object
    cy::check(""_d < cy::dynamic());

    // Object vs object
    cy::check(cy::dynamic() <= cy::dynamic());
}

void literals()
{
    cy::check_equal(12_d, 12);
    cy::check(3.14_d < 4);
    cy::check("abc"_d < "bcd"_d);
    cy::check_equal("abc"_d[1], 'b'_d);
}

void conversions()
{
    // int conversions
    auto i = 123_d;
    cy::check_equal((cy::dynamic::int_type)i, 123);
    cy::check_equal(i.as_int(), 123);

    // double conversions
    auto d = 3.5_d;
    cy::check_equal((double)i, 123);
    cy::check_equal((double)d, 3.5);
    cy::check_equal(i.as_double(), 123);
    cy::check_equal(d.as_double(), 3.5);
    cy::check_equal((cy::dynamic::int_type)d, 3);
    cy::check_equal(d.as_int(), 3);

    // bool conversions
    cy::check(i);
    cy::check(!0_d);
}

void ranges()
{
    static_assert(std::ranges::range<cy::dynamic>);
    static_assert(std::ranges::bidirectional_range<cy::dynamic>);
    static_assert(std::input_or_output_iterator<cy::dynamic>);

    const auto x = "abc"_d;
    cy::check_equal(std::ranges::begin(x), x.begin());

    // Construct a list from a range
    // !!

    // Reverse - bidirectional range??
}

void operators()
{
    cy::check_equal(-1_d, -1);
    cy::check_equal(+1_d, 1);

    {
        cy::dynamic i = 10;
        ++i;
        cy::check_equal(i, 11);
        --i;
        cy::check_equal(i, 10);
        cy::check_equal(i++, 10);
        cy::check_equal(i--, 11);
    }
}

int main(int argc, const char *argv[])
{
    return cy::test(argc, argv,
                    {{"empty", empty},
                     {"bools", bools},
                     {"integers", integers},
                     {"floats", floats},
                     {"strings", strings},
                     {"references", references},
                     {"consts", consts},
                     {"lists", lists},
                     {"queues", queues},
                     {"maps", maps},
                     {"dicts", dicts},
                     {"objects", objects},
                     {"shared_pointers", shared_pointers},
                     {"comparisons", comparisons},
                     {"literals", literals},
                     {"conversions", conversions},
                     {"ranges", ranges},
                     {"operators", operators}});
}
