#include <cutty/dynamic/enable.hpp>

#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>

namespace cy=cutty;

using list_type = std::vector<cy::dynamic>;
using map_type = std::map<cy::dynamic, cy::dynamic>;
using set_type = std::set<cy::dynamic>;
using unordered_map_type = std::unordered_map<cy::dynamic, cy::dynamic>;
using unordered_set_type = std::unordered_set<cy::dynamic>;
using queue_type = std::deque<cy::dynamic>;

std::ostream &operator<<(std::ostream &os, const list_type &);
std::ostream &operator<<(std::ostream &os, const map_type &);
std::ostream &operator<<(std::ostream &os, const unordered_map_type &);
std::ostream &operator<<(std::ostream &os, const set_type &);
std::ostream &operator<<(std::ostream &os, const unordered_set_type &);
std::ostream &operator<<(std::ostream &os, const queue_type &);

#define BYVALUE_TYPE(T) template void cy::dynamic::enable<T>();

bool operator==(cy::dynamic::empty, cy::dynamic::empty)
{
    return true;
}

bool operator<(cy::dynamic::empty, cy::dynamic::empty)
{
    return false;
}

std::ostream &operator<<(std::ostream &os, cy::dynamic::empty)
{
    return os << "(empty)";
}

BYVALUE_TYPE(cy::dynamic::empty)
BYVALUE_TYPE(bool)
BYVALUE_TYPE(signed char)
BYVALUE_TYPE(unsigned char)
BYVALUE_TYPE(char)

BYVALUE_TYPE(short)
BYVALUE_TYPE(unsigned short)
BYVALUE_TYPE(int)
BYVALUE_TYPE(unsigned int)
BYVALUE_TYPE(long)
BYVALUE_TYPE(unsigned long)
BYVALUE_TYPE(long long)
BYVALUE_TYPE(unsigned long long)

BYVALUE_TYPE(float)
BYVALUE_TYPE(double)

BYVALUE_TYPE(std::string)
BYVALUE_TYPE(char *)
BYVALUE_TYPE(std::string_view)

namespace
{
void output_list(std::ostream &os, const auto &list)
{
    os << "[";
    bool first = true;
    for (auto &item : list)
    {
        if (first)
            first = false;
        else
            os << ",";
        os << item;
    }
    os << "]";
}

void output_pairs(std::ostream &os, const auto &list)
{
    os << "[";
    bool first = true;
    for (auto &item : list)
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

std::ostream &operator<<(std::ostream &os, const std::vector<cy::dynamic> &list)
{
    output_list(os, list);
    return os;
}

std::ostream &operator<<(std::ostream &os, const set_type &list)
{
    output_list(os, list);
    return os;
}

std::ostream &operator<<(std::ostream &os, const queue_type &queue)
{
    output_list(os, queue);
    return os;
}


std::ostream &operator<<(std::ostream &os, const unordered_map_type &list)
{
    output_pairs(os, list);
    return os;
}

std::ostream &operator<<(std::ostream &os, const map_type &list)
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
    for (auto &item : items)
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

BYVALUE_TYPE(std::function<cy::dynamic()>);
BYVALUE_TYPE(std::function<cy::dynamic(const cy::dynamic &)>);
BYVALUE_TYPE(std::function<cy::dynamic(const cy::dynamic &, const cy::dynamic &)>);
BYVALUE_TYPE(std::function<cy::dynamic(const cy::dynamic &, const cy::dynamic &, const cy::dynamic &)>);
BYVALUE_TYPE(std::function<cy::dynamic(const cy::dynamic &, const cy::dynamic &, const cy::dynamic &, const cy::dynamic &)>);
// Ok to add more here
