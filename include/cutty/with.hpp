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
    class with final
    {
    public:
        with(T &value) : m_previous_value(try_get<T,Tag>())
        {
            detail::with_getter<T,Tag>() = &value;
        }
        with(T&&) = delete;
        with(with&&) = delete;
        with(const with&) = delete;
        with & operator=(const with&) = delete;

        ~with()
        {
            detail::with_getter<T,Tag>() = m_previous_value;
        }
    private:
        T * const m_previous_value;
    };
} // namespace cutty
