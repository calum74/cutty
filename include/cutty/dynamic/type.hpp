#pragma once
#include <cstdint>
#include <optional>

struct cutty::dynamic::type
{
    virtual ~type() = default;
    virtual void construct(dynamic &unassigned, const void *) const = 0;
    virtual void construct(dynamic &unassigned, void *) const = 0;

    virtual void construct_copy(const dynamic &src, dynamic &dest_unassigned) const = 0;
    virtual void construct_const_copy(const dynamic &src, dynamic &dest_unassigned) const = 0;
    virtual void construct_reference(const dynamic &src_self, dynamic &dest_unassigned) const = 0;
    virtual void construct_reference(dynamic &src_self, dynamic &dest_unassigned) const = 0;
    virtual void construct_shared(const dynamic &src, dynamic &dest_unassigned) const = 0;
    virtual void construct_weak_ref(const dynamic &src, dynamic &dest_unassigned) const = 0;

    virtual void destroy(dynamic &) const = 0;

    virtual void assign_to(const dynamic &src, dynamic &dest) const = 0;
    virtual void assign_from(dynamic &dest, const dynamic &src) const = 0;

    virtual void stream_to(const dynamic &, std::ostream &) const = 0;

    virtual std::optional<int_type> try_get_integral(const dynamic &) const = 0;
    virtual std::optional<double> try_get_double(const dynamic &) const = 0;
    virtual std::optional<std::string_view> try_get_string(const dynamic &) const = 0;
    virtual void *try_get(dynamic &, const std::type_info &) const = 0;
    virtual const void *try_get(const dynamic &, const std::type_info &) const = 0;
    virtual const std::string &type_str(const dynamic &) const = 0;

    virtual bool as_bool(const dynamic &) const = 0;
    virtual dynamic::int_type as_int(const dynamic &) const = 0;
    virtual double as_double(const dynamic&) const = 0;
    virtual std::string as_string(const dynamic&) const = 0;
    virtual std::string_view as_string_view(const dynamic&) const = 0;

    virtual bool op_eq(const dynamic &x, const dynamic &y) const = 0;
    virtual std::partial_ordering op_cmp(const dynamic &x, const dynamic &y) const = 0;

    virtual dynamic op_add(const dynamic &x, const dynamic &y) const = 0;
    virtual dynamic op_sub(const dynamic &x, const dynamic &y) const = 0;
    virtual dynamic op_mul(const dynamic &x, const dynamic &y) const = 0;
    virtual dynamic op_div(const dynamic &x, const dynamic &y) const = 0;
    virtual dynamic op_mod(const dynamic &x, const dynamic &y) const = 0;

    virtual std::size_t hash(const dynamic &x) const = 0;

    virtual const std::type_info & type_info(const dynamic &x) const = 0;
    virtual value_category category(const dynamic&) const = 0;

    // Containers
    virtual void push_back(dynamic &x, const dynamic &y) const = 0;
    virtual void pop_back(dynamic &x) const = 0;
    virtual void push_front(dynamic &x, const dynamic &y) const = 0;
    virtual void pop_front(dynamic &x) const = 0;

    virtual std::size_t size(const dynamic &x) const = 0;
    virtual dynamic begin(dynamic &) const = 0;
    virtual dynamic begin(const dynamic &) const = 0;
    virtual dynamic end(dynamic &) const = 0;
    virtual dynamic end(const dynamic &) const = 0;
    virtual dynamic rbegin(dynamic &) const = 0;
    virtual dynamic rbegin(const dynamic &) const = 0;
    virtual dynamic rend(dynamic &) const = 0;
    virtual dynamic rend(const dynamic &) const = 0;

    virtual dynamic front(dynamic &) const = 0;
    virtual dynamic front(const dynamic &) const = 0;
    virtual dynamic back(dynamic &) const = 0;
    virtual dynamic back(const dynamic &) const = 0;

    virtual void insert(dynamic &, const dynamic &) const = 0;
    virtual void insert(dynamic &, const dynamic &k, const dynamic &v) const = 0;
    virtual bool empty(const dynamic&) const = 0;
    virtual void erase(dynamic&, const dynamic&) const = 0;
    virtual void erase(dynamic&, const dynamic&, const dynamic&) const = 0;
    virtual dynamic first(dynamic &) const = 0;
    virtual dynamic first(const dynamic &) const = 0;
    virtual dynamic second(dynamic &) const = 0;
    virtual dynamic second(const dynamic &) const = 0;

    // Iterators
    virtual dynamic op_star(const dynamic &x) const = 0;
    virtual dynamic op_star(dynamic &x) const = 0;
    virtual void op_inc(dynamic &x) const = 0;
    virtual void op_dec(dynamic &x) const = 0;
    virtual dynamic op_plus(const dynamic &x) const = 0;
    virtual dynamic op_minus(const dynamic &x) const = 0;

    // virtual dynamic call0(const dynamic &self) const;
    // virtual dynamic call1(const dynamic &self, const dynamic &arg0) const;
    // virtual dynamic call2(const dynamic &self, const dynamic &arg0, const dynamic &arg1) const;
    virtual dynamic call(const dynamic &self, std::size_t n_args, const dynamic *args) const = 0;

    virtual dynamic op_index(const dynamic &x, int_type i) const = 0;
    virtual dynamic op_index(dynamic &x, int_type i) const = 0;
    virtual dynamic op_index(const dynamic &x, const dynamic &i) const = 0;
    virtual dynamic op_index(dynamic &x, const dynamic &i) const = 0;

    virtual dynamic op_index(const dynamic &x, const char* i) const = 0;
    virtual dynamic op_index(dynamic &x, const char *i) const = 0;
};
