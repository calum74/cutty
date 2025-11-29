// Copyright (C) Calum Grant 2003
// Copying permitted under the terms of the GNU Public Licence (GPL)
//
// Defines common STL containers using the persist::alloc allocator

#ifndef _PERSIST_STL_H
#define _PERSIST_STL_H
#include "persist.h"

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace cutty::persist
{
template <class T> using list = std::list<T, persist::allocator<T>>;

template <class C, class Traits = std::char_traits<C>>
using basic_string = std::basic_string<C, Traits, persist::allocator<C>>;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
} // namespace cutty::persist

namespace persist
{
template <class T> using vector = std::vector<T, persist::allocator<T>>;

template <class T, class L = std::less<T>> using class set = std::set<T, L, persist::allocator<T>>;

template <class T, class L = std::less<T>> using multiset = std::multiset<T, L, persist::allocator<T>>;

template <class T, class V, class L = std::less<T>> using map = std::map<T, V, L, persist::allocator<std::pair<T, V>>>;

template <class T, class V, class L = std::less<T>>
using multimap = std::multimap<T, V, L, persist::allocator<std::pair<T, V>>>;

// owner
// An owner is a pointer to a persistent object
// It's like an auto_ptr, but less broken,
// and works with persistent data and the STL
template <class T, class A = persist::allocator<T>> class owner
{
    T *ptr;

  public:
    owner() : ptr(0)
    {
    }
    owner(T *p) : ptr(p)
    {
    }
    owner(const owner<T, A> &o)
    {
        if (this != &o) // Why copy to self?
        {
            // Wrest the pointer from o, even though it is const
            ptr = o.ptr;
            const_cast<owner<T, A> &>(o).ptr = 0;
        }
    }

    ~owner()
    {
        destroy();
    }

    owner<T, A> &operator=(T *p)
    {
        destroy();
        ptr = p;
        return *this;
    }

    void create()
    {
        destroy();
        ptr = new T;
    }

    void create(const T &c)
    {
        destroy();
        ptr = new T(c);
    }

    void destroy()
    {
        if (ptr)
        {
            A().destroy(ptr);
            A().deallocate(ptr, 1);
            ptr = 0;
        }
    }

    owner<T, A> &operator=(const owner<T, A> &o)
    {
        if (this != &o)
        {
            // Wrest the pointer from o, even though it is const
            ptr = o.ptr;
            const_cast<owner<T, A> &>(o).ptr = 0;
        }
        return *this;
    }

    T *operator->() const
    {
        return ptr;
    }
    T &operator*()
    {
        return *ptr;
    }
    const T &operator*() const
    {
        return *ptr;
    }

    // Returns the pointer
    T *get() const
    {
        return ptr;
    }

    // Returns the pointer, and releases it from the owner.
    // The caller must destroy this pointer.
    T *release() const
    {
        T *p = ptr;
        ptr = 0;
        return p;
    }
};

// fixed_string is a fixed-length string
template <int N, class C = char> class fixed_string
{
    unsigned char len; // The actual length of the string
    C str[N + 1];

  public:
    typedef C *iterator;
    typedef const C *const_iterator;

    fixed_string()
    {
        clear();
    }
    fixed_string(const_iterator d)
    {
        assign(d);
    }

    template <int M> fixed_string(const fixed_string<M, C> &b)
    {
        assign(b.begin(), b.end());
    }

    fixed_string<N, C> &operator=(const_iterator d)
    {
        assign(d);
        return *this;
    }

    template <int M> fixed_string<N, C> &operator=(const fixed_string<M, C> &b)
    {
        assign(b.begin(), b.end());
        return *this;
    }

    iterator begin()
    {
        return str;
    }
    const_iterator begin() const
    {
        return str;
    }

    iterator end()
    {
        return str + len;
    }
    const_iterator end() const
    {
        return str + len;
    }

    const_iterator c_str() const
    {
        return begin();
    }

    size_t size() const
    {
        return len;
    }

    void clear()
    {
        len = 0;
        str[len] = 0;
    }

    void assign(const_iterator a)
    {
        for (len = 0; len < N && *a; ++a, ++len)
            str[len] = *a;

        str[len] = 0; // Zero-terminate
    }

    void assign(const_iterator a, const_iterator b)
    {
        for (len = 0; len < N && a != b; ++a, ++len)
            str[len] = *a;

        str[len] = 0; // Zero-terminate
    }

    C &operator[](size_t n)
    {
        return str[n];
    }
    const C &operator[](size_t n) const
    {
        return str[n];
    }

    template <int M> bool operator==(const fixed_string<M, C> &b) const
    {
        if (size() != b.size())
            return false;

        for (unsigned p = 0; p < size(); ++p)
        {
            if (str[p] != b[p])
                return false;
        }
        return true;
    }

    bool operator==(const_iterator b) const
    {
        for (unsigned p = 0; p <= size(); ++p)
        {
            if (str[p] != b[p])
                return false;
        }

        return true;
    }

    template <int M> bool operator<(const fixed_string<M, C> &b) const
    {
        for (const C *pa = begin(), *pb = b.begin(); *pb; ++pa, ++pb)
        {
            if (*pa < *pb)
                return true;
            else if (*pa > *pb)
                return false;
        }

        return false;
    }
};
} // namespace persist
#endif
