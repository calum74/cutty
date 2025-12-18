/*
    Satellite is a better way to manage globals and thread-local values
    and allows a more principled value-passing whilst not changing an API.

    Written by Calum Grant 20251109
 */

#pragma once
#include <stdexcept>

namespace cutty
{
    namespace detail
    {
        template<typename T, typename Tag>
        T*& with_getter()
        {
            thread_local T *p = 0;
            return p;
        }
    }

    template<typename T, typename Tag=T>
    T * try_get()
    {
        return detail::with_getter<T,Tag>();
    };

    class without : public std::runtime_error
    {
    public:
        without();
    };

    template<typename T, typename Tag=T>
    T & get()
    {
        if(auto *p = try_get<T,Tag>())
        {
            return *p;
        }
        throw without();
    };

    template<typename T, typename Tag=T>
    class with
    {
    public:
        with(T &value) : m_previous_value(try_get<T,Tag>())
        {
            detail::with_getter<T,Tag>() = &value;
        }
        with(T&&) = delete;
        with(with&&) = delete;
        with(const with&) = delete;

        ~with()
        {
            detail::with_getter<T,Tag>() = m_previous_value;
        }
    private:
        T * const m_previous_value;
    };

template <typename T, typename Tag=T> class satellite
{
  public:
    using value_type = T;

    satellite() : m_previous(get())
    {
    }

    satellite(value_type &&new_value) = delete;

    satellite(value_type &new_value) : m_previous(get())
    {
        get() = &new_value;
    }

    satellite(const satellite &) = delete;

    satellite &operator=(const satellite &) = delete;

    ~satellite()
    {
        get() = m_previous;
    }

    satellite &operator=(value_type &v)
    {
        get() = &v;
        return *this;
    }

    satellite &operator=(value_type &&v) = delete;

    value_type &operator*() const
    {
        return *get();
    }

    value_type *operator->() const
    {
        return get();
    }

    explicit operator bool() const
    {
        return get();
    }

  private:
    value_type *m_previous;

    static value_type *&get()
    {
        thread_local value_type *ptr;
        return ptr;
    }
};
} // namespace cutty
