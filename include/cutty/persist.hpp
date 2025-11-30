// Copyright (C) Calum Grant 2003-2021
// Copying permitted under the terms of the GNU Public Licence (GPL)

#ifndef PERSIST_H
#define PERSIST_H

#ifdef _WIN32
#include "persist_win32.h"
#else
#include "persist_unix.h"
#endif

#include "shared_memory.hpp"

#include <atomic>
#include <cassert>
#include <memory>
#include <stdexcept>

namespace cutty
{
// Exception thrown when an invalid datafile is opened,
// or the datafile is the wrong version.
class InvalidVersion : public std::runtime_error
{
  public:
    InvalidVersion();
};

class map_file;

namespace detail
{
class shared_record
{
  public:
    typedef std::size_t size_type;

    // Returns true if the heap is empty: no objects have yet been created
    bool empty() const;

    bool lock(int ms = 0); // Mutex the entire heap
    void unlock();         // Release the entire heap

    bool wait(int ms = 0); // Wait for event
    void signal();         // Signal event

    void *root();             // The root object
    const void *root() const; // The root object

    void free(void *, size_t);
    void clear();

    size_type capacity() const;
    size_type size() const;
    size_type limit() const;
    void limit(size_type);

  private:
    friend map_file;
    shared_record(const shared_record &) = delete;

    // Magic bytes to check we have loaded the correct version
    int magic;
    int applicationId;
    short majorVersion;
    short minorVersion;
    int hardwareId;

    shared_record *address; // The address we expect to be at - we need to reopen if this fails

    size_t current_size; // The size of the allocation
    size_t max_size;

    void *condition;

    std::atomic<char *> top, end;

    void *free_space[64]; // An embarrassingly simple memory manager

    shared_base extra;

    void unmap();
    void lockMem();
    void unlockMem();
};
}

enum
{
    shared_heap = 1,
    private_map = 2,
    temp_heap = 8,
    create_new = 16,
    read_only = 32
};

// map_file
// A wrapper around a block of shared memory.
// This provides memory management functions, locking, and
// extends the heap when necessary.
class map_file
{
    shared_memory memory;
    detail::shared_record *map_address;

  public:
    map_file();

    map_file(const char *filename, int applicationId, short majorVersion, short minorVersion, size_t length = 16384,
             size_t limit = 1000000, int flags = 0, size_t base = default_map_address);

    ~map_file();

    void open(const char *filename, int applicationId, short majorVersion, short minorVersion, size_t length = 16384,
              size_t limit = 1000000, int flags = 0, size_t base = default_map_address);

    void close();

    // Returns true if the heap is valid and usable
    operator bool() const
    {
        return map_address != 0;
    }

    bool empty() const { return data().empty(); }
    void * root() const { return data().root(); }
    void * malloc(size_t x);
    size_t capacity() const { return data().capacity(); }
    void free(void*p, size_t s) { data().free(p, s); }
    void clear() { data().clear(); }

    void *fast_malloc(size_t size)
    {
        auto &d = data();
        auto r = size & 7;
        if (r)
            size += (8 - r);
        assert((size & 7) == 0);
        auto result = d.top += size;
        if (result > d.end)
        {
            d.lockMem();
            bool failed = !extend_to(result);
            if (failed)
                d.top -= size;
            d.unlockMem();
            if (failed)
                return nullptr;
        }
        // top is a std::atmoic, so we need to do it like this:
        return result - size;
    }

    bool extend_to(void * new_top);

    detail::shared_record &data() const { return *map_address; }
};

template <class T> class fast_allocator : public std::allocator<T>
{
  public:
    fast_allocator(map_file &map) : map(map)
    {
    }

    // Construct from another allocator
    template <class O> fast_allocator(const fast_allocator<O> &o) : map(o.map)
    {
    }

    typedef T value_type;
    typedef const T *const_pointer;
    typedef T *pointer;
    typedef const T &const_reference;
    typedef T &reference;
    typedef typename std::allocator<T>::difference_type difference_type;
    typedef typename std::allocator<T>::size_type size_type;

    pointer allocate(size_type n)
    {
        pointer p = static_cast<pointer>(map.fast_malloc(n * sizeof(T)));
        if (!p)
            throw std::bad_alloc();

        return p;
    }

    void deallocate(pointer p, size_type count)
    {
    }

    size_type max_size() const
    {
        return map.capacity() / sizeof(T);
    }

    template <class Other> struct rebind
    {
        typedef fast_allocator<Other> other;
    };

    map_file &map;
};

template <class T> class allocator : public std::allocator<T>
{
  public:
    allocator(map_file &map) : map(map)
    {
    }

    // Construct from another allocator
    template <class O> allocator(const allocator<O> &o) : map(o.map)
    {
    }

    typedef T value_type;
    typedef const T *const_pointer;
    typedef T *pointer;
    typedef const T &const_reference;
    typedef T &reference;
    typedef typename std::allocator<T>::difference_type difference_type;
    typedef typename std::allocator<T>::size_type size_type;

    pointer allocate(size_type n)
    {
        pointer p = static_cast<pointer>(map.malloc(n * sizeof(T)));
        if (!p)
            throw std::bad_alloc();

        return p;
    }

    void deallocate(pointer p, size_type count)
    {
        map.free(p, count * sizeof(T));
    }

    size_type max_size() const
    {
        return map.capacity() / sizeof(T);
    }

    template <class Other> struct rebind
    {
        typedef allocator<Other> other;
    };

    map_file &map;
};

template <class T> class map_data
{
  public:
    typedef T value_type;

    template <typename... ConstructorArgs> map_data(map_file &mem, ConstructorArgs &&...init) : file(mem)
    {
        if (mem.empty())
        {
            new (file) value_type(init...);
        }
    }

    map_data(map_file &mem) : file(mem)
    {
        if (this->file.empty())
        {
            new (file) value_type();
        }
    }

    value_type &operator*()
    {
        return *static_cast<T *>(file.root());
    }

    const value_type &operator*() const
    {
        return *static_cast<T *>(file.root());
    }

    value_type *operator->()
    {
        return static_cast<T *>(file.root());
    }

    const value_type *operator->() const
    {
        return static_cast<T *>(file.root());
    }

  private:
    map_file &file;
};
} // namespace cutty

void *operator new(size_t size, cutty::map_file &mem);
void operator delete(void *p, cutty::map_file &mem);

#endif
