#include <cutty/check.hpp>
#include <cutty/tags.hpp>

namespace cy = cutty;

struct tag1;
struct tag2;
struct Celcius;
struct Farenheit;
using Centigrade = Celcius;

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

template<typename T>
const char * tag_suffix(tagged<T, Celcius>) { return "'C"; }

template<typename T>
const char * tag_suffix(tagged<T, Farenheit>) { return "'F"; }

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
    y = cy::tag<tag2> (1.0);

    auto d = cy::tag<Farenheit>(12);
    std::cout << cy::tag<Farenheit>(12) << std::endl;

    {
        auto x = cy::unit<tag1>;

        // Get could just be an accessor?
        // Idea: auto& y = x / cy::units<tag1>;
    }
}