// A mixin allows you to inject functionality into a base class
// in a safe and principled way. This functionality is not part
// of standard C++, but we can emulate it using templates.

#include <cutty/mixins.hpp>

#include <vector>
#include <iostream>
#include <mutex>

namespace cy = cutty;

// Each mixin is identified using a "tag" class, for example
struct container_tag;
struct tracker_tag;
struct lockable_tag;
struct lockable_container_tag;
struct printable_tag;

// To define a mixin, specialize the type cy::mixin as follows:
template <typename T> struct cy::mixin<T, container_tag>
{
    using size_type = std::size_t;

    size_type count() const
    {
        auto *self = static_cast<const T *>(this);
        return std::distance(self->begin(), self->end());
    }
};

// Existing mixins can be extended further. You can specify a sequence number
// (starting 0), which adds more mixins to the chain. Since this is the second mixin,
// we specify 1.
template <typename T> struct cy::mixin<T, container_tag, 1>
{
    void print() const
    {
        auto *self = static_cast<const T*>(this);
        std::cout << "{";
        for(auto &x : *self)
        {
            std::cout << x;
        }
        std::cout << "}\n";
    }
};

// Leaving a gap in the sequence means that the mixin will not be picked up
template <typename T> struct cy::mixin<T, container_tag, 4>
{
    static_assert(false, "this mixin is not found");
};

// Managing sequence numbers could become difficult, so cy::defines_mixin and 
// CY_REGISTER_MIXIN can be used to chain mixins together.

template<typename T>
struct lockable_impl : cy::defines_mixin<lockable_tag>
{
public:
    void lock() { m.lock(); }
    void unlock() { m.unlock(); }
private:
    std::mutex m;
};

CY_REGISTER_MIXIN(lockable_impl);

template<typename T>
struct lockable_impl2 : cy::defines_mixin<lockable_tag, lockable_impl<T>>
{
public:
    void f() {}
};

CY_REGISTER_MIXIN(lockable_impl2);

// Here is another mixin

template<typename T>
struct tracker_impl : cy::defines_mixin<tracker_tag>
{
    tracker_impl() { std::cout << "Created object!\n"; }
};

CY_REGISTER_MIXIN(tracker_impl);

// Mixins must be defined before they are used, otherwise they will
// not be applied. It is ok to not implement a mixin - it will be there as a
// hook for future use.

// To add a mixin to a type, you must inherit from mixin::implements,
// specifying the mixins you want implemented.
class mylist : public cy::implements<mylist, lockable_tag, container_tag>
{
  public:
    auto begin() const
    {
        return items.begin();
    }
    auto end() const
    {
        return items.end();
    }

  private:
    std::vector<int> items = {1,2,3};
};

// This example shows a class implementing multiple mixins
class myobject : public cy::implements<myobject, tracker_tag, lockable_tag>
{
};

// Concepts can be used to check the status of a mixin
static_assert(cy::has_mixin<myobject, tracker_tag>);
static_assert(!cy::has_mixin<myobject, container_tag>);

template<cy::has_mixin<lockable_tag> Lockable>
struct mywrap
{
};

using wrap_object = mywrap<myobject>;

int main()
{
    // Output: Created object!
    mylist l;
    mylist::size_type t;

    // Output: The list contains 3 items
    std::cout << "The list contains " << l.count() << " items\n";

    // Output: {123}
    l.print();
    l.lock();
    l.unlock();

    // Output: Created object!
    myobject o;
    o.lock();
    o.unlock();
}
