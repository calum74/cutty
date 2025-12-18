#include <cutty/units.hpp>

#include <cutty/approx.hpp>
#include <cutty/test.hpp>

namespace cy = cutty;
using namespace cy::literals;

void angles()
{
    static_assert(cy::common_type<cy::degree, cy::radian>);
    static_assert(cy::convertible_to<cy::degree, cy::radian>);

    auto right_angle = 90.0_degree;
    cy::check_equal("1.5708 radians 100 gradians", cy::print_str(cy::tag<cy::radian>(right_angle), cy::tag<cy::gradian>(right_angle)));

    cy::check_equal(cy::approx(M_PI), *cy::tag<cy::radian>(180.0_degree));
    cy::check_equal(cy::approx(0.25), *cy::tag<cy::rotation>(right_angle));
    cy::check_equal(cy::approx(200), *cy::tag<cy::gradian>(180.0_degree));

    cy::check_equal(cy::tag<cy::radian>(cy::approx(M_PI)), 180.0_degree);
    cy::check_equal(cy::tag<cy::rotation>(cy::approx(0.25)), right_angle);

}


void length()
{
    cy::print(cy::tag<cy::lightyear>(1.0));

    cy::print("1ly =", cy::tag<cy::mile>(cy::tag<cy::lightyear>(1.0)));
    cy::print("1pc =", cy::tag<cy::mile>(cy::tag<cy::parsec>(1.0)));
}

void information()
{
    8_bit;
}

int main()
{
    return cy::test({angles, length, information});
}
