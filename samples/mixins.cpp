// A mixin allows you to inject functionality into a base class
// in a safe and principled way. This functionality is not part
// of standard C++, but we can emulate it using templates.

#include <cutty/mixins.hpp>

#include <vector>
#include <iostream>
#include <mutex>

// Each mixin is identified using a "tag" class, for example
struct container_tag;
struct tracker_tag;
struct lockable_tag;
struct lockable_container_tag;
struct printable_tag;

// To define a mixin, specialize the type cutty::mixin as follows:
template <typename T> struct cutty::mixin<T, container_tag>
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
template <typename T> struct cutty::mixin<T, container_tag, 1>
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
template <typename T> struct cutty::mixin<T, container_tag, 4>
{
    static_assert(false, "this mixin is not found");
};

// You can't reuse the same index or your program will not compile.
// That's the one definition rule.
//
// template <typename T> struct cutty::mixin<T, container_tag, 1>
// {
// };

// Managing sequence numbers could become difficult, so the DECLARE_MIXIN macro
// can be used to automatically extend a mixin with the next available sequence number.

template<typename T>
struct lockable_impl
{
public:
    void lock() { m.lock(); }
    void unlock() { m.unlock(); }
private:
    std::mutex m;
};

CY_DECLARE_MIXIN(lockable_tag, lockable_impl);

// Mixins can be combined using the EXTEND_MIXIN macro
CY_EXTEND_MIXIN(container_tag, tracker_tag);  // All containers are tracked
CY_EXTEND_MIXIN(lockable_container_tag, container_tag);
CY_EXTEND_MIXIN(lockable_container_tag, lockable_tag);

// Here is another mixin

template<typename T>
struct tracker_impl
{
    tracker_impl() { std::cout << "Created object!\n"; }
};

CY_DECLARE_MIXIN(tracker_tag, tracker_impl);

// Mixins must be defined before they are used, otherwise they will
// not be applied. It is ok to not implement a mixin - it will be there as a
// hook for future use.

// To add a mixin to a type, you must inherit from mixin::implements,
// specifying the mixins you want implemented.
class mylist : public cutty::implements<mylist, lockable_container_tag>
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
class myobject : public cutty::implements<myobject, tracker_tag, lockable_tag>
{
};

// Function injection
// You can inject functions directly as an alternative syntax

void print(const class myvalue &foo);

// We still need to include INJECT before the definition of Foo
CY_FUNCTION_MIXIN(printable_tag, print);

struct myvalue : cutty::implements<myvalue, printable_tag>
{
    int value = 42;
};

void print(const myvalue &f)
{
    std::cout << "Print called with " << f.value << std::endl;
}

// Concepts can be used to check the status of a mixin
static_assert(cutty::has_mixin<myobject, tracker_tag>);
static_assert(cutty::has_mixin<mylist, lockable_tag>);
static_assert(!cutty::has_mixin<myobject, container_tag>);

template<cutty::has_mixin<lockable_tag> Lockable>
struct mywrap
{
};

using wrap_object = mywrap<myobject>;

// Don't include the same mixin more than once - even indirectly. The following
// code will give a warning and not work as expected.
//
// class myobject2 : public cutty::implements<myobject, lockable_tag, lockable_container_tag>
// {
// };


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

    // Output: Print called with 42
    myvalue v;
    v.print();
}
