# Mixins

Sample: [mixins.cpp](../samples/mixins.cpp)

Mixins offer a way to add methods to existing classes, and allow clients to inject their own extensions, behaviour or specialisation.

```c++
// Define the name of your mixin
struct container_tag;

// Add implementations of your mixin
template <typename T> struct cy::mixin<T, container_tag>
{
    using size_type = std::size_t;

    size_type count() const
    {
        auto *self = static_cast<const T *>(this);
        return std::distance(self->begin(), self->end());
    }
};

// A class that implements 2 mixins
class mylist : public cy::implements<mylist, lockable_tag, container_tag>
{
};
```

# Reference

## Header file and namespace

```c++
#include <cutty/mixins.hpp>

using namespace cy = cutty;
```

## Types

### `struct mixin`

```c++
template<typename Derived, typename Tag, int Sequence = 0>
struct mixin;
```

This struct is designed to be specialised to provide customisation. Multiple mixins can be implemented provided using different `Sequence` numbers. Mixins can use `static_cast<Derived*>(this)` or use [explicit object member functions](https://en.cppreference.com/w/cpp/language/member_functions.html#Explicit_object_member_functions) to access the object.

### `struct implements`

```c++
template<typename Derived, typename... Tag>
struct implements;
```

Provides a class that inherits from all of the specified mixins. Classes that implement mixins derive from `implements`. Classes should use [CRTP](https://en.cppreference.com/w/cpp/language/crtp.html) to pass the derived class `Derived` to the mixin.
