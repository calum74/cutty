#pragma once

#include "tags.hpp"

#include <iostream>

namespace cutty
{
/// Scalars
struct degree;
struct radian;
struct gradian;

using kilo = tags::scalar<1000>;
template <> const char *tag_symbol<kilo> = "k";

struct tera;
struct giga;
struct mega;

using milli = tags::scalar<{1, 1000}>;
template <> const char *tag_symbol<milli> = "m";

using micro = tags::scalar<{1, 1000000}>;
template <> const char *tag_symbol<micro> = "µ";

using centi = tags::scalar<{1, 100}>;
template <> const char *tag_symbol<centi> = "c";

struct nano;
struct pico;

/// SI units
template <fraction Meter = 0, fraction Kilogram = 0, fraction Seconds = 0, fraction Ampere = 0, fraction Kelvin = 0,
          fraction Mole = 0, fraction Candela = 0>
struct SI;

template <fraction Meter, fraction Kilogram, fraction Second, fraction Ampere, fraction Kelvin, fraction Mole,
          fraction Candela>
struct tag_traits<SI<Meter, Kilogram, Second, Ampere, Kelvin, Mole, Candela>> : default_tag_traits<SI<Meter, Kilogram, Second, Ampere, Kelvin, Mole, Candela>>
{
    using tag_type = SI<Meter, Kilogram, Second, Ampere, Kelvin, Mole, Candela>;

    static void write(std::ostream &os, Plural plural, PadWithSpace pad)
    {
        if(!detail::write_tag<tag_type>(os, plural, pad))
        {
            auto output = [&](fraction p, const char *name) {
                if (p != 0)
                {
                    os << name;
                    if (p != 1)
                    {
                        os << "^" << p;
                    }
                }
            };
            output(Meter, "m");
            output(Kilogram, "kg");
            output(Second, "s");
            output(Ampere, "A");
            output(Kelvin, "K");
            output(Mole, "mol");
            output(Candela, "cd");
        }
    }
};

template<>
struct simplify<SI<>> : simplify<tags::unit> {};

template <fraction Meter, fraction Kilogram, fraction Second, fraction Ampere, fraction Kelvin, fraction Mole,
          fraction Candela, fraction P>
struct simplify<tags::power<SI<Meter, Kilogram, Second, Ampere, Kelvin, Mole, Candela>, P>> :
    simplify<SI<Meter*P, Kilogram*P, Second*P, Ampere*P, Kelvin*P, Mole*P, Candela*P>>
{
};

template <fraction Meter1, fraction Kilogram1, fraction Second1, fraction Ampere1, fraction Kelvin1, fraction Mole1,
          fraction Candela1,
          fraction Meter2, fraction Kilogram2, fraction Second2, fraction Ampere2, fraction Kelvin2, fraction Mole2,
          fraction Candela2,
          typename... Tags
          >
struct simplify<
    tags::product<SI<Meter1, Kilogram1, Second1, Ampere1, Kelvin1, Mole1, Candela1>, SI<Meter2, Kilogram2, Second2, Ampere2, Kelvin2, Mole2, Candela2>, Tags...>> :
    simplify<tags::product<SI<Meter1+Meter2, Kilogram1+Kilogram2, Second1+Second2, Ampere1+Ampere2, Kelvin1+Kelvin2, Mole1+Mole2, Candela1+Candela2>, Tags...>>
{
};


using meter = SI<1>;
using kilogram = SI<0, 1>;
using second = SI<0, 0, 1>;
using amp = SI<0, 0, 0, 1>;
using Kelvin = SI<0, 0, 0, 0, 1>;
using mol = SI<0, 0, 0, 0, 0, 1>;
using candela = SI<0, 0, 0, 0, 0, 0, 1>;

using Joule = SI<2, 1, -2>;
template <> const char *tag_symbol<Joule> = "J";
using Volt = SI<2, 1, -3, -1>;
template <> const char *tag_symbol<Volt> = "V";
using Newton = SI<1, 1, -2>;
template <> const char *tag_symbol<Newton> = "N";

using Ohm = SI<2, 1, -3, -2>;
template <> const char *tag_symbol<Ohm> = "Ω";

using speed = tags::divide<meter, second>;

/// Temperature
struct Celcius;
template <> const char *tag_symbol<Celcius> = "°C";
template <> struct tag_traits<Celcius> : default_tag_traits<Celcius>
{
    using common_type = Kelvin;
};

using Centigrade = Celcius;

struct Farenheit;
template <> const char *tag_symbol<Farenheit> = "°F";
template <> struct tag_traits<Farenheit> : default_tag_traits<Farenheit>
{
    using common_type = Kelvin;
};

template <typename T> void convert(const tagged<T, Celcius> &c, tagged<T, Kelvin> &k)
{
    *k = *c + 273.15;
}

template <typename T> void convert(const tagged<T, Kelvin> &k, tagged<T, Celcius> &c)
{
    *c = *k - 273.15;
}

template <typename T> void convert(const tagged<T, Kelvin> &k, tagged<T, Farenheit> &f)
{
    *f = 9 * (*k - 273.15) / 5 + 32;
}

template <typename T> void convert(const tagged<T, Farenheit> &f, tagged<T, Kelvin> &k)
{
    *k = 273.15 + 5 * (*f - 32) / 9;
}

/// Time

using milliseconds = tags::product<milli, second>;
using minute = tags::product<second, tags::scalar<60>>;
template <> const char *tag_text<minute> = "minute";

using hour = tags::product<minute, tags::scalar<60>>;
template <> const char *tag_text<hour> = "hour";

using day = tags::product<hour, tags::scalar<24>>;

template <> const char *tag_text<day> = "day";

using week = tags::product<day, tags::scalar<7>>;
template <> const char *tag_text<week> = "week";

struct months;
using years = tags::product<months, tags::scalar<12>>;
using centuries = tags::product<years, tags::scalar<100>>;
using millenia = tags::product<years, tags::scalar<1000>>;

///////////////////////////////////////////////////////////////////////////////
// Length
using yard = tags::product<meter, tags::scalar<{9144, 10000}>>;
template <> const char *tag_text<yard> = "yard";
using foot = tags::product<yard, tags::scalar<{1, 3}>>;
template <> const char *tag_symbol<foot> = "\'";
using inch = tags::product<foot, tags::scalar<{1, 12}>>;
template <> const char *tag_symbol<inch> = "\"";
using mile = tags::product<yard, tags::scalar<1760>>;
template <> const char *tag_text<mile> = "mile";

using cm = tags::product<centi, meter>;
using kilometer = tags::product<kilo, meter>;

// Mass
using gram = tags::product<tags::scalar<{1, 1000}>, kilogram>;
template <> const char *tag_symbol<gram> = "g";

using ounce = tags::product<gram, tags::scalar<{283495, 10000}>>;
template <> const char *tag_symbol<ounce> = "oz";

using pound = tags::product<ounce, tags::scalar<16>>;
template <> const char *tag_symbol<pound> = "lb";
using stone = tags::product<pound, tags::scalar<14>>;
template <> const char *tag_symbol<stone> = "st";

// Volume
// !! TODO

using percent = tags::scalar<{1, 100}>;
// template <> const char *tag_suffix<percent> = "%";

// !! Conversion from chrono
struct epoch_time;
struct unix_time;

struct byte;
template <> const char *tag_text<byte> = "byte";

using bit = tags::product<byte, tags::scalar<{1, 8}>>;
template <> const char *tag_text<bit> = "bit";

// using kilobytes = scalar_tag<bytes, {1024}>;
// using megabytes = scalar_tag<kilobytes, {1024}>;
// using gigabytes = scalar_tag<megabytes, 1024>;
//  terabytes
} // namespace cutty