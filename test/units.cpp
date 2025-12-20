#include <cutty/units.hpp>

#include <cutty/approx.hpp>
#include <cutty/test.hpp>

namespace cy = cutty;
using namespace cy::literals;

template <typename T, typename Tag1, typename Tag2>
void check_approx_equal(cy::tagged<T, Tag1> x, cy::tagged<T, Tag2> y,
                        std::source_location src = std::source_location::current())
{
    cy::check_equal(cy::approx(*x), *tag<Tag1>(y), src);
}

void angles()
{
    static_assert(cy::common_type<cy::degree, cy::radian>);
    static_assert(cy::convertible_to<cy::degree, cy::radian>);

    auto right_angle = 90.0_degree;
    cy::check_equal("1.5708 radians 100 gradians",
                    cy::print_str(cy::tag<cy::radian>(right_angle), cy::tag<cy::gradian>(right_angle)));

    check_approx_equal(cy::tagged<long double, cy::radian>(M_PI), 180.0_degree);
    check_approx_equal(0.25_rotation, right_angle);
    check_approx_equal(100_gradian, 90_degree);
}

void length()
{
    cy::check_equal(cy::print_str(cy::tag<cy::lightyear>(1.0)), "1 light-year");
    check_approx_equal(1.0_lightyear, 5.87863e+12_mile);
    check_approx_equal(1.0_parsec, 1.91735e+13_mile);
}

void time()
{    
    cy::check_equal(10_second, 10000_millisecond);
    cy::check_equal(60_second, 1_minute);
    cy::check_equal(3600_second, 1_hour);
    cy::check_equal(1_day, 24_hour);
    cy::check_equal(2_week, 14_day);
    cy::check_equal(12_month, 1_year);
    cy::check_equal(1200_month, 1_century);
}

void information()
{
    check_approx_equal(0.693147_nat, 1.0_bit);
    cy::check_equal(1024_byte, 1_kilobyte);
    cy::check_equal(16_bit, 2_byte);
    cy::check_equal(1024_kilobyte, 1_megabyte);
    cy::check_equal(1_megabyte, 1024ull * 1024ull * 8_bit);

    // This does not work due to integer overflows
    // cy::check_equal(1_terabyte, 1024_gigabyte);
}

void energy()
{
    cy::tagged<double, cy::joule> j = 1_joule;

    cy::check_equal(1_joule, 1_meter * 1_newton);
    cy::check_equal(1000_joule, 1_kilojoule);
    cy::check_equal(2_joule, 2ull * 1_meter * 1_newton);
    check_approx_equal(1.0_calorie, 4.184_joule);
    cy::check_equal(2000_calorie, 2_kcal);
    cy::check_equal(1.0_erg, 1e-7_joule);
    cy::check_equal(1.0_electronvolt, 1.602176634e-19_joule);
    check_approx_equal(2200.0_calorie, 5.745184273e+22_electronvolt);
}

void temperature()
{
    cy::check_equal(273.15_kelvin, 0.0_celsius);
    cy::check_equal(89.6_farenheit, 32.0_celsius);
}

void electrical()
{
    // V=IR
    cy::check_equal(6_volt, 3_ampere * 2_ohm);
    
    // q = CV
    cy::check_equal(15_coulomb, 3_farad * 5_volt);

    cy::check_equal(1_coulomb, 1_ampere * 1_second);

    cy::check_equal(1_joule, 1_henry * 1_ampere * 1_ampere);

    cy::check_equal(1_tesla, 1_newton * 1_second / (1_coulomb * 1_meter));

    //cy::check_equal(1_tesla, 1_newton * 1_second / 1_coulomb / 1_meter);

    cy::check_equal(1_weber, 1_volt * 1_second);
    cy::check_equal(1_weber, 1_tesla * 1_meter * 1_meter);
}

int main()
{
    return cy::test({angles, length, temperature, time, information, energy, {"electrical", electrical}});
}
