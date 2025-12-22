#include <cutty/check.hpp>
#include <cutty/pretty_type.hpp>
namespace cy = cutty;

namespace Foo
{
struct Bar
{
    struct Baz
    {
    };

    template <typename T> struct Boo
    {
    };
};
} // namespace Foo

int main()
{
    cy::check_equal(cy::pretty_type<std::string>(), "std::string");
    cy::check_equal(cy::pretty_type<int>(), "int");
    cy::check_equal(cy::pretty_type<bool>(), "bool");
    cy::check_equal(cy::pretty_type<Foo::Bar>(), "Foo::Bar");
    cy::check_equal(cy::pretty_type<Foo::Bar::Baz>(), "Foo::Bar::Baz");
    cy::check_equal(cy::pretty_type<Foo::Bar::Boo<Foo::Bar>>(), "Foo::Bar::Boo<Foo::Bar>");
}