# Properties
Really simple properties in C++.

# Summary
This library replaces slightly cumbersome methods calls like
```c++
person.set_Age(30);
assert(person.get_Adult());
```
with a more natural field-like interface like
```c++
person.Age = 30;
assert(person.Adult);
```
It achieves this by defining the `Age` and `Adult` properties as follows:
```c++
#include <Property.hpp>
	
class Person
{
public:
    CY_PROPERTY(Person, Age, int);
    CY_GETTER(Person, Adult, bool);

private:
    int get_Age() const { return age; }
    void set_Age(int value) { age = value; }
    bool get_Adult() const { return Age >= 18; }
    int age;
};
```

# Implementation
The implementation is extremely simple and efficient because the property can simply offset the `this` pointer of the property to obtain a pointer of the object containing the property. An optimizing compiler can remove the pointer manipulation, so in most cases there is no overhead of using properties. (Verified by looking at the disassembly of the optimized code.)

There is no dynamic memory allocation, and minimal change to the size of the declaring type because the additional property fields do not store any data. Each property increases the size of the type by just 1 byte. (Verified by looking at `sizeof`.)

## Rejected alternatives
Using `std::function`. This can be quite convenient but simply adds too much overhead. For example,
```c++
class Person
{
public:
    Property<int> Age = Property<int>([this]() { return age; }, [this](int a) { age = a; });
private:
    int age;
};
```

Alternatives to macros, such as,
```c++
class Person
{
public:
    Property<Person, bool> Adult; 
};

bool Property<Person, bool>::Get() const
{
    return Object<&Person::Adult>().Age >= 18;
}
```
The problem with this approach is that there sometimes needs to be a type-tag to distinguish different properties with the same type in the same object, and there were just too many ways to shoot yourself in the foot. It requires the methods to be defined out of line, and has the slightly awkward `Object<&Person::Adult>()` call to map from the property to the object.

The assignment operator and copy constructors do NOT set the property. This is because it's not clear which properties should be set, and
often the underlying field is copied instead. It could be dangerous to set a property before the underlying field has been initialized, making properties sensitive to declaration order.

The getter is `const` to match const semantics. Getters often have side-effects, which makes the `const` occasionally inconvenient, however there are other ways to solve this problem. Getters should be semantically const.

# Reference

## Macro `CY_PROPERTY(Class, Property, Type)`
This defines a property `Property` in the struct or class `Class`. The property has the type `Type` which is the type used to both get and set the property. (Note that you can make this type a reference if it is safe to do so.) It is not currently supported to have a different type for the getter and setter.

`Class` is the name of the class or struct defining the property. The property may be declared in a `public`, `private` or `protected` scope.

It is a compile time error if:

- `Class` does not match the type containing the property.
- Two properties have the same name in the same class.
- The `get_##Property` or `set_##Property` methods do not exist or have unsuitable signatures.
- The `get_##Property` method is not const. (Note that the getter is semantically const, and may need to use various other techniques, such as mutable or pimpls to get around the const requirement. Such discussion is beyond the scope of this document. On balance, making the getter const is probably the right interface.)

The defined property has the following methods:

- `Type Get() const;` Calls `get_##Property()` in the declaring object and returns the result. The `get_##Property` method may be `virtual` (to implement virtual properties), and must be const.
- `void Set(Type t)` Calls `set_##Property(t)` in the declaring object. The `set_##Property` method may be `virtual` (to implement virtual properties.)
- `operator Type() const` Same as `Get()`.	
- `Type operator*() const` Same as Get(). Use this when `operator Type()` is not working as expected.
- `ImplementationDefined & operator=(Type t)` Same as `Set()`.	
- Default constructor. The default constructor is private and it is not possible to construct the property object. (This is to prevent unsafe use of the property.)
- Copy constructor. The copy constructor is available but does not set the value of the property. (Rationale: Often, the underlying fields will be copied, and it may not make sense to set the new property. It may be unsafe for a setter to access underlying fields before they have been initialized.)

## Macro `CY_GETTER(Class, Property, Type)`
This defines a read-only property, that behaves exactly the same as `PROPERTY`, but it does not support the `Set()` method, or other functions that use `Set()`.

## Macro `CY_SETTER(Class, Property, Type)`
This defines a write-only property, that behaves exactly the same as `PROPERTY`, but it does not support the `Get()` method, or other functions that use `Get()`.

## Thread safety
Properties are themselves thread-safe, and may be called from multiple threads. However, it is the responsibility of the the `get_` and `set_` methods to implement the appropriate thread-safety.

## Exceptions
Properties do not themselves raise exceptions, and are exception-neutral. Exceptions thrown by `get_` and `set_` methods are passed to the caller. It is the responsibility of the `get_` and `set_` methods to implement appropriate exception guarantees, consistent with C++ best practice.
