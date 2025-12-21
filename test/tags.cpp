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
#include <cutty/test.hpp>

namespace cy = cutty;

// Basic usage
// Decide on a unique tag name, for example verbosity_t,
// then define a tagged type to use it.

using Verbose = cy::tagged<bool, struct verbosity_t>;

void welcome(Verbose verbose)
{
    // The * operator can be used to access the underlying value
    if (*verbose)
    {
        cy::print("Welcome to tags!");
    }
}

void basic_tags()
{
    welcome(Verbose{false});
}

void conversions()
{
    // The cy::tag function creates a tagged value of a given tag.
    // The units library provides many tags representing different quantities
    auto grams = cy::tag<cy::gram>(1000);

    // Compatible tags are automatically scaled as needed
    cy::check_equal(1000, *cy::tag<cy::gram>(cy::tag<cy::kilogram>(1)));
    cy::check_equal(cy::tag<cy::gram>(1000), cy::tag<cy::kilogram>(1));

    // Passing or assigning one unit to another will automatically convert the value
    // to the new unit.
    grams = cy::tag<cy::pound>(4);
    cy::check_equal(cy::tag<cy::gram>(1814), cy::tag<cy::pound>(4));

    // Be a bit careful with integers however, as they might get rounded down
    cy::check_equal(cy::tag<cy::stone>(0), cy::tag<cy::kilogram>(1));
}

// User-defined conversions

struct USD;
struct GBP;

double rate = 1.1;

namespace cutty
{
// A user-defined conversion between tags of different types.
template <typename T> void convert(const cy::tagged<T, USD> &src, cy::tagged<T, GBP> &dest)
{
    *dest = *src / rate;
}

template <typename T> void convert(const cy::tagged<T, GBP> &src, cy::tagged<T, USD> &dest)
{
    *dest = *src * rate;
}
} // namespace cutty

// Scalar conversions are defined using the scalar type as follows:
using GBp = cy::tags::product<GBP, cy::tags::scalar<{1,100}>>;

void pay_me(cy::tagged<double, USD> amount)
{
}

void user_conversions()
{
    // Different units are all converted correctly
    pay_me(cy::tag<USD>(10.0));
    pay_me(cy::tag<GBP>(1.0));
    pay_me(cy::tag<GBp>(250.0));

    cy::check_equal(cy::tag<GBP>(4), cy::tag<GBp>(400));
}

// Tag IO

template <> const char *cy::tag_text<USD> = "dollar";
template <> const char *cy::tag_text<GBP> = "pound";
template <> const char *cy::tag_symbol<GBp> = "p";

void io()
{
    // Output: 10 dollars
    std::cout << cy::tag<USD>(10) << std::endl;
    cy::check_equal(cy::print_str(cy::tag<USD>(1)), "1 dollar");
    cy::check_equal(cy::print_str(cy::tag<GBP>(2)), "2 pounds");
    cy::check_equal(cy::print_str(cy::tag<GBp>(5)), "5p");
}

void arithmetic()
{
    // Addition and subtraction can be done between compatible types
    cy::check_equal(cy::tag<USD>(5), cy::tag<USD>(2) + cy::tag<USD>(3));
    cy::check_equal(cy::tag<USD>(5), cy::tag<USD>(7) - cy::tag<USD>(2));

    // Addition of incompatible types is not allowed
    // cy::tag<USD>(2) + 3;

    // Multiplication and division by a scalar results in the same type
    cy::check_equal(cy::tag<USD>(15), cy::tag<USD>(5) * 3);
    cy::check_equal(cy::tag<USD>(10), 2 * cy::tag<USD>(5));
    cy::check_equal(cy::tag<USD>(4), cy::tag<USD>(12)/3);

    // Addition and subtraction of convertible types results in the LHS type
    cy::check_equal(cy::tag<GBP>(4), cy::tag<GBP>(3) + cy::tag<GBp>(100));

    // Multiplication of general tags multiplies the tags together
    cy::check_equal(cy::tag<cy::meter>(1) * cy::tag<cy::foot>(1), cy::tag<cy::centimeter>(100) * cy::tag<cy::inch>(12));  
    cy::check_equal(cy::tag<cy::joule>(10), cy::tag<cy::meter>(2) * cy::tag<cy::newton>(5));

    auto rate = cy::tag<USD>(20) / cy::tag<cy::mile>(4);
    cy::check_equal("Cost is 5 dollars/mile", cy::print_str("Cost is", rate));
}

void comparisons()
{
    // Identical tags can be compared
    cy::check(cy::tag<cy::meter>(1) < cy::tag<cy::meter>(2));

    // Convertible tags can also be compared
    cy::check(cy::tag<USD>(1.00) < cy::tag<GBP>(1.00));
}

// Mixins

// You can define mixins on cy::tagged, using the mixin cy::tagged_methods<T>

struct Version;
using Verbosity = cy::tagged<int, struct verbosity_tag>;

template <typename V> struct cy::mixin<cy::tagged<V, Version>, cy::tagged_methods<Version>>
{
    // Sadly, g++ does not support this yet:
    // bool supports_feature_A(this const cy::tagged<V, Version> &v)
    bool supports_feature_A() const
    {
        return **static_cast<const cy::tagged<V, Version>*>(this) >= 2;
    }

    // bool compatible_with(this const cy::tagged<V, Version> &v, const cy::tagged<V, Version> &v2)
    bool compatible_with(const cy::tagged<V, Version> &v2) const
    {
        return **static_cast<const cy::tagged<V, Version>*>(this) >= *v2;
    }
};

// This mixin applies to a specific tagged type, Verbosity.
// Note that we need to specify the *tag* verbosity_tag in tagged_methods.
template <> struct cy::mixin<Verbosity, cy::tagged_methods<verbosity_tag>>
{
    // bool quiet(this const Verbosity &v)
    bool quiet() const
    {
        return **static_cast<const Verbosity*>(this) == 0;
    }

    // bool output_progress(this const Verbosity &v)
    bool output_progress() const
    {
        return **static_cast<const Verbosity*>(this) >= 2;
    }
};

void mixins()
{
    cy::check(cy::tag<Version>(5).supports_feature_A());
    cy::check(cy::tag<Version>(5).compatible_with(cy::tag<Version>(4)));
    cy::check(Verbosity(0).quiet());
}

int main()
{
    // Conversions
    {
        auto x = cy::tag<cy::farenheit>(80.0);
        cy::tagged<double, cy::celsius> y = x;
        cy::print(cy::tag<cy::celsius>(x));
        cy::check_equal(*y, cy::approx(26.67, 0.1));
        std::cout << x << std::endl;
        std::cout << y << std::endl;
    }

    // Multiplications with a scalar
    {
        // Technically, multiplying a temperature by a scalar is nonsense,
        // but it has an interpretation so no need to be pedantic about it
        cy::check(cy::tag<cy::farenheit>(3.0) == cy::tag<cy::farenheit>(1.0) * 3.0);

        // TODO: Division
    }

    // Addition of the same tag type
    {
        cy::check_equal(cy::tag<cy::farenheit>(3), cy::tag<cy::farenheit>(1) + cy::tag<cy::farenheit>(2));
    }

    // Addition of compatible types
    {
        auto c = cy::tag<cy::celsius>(2.0);
        auto d = cy::delta<cy::farenheit>(c);
        cy::check_equal(*d, cy::approx(4.6));
        auto e = cy::tag<cy::farenheit>(1.0) + cy::tag<cy::celsius>(2.0);
        cy::check_equal(*e, cy::approx(36.6));
    }

    // Comparisons
    {
        cy::check(cy::tag<cy::celsius>(0) > cy::tag<cy::farenheit>(0));
    }

    // Scaling conversions
    {
        static_assert(cy::common_tags<cy::byte, cy::tags::product<cy::tags::scalar<{1, 8}>, cy::byte>>);
        static_assert(cy::common_tags<cy::byte, cy::tags::product<cy::byte, cy::tags::scalar<{1, 8}>>>);
        cy::check_equal(*cy::tag<cy::bit>(cy::tag<cy::byte>(2)), 16);

        cy::check_equal(cy::tag<cy::second>(60), cy::tag<cy::minute>(1));
    };

    // Length conversions
    cy::print(cy::tag<cy::yard>(1), '=', cy::tag<cy::meter>(cy::tag<cy::yard>(1.0)));
    cy::check_equal(*cy::tag<cy::meter>(cy::tag<cy::yard>(1.0)), cy::approx(0.9144));

    static_assert(std::same_as<cy::meter, cy::detail::strip_scalars_t<cy::inch>>);

    cy::print(cy::tag<cy::yard>(1), '=', cy::tag<cy::inch>(cy::tag<cy::yard>(1.0)));
    cy::print(cy::tag<cy::inch>(1), '=', cy::tag<cy::centimeter>(cy::tag<cy::inch>(1.0)));
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
        cy::check_equal(cy::print_str(x, "in", y, "=", x / y), "70 miles in 2 hours = 35 miles/hour");

        auto a = cy::tag<cy::meter>(10.0);
        auto b = cy::tag<cy::second>(2.0);
        cy::check_equal(cy::print_str(a, "in", b, "=", a / b), "10m in 2s = 5m/s");

        auto z = x / y;
        cy::tagged<double, cy::speed> c = a / b;

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

    return cy::test({basic_tags, io, conversions, arithmetic, comparisons, user_conversions, mixins});
}
