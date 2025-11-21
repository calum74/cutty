#include <cutty/dynamic/enable.hpp>
#include <vector>

dynamic::incompatible::incompatible(const char *msg) : std::runtime_error(msg)
{
}

dynamic::unsupported::unsupported(const char *msg) : std::runtime_error(msg)
{
}

std::string dynamic::str() const
{
    std::stringstream ss;
    m_type->stream_to(*this, ss);
    return ss.str();
}

dynamic dynamic::shared_ref() const &
{
    return dynamic(*this, dynamic::shared_tag());
}

dynamic dynamic::shared_ref() &&
{
    return dynamic(std::move(*this), dynamic::shared_tag());
}

dynamic dynamic::weak_ref() const
{
    return dynamic(*this, weak_reference_tag());
}

dynamic::dynamic(const dynamic &src, weak_reference_tag)
{
    src.m_type->construct_weak_ref(src, *this);
}

dynamic::dynamic(dynamic &&src, shared_tag)
{
    src.m_type->construct_shared(src, *this);
}

const std::string &dynamic::type_str() const
{
    return m_type->type_str(*this);
}

template <> const dynamic &dynamic_detail::try_convert<dynamic>(const dynamic &x, const char *op)
{
    return x;
}

dynamic::dynamic() : dynamic(empty{})
{
}

dynamic::dynamic(const dynamic &src) : m_type(src.m_type)
{
    src.m_type->construct_copy(src, *this);
}

dynamic::dynamic(dynamic &src, by_reference_tag)
{
    src.m_type->construct_reference(src, *this);
}

dynamic::dynamic(const dynamic &src, by_reference_tag)
{
    src.m_type->construct_reference(src, *this);
}

dynamic::dynamic(const dynamic &src, const_value_tag)
{
    src.m_type->construct_const_copy(src, *this);
}

dynamic::dynamic(dynamic &src, const_value_tag)
{
    src.m_type->construct_const_copy(src, *this);
}

dynamic::dynamic(dynamic &src, shared_tag)
{
    src.m_type->construct_shared(src, *this);
}

dynamic::dynamic(const dynamic &src, shared_tag)
{
    src.m_type->construct_shared(src, *this);
}

dynamic::dynamic(dynamic &&other) : dynamic() /* Important to construct empty first */
{
    swap(other);
}

dynamic::dynamic(const char *str) : dynamic(std::string(str))
{
}

dynamic::dynamic(std::string_view str) : dynamic(std::string(str))
{
}

void dynamic::construct(const type *t, void *p)
{
    m_type = t;
    t->construct(*this, p);
}

void dynamic::construct(const type *t, const void *p)
{
    m_type = t;
    t->construct(*this, p);
}

dynamic::~dynamic()
{
    m_type->destroy(*this);
}

dynamic &dynamic::operator=(dynamic &&src)
{
    m_type->assign_from(*this, std::move(src));
    return *this;
}

dynamic &dynamic::operator=(const dynamic &src)
{
    m_type->assign_from(*this, src);
    return *this;
}

void dynamic::swap(dynamic &other)
{
    std::swap(m_type, other.m_type);
    std::swap(m_ptr, other.m_ptr);
}

void *dynamic::get_as(const std::type_info &t)
{
    return m_type->try_get(*this, t);
}

const void *dynamic::get_as(const std::type_info &t) const
{
    return m_type->try_get(*this, t);
}

void *dynamic::as(const std::type_info &t)
{
    if(void *p = get_as(t))
    {
        return p;
    }
    else throw incompatible("Failed conversion to C++ type");
}

const void *dynamic::as(const std::type_info &t) const
{
    if(const void *p = get_as(t))
    {
        return p;
    }
    else throw incompatible("Failed conversion to C++ type");
}


void dynamic::push_back(const dynamic &item)
{
    m_type->push_back(*this, item);
}

std::size_t dynamic::size() const
{
    return m_type->size(*this);
}

dynamic dynamic::begin()
{
    return m_type->begin(*this);
}
dynamic dynamic::begin() const
{
    return m_type->begin(*this);
}

dynamic dynamic::end()
{
    return m_type->end(*this);
}
dynamic dynamic::end() const
{
    return m_type->end(*this);
}

dynamic &dynamic::operator++()
{
    m_type->op_inc(*this);
    return *this;
}

dynamic operator*(const dynamic &x)
{
    return x.m_type->op_star(x);
}

dynamic operator-(const dynamic &x)
{
    return x.m_type->op_minus(x);
}

std::ostream &operator<<(std::ostream &os, const dynamic &x)
{
    x.m_type->stream_to(x, os);
    return os;
}

bool operator==(const dynamic &x, const dynamic &y)
{
    return x.m_type->op_eq(x, y);
}

bool operator<(const dynamic &x, const dynamic &y)
{
    return x.m_type->op_lt(x, y);
}

bool operator!=(const dynamic &x, const dynamic &y)
{
    return !x.m_type->op_eq(x, y);
}

std::strong_ordering operator<=>(const dynamic &x, const dynamic &y)
{
    return std::strong_ordering::less;
//    return x.m_type->op_spaceship(x, y);
}

dynamic operator+(const dynamic &x, const dynamic &y)
{
    return x.m_type->op_add(x, y);
}

dynamic operator-(const dynamic &x, const dynamic &y)
{
    return x.m_type->op_sub(x, y);
}

dynamic operator*(const dynamic &x, const dynamic &y)
{
    return x.m_type->op_mul(x, y);
}

dynamic operator/(const dynamic &x, const dynamic &y)
{
    return x.m_type->op_div(x, y);
}

dynamic operator%(const dynamic &x, const dynamic &y)
{
    return x.m_type->op_mod(x, y);
}

dynamic::operator bool() const
{
    return m_type->as_bool(*this);
}

dynamic::dynamic(std::initializer_list<dynamic> l) : dynamic(list(l))
{
}

dynamic dynamic::call(std::size_t n_args, const dynamic * args) const
{
    return m_type->call(*this, n_args, args);
}

dynamic dynamic::operator[](size_type index)
{
    return m_type->op_index(*this, index);
}

dynamic dynamic::operator[](size_type index) const
{
    return m_type->op_index(*this, index);
}

dynamic dynamic::operator[](const dynamic & index)
{
    return m_type->op_index(*this, index);
}

dynamic dynamic::operator[](const dynamic & index) const
{
    return m_type->op_index(*this, index);
}


dynamic dynamic::front()
{
    return m_type->front(*this);
}

dynamic dynamic::front() const
{
    return m_type->front(*this);
}

dynamic dynamic::back() const
{
    return m_type->back(*this);
}

dynamic dynamic::back()
{
    return m_type->back(*this);
}


void dynamic::pop_back()
{
    m_type->pop_back(*this);
}

void dynamic::pop_front()
{
    m_type->pop_front(*this);
}


std::size_t std::hash<dynamic>::operator()(const dynamic &x) const
{
    return x.hash();
}

std::size_t dynamic::hash() const
{
    return m_type->hash(*this);
}

void dynamic::insert(const dynamic &v)
{
    m_type->insert(*this, v);
}

void dynamic::insert(const dynamic &k, const dynamic &v)
{
    m_type->insert(*this, k, v);
}

template<>
dynamic get<0ul>(const dynamic&x)
{
    return x.first();
}

template<>
dynamic get<1ul>(const dynamic&x)
{
    return x.second();
}

dynamic dynamic::first() const
{
    return m_type->first(*this);
}

dynamic dynamic::first()
{
    return m_type->first(*this);
}

dynamic dynamic::second() const
{
    return m_type->second(*this);
}

dynamic dynamic::second()
{
    return m_type->second(*this);
}

bool operator>(const dynamic &x, const dynamic &y)
{
    return y<x;
}

bool operator<=(const dynamic &x, const dynamic &y)
{
    return !(y>x);
}

bool operator>=(const dynamic &x, const dynamic &y)
{
    return !(x>y);
}

dynamic operator""_d(unsigned long long x)
{
    return dynamic(x);
}

dynamic operator""_d(long double x)
{
    return dynamic(x);
}

dynamic operator""_d(const char * str, std::size_t s)
{
    return dynamic(std::string(str, s));
}

dynamic operator""_d(char ch)
{
    return dynamic(ch);
}
