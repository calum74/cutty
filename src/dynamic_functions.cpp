#include <cutty/dynamic/instantiate.hpp>

#include <functional>

namespace cy = cutty;

CY_INSTANTIATE(std::function<cy::dynamic()>);
CY_INSTANTIATE(std::function<cy::dynamic(const cy::dynamic&)>);
CY_INSTANTIATE(std::function<cy::dynamic(const cy::dynamic&, const cy::dynamic&)>);
CY_INSTANTIATE(std::function<cy::dynamic(const cy::dynamic&, const cy::dynamic&, const cy::dynamic&)>);
CY_INSTANTIATE(std::function<cy::dynamic(const cy::dynamic&, const cy::dynamic&, const cy::dynamic&, const cy::dynamic&)>);
// Ok to add more here
