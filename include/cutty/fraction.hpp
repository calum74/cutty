#pragma once

namespace cutty
{
struct fraction
{
    int numerator;
    int denominator;

    constexpr int hcf(int a, int b)
    {
        while (b)
        {
            int r = a % b;
            a = b;
            b = r;
        }
        return a < 0 ? -a : a;
    }

    constexpr fraction(int n, int d = 1)
    {
        auto h = hcf(n, d);
        if (d < 0)
            h = -h;
        numerator = n / h;
        denominator = d / h;
    }

    constexpr bool operator==(const fraction &) const = default;
};
} // namespace cutty