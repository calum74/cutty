# Units

Sample: [units.cpp](../samples/units.cpp)

Units provides a set of metric and imperial units for the [tags](tags.md) library.

## Tutorial

The purpose of the units library is to provide better code safety and convenience when storing physical quantities. If you accidentally try to do something nonsensical, like adding a length to a time, then you will get a compilation error. If you are representing units of different scales, for example bytes versus kilobytes, then the value will be automatically converted. These features improve the documentation of your code and avoid many classes of error.

The main header file for this library is

```c++
#include <cutty/units.hpp>
```

As usual, all symbols are in the `cutty` namespace, which we will alias with

```c++
namespace cy = cutty;
```

To use unit literals, you must use the namespace

```c++
using namespace cy::literals;
```

Defining a tagged value is done using the `tagged` class, supplying the data type and the unit type.

```c++
    cy::tagged<int, cy::meter> length;
```

See the [tags](tags.md) library for working with `tagged` data. For example to create a value of 10 meters, you can write

```c++
    cy::tagged<int, cy::meter> length(10);
```

The `*` operator provides access to the stored value. Literals provide a shortcut to creating values tagged with the appropriate unit.

```c++
    auto length = 10_meter;
```

Conversions are handled automatically, provided that the units are compatible. To explicitly convert to a different unit, you can use the `tag<>` function which creates a tagged value with the specified unit. For example

```c++
    double km = *cy::tag<cy::kilometer>(1.0_mile);
```

Tagged values support natural arithmetic and comparison operations. When different units are added or subtracted, the second operand is converted to the first. 
```c++
    cy::print("One mile and 100 yards in km is", cy::tag<cy::kilometer>(1.0_mile + 100.0_yard));
    // Output: One mile and 100 yards in km is 1.70078km 
```

When different units are multiplied or divided, then the units are also multiplied and divided

```c++
    cy::tagged<double, cy::ohm> resistance = 220.0_volt / 13.0_ampere;
```

By convention, all units are lowercase, and in the singular form.

## List of units

| Type        | SI            | Non-SI   |
| ----------- | ------------- | -------- |
| Angle       | `radian`      | `degree`  |
|             |               | `gradian` |
|             |               | `rotation` |
| Mass        | `gram`        | `ounce`  |
|             | `kilogram`    | `pound`  |
|             | `tonne`       | `stone`  |
|             |               | `short_ton` |
|             |               | `long_ton` |
| Length      | `millimeter`  | `inch`   |
|             | `centimeter`  | `foot`   |
|             | `meter`       | `yard`   |
|             | `kilometer`   | `mile`   |
|             |               | `lightyear` |
|             |               | `parsec`  |
| Time        | `millisecond` | `hour`    |
|             | `second`      | `day`     |
|             | `hertz`       | `week`    |
|             |               | `month`   |
|             |               | `year`    |
|             |               | `century` |
|             |               | `millennium` |
| Volume      | `milliliter`  | `us_teaspoon` |
|             | `centiliter`  | `us_tablespoon` |
|             | `deciliter`   | `us_fluid_ounce` |
|             | `liter`       | `us_cup`   |
|             |               | `us_pint`  |
|             |               | `us_quart`  |
|             |               | `us_gallon`  |
|             |               | `imperial_teaspoon`  |
|             |               | `imperial_tablespoon`  |
|             |               | `imperial_fluid_ounce`  |
|             |               | `imperial_cup`  |
|             |               | `imperial_pint`  |
|             |               | `imperial_quart`  |
|             |               | `imperial_gallon`  |
| Temperature | `kelvin`      | `farenheit` |
|             |               | `celsius`  |
| Mole        | `mole`        |      |
| Candela     | `candela`     |      |
| Electrical  | `ampere`      | |
|             | `coulomb`     | |
|             | `henry`       | |
|             | `ohm`         | |
|             | `tesla`       | |
|             | `volt`        | |
|             | `watt`        | |
|             | `weber`       | |
| Force       | `newton`      | |
| Energy      | `joule`       | `electronvolt` |
|             |               | `erg`      |
|             |               | `calorie`  |
|             |               | `kcal`     |
| Information | `bit`         | `byte`     |
|             |               | `nat`      |
|             |               | `kilobyte` |
|             |               | `megabyte` |
|             |               | `gigabyte` |
|             |               | `terabyte` |
