#include <cutty/units.hpp>
#include <cutty/print.hpp>

namespace cy = cutty;

using namespace cy::literals;

int main()
{
    auto t = 1_hour;
    cy::print(t, "is", cy::tag<cy::minute>(t));
    cy::tagged<double, cy::ohm> resistance = 220.0_volt / 13.0_ampere;
    cy::print("The required resistance is", resistance);
    cy::print("One mile and 100 yards in km is", cy::tag<cy::kilometer>(1.0_mile + 100.0_yard));
    cy::print(100_celsius > 100_farenheit ? "100 Celsius is greater" : "100 Farenheit is greater");

    /*
        Output:
        1 hour is 60 minutes 
        The required resistance is 16.9231Ω 
        One mile and 100 yards in km is 1.70078km 
        100 Celsius is greater        
    */
}
