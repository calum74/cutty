#include <cutty/dynamic/instantiate.hpp>

#include <deque>
#include <vector>

namespace cy = cutty;

using list_type = std::vector<cy::dynamic>;
using queue_type = std::deque<cy::dynamic>;

void fn()
{
    queue_type queue;
    queue.push_front(cy::dynamic(123));
}

CY_INSTANTIATE(std::vector<cy::dynamic>)

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
