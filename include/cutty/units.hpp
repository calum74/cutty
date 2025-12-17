#pragma once

#include "tags.hpp"

#include <iostream>

namespace cutty
{
////////////////////////////////////////////////////////////////////////    
// Angles

struct radian;
template<> inline const char *tag_text<radian> = "radian";

template <> struct tag_traits<radian> : default_tag_traits<radian>
{
    // Angles are pure ratios so can be converted via unit
    using common_type = tags::unit;
};

using degree = tags::product<tags::dscalar<M_PI/180>, radian>;
template<> inline const char *tag_text<degree> = "degree";

template <> struct tag_traits<degree> : default_tag_traits<degree>
{
    using common_type = tags::unit;
};

using gradian = tags::product<tags::scalar<{360,400}>, degree>;
template<> inline const char *tag_text<gradian> = "gradian";

template <> struct tag_traits<gradian> : default_tag_traits<gradian>
{
    using common_type = tags::unit;
};

using rotation = tags::product<tags::dscalar<2*M_PI>, radian>;
template<> inline const char *tag_text<rotation> = "rotation";

template <> struct tag_traits<rotation> : default_tag_traits<rotation>
{
    using common_type = tags::unit;
};

namespace literals
{
    tagged<double, radian> operator""_radian(long double d);
    tagged<double, degree> operator""_degree(long double d);
    tagged<double, gradian> operator""_gradian(long double d);
    tagged<double, rotation> operator""_rotation(long double d);
}

////////////////////////////////////////////////////////////////////////    
// Scalars

using kilo = tags::scalar<1000>;
template <> inline const char *tag_symbol<kilo> = "k";
template<typename T> using kilo_t = tags::product<kilo, T>;

using mega = tags::scalar<1000000>;
template <> inline const char *tag_symbol<mega> = "M";
template<typename T> using mega_t = tags::product<mega, T>;

using giga = tags::dscalar<1e9>;
template <> inline const char *tag_symbol<giga> = "G";
template<typename T> using giga_t = tags::product<giga, T>;

using tera = tags::dscalar<1e12>;
template <> inline const char *tag_symbol<tera> = "T";
template<typename T> using tera_t = tags::product<tera, T>;

using milli = tags::scalar<{1, 1000}>;
template <> inline const char *tag_symbol<milli> = "m";
template<typename T> using milli_t = tags::product<milli, T>;

using micro = tags::scalar<{1, 1000000}>;
template <> inline const char *tag_symbol<micro> = "µ";
template<typename T> using micro_t = tags::product<micro, T>;

using deci = tags::scalar<{1,10}>;
template <> inline const char *tag_symbol<deci> = "d";
template<typename T> using deci_t = tags::product<deci, T>;

using centi = tags::scalar<{1, 100}>;
template <> inline const char *tag_symbol<centi> = "c";
template<typename T> using centi_t = tags::product<centi, T>;

using nano = tags::dscalar<1e-9>;
template<> inline const char *tag_symbol<nano> = "n";
template<typename T> using nano_t = tags::product<nano, T>;

using pico = tags::dscalar<1e-12>;
template<> inline const char * tag_symbol<pico> = "p";
template<typename T> using pico_t = tags::product<pico, T>;

////////////////////////////////////////////////////////////////////////    
// SI units

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
using Ampere = SI<0, 0, 0, 1>;
using Kelvin = SI<0, 0, 0, 0, 1>;
using mole = SI<0, 0, 0, 0, 0, 1>;
using candela = SI<0, 0, 0, 0, 0, 0, 1>;

using Joule = SI<2, 1, -2>;
template <> inline const char *tag_symbol<Joule> = "J";
using Volt = SI<2, 1, -3, -1>;
template <> inline const char *tag_symbol<Volt> = "V";
using Newton = SI<1, 1, -2>;
template <> inline const char *tag_symbol<Newton> = "N";

using Ohm = SI<2, 1, -3, -2>;
template <> inline const char *tag_symbol<Ohm> = "Ω";

using speed = tags::divide<meter, second>;
using Watt = simplify_t<tags::divide<Joule, second>>;
template<> inline const char * tag_symbol<Watt> = "W";

// using Coloumb = ...
// using Farad = ...
// Induction???
// Magnetic flux?

// ?? Should we put everything into units 
namespace literals
{
    tagged<double, meter> operator""_meter(long double);
    tagged<double, kilogram> operator""_kilogram(long double);
    tagged<double, second> operator""_second(long double);
    tagged<double, Ampere> operator""_Ampere(long double);

    tagged<double, Kelvin> operator""_Kelvin(long double);
    tagged<double, mole> operator""_mole(long double);
    tagged<double, candela> operator""_candela(long double);
    tagged<double, Joule> operator""_Joule(long double);
    tagged<double, Volt> operator""_Volt(long double);
    tagged<double, Newton> operator""_Newton(long double);
    tagged<double, Ohm> operator""_Ohm(long double);
    tagged<double, Watt> operator""_Watt(long double);
}

////////////////////////////////////////////////////////////////////////    
/// Temperature

struct Celcius;
template <> inline const char *tag_symbol<Celcius> = "°C";
template <> struct tag_traits<Celcius> : default_tag_traits<Celcius>
{
    using common_type = Kelvin;
};

using Centigrade = Celcius;

struct Farenheit;
template <> inline const char *tag_symbol<Farenheit> = "°F";
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

namespace literals
{
    tagged<double, Celcius> operator""_Celcius(long double);
    tagged<double, Farenheit> operator""_Farenheit(long double);
}

////////////////////////////////////////////////////////////////////////
// Time

using millisecond = tags::product<milli, second>;

using minute = tags::product<second, tags::scalar<60>>;
template <> inline const char *tag_text<minute> = "minute";

using hour = tags::product<minute, tags::scalar<60>>;
template <> inline const char *tag_text<hour> = "hour";

using day = tags::product<hour, tags::scalar<24>>;

template <> inline const char *tag_text<day> = "day";

using week = tags::product<day, tags::scalar<7>>;
template <> inline const char *tag_text<week> = "week";

struct month;
template <> inline const char *tag_text<month> = "month";

using year = tags::product<month, tags::scalar<12>>;
template <> inline const char *tag_text<year> = "year";

using century = tags::product<year, tags::scalar<100>>;
template <> inline const char *tag_text<century> = "century";  // Plural is incorrect FIXME

using millenium = tags::product<year, tags::scalar<1000>>;
template <> inline const char *tag_text<millenium> = "millenium";  // Plural is incorrect FIXME

namespace literals
{
    tagged<double, millisecond> operator""_millisecond(long double);
    tagged<double, minute> operator""_minute(long double);
    tagged<double, hour> operator""_hour(long double);
    tagged<double, day> operator""_day(long double);
    tagged<double, week> operator""_week(long double);
    tagged<double, month> operator""_month(long double);
    tagged<double, year> operator""_year(long double);
    tagged<double, century> operator""_century(long double);
    tagged<double, millenium> operator""_millenium(long double);
}

///////////////////////////////////////////////////////////////////////////////
// Length

using yard = tags::product<meter, tags::scalar<{9144, 10000}>>;
template <> inline const char *tag_text<yard> = "yard";

using foot = tags::product<yard, tags::scalar<{1, 3}>>;
template <> inline const char *tag_symbol<foot> = "\'";

using inch = tags::product<foot, tags::scalar<{1, 12}>>;
template <> inline const char *tag_symbol<inch> = "\"";

using mile = tags::product<yard, tags::scalar<1760>>;
template <> inline const char *tag_text<mile> = "mile";

using centimeter = tags::product<centi, meter>;
using kilometer = tags::product<kilo, meter>;

using lightyear = tags::product<tags::dscalar<9460730472580800.0>, meter>;
template<> inline const char * tag_text<lightyear> = "light-year";

using parsec = tags::product<tags::dscalar<3.085677581491367e16>, meter>;
template<> inline const char * tag_symbol<parsec> = "pc";

///////////////////////////////////////////////////////////////////////////////
// Mass

using gram = tags::product<tags::scalar<{1, 1000}>, kilogram>;
template <> inline const char *tag_symbol<gram> = "g";

using ounce = tags::product<gram, tags::scalar<{283495, 10000}>>;
template <> inline const char *tag_symbol<ounce> = "oz";

using pound = tags::product<ounce, tags::scalar<16>>;
template <> inline const char *tag_symbol<pound> = "lb";
using stone = tags::product<pound, tags::scalar<14>>;
template <> inline const char *tag_symbol<stone> = "st";

////////////////////////////////////////////////////////////////////////
// Volume

using decimeter = deci_t<meter>;
using liter = tags::power<decimeter, 3>;
using milliliter = milli_t<liter>;

// !! TODO
// US pint = 473.176473ml
// UK pint = 568.261m = 20 fluid ounces


// uk fluid ounce = 28.4130625 
// pint
// quart
// gallon

////////////////////////////////////////////////////////////////////////
// Information

struct bit;
template <> inline const char *tag_symbol<bit> = "b";

using byte = tags::product<bit, tags::scalar<8>>;
template <> inline const char *tag_symbol<byte> = "B";

using kilo_base2 = tags::scalar<1024>;
template<> inline const char * tag_symbol<kilo_base2> = "k";

using mega_base2 = tags::scalar<1024*1024>;
template<> inline const char * tag_symbol<mega_base2> = "M";

using giga_base2 = tags::scalar<(1<<30)>;
template<> inline const char * tag_symbol<giga_base2> = "G";

using kilobyte = tags::product<kilo_base2, byte>;
using megabyte = tags::product<mega_base2, byte>;
using gigabyte = tags::product<giga_base2, byte>;

using nat = tags::product<bit, tags::dscalar<0.69314718>>;
template<> inline const char * tag_text<nat> = "nat";

namespace literals
{
    tagged<double, bit> operator""_bit(long double d);
    tagged<double, byte> operator""_byte(long double d);
    tagged<double, kilobyte> operator""_kilobyte(long double d);
    tagged<double, megabyte> operator""_megabyte(long double d);
    tagged<double, gigabyte> operator""_gigabyte(long double d);
    tagged<double, nat> operator""_nat(long double d);
}

} // namespace cutty