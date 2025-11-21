#pragma once
#include <iostream>

struct MyStruct
{
    int x;
};

inline std::ostream & operator<<(std::ostream &os, const MyStruct&s)
{
    return os << "MyStruct(" << s.x << ")";
}

inline MyStruct operator+(MyStruct a, MyStruct b)
{
    return {a.x + b.x};
}
