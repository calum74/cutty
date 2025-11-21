/*
    Satellite is a better way to manage globals and thread-local values
    and allows a more principled value-passing whilst not changing an API.

    Written by Calum Grant 20251109
 */



#pragma once

template <typename T, typename Tag> class satellite
{
  public:
    using value_type = T;

    satellite() : m_previous(get())
    {
    }

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
