/**
    Cutty Tags library

    A "tag" is a way to wrap "naked" data types with information, which improves documentation
    and reduces common errors. Tags are a compile-time feature and do not have runtime overheads.
 */

// This is the main header file for this library.
#include <cutty/tags.hpp>

// This library includes lots of predefined units, such as time,
// SI units, bytes etc.
#include <cutty/units.hpp>

// Header files for testing
#include <cutty/approx.hpp>
#include <cutty/check.hpp>

namespace cy = cutty;

/*
    User-defined tags.
 */
struct tag1;
struct tag2;

namespace cutty
{
    // A user-defined conversion between tags of different types.
    // Specialise
template <typename T> void convert(const cy::tagged<T, tag1> &src, cy::tagged<T, tag2> &dest)
{
    *dest = *src;
}

template <typename T> void convert(const cy::tagged<T, tag2> &src, cy::tagged<T, tag1> &dest)
{
    *dest = *src;
}

} // namespace cutty

int main()
{
    cy::tagged<double, tag1> x;
    cy::tagged<double, tag1> a{12.0};
    *a = 12;
    a = cy::tag<tag1>(0.0);

    cy::tagged<double, tag2> y{10};
    y = x;
    x = x;
    x = y;
    y = cy::tag<tag2>(1.0);

    auto d = cy::tag<cy::Farenheit>(12);
    std::cout << cy::tag<cy::Farenheit>(12) << std::endl;

    {
        auto x = cy::unit<tag1>;

        // Get could just be an accessor?
        // Idea: auto& y = x / cy::units<tag1>;
    }

    // Conversions
    {
        auto x = cy::tag<cy::Farenheit>(80.0);
        cy::tagged<double, cy::Celcius> y = x;
        cy::print(cy::tag<cy::Celcius>(x));
        cy::check_equal(*y, cy::approx(26.67, 0.1));
        std::cout << x << std::endl;
        std::cout << y << std::endl;
    }

    // Multiplications with a scalar
    {
        // Technically, multiplying a temperature by a scalar is nonsense,
        // but it has an interpretation so no need to be pedantic about it
        cy::check(cy::tag<cy::Farenheit>(3.0) == cy::tag<cy::Farenheit>(1.0) * 3.0);

        // TODO: Division
    }

    // Addition of the same tag type
    {
        cy::check_equal(cy::tag<cy::Farenheit>(3), cy::tag<cy::Farenheit>(1) + cy::tag<cy::Farenheit>(2));
    }

    // Addition of compatible types
    {
        auto c = cy::tag<cy::Celcius>(2.0);
        auto d = cy::delta<cy::Farenheit>(c);
        cy::check_equal(*d, cy::approx(4.6));
        auto e = cy::tag<cy::Farenheit>(1.0) + cy::tag<cy::Celcius>(2.0);
        cy::check_equal(*e, cy::approx(36.6));
    }

    // Comparisons    
    {
        cy::check(cy::tag<cy::Celcius>(0) > cy::tag<cy::Farenheit>(0));
    }

    // Scaling conversions
    {
        static_assert(cy::common_tags<cy::byte, cy::tags::product<cy::tags::scalar<{1,8}>, cy::byte>>);
        static_assert(cy::common_tags<cy::byte, cy::tags::product<cy::byte, cy::tags::scalar<{1,8}>>>);
        cy::check_equal(*cy::tag<cy::bit>(cy::tag<cy::byte>(2)), 16);

        cy::check_equal(cy::tag<cy::second>(60), cy::tag<cy::minute>(1));
    };

    // Length conversions
    cy::print(cy::tag<cy::yard>(1), '=', cy::tag<cy::meter>(cy::tag<cy::yard>(1.0)));
    cy::check_equal(*cy::tag<cy::meter>(cy::tag<cy::yard>(1.0)), cy::approx(0.9144));

    static_assert(std::same_as<cy::meter, cy::detail::strip_scalars_t<cy::inch>>);

    cy::print(cy::tag<cy::yard>(1), '=', cy::tag<cy::inch>(cy::tag<cy::yard>(1.0)));
    cy::print(cy::tag<cy::inch>(1), '=', cy::tag<cy::cm>(cy::tag<cy::inch>(1.0)));
    cy::check_equal(cy::print_str(cy::tag<cy::inch>(1)), "1\"");

    // Weight conversions
    cy::check_equal(cy::print_str(cy::tag<cy::gram>(10)), "10g");
    cy::check_equal(cy::print_str(cy::tag<cy::gram>(cy::tag<cy::ounce>(1.0))), "28.3495g");

    // SI units
    {
        cy::print("5 seconds = ", cy::tag<cy::second>(5));
        cy::print("5 seconds = ", cy::tag<cy::hour>(cy::tag<cy::second>(5.0)));
    }

    // Convert meters to second to miles per hour
    {
        auto x = cy::tag<cy::mile>(70.0);
        auto y = cy::tag<cy::hour>(2.0);
        cy::check_equal(cy::print_str(x, "in", y, "=", x/y), "70 miles in 2 hours = 35 miles/hour");


        auto a = cy::tag<cy::meter>(10.0);
        auto b = cy::tag<cy::second>(2.0);
        cy::check_equal(cy::print_str(a, "in", b, "=", a/b), "10m in 2s = 5m/s");

        auto z = x/y;
        cy::tagged<double, cy::speed> c = a/b;

        using T1 = cy::speed;
        using T2 = cy::tags::divide<cy::mile, cy::hour>;
        using S1 = typename cy::detail::strip_scalars<T1>::type;
        using S2 = typename cy::detail::strip_scalars<T2>::type;
        static_assert(std::same_as<S1, S2>);
        z = c;
        cy::check_equal(*cy::tag<cy::tags::divide<cy::mile, cy::hour>>(c), cy::approx(11.18468146));
        cy::check_equal(*cy::tag<cy::kilometer>(cy::tag<cy::mile>(1.0)), cy::approx(1.609344));
    }

    // Literals
}
