#include <cutty/dynamic/enable.hpp>

#include <functional>

namespace cy = cutty;

#define BYVALUE_TYPE(T) template const cy::dynamic::types &cy::dynamic::instantiate<T>();

BYVALUE_TYPE(std::function<cy::dynamic()>);
BYVALUE_TYPE(std::function<cy::dynamic(const cy::dynamic&)>);
BYVALUE_TYPE(std::function<cy::dynamic(const cy::dynamic&, const cy::dynamic&)>);
BYVALUE_TYPE(std::function<cy::dynamic(const cy::dynamic&, const cy::dynamic&, const cy::dynamic&)>);
BYVALUE_TYPE(std::function<cy::dynamic(const cy::dynamic&, const cy::dynamic&, const cy::dynamic&, const cy::dynamic&)>);
// Ok to add more here
