#include <cutty/einsum.hpp>

#include <cutty/test.hpp>

namespace cy = cutty;

// static_assert(cy::einsum<"->">);
// static_assert(cy::einsum<"a->a">);

static_assert(cy::einsum::rhs<"a->b"> == "b");
static_assert(cy::einsum::cost_estimates::unique<"abc", 'a'>, "a is unique in abc");
static_assert(cy::einsum::cost_estimates::duplicated<"abca", 'a'>, "a is duplicated in abca");
static_assert(cy::einsum::contains_duplicates<"abcb">, "abcb contains duplicates");
static_assert(!cy::einsum::contains_duplicates<"abc">, "abbc contains duplicates");

int main()
{
    return cy::test({});
}