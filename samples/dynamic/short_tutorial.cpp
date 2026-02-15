#include <cutty/dynamic.hpp>
#include <cutty/dynamic/function.hpp>
#include <cutty/print.hpp>

namespace cy = cutty;

int main()
{
    // Creation
    cy::dynamic a;         // An empty dynamic
    cy::dynamic b = 12;    // An integer dynamic
    cy::dynamic c = "abc"; // A string dynamic

    // Output
    cy::print(a);
    std::cout << b << std::endl;
    std::cout << c.str() << std::endl;

    // Operations
    cy::dynamic(1) + cy::dynamic(2); // 3
    cy::dynamic(1) + 2;              // 3

    {
        cy::dynamic x = 12;
        int &i = x.as<int>();
        // double &d = x.as<double>();  // throws dynamic::incompatible
        x.type_info();

        if (int *p = x.try_get<int>())
        {
            cy::print("try_get =", *p);
        }
    }

    // Lists
    {
        cy::dynamic list = {1, 2.1, "joe"};
        auto map = cy::dynamic::dict({{"joe", 12}, {"sam", {1, 2, 3}}});
        for (auto [k, v] : map)
        {
        }
    }

    // References
    {
        int value = 42;
        auto r = cy::dynamic::reference(value);
        value = 67;
        cy::print(r); // 67

        cy::dynamic i = 42;
        cy::dynamic j = i.ref();
        j = 41;
        cy::print(i); // 41
    }

    // Shared
    {
        cy::dynamic i = cy::dynamic::shared(42);
        cy::dynamic j = i;
        ++i;
        cy::print(j); // 43;
    }

    // Objects
    {
        auto o = cy::dynamic::object();
        o["joe"] = 12;
        auto o2 = o.const_ref();

        cy::print(o2["joe"]); // Ok
        // cy::print(o2["fred"]);  // throws std::out_of_range
    }

    // Functions
    {
        auto fn = cy::dynamic::function([] { return cy::dynamic(42); });
        cy::print(fn());

        fn = cy::dynamic::function([&](int x) { return x + 1; });
        cy::print(fn(10));

        // fn(); // throws unsupported
    }
}
