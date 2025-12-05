#include <cutty/approx.hpp>
#include <cutty/check.hpp>
#include <cutty/tags.hpp>
#include <cutty/units.hpp>

namespace cy = cutty;

struct tag1;
struct tag2;

namespace cutty
{
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
    y = x; // x = y;
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
}