#include <cutty/dynamic/instantiate.hpp>
#include <vector>

namespace cy = cutty;

cy::dynamic::exception::exception(const char *msg) : std::runtime_error(msg)
{
}

cy::dynamic::incompatible::incompatible(const char *msg) : exception(msg)
{
}

cy::dynamic::unsupported::unsupported(const char *msg) : exception(msg)
{
}

std::string cy::dynamic::str() const
{
    std::stringstream ss;
    m_type->stream_to(*this, ss);
    return ss.str();
}

cy::dynamic cy::dynamic::shared_ref() const &
{
    return dynamic(*this, dynamic::shared_tag());
}

cy::dynamic cy::dynamic::shared_ref() &&
{
    return dynamic(std::move(*this), dynamic::shared_tag());
}

cy::dynamic cy::dynamic::weak_ref() const
{
    return dynamic(*this, weak_reference_tag());
}

cy::dynamic::dynamic(const dynamic &src, weak_reference_tag)
{
    src.m_type->construct_weak_ref(src, *this);
}

cy::dynamic cy::dynamic::as_const() const
{
    return cy::dynamic(*this, const_value_tag());
}

cy::dynamic::dynamic(dynamic &&src, shared_tag)
{
    src.m_type->construct_shared(src, *this);
}

const std::string &cy::dynamic::type_str() const
{
    return m_type->type_str(*this);
}

template <> const cy::dynamic &cy::dynamic_detail::try_convert<cy::dynamic>(const dynamic &x, const char *op)
{
    return x;
}

cy::dynamic::dynamic() : dynamic(empty_type{})
{
}

cy::dynamic::dynamic(const dynamic &src) : m_type(src.m_type)
{
    src.m_type->construct_copy(src, *this);
}

cy::dynamic::dynamic(dynamic &src, by_reference_tag)
{
    src.m_type->construct_reference(src, *this);
}

cy::dynamic::dynamic(const dynamic &src, by_reference_tag)
{
    src.m_type->construct_reference(src, *this);
}

cy::dynamic::dynamic(const dynamic &src, const_value_tag)
{
    src.m_type->construct_const_copy(src, *this);
}

cy::dynamic::dynamic(dynamic &src, const_value_tag)
{
    src.m_type->construct_const_copy(src, *this);
}

cy::dynamic::dynamic(dynamic &src, shared_tag)
{
    src.m_type->construct_shared(src, *this);
}

cy::dynamic::dynamic(const dynamic &src, shared_tag)
{
    src.m_type->construct_shared(src, *this);
}

cy::dynamic::dynamic(dynamic &&other) : dynamic() /* Important to construct empty first */
{
    swap(other);
}

cy::dynamic::dynamic(const char *str) : dynamic(std::string(str))
{
}

cy::dynamic::dynamic(std::string_view str) : dynamic(std::string(str))
{
}

void cy::dynamic::construct(const type *t, void *p)
{
    m_type = t;
    t->construct(*this, p);
}

void cy::dynamic::construct(const type *t, const void *p)
{
    m_type = t;
    t->construct(*this, p);
}

void cy::dynamic::construct_by_value(const types &t, const void *p)
{
    construct(&t.by_value, p);
}

void cy::dynamic::construct_by_rvalue(const types &t, void *p)
{
    construct(&t.by_value, p);
}

void cy::dynamic::construct_by_ref(const types &t, const void *p)
{
    construct(&t.by_ref, p);
}

void cy::dynamic::construct_shared(const types &t, const void *p)
{
    construct(&t.shared, p);
}

cy::dynamic::~dynamic()
{
    m_type->destroy(*this);
}

cy::dynamic &cy::dynamic::operator=(dynamic &&src)
{
    m_type->assign_from(*this, std::move(src));
    return *this;
}

cy::dynamic &cy::dynamic::operator=(const dynamic &src)
{
    m_type->assign_from(*this, src);
    return *this;
}

void cy::dynamic::swap(dynamic &other)
{
    std::swap(m_type, other.m_type);
    std::swap(m_ptr, other.m_ptr);
}

void *cy::dynamic::get_as(const std::type_info &t)
{
    return m_type->try_get(*this, t);
}

const void *cy::dynamic::get_as(const std::type_info &t) const
{
    return m_type->try_get(*this, t);
}

void *cy::dynamic::as(const std::type_info &t)
{
    if (void *p = get_as(t))
    {
        return p;
    }
    else
        throw incompatible("Failed conversion to C++ type");
}

const void *cy::dynamic::as(const std::type_info &t) const
{
    if (const void *p = get_as(t))
    {
        return p;
    }
    else
        throw incompatible("Failed conversion to C++ type");
}

const std::type_info &cy::dynamic::type_info() const
{
    return m_type->type_info(*this);
}

void cy::dynamic::push_back(const dynamic &item)
{
    m_type->push_back(*this, item);
}

void cy::dynamic::push_front(const dynamic &item)
{
    m_type->push_front(*this, item);
}

std::size_t cy::dynamic::size() const
{
    return m_type->size(*this);
}

cy::dynamic cy::dynamic::begin()
{
    return m_type->begin(*this);
}
cy::dynamic cy::dynamic::begin() const
{
    return m_type->begin(*this);
}

cy::dynamic cy::dynamic::end()
{
    return m_type->end(*this);
}

cy::dynamic cy::dynamic::end() const
{
    return m_type->end(*this);
}

cy::dynamic cy::dynamic::rbegin()
{
    return m_type->rbegin(*this);
}

cy::dynamic cy::dynamic::rbegin() const
{
    return m_type->rbegin(*this);
}

cy::dynamic cy::dynamic::crbegin() const
{
    return m_type->rbegin(*this);
}

cy::dynamic cy::dynamic::rend()
{
    return m_type->rend(*this);
}

cy::dynamic cy::dynamic::rend() const
{
    return m_type->rend(*this);
}

cy::dynamic cy::dynamic::crend() const
{
    return m_type->rend(*this);
}

cy::dynamic &cy::dynamic::operator++()
{
    m_type->op_inc(*this);
    return *this;
}

cy::dynamic cy::dynamic::operator++(int)
{
    dynamic copy = *this;
    m_type->op_inc(*this);
    return copy;
}

cy::dynamic &cy::dynamic::operator--()
{
    m_type->op_dec(*this);
    return *this;
}

cy::dynamic cy::dynamic::operator--(int)
{
    dynamic copy = *this;
    m_type->op_dec(*this);
    return copy;
}

cy::dynamic cy::dynamic::operator*() const
{
    return m_type->op_star(*this);
}

cy::dynamic cy::operator-(const cy::dynamic &x)
{
    return x.m_type->op_minus(x);
}

cy::dynamic cutty::operator+(const cy::dynamic &x)
{
    return x.m_type->op_plus(x);
}

std::ostream &cy::operator<<(std::ostream &os, const dynamic &x)
{
    x.m_type->stream_to(x, os);
    return os;
}

bool cy::dynamic::operator==(const cy::dynamic &y) const
{
    return m_type->op_eq(*this, y);
}

std::partial_ordering cy::dynamic::operator<=>(const cy::dynamic &y) const
{
    return m_type->op_cmp(*this, y);
}

cy::dynamic cy::operator+(const cy::dynamic &x, const cy::dynamic &y)
{
    return x.m_type->op_add(x, y);
}

cy::dynamic cy::operator-(const cy::dynamic &x, const cy::dynamic &y)
{
    return x.m_type->op_sub(x, y);
}

cy::dynamic cy::operator*(const cy::dynamic &x, const cy::dynamic &y)
{
    return x.m_type->op_mul(x, y);
}

cy::dynamic cy::operator/(const cy::dynamic &x, const cy::dynamic &y)
{
    return x.m_type->op_div(x, y);
}

cy::dynamic cy::operator%(const cy::dynamic &x, const cy::dynamic &y)
{
    return x.m_type->op_mod(x, y);
}

cy::dynamic::operator bool() const
{
    return m_type->as_bool(*this);
}

cy::dynamic::operator int_type() const
{
    return m_type->as_int(*this);
}

cy::dynamic::operator double() const
{
    return m_type->as_double(*this);
}

cy::dynamic::int_type cy::dynamic::as_int() const
{
    return m_type->as_int(*this);
}

double cy::dynamic::as_double() const
{
    return m_type->as_double(*this);
}

cy::dynamic::dynamic(std::initializer_list<dynamic> l) : dynamic(list(l))
{
}

cy::dynamic cy::dynamic::call(std::size_t n_args, const dynamic *args) const
{
    return m_type->call(*this, n_args, args);
}

cy::dynamic cy::dynamic::operator[](size_type index)
{
    return m_type->op_index(*this, std::int64_t(index));
}

cy::dynamic cy::dynamic::operator[](size_type index) const
{
    return m_type->op_index(*this, std::int64_t(index));
}

cy::dynamic cy::dynamic::operator[](int index)
{
    return m_type->op_index(*this, std::int64_t(index));
}

cy::dynamic cy::dynamic::operator[](int index) const
{
    return m_type->op_index(*this, std::int64_t(index));
}

cy::dynamic cy::dynamic::operator[](const dynamic &index)
{
    return m_type->op_index(*this, index);
}

cy::dynamic cy::dynamic::operator[](const dynamic &index) const
{
    return m_type->op_index(*this, index);
}

cy::dynamic cy::dynamic::operator[](const char *index)
{
    return m_type->op_index(*this, index);
}

cy::dynamic cy::dynamic::operator[](const char *index) const
{
    return m_type->op_index(*this, index);
}

cy::dynamic cy::dynamic::front()
{
    return m_type->front(*this);
}

cy::dynamic cy::dynamic::front() const
{
    return m_type->front(*this);
}

cy::dynamic cy::dynamic::back() const
{
    return m_type->back(*this);
}

cy::dynamic cy::dynamic::back()
{
    return m_type->back(*this);
}

void cy::dynamic::pop_back()
{
    m_type->pop_back(*this);
}

void cy::dynamic::pop_front()
{
    m_type->pop_front(*this);
}

std::size_t std::hash<cy::dynamic>::operator()(const cy::dynamic &x) const
{
    return x.hash();
}

std::size_t cy::dynamic::hash() const
{
    return m_type->hash(*this);
}

void cy::dynamic::insert(const dynamic &v)
{
    m_type->insert(*this, v);
}

void cy::dynamic::insert(const dynamic &k, const dynamic &v)
{
    m_type->insert(*this, k, v);
}

template <> cy::dynamic cy::get<0ul>(const cy::dynamic &x)
{
    return x.first();
}

template <> cy::dynamic cy::get<0ul>(cy::dynamic &x)
{
    return x.first();
}

template <> cy::dynamic cy::get<1ul>(const cy::dynamic &x)
{
    return x.second();
}

template <> cy::dynamic cy::get<1ul>(cy::dynamic &x)
{
    return x.second();
}

cy::dynamic cy::dynamic::first() const
{
    return m_type->first(*this);
}

cy::dynamic cy::dynamic::first()
{
    return m_type->first(*this);
}

cy::dynamic cy::dynamic::second() const
{
    return m_type->second(*this);
}

cy::dynamic cy::dynamic::second()
{
    return m_type->second(*this);
}

cy::dynamic cy::literals::operator""_d(unsigned long long x)
{
    return cy::dynamic(x);
}

cy::dynamic cy::literals::operator""_d(long double x)
{
    return cy::dynamic(x);
}

cy::dynamic cy::literals::operator""_d(const char *str, std::size_t s)
{
    return cy::dynamic(std::string(str, s));
}

cy::dynamic cy::literals::operator""_d(char ch)
{
    return cy::dynamic(ch);
}

std::size_t std::hash<cy::dynamic::empty_type>::operator()(const cy::dynamic::empty_type&) const { return 0; }

bool cy::dynamic::empty() const
{
    return m_type->empty(*this);
}

void cy::dynamic::erase(const dynamic &i)
{
    m_type->erase(*this, i);
}

void cy::dynamic::erase(const dynamic &i, const dynamic &j)
{
    m_type->erase(*this, i, j);
}
