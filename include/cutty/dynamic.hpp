#pragma once
#include <iosfwd>
#include <stdexcept>
#include <string>
#include <typeinfo>

#ifndef CY_DYNAMIC_EXPLICIT
#define CY_DYNAMIC_EXPLICIT
#endif

namespace cutty
{
/**
 * A dynamic wrapper for any C++ type.
 */
class dynamic
{
  public:
    struct type;
    struct types;

    using value_type = dynamic;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using int_type = std::ptrdiff_t;
    using iterator = dynamic;
    using const_iterator = dynamic;

    template <typename T> class default_traits;
    template <typename T> class traits;
    struct empty_type;

    class exception : public std::runtime_error
    {
      protected:
        exception(const char *msg);
    };

    // Exception thrown when something isn't implemented on the underlying type
    class unsupported : public exception
    {
      public:
        unsupported(const char *msg);
    };

    // Exception thrown when a type cannot be converted
    class incompatible : public exception
    {
      public:
        incompatible(const char *msg);
    };

    struct by_value_tag
    {
    };
    struct by_reference_tag
    {
    };
    struct shared_tag
    {
    };
    struct weak_reference_tag
    {
    };
    struct const_value_tag
    {
    };

    // Instantiate this to add a new dynamic type
    // #include <dynamic/instantiate.hpp>
    template <typename T> static const types &instantiate();

    // Constructors
    dynamic();
    dynamic(const dynamic &src);
    dynamic(dynamic &&other);

    // Copies str to an internal std::string
    CY_DYNAMIC_EXPLICIT dynamic(const char *str);
    CY_DYNAMIC_EXPLICIT dynamic(std::string_view str);

    // Construct a list (vector<dynamic>)
    CY_DYNAMIC_EXPLICIT dynamic(std::initializer_list<dynamic>);

    template <typename T> CY_DYNAMIC_EXPLICIT dynamic(const T &v)
    {
        construct_by_value(instantiate<std::decay_t<T>>(), &v);
    }

    template <typename T>
    CY_DYNAMIC_EXPLICIT dynamic(T &&v)
        requires(std::is_rvalue_reference_v<T>)
    {
        construct_by_rvalue(instantiate<std::decay_t<T>>(), &v);
    }

    template <typename T> CY_DYNAMIC_EXPLICIT dynamic(const T *v)
    {
        construct_by_value(instantiate<const T *>(), &v);
    }

    ~dynamic();

    void swap(dynamic &other);

    // Introspection

    template <typename T> T *try_get()
    {
        return (T *)get_as(typeid(T));
    }

    template <typename T> const T *try_get() const
    {
        return (const T *)get_as(typeid(T));
    }

    template <typename T> T &as()
    {
        return *(T *)as(typeid(T));
    }

    template <typename T> const T &as() const
    {
        return *(const T *)as(typeid(T));
    }

    // Gets type info of the internal value
    const std::type_info &type_info() const;

    // Constructors
    static dynamic list(std::initializer_list<dynamic>);
    static dynamic list();
    static dynamic dict();
    static dynamic dict(std::initializer_list<std::pair<dynamic, dynamic>>);
    static dynamic object();
    static dynamic object(std::initializer_list<std::pair<std::string, dynamic>>);
    static dynamic map();
    static dynamic map(std::initializer_list<std::pair<dynamic, dynamic>>);
    static dynamic set();
    static dynamic set(std::initializer_list<dynamic>);
    static dynamic queue();
    static dynamic queue(std::initializer_list<dynamic>);

    // Functions

    // You must #include <dynamic/function.hpp>
    template <typename Fn> static dynamic function(Fn);

    template <typename... Args> dynamic operator()(Args &&...args) const
    {
        dynamic vec[] = {dynamic(args)...};
        return call(sizeof...(args), vec);
    }

    template <typename... Args> dynamic operator()() const
    {
        return call(0, {});
    }

    // References
    dynamic ref();
    dynamic ref() const;
    dynamic const_ref() const;

    // Const
    dynamic as_const() const;

    template <typename T> static dynamic reference(T &t)
    {
        return dynamic(t, by_reference_tag{});
    }

    template <typename T> static dynamic const_reference(const T &t)
    {
        return dynamic(t, by_reference_tag{});
    }

    // Shared pointers
    dynamic shared_ref() const &;
    dynamic shared_ref() &&;

    dynamic weak_ref() const;
    void make_shared();

    template <typename T> static dynamic shared(const T &t)
    {
        return dynamic(t, shared_tag());
    }

    template <typename T> static dynamic shared(T &&t)
    {
        return dynamic(std::move(t), shared_tag());
    }

    // Containers
    bool empty() const;
    size_type size() const;

    dynamic begin();
    dynamic begin() const;
    dynamic cbegin() const;
    dynamic end();
    dynamic end() const;
    dynamic cend() const;

    dynamic rbegin();
    dynamic rbegin() const;
    dynamic crbegin() const;

    dynamic rend();
    dynamic rend() const;
    dynamic crend() const;

    dynamic front();
    dynamic front() const;
    dynamic back();
    dynamic back() const;

    void push_back(const dynamic &item);
    void pop_back();
    void push_front(const dynamic &item);
    void pop_front();

    void insert(const dynamic &);
    void insert(const dynamic &k, const dynamic &v);
    void erase(const dynamic &);
    void erase(const dynamic &, const dynamic &);

    dynamic first();
    dynamic first() const;
    dynamic second();
    dynamic second() const;

    // Operators
    dynamic operator*() const;
    dynamic &operator++();
    dynamic &operator--();
    dynamic operator++(int);
    dynamic operator--(int);

    // Properties
    std::string str() const;
    const std::string &type_str() const;
    size_type hash() const;

    // Conversions
    explicit operator bool() const;
    explicit operator int_type() const;
    explicit operator double() const;

    int_type as_int() const;
    double as_double() const;

    // Indexers
    dynamic operator[](int index);
    dynamic operator[](int index) const;
    dynamic operator[](size_type index);
    dynamic operator[](size_type index) const;
    dynamic operator[](const dynamic &key);
    dynamic operator[](const dynamic &key) const;
    dynamic operator[](const char *key);
    dynamic operator[](const char *key) const;

    dynamic at(size_type index);
    dynamic at(size_type index) const;
    dynamic at(const dynamic &key);
    dynamic at(const dynamic &key) const;

    // Assignment operators
    dynamic &operator=(dynamic &&src);
    dynamic &operator=(const dynamic &src);
    dynamic &operator+=(const dynamic &);
    dynamic &operator-=(const dynamic &);
    dynamic &operator*=(const dynamic &);
    dynamic &operator/=(const dynamic &);
    dynamic &operator%=(const dynamic &);
    dynamic &operator&=(const dynamic &);
    dynamic &operator|=(const dynamic &);
    dynamic &operator^=(const dynamic &);
    dynamic &operator<<=(const dynamic &);
    dynamic &operator>>=(const dynamic &);

    // Comparison operators
    bool operator==(const dynamic &) const;
    std::weak_ordering operator<=>(const dynamic &) const;

    const type *m_type;
    void *m_ptr;

  private:
    dynamic(dynamic &src, by_reference_tag);
    dynamic(const dynamic &src, by_reference_tag);
    dynamic(const dynamic &src, const_value_tag);
    dynamic(dynamic &src, const_value_tag);
    dynamic(dynamic &src, shared_tag);
    dynamic(dynamic &&src, shared_tag);
    dynamic(const dynamic &src, shared_tag);
    dynamic(const dynamic &src, weak_reference_tag);

    template <typename T> dynamic(const T &v, shared_tag)
    {
        construct_shared(instantiate<std::decay_t<T>>(), &v);
    }

    template <typename T> dynamic(T &&v, shared_tag)
    {
        construct_shared(instantiate<std::decay_t<T>>(), &v);
    }

    template <typename T> dynamic(T &v, by_reference_tag) : m_type()
    {
        construct_by_ref(instantiate<T>(), &v);
    }

    void construct(const type *t, void *p);
    void construct(const type *t, const void *p);

    void construct_by_value(const types &, const void *p);
    void construct_by_rvalue(const types &, void *p);
    void construct_by_ref(const types &, const void *p);
    void construct_shared(const types &, const void *p);

    void *get_as(const std::type_info &);
    const void *get_as(const std::type_info &) const;
    void *as(const std::type_info &);
    const void *as(const std::type_info &) const;
    dynamic call(std::size_t n_args, const dynamic *args) const;
};

template <std::size_t> dynamic get(const dynamic &);
template <std::size_t> dynamic get(const dynamic &);
template <std::size_t> dynamic get(dynamic &);
template <> dynamic get<0>(const dynamic &e);
template <> dynamic get<1>(const dynamic &e);
template <> dynamic get<0>(dynamic &e);
template <> dynamic get<1>(dynamic &e);

// Operators
dynamic operator+(const dynamic &x, const dynamic &y);
dynamic operator-(const dynamic &x, const dynamic &y);
dynamic operator*(const dynamic &x, const dynamic &y);
dynamic operator/(const dynamic &x, const dynamic &y);
dynamic operator%(const dynamic &x, const dynamic &y);

dynamic operator|(const dynamic &x, const dynamic &y);
dynamic operator&(const dynamic &x, const dynamic &y);
dynamic operator^(const dynamic &x, const dynamic &y);

dynamic operator<<(const dynamic &x, const dynamic &y);
dynamic operator>>(const dynamic &x, const dynamic &y);

dynamic operator+(const dynamic &x);
dynamic operator-(const dynamic &x);
dynamic operator~(const dynamic &x);

std::ostream &operator<<(std::ostream &os, const dynamic &x);

namespace literals
{
dynamic operator""_d(const char *, std::size_t);
dynamic operator""_d(unsigned long long);
dynamic operator""_d(long double);
dynamic operator""_d(char);
} // namespace literals

std::ostream &operator<<(std::ostream &os, dynamic::empty_type);
bool operator==(dynamic::empty_type, dynamic::empty_type);
std::strong_ordering operator<=>(dynamic::empty_type, dynamic::empty_type);
} // namespace cutty

template <> struct std::hash<cutty::dynamic>
{
    std::size_t operator()(const cutty::dynamic &) const;
};

template <> struct std::tuple_size<cutty::dynamic>
{
    static constexpr std::size_t value = 2;
};

template <std::size_t Element> struct std::tuple_element<Element, cutty::dynamic>
{
    using type = cutty::dynamic;
};

namespace std
{
template void swap(cutty::dynamic &, cutty::dynamic &);
} // namespace std
