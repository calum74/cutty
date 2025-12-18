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
    #define CY_UNIT(X) \
        inline tagged<long double, X> operator"" _##X(long double d) { return tag<X>(d); };\
        inline tagged<unsigned long long, X> operator"" _##X(unsigned long long i) { return tag<X>(i); };

    CY_UNIT(radian);
    CY_UNIT(degree);
    CY_UNIT(gradian);
    CY_UNIT(rotation);
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
using ampere = SI<0, 0, 0, 1>;
using kelvin = SI<0, 0, 0, 0, 1>;
using mole = SI<0, 0, 0, 0, 0, 1>;
using candela = SI<0, 0, 0, 0, 0, 0, 1>;

using joule = SI<2, 1, -2>;
template <> inline const char *tag_symbol<joule> = "J";
using volt = SI<2, 1, -3, -1>;
template <> inline const char *tag_symbol<volt> = "V";
using newton = SI<1, 1, -2>;
template <> inline const char *tag_symbol<newton> = "N";

using ohm = SI<2, 1, -3, -2>;
template <> inline const char *tag_symbol<ohm> = "Ω";

using speed = tags::divide<meter, second>;
using watt = simplify_t<tags::divide<joule, second>>;
template<> inline const char * tag_symbol<watt> = "W";

// using Coloumb = ...
// using Farad = ...
// henry, tesla
// Magnetic flux?

namespace literals
{
    CY_UNIT(meter);
    CY_UNIT(kilogram);
    CY_UNIT(second);
    CY_UNIT(ampere);
    CY_UNIT(kelvin);
    CY_UNIT(mole);
    CY_UNIT(candela);
    CY_UNIT(joule);
    CY_UNIT(volt);
    CY_UNIT(newton);
    CY_UNIT(ohm);
    CY_UNIT(watt);
}

////////////////////////////////////////////////////////////////////////    
/// Temperature

struct celsius;
template <> inline const char *tag_symbol<celsius> = "°C";
template <> struct tag_traits<celsius> : default_tag_traits<celsius>
{
    using common_type = kelvin;
};

struct farenheit;
template <> inline const char *tag_symbol<farenheit> = "°F";
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

namespace literals
{
    CY_UNIT(celsius);
    CY_UNIT(farenheit);
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
    CY_UNIT(millisecond);
    CY_UNIT(minute);
    CY_UNIT(hour);
    CY_UNIT(day);
    CY_UNIT(week);
    CY_UNIT(month);
    CY_UNIT(year);
    CY_UNIT(century);
    CY_UNIT(millenium);
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

namespace literals
{
    CY_UNIT(yard);
    CY_UNIT(foot);
    CY_UNIT(inch);
    CY_UNIT(mile);
    CY_UNIT(centimeter);
    CY_UNIT(kilometer)
    CY_UNIT(lightyear);
    CY_UNIT(parsec);
}

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

// !! ton, tonne

namespace literals
{
    CY_UNIT(gram);
    CY_UNIT(ounce);
    CY_UNIT(pound);
    CY_UNIT(stone);
    // !! ton, tonne
}

////////////////////////////////////////////////////////////////////////
// Energy

// !! erg, calorie

////////////////////////////////////////////////////////////////////////
// Volume

using decimeter = deci_t<meter>;
using liter = tags::power<decimeter, 3>;
using milliliter = milli_t<liter>;

// !! TODO
// US pint = 473.176473ml
// UK pint = 568.261m = 20 fluid ounces

// teaspoon
// cup

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

// FIXME: terabyte currently overflows the ratios. Unless I use long long ratios?

using nat = tags::product<bit, tags::dscalar<0.69314718>>;
template<> inline const char * tag_text<nat> = "nat";

namespace literals
{
    CY_UNIT(bit);
    CY_UNIT(byte);
    CY_UNIT(kilobyte);
    CY_UNIT(megabyte);
    CY_UNIT(gigabyte);
    CY_UNIT(nat);
}

#undef CY_UNIT

} // namespace cutty
