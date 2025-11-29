#include <cutty/check.hpp>
#include <cutty/units.hpp>

namespace cy = cutty;

int main()
{
    auto x = cy::tag<cy::Farenheit>(100.0);
    cy::tagged<double, cy::Kelvin> y = x;
}
