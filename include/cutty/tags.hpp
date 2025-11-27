#pragma once

#include <iostream>

namespace cutty
{
    template<typename T, typename Tag>
    class tagged;

    template<typename T, typename Tag>
    void initialise(tagged<T, Tag>&v)
    {
        *v = T();
    }

    template<typename T, typename Tag>
    class tagged
    {
    public:
        using value_type = T;
        using tag_type = Tag;

        tagged()  // !! Concept requires has_default_value<Tag,T>
        {
            initialise(*this);
        }

        template<typename T2, typename Tag2>
        tagged(const tagged<T2, Tag2> &src)
        {
            convert(src, *this);
        }

        explicit tagged(const T & v) : value(v) {}

        // explicit tagged(value_type v) : value(std::move(v)) {}

        const value_type & operator*() const
        {
            return value;
        }

        value_type & operator*()
        {
            return value;
        }

        template<typename T2, typename Tag2>
        tagged & operator=(const tagged<T2, Tag2> &src)
        {
            convert(src, *this);
            return *this;
        }

    private:
        value_type value;
    };

    template<typename T, typename Tag>
    const char * tag_suffix(tagged<T,Tag>) { return ""; }

    template<typename T, typename Tag>
    const char * tag_prefix(tagged<T,Tag>) { return ""; }

    template<typename T, typename Tag>
    std::ostream & operator<<(std::ostream &os, const tagged<T, Tag> &t)
    {
        return os << tag_prefix(t) << *t << tag_suffix(t);
    }

    template<typename Tag, typename T>
    tagged<T,Tag> tag(const T&v)
    {
        return tagged<T,Tag> {v};
    }

    template<typename Tag, typename T, typename Tag2>
     void tag(const tagged<T, Tag2>&) = delete;
}
