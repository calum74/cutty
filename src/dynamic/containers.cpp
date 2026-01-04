#include <cutty/dynamic/instantiate.hpp>

#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cy = cutty;

using list_type = std::vector<cy::dynamic>;
using map_type = std::map<cy::dynamic, cy::dynamic>;
using set_type = std::set<cy::dynamic>;
using unordered_map_type = std::unordered_map<cy::dynamic, cy::dynamic>;
using unordered_set_type = std::unordered_set<cy::dynamic>;
using queue_type = std::deque<cy::dynamic>;
using object_type = std::unordered_map<std::string, cy::dynamic>;

std::ostream &operator<<(std::ostream &os, const list_type &);
std::ostream &operator<<(std::ostream &os, const map_type &);
std::ostream &operator<<(std::ostream &os, const unordered_map_type &);
std::ostream &operator<<(std::ostream &os, const set_type &);
std::ostream &operator<<(std::ostream &os, const unordered_set_type &);
std::ostream &operator<<(std::ostream &os, const queue_type &);

template<typename T> requires (
    std::same_as<std::map<cy::dynamic, cy::dynamic>, std::remove_cvref_t<T>>
    ||     std::same_as<std::set<cy::dynamic>, std::remove_cvref_t<T>>
    )
struct cy::dynamic::traits<T> : public default_traits<T>
{
   // rbegin and rend are broken 
   // because rbegin(c) + n does not compile.
   static dynamic rbegin(auto&&)
   {
        throw unsupported("rbegin()");
   }  

   static dynamic rend(auto&&)
   {
        throw unsupported("rend()");
   }  
};


CY_INSTANTIATE(std::vector<cy::dynamic>)

cy::dynamic cy::dynamic::list()
{
    return dynamic(std::vector<dynamic>{});
}

CY_INSTANTIATE(object_type);

cy::dynamic cy::dynamic::object()
{
    return dynamic(object_type{});
}

cy::dynamic cy::dynamic::list(std::initializer_list<dynamic> items)
{
    auto l = dynamic(std::vector<dynamic>{});
    for (auto &item : items)
        l.push_back(item);
    return l;
}

CY_INSTANTIATE(map_type)

cy::dynamic cy::dynamic::map()
{
    return dynamic(map_type{});
}

cy::dynamic cy::dynamic::dict()
{
    return dynamic(unordered_map_type{});
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

cy::dynamic cy::dynamic::queue()
{
    return dynamic(queue_type{});
}

cy::dynamic cy::dynamic::queue(std::initializer_list<dynamic> items)
{
    auto s = dynamic::queue();
    for (auto &item : items)
        s.push_back(item);
    return s;
}

cy::dynamic cy::dynamic::dict(std::initializer_list<std::pair<dynamic, dynamic>> items)
{
    auto d = dynamic::dict();
    for (auto &&[x, y] : items)
        d.insert(dynamic(std::move(x)), dynamic(std::move(y)));
    return d;
}

cy::dynamic cy::dynamic::map(std::initializer_list<std::pair<dynamic, dynamic>> items)
{
    auto m = dynamic::map();
    for (auto &&[x, y] : items)
        m.insert(dynamic(std::move(x)), dynamic(std::move(y)));
    return m;
}
