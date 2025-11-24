// This file forms the test suite for the Sequence library
// It also contains lots of examples used throughout the documentation.

// This is the main header file to include, which includes everything
// You can also #include <sequence_fwd.hpp> if you just need the forward declaration.

#include <cutty/sequence.hpp>

#include <cutty/check.hpp>

#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

namespace cy = cutty;

void test_eq(const cy::sequence<int> &s1, const cy::sequence<int> &s2)
{
    cy::check(s1 == s2);
}

void handleOption(const char *opt)
{
    std::cout << "Option is " << opt << std::endl;
}

// An example of a function that takes a sequence
void init(const cy::pointer_sequence<const char *> &params)
{
    for (auto p : params.where([](const char *str) { return str[0] == '-'; }))
        handleOption(p + 1);

    auto options =
        params.where([](const char *arg) { return arg[0] == '-'; }).select([](const char *arg) { return arg + 1; });

    for (auto p : options)
        handleOption(p);
}

// Helper to validate object lifetimes.
class Element
{
    bool constructed;

  public:
    Element() : constructed(true)
    {
    }
    ~Element()
    {
        constructed = false;
        cy::check(!valid());
    }

    bool valid() const
    {
        return constructed;
    }
};

std::vector<Element> getVector()
{
    return std::vector<Element>{4};
}

void testVector(const cy::sequence<Element> &elements)
{
    // cy::check(elements.size() == 4);
    for (auto &i : elements)
        cy::check(i.valid());
}

void testVector2(const cy::pointer_sequence<Element> &elements)
{
    cy::check(elements.size() == 4);
    for (auto &i : elements)
        cy::check(i.valid());
}

void test_lifetimes()
{
    testVector(cy::seq(getVector()));
    testVector(cy::list(Element(), Element(), Element(), Element()));
    testVector2(cy::list(Element(), Element(), Element(), Element()));
    testVector(cy::seq({Element(), Element(), Element(), Element()}));

    // Here be dragons - we could be creating a dangling reference to a temporary object
    // If in doubt, don't store temporary variables in auto variables.
    auto tmp = cy::seq(getVector());

    for (auto i : cy::seq(getVector()))
        cy::check(i.valid());

    testVector(tmp);
}

void copy(const cy::sequence<const char *> &input, const cy::output_sequence<const char *> &output)
{
    output << input;
}

void test_writers()
{
    std::vector<const char *> vec;

    copy(cy::list("a", "b"), cy::writer(vec));
    cy::check(cy::seq(vec).as<std::string>() == cy::list("a", "b"));

    copy(cy::list("writer1", "writer2"), cy::receiver([](const char *str) { std::cout << str << std::endl; }));
    copy(cy::list("writer1", "writer2"), cy::receiver([&](const char *str) { vec.push_back(str); }));

    copy(cy::list("writer1", "writer2"), cy::receiver([&](const char *str) { vec.push_back(str); }));
}

void test_repeat()
{
    cy::check(cy::list(1, 1, 1) == cy::list(1).repeat(3));
    cy::check(cy::list(1, 2) == cy::list(1, 2).repeat(1));
    cy::check(cy::list(1, 2, 1, 2) == cy::list(1, 2).repeat(2));
    cy::check(cy::list<int>() == cy::list(1).repeat(0));
    cy::check(cy::list<int>() == cy::list<int>().repeat(2));
    cy::check(cy::list<int>() == cy::list<int>().repeat(0));

    // Invalid input
    cy::check(cy::list('a').repeat(-1) == cy::list<char>());
}

void test_take()
{
    cy::check(cy::list(1, 2, 3, 4).take(-1) == cy::list<int>());
    cy::check(cy::list(1, 2, 3, 4).take(0) == cy::list<int>());
    cy::check(cy::list(1, 2, 3, 4).take(1) == cy::list<int>(1));
    cy::check(cy::list(1, 2, 3, 4).take(2) == cy::list<int>(1, 2));
    cy::check(cy::list(1, 2, 3, 4).take(3) == cy::list<int>(1, 2, 3));
    cy::check(cy::list(1, 2, 3, 4).take(4) == cy::list<int>(1, 2, 3, 4));
    cy::check(cy::list(1, 2, 3, 4).take(5) == cy::list<int>(1, 2, 3, 4));
}

void test_skip()
{
    cy::check(cy::list(1, 2, 3, 4).skip(-1) == cy::list<int>(1, 2, 3, 4));
    cy::check(cy::list(1, 2, 3, 4).skip(0) == cy::list<int>(1, 2, 3, 4));
    cy::check(cy::list(1, 2, 3, 4).skip(1) == cy::list<int>(2, 3, 4));
    cy::check(cy::list(1, 2, 3, 4).skip(2) == cy::list<int>(3, 4));
    cy::check(cy::list(1, 2, 3, 4).skip(3) == cy::list<int>(4));
    cy::check(cy::list(1, 2, 3, 4).skip(4) == cy::list<int>());
    cy::check(cy::list(1, 2, 3, 4).skip(5) == cy::list<int>());
}

void test_take_while()
{
    cy::check(cy::list(1, 2, 3, 4).take_while([](int x) { return false; }) == cy::list<int>());
    cy::check(cy::list(1, 2, 3, 4, 1).take_while([](int x) { return x <= 3; }) == cy::list(1, 2, 3));
    cy::check(cy::list(1, 2, 3, 4).take_while([](int x) { return true; }) == cy::list(1, 2, 3, 4));
}

void test_skip_until()
{
    cy::check(cy::list(1, 2, 3, 4).skip_until([](int x) { return false; }) == cy::list<int>());
    cy::check(cy::list(1, 2, 3, 4, 1).skip_until([](int x) { return x >= 3; }) == cy::list(3, 4, 1));
    cy::check(cy::list(1, 2, 3, 4).skip_until([](int x) { return true; }) == cy::list(1, 2, 3, 4));
}

void test_files()
{
    std::stringstream ss("abc\ndef\r\n   ghi   \n\n");
    cy::check(cy::seq(ss).split("\r\n") == cy::list("abc", "def", "   ghi   "));
}

// This function should return 0, but actually does not because
// sum() is iterating over the same sequence concurrently which is
// undefined.
int computeAsyncUnsafe(const cy::sequence<int> &values)
{
    auto f1 = std::async(std::launch::async, [&]() { return values.sum(); });
    auto f2 = std::async(std::launch::async, [&]() { return values.sum(); });
    return f1.get() - f2.get();
}

// A safe version of computeAsyncUnsafe(), ensuring that the sequence
// is copied by value.
template <typename Seq> int computeAsyncSafe1(const Seq &values)
{
    auto f1 = std::async(std::launch::async, [=]() { return values.sum(); });
    auto f2 = std::async(std::launch::async, [=]() { return values.sum(); });
    return f1.get() - f2.get();
}

// A safe version of computeAsyncUnsafe(), ensuring that the sequence
// is copied by value.
int computeAsyncSafe2(const cy::pointer_sequence<int> &values)
{
    auto f1 = std::async(std::launch::async, [=]() { return values.sum(); });
    auto f2 = std::async(std::launch::async, [=]() { return values.sum(); });
    return f1.get() - f2.get();
}

void test_async()
{
    // This hangs so don't call it.
    // std::cout << computeAsyncUnsafe(seq(1,10000000)) << std::endl;
    std::cout << computeAsyncSafe1(cy::seq(1, 10000000)) << std::endl;

    auto values = cy::seq(1, 1000000).make<std::vector<int>>();
    std::cout << computeAsyncSafe1(cy::seq(values)) << std::endl;
}

void test_range()
{
    cy::check(cy::seq(1, 10).size() == 10);
    cy::check(cy::seq(1, 10).where([](int x) { return x > 4; }).size() == 6);
    cy::check(cy::seq(1, 10).select([](int x) { return x + 1; }).size() == 10);

    cy::check(cy::seq(1, 10).select([](int x) { return x + 1; }).where([](int x) { return x > 4; }).size() == 7);

    // std::vector<int> x = seq(1,10).to_vec();

    cy::check(cy::seq(1, 10).take(2).size() == 2);
    cy::check(cy::seq(1, 10).take(0).size() == 0);

    cy::check(cy::seq<int>().size() == 0);

    // This has a bug in it!
    // for(auto x : seq(1,100).
    //    where([](int x) { return x%3!=0;}).
    //    select([](int x) { return "pika "; }))
    //    std::cout << x << std::endl;
}

void test_comparisons()
{
    auto e = cy::list<int>();

    // Equality
    cy::check(e == e);
    cy::check(cy::list(1) == cy::list(1));
    cy::check(cy::list(1, 2) == cy::list(1, 2));

    // !=
    cy::check(e != cy::list(1));
    cy::check(cy::list(1) != e);
    cy::check(cy::list(1) != cy::list(2));
    cy::check(cy::list(1) != cy::list(1, 2));

    // <
    cy::check(e < cy::list(1));
    cy::check(cy::list(1) < cy::list(1, 2));
    cy::check(cy::list(1) < cy::list(2));

    // <=
    cy::check(e <= e);
    cy::check(e <= cy::list(1));
    cy::check(cy::list(1) <= cy::list(1));
    cy::check(cy::list(1) <= cy::list(1, 2));
    cy::check(cy::list(1) <= cy::list(2));
    cy::check(cy::list(1, 2) <= cy::list(1, 2));
    cy::check(cy::list(1, 2) <= cy::list(1, 3));
    cy::check(cy::list(1, 2) <= cy::list(2, 1));

    // >
    cy::check(cy::list(1) > e);
    cy::check(cy::list(1, 2) > cy::list(1));
    cy::check(cy::list(2) > cy::list(1));

    // >=
    cy::check(e >= e);
    cy::check(cy::list(1) >= e);
    cy::check(cy::list(1) >= cy::list(1));
    cy::check(cy::list(1, 2) >= cy::list(1));
    cy::check(cy::list(2) >= cy::list(1));
    cy::check(cy::list(1, 2) >= cy::list(1, 2));
    cy::check(cy::list(1, 3) >= cy::list(1, 2));
    cy::check(cy::list(2, 1) >= cy::list(1, 2));
}

void test_single()
{
    cy::check(cy::single(3) == cy::list(3));
}

void test_list()
{
    cy::check(cy::list<int>().size() == 0);

    auto e = cy::list(1, 2, 3);
    cy::check(e.size() == 3);
    for (int i : e)
        std::cout << i << std::endl;
    for (int i : e.where([](int x) { return x > 1; }))
        std::cout << i << std::endl;

    for (int i : e.where([](int x) { return x > 1; }).select([](int x) { return x * 2; }))
        std::cout << i << std::endl;

    for (int i : cy::list(1, 2, 3).where([](int x) { return x > 1; }).select([](int x) { return x * 2; }))
        std::cout << i << std::endl;

    for (int i : cy::list(1, 2, 3))
        std::cout << i << std::endl;

    cy::check(cy::list(1, 2, 3).sum() == 6);
    cy::check(cy::list(1, 2, 3).select([](int x) { return x * 2 + 1; }).sum() == 15);

    cy::check(cy::list(1, 2, 3).front() == 1);
    cy::check(cy::list(1, 2, 3).back() == 3);
    cy::check(cy::list(2, 3, 4).at(1) == 3);
    cy::check(cy::list(2, 3, 4).at(0) == 2);
    cy::check(cy::list(2, 3, 4).at(2) == 4);

    cy::check(cy::list(1, 2, 3) == cy::list(1, 2, 3));
    cy::check(cy::list(1, 2) < cy::list(1, 2, 3));

    test_eq(cy::list(1, 2, 3), cy::list(1, 2, 3));

    init(cy::list("-a", "-b", "-c"));
    init(cy::list("a", "b", "c"));
}

void test_primes()
{
    auto primes = cy::seq(2, 1000).where([](int n) { return !cy::seq(2, n - 1).any([=](int m) { return n % m == 0; }); });

    cy::check(primes.take(5) == cy::list(2, 3, 5, 7, 11));

    std::vector<int> primes2;
    for (int n = 2; n <= 1000; ++n)
    {
        bool is_prime = true;
        for (int m = 2; m < n; m++)
            if (n % m == 0)
            {
                is_prime = false;
                break;
            }
        if (is_prime)
            primes2.push_back(n);
    }

    cy::check(cy::seq(primes2) == primes);
}

void test_generator()
{
    // Generated sequence
    int i = 0;
    auto g = generator(
        [&](int &x) {
            x = 0;
            i = 1;
            return true;
        },
        [&](int &x) {
            x = i;
            return i++ < 10;
        });
    cy::check(g == cy::seq(0, 9));
}

void test_keys_and_values()
{
    std::map<std::string, int> map1 = {{"a", 1}, {"b", 2}, {"c", 3}};

    cy::check(cy::seq(map1).keys() == cy::list("a", "b", "c"));
    cy::check(cy::seq(map1).values() == cy::list(1, 2, 3));

    cy::check(cy::seq(map1).keys().merge(cy::seq(map1).values(), [](const std::string &str, int i) {
        return std::make_pair(str, i);
    }) == cy::seq(map1));
}

void test_merge()
{
    auto sum = [](int a, int b) { return a + b; };
    auto e = cy::list<int>();

    cy::check(cy::list(1, 2).merge(cy::list(3, 4), sum) == cy::list(4, 6));
    cy::check(e.merge(cy::list(3, 4), sum) == e);
    cy::check(cy::list(1, 2).merge(e, sum) == e);

    cy::check(cy::list(1).merge(cy::list(3, 4), sum) == cy::list(4));
    cy::check(cy::list(1, 2).merge(cy::list(3), sum) == cy::list(4));
}

void test_sum()
{
    cy::check(cy::list<int>().sum() == 0);
    cy::check(cy::list(1).sum() == 1);
    cy::check(cy::list(1, 2, 3).sum() == 6);

    cy::check(cy::list<std::string>().sum() == "");
    cy::check(cy::list<std::string>("a", "b", "c").sum() == "abc");

    cy::check(cy::list(0.5, 1.5).sum() == 2.0);
}

void test_any()
{
    cy::check(!cy::list<int>().any());
    cy::check(cy::list<int>().empty());
    cy::check(cy::list(1).any());
    cy::check(!cy::list(1).empty());

    cy::check(!cy::list(1, 2).any([](int x) { return x == 3; }));
    cy::check(cy::list(1, 2).any([](int x) { return x == 2; }));
}

void test_count()
{
    cy::check(cy::list<int>().count([](int x) { return x == 1; }) == 0);
    cy::check(cy::list<int>(2, 3).count([](int x) { return x == 1; }) == 0);
    cy::check(cy::list<int>(1, 2, 3).count([](int x) { return x == 1; }) == 1);
    cy::check(cy::list<int>(1, 2, 3, 1, 1).count([](int x) { return x == 1; }) == 3);
}

void test_aggregate()
{
    auto sum = [](int a, int b) { return a + b; };

    cy::check(cy::list<int>().aggregate(0, sum) == 0);
    cy::check(cy::list<int>().aggregate(1, sum) == 1);
    cy::check(cy::list(3, 4, 5).aggregate(2, sum) == 14);
    cy::check(cy::list(3, 4, 5).aggregate(sum) == 12);
    cy::check(cy::list(3, 4, 5).aggregate(std::string(), [](const std::string &str, int n) { return str + 'x'; }) == "xxx");
}

void test_accumulate()
{
    cy::check(cy::list<int>().accumulate(5, [](int &total, int n) { total += n; }) == 5);
    cy::check(cy::list(3, 4, 5).accumulate(5, [](int &total, int n) { total += n; }) == 17);
    cy::check(cy::list(3, 4, 5).accumulate(std::string(), [](std::string &str, int n) { str += 'x'; }) == "xxx");
}

int main()
{
    test_lifetimes();
    test_writers();
    test_range();
    test_comparisons();
    test_single();
    test_list();
    test_primes();
    test_keys_and_values();
    test_repeat();
    test_files();
    test_async();
    test_take();
    test_skip();
    test_take_while();
    test_skip_until();
    test_merge();
    test_sum();
    test_any();
    test_count();
    test_aggregate();
    test_accumulate();
    return 0;
}