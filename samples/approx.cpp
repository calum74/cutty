#include <cutty/approx.hpp>
#include <cutty/check.hpp>

#include <numbers>

namespace cy = cutty;

int main()
{
    // Approx gives a neater syntax for comparing values within a tolerance
    cy::check(1 == cy::approx(1));

    // You can also specify a tolerance explicitly
    cy::check(3.1415 == cy::approx(std::numbers::pi, 1e-3));

    // The default tolerance is usually within 1e-6
    std::cout << "1 is approximately " << cy::approx(1) << std::endl;

    auto test = [](auto lhs, auto rhs, std::weak_ordering result)
    {
        cy::check((lhs<=>rhs) == result);
        cy::check(((lhs<=>rhs) == std::weak_ordering::equivalent) == (lhs==rhs));
    };

    // Test equal values
    test(1, cy::approx(1), std::weak_ordering::equivalent);
    test(cy::approx(1), 1, std::weak_ordering::equivalent);

    test(1.00001, cy::approx(1, 0.001), std::weak_ordering::equivalent);
    test(cy::approx(1, 0.001), 1.00001, std::weak_ordering::equivalent);

    test(0.9999, cy::approx(1, 0.001), std::weak_ordering::equivalent);
    test(cy::approx(1, 0.001), 0.9999, std::weak_ordering::equivalent);

    test(0.99, cy::approx(1, 0.001), std::weak_ordering::less);
    test(cy::approx(1, 0.001), 0.99, std::weak_ordering::greater);

    test(1.01, cy::approx(1, 0.001), std::weak_ordering::greater);
    test(cy::approx(1, 0.001), 1.01, std::weak_ordering::less);
}