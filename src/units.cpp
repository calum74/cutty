#include <cutty/units.hpp>

namespace cy = cutty;

cy::tagged<double, cy::radian> cy::literals::operator""_radian(long double d)
{
    return tag<radian>(d);
}

cy::tagged<double, cy::degree> cy::literals::operator""_degree(long double d)
{
    return tag<degree>(d);
}

cy::tagged<double, cy::gradian> cy::literals::operator""_gradian(long double d)
{
    return tag<gradian>(d);
}

cy::tagged<double, cy::rotation> cy::literals::operator""_rotation(long double d)
{
    return tag<rotation>(d);
}
