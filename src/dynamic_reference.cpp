#include <cutty/dynamic/instantiate.hpp>
#include <cutty/dynamic/reference.hpp>

#include <sstream>

namespace cy = cutty;

namespace cutty::dynamic_detail
{
template <typename Dynamic> class reference_type : public dynamic::type
{
  public:
    Dynamic &get(const dynamic &r) const
    {
        return *(Dynamic *)r.m_ptr;
    }

    std::remove_const_t<Dynamic> &get_mut(dynamic &r, const char *op) const
    {
        if constexpr (std::is_const_v<Dynamic>)
        {
            std::stringstream ss;
            ss << "Operation '" << op << "' is not supported on const reference";
            throw dynamic::unsupported(ss.str().c_str());
        }
        else
        {
            return *(std::remove_const_t<Dynamic> *)r.m_ptr;
        }
    }

    void construct(dynamic &r, const void *p) const override
    {
    }

    void construct(dynamic &r, void *p) const override
    {
    }

    void construct_copy(const dynamic &p, dynamic &r) const override
    {
        get(p).m_type->construct_copy(get(p), r);
    }

    void construct_const_copy(const dynamic &src, dynamic &dest) const override
    {
        construct_reference_to_dynamic(src, dest);
    }

    void construct_shared(const dynamic &src, dynamic &dest_unassigned) const override
    {
        get(src).m_type->construct_shared(get(src), dest_unassigned);
    }

    void construct_weak_ref(const dynamic &src, dynamic &dest_unassigned) const override
    {
        get(src).m_type->construct_weak_ref(get(src), dest_unassigned);
    }

    void destroy(dynamic &) const override
    {
        // Do nothing
    }

    void stream_to(const dynamic &x, std::ostream &os) const override
    {
        get(x).m_type->stream_to(get(x), os);
    }

    std::optional<std::int64_t> try_get_integral(const dynamic &x) const override
    {
        return get(x).m_type->try_get_integral(get(x));
    }

    std::optional<double> try_get_double(const dynamic &x) const override
    {
        return get(x).m_type->try_get_double(get(x));
    }

    std::optional<std::string_view> try_get_string(const dynamic &x) const override
    {
        return get(x).m_type->try_get_string(get(x));
    }

    void *try_get(dynamic &x, const std::type_info &t) const override
    {
        return get(x).m_type->try_get(get_mut(x, "try_get()"), t);
    }

    const void *try_get(const dynamic &x, const std::type_info &t) const override
    {
        return get(x).m_type->try_get(get(x), t);
    }

    // Three flavours
    // const - target is const, e.g. size()
    // mut - target must be mutable (e.g. push_back()
    // const_wrapper - target can be const or mut

#define FORWARD1(OP, RET)                                                                                              \
    RET OP(const dynamic &x) const override                                                                            \
    {                                                                                                                  \
        return get(x).m_type->OP(get(x));                                                                              \
    }
#define FORWARDM1(OP, RET, NAME)                                                                                       \
    RET OP(dynamic &x) const override                                                                                  \
    {                                                                                                                  \
        return get(x).m_type->OP(get_mut(x, NAME));                                                                    \
    }
#define FORWARD2(OP, RET)                                                                                              \
    RET OP(const dynamic &x, const dynamic &y) const override                                                          \
    {                                                                                                                  \
        return get(x).m_type->OP(get(x), y);                                                                           \
    }
#define FORWARDM2(OP, RET)                                                                                             \
    RET OP(dynamic &x, const dynamic &y) const override                                                                \
    {                                                                                                                  \
        return get(x).m_type->OP(get_mut(x, #OP), y);                                                                  \
    }

    FORWARD2(op_eq, bool)
    FORWARD2(op_lt, bool)

    FORWARD2(op_add, dynamic)
    FORWARD2(op_sub, dynamic)
    FORWARD2(op_mul, dynamic)
    FORWARD2(op_div, dynamic)
    FORWARD2(op_mod, dynamic)

    // Containers
    FORWARDM2(push_back, void);
    FORWARDM1(pop_back, void, "pop_back()");
    FORWARDM2(push_front, void);
    FORWARDM1(pop_front, void, "pop_front()");

    FORWARD1(front, dynamic)
    FORWARD1(back, dynamic)

    FORWARD1(size, std::size_t);
    FORWARD1(begin, dynamic)
    FORWARD1(end, dynamic)

    dynamic begin(dynamic &x) const override
    {
        return get(x).m_type->begin(get(x));
    }

    dynamic end(dynamic &x) const override
    {
        return get(x).m_type->end(get(x));
    }

    dynamic front(dynamic &x) const override
    {
        return get(x).m_type->front(get(x));
    }

    dynamic back(dynamic &x) const override
    {
        return get(x).m_type->back(get(x));
    }

    FORWARD1(op_star, dynamic)

    dynamic op_star(dynamic &x) const override
    {
        return get(x).m_type->op_star(get(x));
    }

    FORWARD1(op_minus, dynamic)

    FORWARD1(hash, std::size_t)

    FORWARDM1(op_inc, void, "++")
    FORWARDM1(op_dec, void, "--")

    void construct_reference(dynamic &src, dynamic &dest) const override
    {
        dest.m_type = src.m_type;
        dest.m_ptr = src.m_ptr;
    }

    void construct_reference(const dynamic &src, dynamic &dest) const override
    {
        dest.m_type = src.m_type;
        dest.m_ptr = src.m_ptr;
    }

    void assign_to(const dynamic &self, dynamic &dest) const override
    {
        get(self).m_type->assign_to(get(self), dest);
    }

    void assign_from(dynamic &self, const dynamic &src) const override
    {
        get(self).m_type->assign_from(get_mut(self, "="), src);
    }

    FORWARD1(type_str, const std::string &);
    FORWARD1(as_bool, bool);

    dynamic call(const dynamic &self, std::size_t n_args, const dynamic *args) const override
    {
        return get(self).m_type->call(get(self), n_args, args);
    }

    dynamic op_index(const dynamic &self, std::size_t i) const override
    {
        return get(self).m_type->op_index(get(self), i);
    }

    dynamic op_index(dynamic &self, std::size_t i) const override
    {
        return get(self).m_type->op_index(get(self), i);
    }

    dynamic op_index(const dynamic &self, const dynamic &i) const override
    {
        return get(self).m_type->op_index(get(self), i);
    }

    dynamic op_index(dynamic &self, const dynamic &i) const override
    {
        return get(self).m_type->op_index(get(self), i);
    }

    void insert(dynamic &self, const dynamic &value) const override
    {
        get(self).m_type->insert(get_mut(self, "insert()"), value);
    }

    void insert(dynamic &self, const dynamic &k, const dynamic &v) const override
    {
        get(self).m_type->insert(get_mut(self, "insert()"), k, v);
    }

    dynamic first(dynamic &self) const override
    {
        return get(self).m_type->first(get(self));
    }

    dynamic first(const dynamic &self) const override
    {
        return get(self).m_type->first(get(self));
    }

    dynamic second(dynamic &self) const override
    {
        return get(self).m_type->second(get(self));
    }

    dynamic second(const dynamic &self) const override
    {
        return get(self).m_type->second(get(self));
    }
};

} // namespace cutty::dynamic_detail

cy::dynamic cy::dynamic::ref()
{
    return dynamic(*this, dynamic::by_reference_tag());
}

cy::dynamic cy::dynamic::ref() const
{
    return dynamic(*this, dynamic::by_reference_tag());
}

cy::dynamic cy::dynamic::const_ref() const
{
    return cy::dynamic(*this, dynamic::by_reference_tag());
}

void cy::dynamic_detail::construct_reference_to_dynamic(const dynamic &src, dynamic &dest)
{
    static reference_type<const dynamic> type;
    dest.m_type = &type;
    dest.m_ptr = (void *)&src;
}

void cy::dynamic_detail::construct_reference_to_dynamic(dynamic &src, dynamic &dest)
{
    static reference_type<dynamic> type;
    dest.m_type = &type;
    dest.m_ptr = &src;
}
