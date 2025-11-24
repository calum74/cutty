#include <cutty/check.hpp>
#include <cutty/sequence.hpp>

namespace cy = cutty;

static_assert(std::input_or_output_iterator<cy::sequence<int>::iterator>);
static_assert(std::ranges::input_range<cy::sequence<int>>);

void fn(const cy::sequence<int>&n)
{
}

int main()
{
    auto s1 = cy::seq(1,10);

    // !! Make this a typedef
    const cy::sequence<int> &s2 = s1;

    for(auto x : s1 | std::views::filter([](int x) { return x<=5;}))
    {
    }

    for(auto y : cy::seq(s2 | std::views::filter([](int x) { return x<=5;})))
    {
    }

    fn(cy::seq(s2 | std::views::filter([](int x) { return x<=5;})));
}
