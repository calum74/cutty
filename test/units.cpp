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

namespace cutty
{
template<typename T, typename Tag1, typename Tag2>
bool approx_equal(tagged<T, Tag1> x, tagged<T, Tag2> y)
{
    return approx(*x) == *tag<Tag1>(y); 
}
}

void time()
{
    cy::check_equal(60_second, 1_minute);
    cy::check_equal(3600_second, 1_hour);
}

void information()
{
    cy::check(cy::approx_equal(0.693147_nat, 1.0_bit));
    cy::check_equal(1024_byte, 1_kilobyte);
    cy::check_equal(16_bit, 2_byte);
    cy::check_equal(1024_kilobyte, 1_megabyte);
    cy::check_equal(1_megabyte, 1024ull*1024ull*8_bit);

    // This does not work due to integer overflows
    // cy::check_equal(1_terabyte, 1024_gigabyte);
}

int main()
{
    return cy::test({angles, length, time, information});
}
