#include <cutty/dynamic/enable.hpp>

#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>

using list_type = std::vector<dynamic>;
using map_type = std::map<dynamic, dynamic>;
using set_type = std::set<dynamic>;
using unordered_map_type = std::unordered_map<dynamic, dynamic>;
using unordered_set_type = std::unordered_set<dynamic>;
using queue_type = std::deque<dynamic>;

std::ostream &operator<<(std::ostream &os, const list_type &);
std::ostream &operator<<(std::ostream &os, const map_type &);
std::ostream &operator<<(std::ostream &os, const unordered_map_type &);
std::ostream &operator<<(std::ostream &os, const set_type &);
std::ostream &operator<<(std::ostream &os, const unordered_set_type &);
std::ostream &operator<<(std::ostream &os, const queue_type &);

#define BYVALUE_TYPE(T) template void dynamic::enable<T>();

bool operator==(dynamic::empty, dynamic::empty)
{
    return true;
}

bool operator<(dynamic::empty, dynamic::empty)
{
    return false;
}

std::ostream &operator<<(std::ostream &os, dynamic::empty)
{
    return os << "(empty)";
}

BYVALUE_TYPE(dynamic::empty)
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

std::ostream &operator<<(std::ostream &os, const std::vector<dynamic> &list)
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

BYVALUE_TYPE(std::vector<dynamic>)

dynamic dynamic::list()
{
    return dynamic(std::vector<dynamic>{});
}

dynamic dynamic::list(std::initializer_list<dynamic> items)
{
    auto l = dynamic(std::vector<dynamic>{});
    for (auto &item : items)
        l.push_back(item);
    return l;
}

BYVALUE_TYPE(map_type)

dynamic dynamic::map()
{
    return dynamic(map_type{});
}

dynamic dynamic::dict()
{
    return dynamic(unordered_map_type{});
}

dynamic dynamic::set()
{
    return dynamic(set_type{});
}

dynamic dynamic::set(std::initializer_list<dynamic> items)
{
    auto s = dynamic::set();
    for (auto &item : items)
        s.insert(item);
    return s;
}

dynamic dynamic::queue()
{
    return dynamic(queue_type{});
}

dynamic dynamic::queue(std::initializer_list<dynamic> items)
{
    auto s = dynamic::queue();
    for (auto &item : items)
        s.push_back(item);
    return s;
}

dynamic dynamic::dict(std::initializer_list<std::pair<dynamic, dynamic>> items)
{
    auto d = dynamic::dict();
    for (auto &&[x, y] : items)
        d.insert(dynamic(std::move(x)), dynamic(std::move(y)));
    return d;
}

dynamic dynamic::map(std::initializer_list<std::pair<dynamic, dynamic>> items)
{
    auto m = dynamic::map();
    for (auto &&[x, y] : items)
        m.insert(dynamic(std::move(x)), dynamic(std::move(y)));
    return m;
}

BYVALUE_TYPE(std::function<dynamic()>);
BYVALUE_TYPE(std::function<dynamic(const dynamic &)>);
BYVALUE_TYPE(std::function<dynamic(const dynamic &, const dynamic &)>);
BYVALUE_TYPE(std::function<dynamic(const dynamic &, const dynamic &, const dynamic &)>);
BYVALUE_TYPE(std::function<dynamic(const dynamic &, const dynamic &, const dynamic &, const dynamic &)>);
