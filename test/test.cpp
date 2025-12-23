#include <cutty/test.hpp>
#include <cutty/with.hpp>

namespace cy = cutty;

struct run1;
struct run2;
struct run3;

void test1()
{
    cy::get<bool, run1>() = true;
}

void test2()
{
    cy::get<bool, run2>() = true;
}

void fail1()
{
    cy::get<bool, run3>() = true;
    cy::check(false, "Expected failure, ignore");
}

void all_pass()
{
    bool r1 = false, r2 = false, r3 = false;
    cy::with<bool, run1> with1{r1};
    cy::with<bool, run2> with2{r2};
    cy::with<bool, run3> with3{r3};
    cy::check_equal(0, cy::test({test1, test2}));
    cy::check(cy::get<bool, run1>());
    cy::check(cy::get<bool, run2>());
    cy::check(!cy::get<bool, run3>());
}

void all_fail()
{
    bool r1 = false, r2 = false, r3 = false;
    cy::with<bool, run1> with1{r1};
    cy::with<bool, run2> with2{r2};
    cy::with<bool, run3> with3{r3};

    cy::check(0 != cy::test({test1, test2, fail1}));

    cy::check(cy::get<bool, run1>());
    cy::check(cy::get<bool, run2>());
    cy::check(cy::get<bool, run3>());
}

void specify_test_index()
{
    bool r1 = false, r2 = false, r3 = false;
    cy::with<bool, run1> with1{r1};
    cy::with<bool, run2> with2{r2};
    cy::with<bool, run3> with3{r3};

    const char *tests[] = {"", "1", "2"};
    cy::check_equal(0, cy::test(3, tests, {test1, test2, fail1}));

    cy::check(cy::get<bool, run1>());
    cy::check(cy::get<bool, run2>());
    cy::check(!cy::get<bool, run3>());
}

void specify_test_names()
{
    bool r1 = false, r2 = false, r3 = false;
    cy::with<bool, run1> with1{r1};
    cy::with<bool, run2> with2{r2};
    cy::with<bool, run3> with3{r3};

    const char *tests[] = {"", "t1", "t2"};
    cy::check_equal(0, cy::test(3, tests, {{"t1", test1}, {"t2", test2}, {"t3", fail1}}));

    cy::check(cy::get<bool, run1>());
    cy::check(cy::get<bool, run2>());
    cy::check(!cy::get<bool, run3>());
}

void specify_invalid_test()
{
    bool r1 = false, r2 = false, r3 = false;
    cy::with<bool, run1> with1{r1};
    cy::with<bool, run2> with2{r2};
    cy::with<bool, run3> with3{r3};

    const char *tests[] = {"", "t1", "no_such_test"};
    cy::check(0 != cy::test(3, tests, {{"t1", test1}, {"t2", test2}, {"t3", fail1}}));

    cy::check(!cy::get<bool, run1>());
    cy::check(!cy::get<bool, run2>());
    cy::check(!cy::get<bool, run3>());
}

int main(int argc, const char **argv)
{
    return cy::test(argc, argv,
                    {{"all_pass", all_pass},
                     {"all_fail", all_fail},
                     {"specify_test_index", specify_test_index},
                     {"specify_test_names", specify_test_names},
                     {"specify_invalid_test", specify_invalid_test}});
}