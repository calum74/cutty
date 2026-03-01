#include <cutty/fixed_string.hpp>

#include <cutty/test.hpp>

namespace cy = cutty;

static_assert (cy::fixed_string("abc") == "abc");
static_assert (cy::fixed_string("abc").index_of('b') == 1);
static_assert (cy::fixed_string("abc").index_of('z') == -1);

static_assert (cy::fixed_string("abcd").index_of(cy::fixed_string("bc")) == 1);
static_assert (cy::fixed_string("abcd").index_of(cy::fixed_string("abc")) == 0);
static_assert (cy::fixed_string("abcd").index_of(cy::fixed_string("abcde")) == -1);
static_assert (cy::fixed_string_substr<"abcd", 1, 2> == "b");
static_assert (cy::fixed_string_substr<"abcd", 1, 4> == "bcd");

static_assert (cy::fixed_string_count<"abcabc", "ab"> == 2);
static_assert (cy::fixed_string_count<"abcabc", "x"> == 0);

static_assert (cy::fixed_string_split<"a,b,c", ",", 0> == "a");
static_assert (cy::fixed_string_split<"a,b,c", ",", 1> == "b");
static_assert (cy::fixed_string_split<"a,b,c", ",", 2> == "c");

static_assert(cy::fixed_string_before<"a->b", "->"> == "a"); 
static_assert(cy::fixed_string_after<"a->b", "->"> == "b"); 

static_assert(cy::fixed_string_count<"abc", "x"> == 0);
static_assert(cy::fixed_string_count<"abc", "bc"> == 1);
static_assert(cy::fixed_string_count<"", "bc"> == 0);

void output()
{
    cy::check_equal(cy::print_str(cy::fixed_string("abc")), "abc");
}

// Some einsum testing

template<cy::fixed_string E>
constexpr auto einsum_lhs = cy::fixed_string_split<E, "->", 0>;

template<cy::fixed_string E>
constexpr auto einsum_rhs = cy::fixed_string_split<E, "->", 1>;

template<cy::fixed_string E>
constexpr auto einsum_lhs_parts = cy::fixed_string_count<einsum_lhs<E>, ",">+1;

static_assert (einsum_lhs<"a->b"> == "a");
static_assert (einsum_rhs<"a->b"> == "b");

static_assert (einsum_lhs_parts<"a->ab"> == 1);
static_assert (einsum_lhs_parts<"a,b->ab"> == 2);
static_assert (einsum_lhs_parts<"->ab"> == 1);

template<cy::fixed_string Prefix> struct Logger
{
    static_assert(Prefix.front() == '[', "prefix must start with '['");
    static_assert(Prefix.back() == ']', "prefix must end with ']'");
    static constexpr bool is_warning = Prefix.contains(cy::fixed_string("WARNING"));
};

using Warning = Logger<"[WARNING]">;

static_assert(Warning::is_warning);

Warning w;

static_assert(std::ranges::random_access_range<cy::fixed_string<1>>, "strings are ranges");
static_assert(std::ranges::bidirectional_range<cy::fixed_string<1>>, "strings are ranges");

void ranges()
{
    for(char &ch : cy::fixed_string("abc"))
    {
        ch = ' ';
    }
}

int main()
{
    return cy::test({output, ranges});
}
