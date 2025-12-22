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
    cy::print(cy::pretty_type<std::string>());
    cy::print(cy::pretty_type<int>());
    cy::print(cy::pretty_type<bool>());
    cy::print(cy::pretty_type<Foo::Bar>());
    cy::print(cy::pretty_type<Foo::Bar::Baz>());
    cy::print(cy::pretty_type<Foo::Bar::Boo<Foo::Bar>>());
}