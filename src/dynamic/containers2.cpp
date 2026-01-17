#include <cutty/dynamic/instantiate.hpp>

#include <map>
#include <set>

namespace cy = cutty;

using map_type = std::map<cy::dynamic, cy::dynamic>;
using set_type = std::set<cy::dynamic>;

template <typename T>
    requires(std::same_as<std::map<cy::dynamic, cy::dynamic>, std::remove_cvref_t<T>> ||
             std::same_as<std::set<cy::dynamic>, std::remove_cvref_t<T>>)
struct cy::dynamic::traits<T> : public default_traits<T>
{
    // rbegin and rend are broken
    // because rbegin(c) + n does not compile.
    static dynamic rbegin(auto &&)
    {
        throw unsupported("rbegin()");
    }

    static dynamic rend(auto &&)
    {
        throw unsupported("rend()");
    }
};

CY_INSTANTIATE(map_type)

cy::dynamic cy::dynamic::map()
{
    return dynamic(map_type{});
}

cy::dynamic cy::dynamic::set()
{
    return dynamic(set_type{});
}

cy::dynamic cy::dynamic::set(std::initializer_list<dynamic> items)
{
    auto s = dynamic::set();
    for (auto &item : items)
        s.insert(item);
    return s;
}

cy::dynamic cy::dynamic::map(std::initializer_list<std::pair<dynamic, dynamic>> items)
{
    auto m = dynamic::map();
    for (auto &&[x, y] : items)
        m.insert(dynamic(std::move(x)), dynamic(std::move(y)));
    return m;
}
