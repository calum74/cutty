# Units

Units provides a set of metric and imperial units for the [tags](tags.md) library.

## Tutorial

```c++
#include <cutty/units.hpp>
namespace cy = cutty;

```


## List of units

| Type        | SI            | Non-SI |
| ----------- | ------------- | -------- |
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
| Temperature | `Kelvin`      | `Farenheit` |
|             |               | `Celcius`  |
| Mole        | `mole`        |      |
| Candela     | `Candela`     |      |
| Natural     | `unit`        |
|             | `deci`        |
|             | `centi`       |
|             | `milli`       |
|             | `micro`       |
|             | `nano`        |
|             | `pico`        |
|             | `kilo`        |
|             | `Mega`        |
|             | `Giga`        |
| Electrical  | `Ampere` `Volt` `Ohm` | |
| Energy      | `Joule` | `erg` `calorie` |
| Power       | `Watt` |
| Speed | 
| Information | `nat` | `bit` `byte` `kilobyte` `megabyte` `gigabyte` |
| Angle       | `radian` | `degree` `gradian` `rotation` |

* SI units

```c++
template <fraction Meter = 0, fraction Kilogram = 0, fraction Seconds = 0, fraction Ampere = 0, fraction Kelvin = 0,
          fraction Mole = 0, fraction Candela = 0>
struct SI;
```

* `milli_t<T>`, `centi_t<T>`, `mega_t<T>`, `kilo_t<T>`, `micro_t<T>`
* `product_t<T1,T2>`, `per_t<T1,T2>`
