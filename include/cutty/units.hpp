#pragma once

#include "tags.hpp"

#include <iostream>
#include <numbers>

#define CY_UNIT(X)                                                                                                     \
    namespace literals                                                                                                 \
    {                                                                                                                  \
    inline tagged<long double, X> operator"" _##X(long double d)                                                       \
    {                                                                                                                  \
        return tag<X>(d);                                                                                              \
    };                                                                                                                 \
    inline tagged<unsigned long long, X> operator"" _##X(unsigned long long i)                                         \
    {                                                                                                                  \
        return tag<X>(i);                                                                                              \
    };                                                                                                                 \
    }

#define CY_UNIT_TEXT(Name, Text)                                                                                       \
    template <> inline const char *tag_text<Name> = Text;                                                              \
    CY_UNIT(Name);

#define CY_UNIT_SYMBOL(Name, Text)                                                                                     \
    template <> inline const char *tag_symbol<Name> = Text;                                                            \
    CY_UNIT(Name);

namespace cutty
{
////////////////////////////////////////////////////////////////////////
// Angles

struct radian;

template <> struct tag_traits<radian> : default_tag_traits<radian>
{
    // Angles are pure ratios so can be converted via unit
    using common_type = tags::unit;
};

using degree = tags::product<tags::dscalar<std::numbers::pi / 180>, radian>;

template <> struct tag_traits<degree> : default_tag_traits<degree>
{
    using common_type = tags::unit;
};

using gradian = tags::product<tags::scalar<{360, 400}>, degree>;

template <> struct tag_traits<gradian> : default_tag_traits<gradian>
{
    using common_type = tags::unit;
};

using rotation = tags::product<tags::dscalar<2 * std::numbers::pi>, radian>;

template <> struct tag_traits<rotation> : default_tag_traits<rotation>
{
    using common_type = tags::unit;
};

CY_UNIT_TEXT(radian, "radian");
CY_UNIT_TEXT(degree, "degree");
CY_UNIT_TEXT(gradian, "gradian")
CY_UNIT_TEXT(rotation, "rotation");

////////////////////////////////////////////////////////////////////////
// Scalars

using kilo = tags::scalar<1000>;
template <> inline const char *tag_symbol<kilo> = "k";
template <typename T> using kilo_t = tags::product<kilo, T>;

using mega = tags::scalar<1000000>;
template <> inline const char *tag_symbol<mega> = "M";
template <typename T> using mega_t = tags::product<mega, T>;

using giga = tags::dscalar<1e9>;
template <> inline const char *tag_symbol<giga> = "G";
template <typename T> using giga_t = tags::product<giga, T>;

using tera = tags::dscalar<1e12>;
template <> inline const char *tag_symbol<tera> = "T";
template <typename T> using tera_t = tags::product<tera, T>;

using milli = tags::scalar<{1, 1000}>;
template <> inline const char *tag_symbol<milli> = "m";
template <typename T> using milli_t = tags::product<milli, T>;

using micro = tags::scalar<{1, 1000000}>;
template <> inline const char *tag_symbol<micro> = "µ";
template <typename T> using micro_t = tags::product<micro, T>;

using deci = tags::scalar<{1, 10}>;
template <> inline const char *tag_symbol<deci> = "d";
template <typename T> using deci_t = tags::product<deci, T>;

using centi = tags::scalar<{1, 100}>;
template <> inline const char *tag_symbol<centi> = "c";
template <typename T> using centi_t = tags::product<centi, T>;

using nano = tags::dscalar<1e-9>;
template <> inline const char *tag_symbol<nano> = "n";
template <typename T> using nano_t = tags::product<nano, T>;

using pico = tags::dscalar<1e-12>;
template <> inline const char *tag_symbol<pico> = "p";
template <typename T> using pico_t = tags::product<pico, T>;

////////////////////////////////////////////////////////////////////////
// SI units

template <fraction Meter = 0, fraction Kilogram = 0, fraction Seconds = 0, fraction Ampere = 0, fraction Kelvin = 0,
          fraction Mole = 0, fraction Candela = 0>
struct SI;

template <fraction Meter, fraction Kilogram, fraction Second, fraction Ampere, fraction Kelvin, fraction Mole,
          fraction Candela>
struct tag_traits<SI<Meter, Kilogram, Second, Ampere, Kelvin, Mole, Candela>>
    : default_tag_traits<SI<Meter, Kilogram, Second, Ampere, Kelvin, Mole, Candela>>
{
    using tag_type = SI<Meter, Kilogram, Second, Ampere, Kelvin, Mole, Candela>;

    static void write(std::ostream &os, Plural plural, PadWithSpace pad)
    {
        if (!detail::write_tag<tag_type>(os, plural, pad))
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

template <> struct simplify<SI<>> : simplify<tags::unit>
{
};

template <fraction Meter, fraction Kilogram, fraction Second, fraction Ampere, fraction Kelvin, fraction Mole,
          fraction Candela, fraction P>
struct simplify<tags::power<SI<Meter, Kilogram, Second, Ampere, Kelvin, Mole, Candela>, P>>
    : simplify<SI<Meter * P, Kilogram * P, Second * P, Ampere * P, Kelvin * P, Mole * P, Candela * P>>
{
};

template <fraction Meter1, fraction Kilogram1, fraction Second1, fraction Ampere1, fraction Kelvin1, fraction Mole1,
          fraction Candela1, fraction Meter2, fraction Kilogram2, fraction Second2, fraction Ampere2, fraction Kelvin2,
          fraction Mole2, fraction Candela2, typename... Tags>
struct simplify<tags::product<SI<Meter1, Kilogram1, Second1, Ampere1, Kelvin1, Mole1, Candela1>,
                              SI<Meter2, Kilogram2, Second2, Ampere2, Kelvin2, Mole2, Candela2>, Tags...>>
    : simplify<tags::product<SI<Meter1 + Meter2, Kilogram1 + Kilogram2, Second1 + Second2, Ampere1 + Ampere2,
                                Kelvin1 + Kelvin2, Mole1 + Mole2, Candela1 + Candela2>,
                             Tags...>>
{
};

using meter = SI<1>;
using kilogram = SI<0, 1>;
using second = SI<0, 0, 1>;
using ampere = SI<0, 0, 0, 1>;
using kelvin = SI<0, 0, 0, 0, 1>;
using mole = SI<0, 0, 0, 0, 0, 1>;
using candela = SI<0, 0, 0, 0, 0, 0, 1>;

CY_UNIT(meter);
CY_UNIT(kilogram);
CY_UNIT(second);
CY_UNIT(ampere);
CY_UNIT(kelvin);
CY_UNIT(mole);
CY_UNIT(candela);

////////////////////////////////////////////////////////////////////////
// Derived SI units

using speed = tags::divide<meter, second>;

using joule = SI<2, 1, -2>;
using newton = SI<1, 1, -2>;
using hertz = SI<0, 0, -1>;

CY_UNIT_SYMBOL(joule, "J");
CY_UNIT_SYMBOL(newton, "N");
CY_UNIT_SYMBOL(hertz, "Hz")

////////////////////////////////////////////////////////////////////////
// Electrical

using volt = SI<2, 1, -3, -1>;
using ohm = SI<2, 1, -3, -2>;
using watt = simplify_t<tags::divide<joule, second>>;
using farad = SI<-2, -1, 4, 2>;
using coulomb = SI<0, 0, 1, 1>;
using henry = SI<2, 1, -2, -2>;
using tesla = SI<0, 1, -2, -1>;
using weber = SI<2, 1, -2, -1>;

CY_UNIT_SYMBOL(volt, "V");
CY_UNIT_SYMBOL(ohm, "Ω");
CY_UNIT_SYMBOL(watt, "W");
CY_UNIT_SYMBOL(farad, "F");
CY_UNIT_SYMBOL(coulomb, "C");
CY_UNIT_SYMBOL(henry, "L");
CY_UNIT_TEXT(tesla, "swasticar");
CY_UNIT_SYMBOL(weber, "Wb");

////////////////////////////////////////////////////////////////////////
/// Temperature

struct celsius;
template <> struct tag_traits<celsius> : default_tag_traits<celsius>
{
    using common_type = kelvin;
};

struct farenheit;
template <> struct tag_traits<farenheit> : default_tag_traits<farenheit>
{
    using common_type = kelvin;
};

template <typename T> void convert(const tagged<T, celsius> &c, tagged<T, kelvin> &k)
{
    *k = *c + 273.15;
}

template <typename T> void convert(const tagged<T, kelvin> &k, tagged<T, celsius> &c)
{
    *c = *k - 273.15;
}

template <typename T> void convert(const tagged<T, kelvin> &k, tagged<T, farenheit> &f)
{
    *f = 9 * (*k - 273.15) / 5 + 32;
}

template <typename T> void convert(const tagged<T, farenheit> &f, tagged<T, kelvin> &k)
{
    *k = 273.15 + 5 * (*f - 32) / 9;
}

CY_UNIT_SYMBOL(celsius, "°C");
CY_UNIT_SYMBOL(farenheit, "°F");

////////////////////////////////////////////////////////////////////////
// Time

using millisecond = tags::product<milli, second>;
using minute = tags::product<second, tags::scalar<60>>;
using hour = tags::product<minute, tags::scalar<60>>;
using day = tags::product<hour, tags::scalar<24>>;
using week = tags::product<day, tags::scalar<7>>;
struct month;
using year = tags::product<month, tags::scalar<12>>;
using century = tags::product<year, tags::scalar<100>>;
using millennium = tags::product<year, tags::scalar<1000>>;

CY_UNIT(millisecond);
CY_UNIT_TEXT(minute, "minute");
CY_UNIT_TEXT(hour, "hour");
CY_UNIT_TEXT(day, "day");
CY_UNIT_TEXT(week, "week");
CY_UNIT_TEXT(month, "month");
CY_UNIT_TEXT(year, "year");
CY_UNIT_TEXT(century, "century");      // Plural incorrect FIXME
CY_UNIT_TEXT(millennium, "millenium"); // Plural incorrect FIXME

///////////////////////////////////////////////////////////////////////////////
// Length

using yard = tags::product<meter, tags::scalar<{9144, 10000}>>;
using foot = tags::product<yard, tags::scalar<{1, 3}>>;
using inch = tags::product<foot, tags::scalar<{1, 12}>>;
using mile = tags::product<yard, tags::scalar<1760>>;
using millimeter = milli_t<meter>;
using centimeter = centi_t<meter>;
using kilometer = kilo_t<meter>;
using lightyear = tags::product<tags::dscalar<9460730472580800.0>, meter>;
using parsec = tags::product<tags::dscalar<3.085677581491367e16>, meter>;

CY_UNIT_TEXT(yard, "yard");
CY_UNIT_SYMBOL(foot, "'");
CY_UNIT_SYMBOL(inch, "\"");
CY_UNIT_TEXT(mile, "mile");
CY_UNIT(centimeter);
CY_UNIT(kilometer)
CY_UNIT_TEXT(lightyear, "light-year");
CY_UNIT_SYMBOL(parsec, "pc");

///////////////////////////////////////////////////////////////////////////////
// Mass

using gram = tags::product<tags::scalar<{1, 1000}>, kilogram>;
using ounce = tags::product<gram, tags::dscalar<28.349523125>>;
using pound = tags::product<ounce, tags::scalar<16>>;
using stone = tags::product<pound, tags::scalar<14>>;
using tonne = kilo_t<kilogram>;
using long_ton = tags::product<pound, tags::scalar<2240>>;
using short_ton = tags::product<pound, tags::scalar<2000>>;

CY_UNIT_SYMBOL(gram, "g");
CY_UNIT_SYMBOL(ounce, "oz");
CY_UNIT_SYMBOL(pound, "lb");
CY_UNIT_SYMBOL(stone, "st");
CY_UNIT_TEXT(short_ton, "imperial short tonne");
CY_UNIT_TEXT(long_ton, "imperial long tonne");
CY_UNIT_TEXT(tonne, "metric tonne");

////////////////////////////////////////////////////////////////////////
// Energy

using kilojoule = kilo_t<joule>;
using calorie = tags::product<tags::scalar<{4184, 1000}>, joule>;
using kcal = kilo_t<calorie>;
using erg = tags::product<tags::scalar<{1, 10000000}>, joule>;
using electronvolt = tags::product<tags::dscalar<1.602176634e-19>, joule>;

CY_UNIT(kilojoule);
CY_UNIT_SYMBOL(calorie, "cal");
CY_UNIT(kcal);
CY_UNIT_TEXT(erg, "erg");
CY_UNIT_SYMBOL(electronvolt, "eV");

////////////////////////////////////////////////////////////////////////
// Volume

using decimeter = deci_t<meter>;
using liter = tags::power<decimeter, 3>;
using milliliter = milli_t<liter>;
using centiliter = centi_t<liter>;
using deciliter = deci_t<liter>;
using us_teaspoon = tags::product<tags::dscalar<4.92892>, milliliter>;
using us_tablespoon = tags::product<tags::scalar<3>, us_teaspoon>;
using us_fluid_ounce = tags::product<tags::scalar<6>, us_teaspoon>;
using us_cup = tags::product<tags::scalar<240>, milliliter>;
using us_pint = tags::product<tags::scalar<32>, us_tablespoon>;
using us_quart = tags::product<tags::scalar<2>, us_pint>;
using us_gallon = tags::product<tags::scalar<8>, us_pint>;
using imperial_teaspoon = tags::product<tags::dscalar<5.91939>, milliliter>;
using imperial_tablespoon = tags::product<tags::scalar<3>, imperial_teaspoon>;
using imperial_fluid_ounce = tags::product<tags::scalar<{48, 10}>, imperial_teaspoon>;
using imperial_cup = tags::product<tags::scalar<38>, imperial_teaspoon>;
using imperial_pint = tags::product<tags::scalar<96>, imperial_teaspoon>;
using imperial_quart = tags::product<tags::scalar<2>, imperial_pint>;
using imperial_gallon = tags::product<tags::scalar<8>, imperial_pint>;

CY_UNIT_SYMBOL(liter, "l");
CY_UNIT(milliliter);
CY_UNIT_TEXT(us_teaspoon, "US teaspoon");
CY_UNIT_TEXT(us_tablespoon, "US tablespoon");
CY_UNIT_TEXT(us_fluid_ounce, "US fluid ounce");
CY_UNIT_TEXT(us_cup, "US cup");
CY_UNIT_TEXT(us_pint, "US pint");
CY_UNIT_TEXT(us_quart, "US quart");
CY_UNIT_TEXT(us_gallon, "US gallon");
CY_UNIT_TEXT(imperial_teaspoon, "imperial teaspoon");
CY_UNIT_TEXT(imperial_tablespoon, "imperial tablespoon");
CY_UNIT_TEXT(imperial_fluid_ounce, "imperial fluid ounce");
CY_UNIT_TEXT(imperial_cup, "imperial cup");
CY_UNIT_TEXT(imperial_pint, "imperial pint");
CY_UNIT_TEXT(imperial_quart, "imperial quart");
CY_UNIT_TEXT(imperial_gallon, "imperial gallon");

////////////////////////////////////////////////////////////////////////
// Information

struct bit;
using byte = tags::product<bit, tags::scalar<8>>;
using kilo_base2 = tags::scalar<1024>;
using mega_base2 = tags::scalar<1024 * 1024>;
using giga_base2 = tags::scalar<(1 << 30)>;
using tera_base2 = tags::scalar<(1ll << 40)>;
using kilobyte = tags::product<kilo_base2, byte>;
using megabyte = tags::product<mega_base2, byte>;
using gigabyte = tags::product<giga_base2, byte>;
using terabyte = tags::product<tera_base2, byte>;
using nat = tags::product<bit, tags::dscalar<1.0 / std::numbers::ln2>>;

template <> inline const char *tag_symbol<kilo_base2> = "k";
template <> inline const char *tag_symbol<mega_base2> = "M";
template <> inline const char *tag_symbol<giga_base2> = "G";
template <> inline const char *tag_symbol<tera_base2> = "T";

CY_UNIT_SYMBOL(bit, "b");
CY_UNIT_SYMBOL(byte, "B");
CY_UNIT_TEXT(nat, "nat");
CY_UNIT(kilobyte);
CY_UNIT(megabyte);
CY_UNIT(gigabyte);
CY_UNIT(terabyte);

#undef CY_UNIT
#undef CY_UNIT_TEXT
#undef CY_UNIT_SYMBOL

} // namespace cutty
