# Units

Units provides a set of metric and imperial units for the [tags](tags.md) library.

## Tutorial

```c++
#include <cutty/units.hpp>
namespace cy = cutty;
using namespace cy::literals;

cy::print("One mile plus one kilometer is", 1_mile + 1_kilometer);

```

The namespace `cutty::literals` provides C++ literals for all units.

By convention, all units are lowercase, and in the singular.

## List of units

| Type        | SI            | Non-SI |
| ----------- | ------------- | -------- |
| Angle       | `radian` | `degree` `gradian` `rotation` |
| Weight      | `kilogram`    | `ton`    |
|             | `gram`        | `stone`  |
|             | `tonne`       | `pound`  |
|             |               | `ounce`  |
| Length      | `meter`       | `mile`   |
|             | `kilometer`   | `yard`   |
|             | `centimeter`  | `foot`   |
|             |               | `inch`   |
|             |               | `lightyear` |
|             |               | `parsec`  |
| Time        | `second`      | `century` |
|             | `millisecond` | `year`    |
|             |               | `month`   |
|             |               | `week`    |
|             |               | `day`     |
| Volume      | `liter`       |             |
| Temperature | `kelvin`      | `Farenheit` |
|             |               | `Celcius`  |
| Mole        | `mole`        |      |
| Candela     | `candela`     |      |
| Natural     | `unit`        |
|             | `deci`        |
|             | `centi`       |
|             | `milli`       |
|             | `micro`       |
|             | `nano`        |
|             | `pico`        |
|             | `kilo`        |
|             | `mega`        |
|             | `giga`        |
| Electrical  | `ampere` `volt` `ohm` | |
| Energy      | `joule` | `erg` `calorie` |
| Power       | `watt` |
| Speed | 
| Information | `nat` | `bit` `byte` `kilobyte` `megabyte` `gigabyte` |

* SI units

```c++
template <fraction Meter = 0, fraction Kilogram = 0, fraction Seconds = 0, fraction Ampere = 0, fraction Kelvin = 0,
          fraction Mole = 0, fraction Candela = 0>
struct SI;
```

* `pico_t<T>`, `nano_t<T>`, `micro_t<T>`, `milli_t<T>`, `centi_t<T>`, `kilo_t<T>`, `mega_t<T>`, `mega_t<T>`, `tera_t<T>`
* `product_t<T1,T2>`, `per_t<T1,T2>`
