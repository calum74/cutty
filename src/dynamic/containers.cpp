#include <cutty/dynamic/instantiate.hpp>

#include <unordered_map>
#include <unordered_set>

namespace cy = cutty;

using unordered_map_type = std::unordered_map<cy::dynamic, cy::dynamic>;
using unordered_set_type = std::unordered_set<cy::dynamic>;
using object_type = std::unordered_map<std::string, cy::dynamic>;

cy::dynamic cy::dynamic::object()
{
    return dynamic(object_type{});
}

cy::dynamic cy::dynamic::dict()
{
    return dynamic(unordered_map_type{});
}

cy::dynamic cy::dynamic::dict(std::initializer_list<std::pair<dynamic, dynamic>> items)
{
    auto d = dynamic::dict();
    for (auto &&[x, y] : items)
        d.insert(dynamic(std::move(x)), dynamic(std::move(y)));
    return d;
}
