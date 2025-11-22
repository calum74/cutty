#include <cutty/dynamic/instantiate.hpp>

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>

namespace cy = cutty;

#define BYVALUE_TYPE(T) template const cy::dynamic::types& cy::dynamic::instantiate<T>();

using list_type = std::vector<cy::dynamic>;
using map_type = std::map<cy::dynamic, cy::dynamic>;
using set_type = std::set<cy::dynamic>;
using unordered_map_type = std::unordered_map<cy::dynamic, cy::dynamic>;
using unordered_set_type = std::unordered_set<cy::dynamic>;
using queue_type = std::deque<cy::dynamic>;

std::ostream& operator<<(std::ostream& os, const list_type&);
std::ostream& operator<<(std::ostream& os, const map_type&);
std::ostream& operator<<(std::ostream& os, const unordered_map_type&);
std::ostream& operator<<(std::ostream& os, const set_type&);
std::ostream& operator<<(std::ostream& os, const unordered_set_type&);
std::ostream& operator<<(std::ostream& os, const queue_type&);

namespace
{
    void output_list(std::ostream& os, const auto& list)
    {
        os << "[";
        bool first = true;
        for (auto& item : list)
        {
            if (first)
                first = false;
            else
                os << ",";
            os << item;
        }
        os << "]";
    }

    void output_pairs(std::ostream& os, const auto& list)
    {
        os << "[";
        bool first = true;
        for (auto& item : list)
        {
            if (first)
                first = false;
            else
                os << ",";
            os << item.first << ":" << item.second;
        }
        os << "]";
    }
} // namespace

std::ostream& operator<<(std::ostream& os, const std::vector<cy::dynamic>& list)
{
    output_list(os, list);
    return os;
}

std::ostream& operator<<(std::ostream& os, const set_type& list)
{
    output_list(os, list);
    return os;
}

std::ostream& operator<<(std::ostream& os, const queue_type& queue)
{
    output_list(os, queue);
    return os;
}


std::ostream& operator<<(std::ostream& os, const unordered_map_type& list)
{
    output_pairs(os, list);
    return os;
}

std::ostream& operator<<(std::ostream& os, const map_type& list)
{
    output_pairs(os, list);
    return os;
}

BYVALUE_TYPE(std::vector<cy::dynamic>)

cy::dynamic cy::dynamic::list()
{
    return dynamic(std::vector<dynamic>{});
}

cy::dynamic cy::dynamic::list(std::initializer_list<dynamic> items)
{
    auto l = dynamic(std::vector<dynamic>{});
    for (auto& item : items)
        l.push_back(item);
    return l;
}

BYVALUE_TYPE(map_type)

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
    for (auto& item : items)
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
    for (auto& item : items)
        s.push_back(item);
    return s;
}

cy::dynamic cy::dynamic::dict(std::initializer_list<std::pair<dynamic, dynamic>> items)
{
    auto d = dynamic::dict();
    for (auto&& [x, y] : items)
        d.insert(dynamic(std::move(x)), dynamic(std::move(y)));
    return d;
}

cy::dynamic cy::dynamic::map(std::initializer_list<std::pair<dynamic, dynamic>> items)
{
    auto m = dynamic::map();
    for (auto&& [x, y] : items)
        m.insert(dynamic(std::move(x)), dynamic(std::move(y)));
    return m;
}
