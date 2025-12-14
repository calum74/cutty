#include <cutty/units.hpp>

#include <cutty/approx.hpp>
#include <cutty/test.hpp>

namespace cy = cutty;

void angles()
{
    static_assert(cy::common_type<cy::degree, cy::radian>);
    static_assert(cy::convertible_to<cy::degree, cy::radian>);

    auto right_angle = cy::tag<cy::degree>(90.0);
    cy::print(cy::tag<cy::radian>(right_angle), cy::tag<cy::gradian>(right_angle));

    cy::check_equal(cy::approx(M_PI), *cy::tag<cy::radian>(cy::tag<cy::degree>(180.0)));
    cy::check_equal(cy::approx(0.25), *cy::tag<cy::rotation>(right_angle));
}

void length()
{
    cy::print(cy::tag<cy::lightyear>(1.0));

    cy::print("1ly =", cy::tag<cy::mile>(cy::tag<cy::lightyear>(1.0)));
    cy::print("1pc =", cy::tag<cy::mile>(cy::tag<cy::parsec>(1.0)));
}

int main()
{
    return cy::test({angles, length});
}
